/*-
 * Copyright (c) 2013, 2016, Juniper Networks, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/* XXX GNU tar format is not supported by this driver */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/fcntl.h>
#include <sys/iconv.h>
#include <sys/libkern.h>
#include <sys/limits.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mount.h>
#include <sys/namei.h>
#include <sys/proc.h>
#include <sys/queue.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/vnode.h>

#include <fs/tarfs/tarfs.h>
#include <fs/tarfs/tarfs_dbg.h>

struct ustar_header {
	char	name[100];		/* File name */
	char	mode[8];		/* Mode flags */
	char	uid[8];			/* User id */
	char	gid[8];			/* Group id */
	char	size[12];		/* Size */
	char	mtime[12];		/* Modified time */
	char	checksum[8];		/* Checksum */
	char	typeflag[1];		/* Type */
	char	linkname[100];		/* "old format" stops here */
	char	magic[6];		/* POSIX UStar "ustar\0" indicator */
	char	version[2];		/* POSIX UStar version "00" */
	char	uname[32];		/* User name */
	char	gname[32];		/* Group name */
	char	major[8];		/* Device major number */
	char	minor[8];		/* Device minor number */
	char	prefix[155];		/* Path prefix */
};

struct tarfs_iconv {
	TAILQ_ENTRY(tarfs_iconv)	tfsiconv_entries;
	char				tfsiconv_charset[ICONV_CSNMAXLEN + 1];
	void *				tfsiconv_handle;
};

#define	TAR_EOF			((off_t)-1)

#define	TAR_TYPE_FILE		'0'
#define	TAR_TYPE_HARDLINK	'1'
#define	TAR_TYPE_SYMLINK	'2'
#define	TAR_TYPE_CHAR		'3'
#define	TAR_TYPE_BLOCK		'4'
#define	TAR_TYPE_DIRECTORY	'5'
#define	TAR_TYPE_FIFO		'6'
#define	TAR_TYPE_CONTIG		'7'
#define	TAR_TYPE_GLOBAL_EXTHDR	'g'
#define	TAR_TYPE_EXTHDR		'x'
#define	TAR_TYPE_GNU_SPARSE	'S'

#define	USTAR_MAGIC		"ustar\0"
#define	USTAR_VERSION		"\0\0"

MALLOC_DEFINE(M_TARFSMNT, "tarfs mount", "tarfs mount structures");
MALLOC_DEFINE(M_TARFSNODE, "tarfs node", "tarfs node structures");

typedef TAILQ_HEAD(, tarfs_iconv) tarfs_iconv_list_t;

struct iconv_functions *tarfs_iconv;

static vfs_mount_t	tarfs_mount;
static vfs_unmount_t	tarfs_unmount;
static vfs_root_t	tarfs_root;
static vfs_statfs_t	tarfs_statfs;
static vfs_fhtovp_t	tarfs_fhtovp;

static const char *tarfs_opts[] = {
	"export", "from", "gid", "mode", "uid",
	NULL
};

static int64_t
tarfs_str2octal(const char *strp, size_t len)
{
	int64_t val;
	size_t idx;
	int sign;

	for (idx = 0; idx < len; idx++)
		if (strp[idx] != ' ' && strp[idx] != '\t')
			break;

	if (idx == len)
		return 0;

	if (strp[idx] == '-') {
		sign = -1;
		idx++;
	} else
		sign = 1;

	val = 0;
	for (; idx < len; idx++) {
		if (strp[idx] < '0' || strp[idx] > '7')
			break;
		val <<= 3;
		val += (strp[idx] - '0');

		/* Truncate on overflow */
		if (val > INT64_MAX / 8) {
			val = INT64_MAX;
			break;
		}
	}

	return (sign > 0) ? val : -val;
}

static int64_t
tarfs_str2base256(const char *strp, size_t len)
{
	int64_t val;
	size_t idx;

	/* Signed bit is 0x40 since 0x80 is the base-256 indicator */
	if ((strp[0] & 0x40) != 0)
		val = (int64_t)-1;
	else
		val = 0;
	val <<= 6;
	val |= (strp[0] & 0x3f);
	for (idx = 1; idx < len; idx++) {
		val <<= 8;
		val |= (0xff & (int64_t)strp[idx]);

		/* Truncate on overflow and underflow */
		if (val > INT64_MAX / 256) {
			val = INT64_MAX;
			break;
		} else if (val < INT64_MAX / 256) {
			val = INT64_MIN;
			break;
		}
	}

	return val;
}

static unsigned int
tarfs_str2int64(const char *strp, size_t len)
{

	if (len < 1)
		return 0;

	if ((strp[0] & 0x80) != 0)
		return tarfs_str2base256(strp, len);
	return tarfs_str2octal(strp, len);
}

static struct tarfs_iconv *
tarfs_get_iconv(tarfs_iconv_list_t *listhead, const char *charset)
{
	struct tarfs_iconv *iconv;

	MPASS(tarfs_iconv != NULL);
	TAILQ_FOREACH(iconv, listhead, tfsiconv_entries) {
		if (strcmp(iconv->tfsiconv_charset, charset) == 0)
			return iconv;
	}

	iconv = (struct tarfs_iconv *)malloc(sizeof(struct tarfs_iconv),
	    M_TEMP, M_WAITOK);
	strncpy(iconv->tfsiconv_charset, charset, ICONV_CSNMAXLEN);
	iconv->tfsiconv_charset[ICONV_CSNMAXLEN] = '\0';
	tarfs_iconv->open("ISO8859-1", iconv->tfsiconv_charset,
	    &iconv->tfsiconv_handle);
	TAILQ_INSERT_TAIL(listhead, iconv, tfsiconv_entries);
	return iconv;
}

static void
tarfs_free_iconv(tarfs_iconv_list_t *listhead)
{
	struct tarfs_iconv *iconv;

	while (!TAILQ_EMPTY(listhead)) {
		iconv = TAILQ_FIRST(listhead);
		TAILQ_REMOVE(listhead, iconv, tfsiconv_entries);

		tarfs_iconv->close(iconv->tfsiconv_handle);
		free(iconv, M_TEMP);
	}
}

static boolean_t
tarfs_checksum(struct ustar_header *hdrp)
{
	const unsigned char *ptr;
	int64_t checksum, hdrsum;
	size_t idx;

	hdrsum = tarfs_str2int64(hdrp->checksum, sizeof(hdrp->checksum));
	TARFS_DPF(CHECKSUM, "%s: header checksum %lx\n", __func__, hdrsum);

	checksum = 0;
	for (ptr = (const unsigned char *)hdrp;
	     ptr < (const unsigned char *)&hdrp->checksum; ptr++)
		checksum += *ptr;
	for (idx = 0; idx < sizeof(hdrp->checksum); idx++)
		checksum += 0x20;
	for (ptr = (const unsigned char *)&hdrp->typeflag;
	     ptr < (const unsigned char *)(hdrp + 1); ptr++)
		checksum += *ptr;
	TARFS_DPF(CHECKSUM, "%s: calc unsigned checksum %lx\n", __func__,
	    checksum);
	if (hdrsum == checksum)
		return true;

	/*
	 * Repeat test with signed bytes, some older formats use a broken
	 * form of the calculation
	 */
	checksum = 0;
	for (ptr = (const unsigned char *)hdrp;
	     ptr < (const unsigned char *)&hdrp->checksum; ptr++)
		checksum += *((const signed char *)ptr);
	for (idx = 0; idx < sizeof(hdrp->checksum); idx++)
		checksum += 0x20;
	for (ptr = (const unsigned char *)&hdrp->typeflag;
	     ptr < (const unsigned char *)(hdrp + 1); ptr++)
		checksum += *((const signed char *)ptr);
	TARFS_DPF(CHECKSUM, "%s: calc signed checksum %lx\n", __func__,
	    checksum);
	if (hdrsum == checksum)
		return true;

	return false;
}


static enum vtype
tarfs_header_to_vtype(struct ustar_header *hdrp, boolean_t ustar_format)
{
	enum vtype type;
	size_t namelen;

	switch (hdrp->typeflag[0]) {
	case '\0':
	case TAR_TYPE_FILE:
		type = VREG;
		break;
	case TAR_TYPE_SYMLINK:
		type = VLNK;
		break;
	case TAR_TYPE_HARDLINK:
		/* Fallthrough */
	default:
		type = VNON;
		break;
	}

	if (!ustar_format) {
		namelen = strnlen(hdrp->name, sizeof(hdrp->name));
		if (namelen > 0 && hdrp->name[namelen - 1] == '/')
			type = VDIR;
		return type;
	}

	switch (hdrp->typeflag[0]) {
	case TAR_TYPE_CHAR:
		type = VCHR;
		break;
	case TAR_TYPE_BLOCK:
		type = VBLK;
		break;
	case TAR_TYPE_DIRECTORY:
		type = VDIR;
		break;
	case TAR_TYPE_FIFO:
		type = VFIFO;
		break;
	case TAR_TYPE_CONTIG:
	case TAR_TYPE_GLOBAL_EXTHDR:
	case TAR_TYPE_EXTHDR:
	case TAR_TYPE_GNU_SPARSE:
		type = VNON;		
		break;
	default:
		type = VREG;
		break;
	}

	return type;
}

static int
tarfs_read_block(struct vnode *vp, struct ustar_header *hdrp, off_t *blknum)
{
	struct uio auio;
	struct iovec aiov;
	off_t offset;
	int error;

	offset = (*blknum) * TARFS_BLOCKSIZE;
	aiov.iov_base = hdrp;
	aiov.iov_len = TARFS_BLOCKSIZE;
	auio.uio_iov = &aiov;
	auio.uio_iovcnt = 1;
	auio.uio_offset = offset;
	auio.uio_segflg = UIO_SYSSPACE;
	auio.uio_rw = UIO_READ;
	auio.uio_resid = TARFS_BLOCKSIZE;
	auio.uio_td = curthread;
	error = VOP_READ(vp, &auio, IO_DIRECT, curthread->td_ucred);
	if (auio.uio_resid == TARFS_BLOCKSIZE)
		*blknum = TAR_EOF;

	return error;
}

static void
tarfs_lookup_path(struct tarfs_mount *tmp, char *name, size_t namelen,
    char **endp, char **sepp, struct tarfs_node **retparent,
    struct tarfs_node **retnode)
{
	struct componentname cn;
	struct tarfs_node *parent, *tnp;
	char *sep;
	size_t len;

	MPASS(name != NULL && namelen != 0);

	parent = tnp = tmp->tfsmnt_root;
	if (tnp == NULL)
		panic("%s: root node not yet created", __func__);

	bzero(&cn, sizeof(cn));

	TARFS_DPF(LOOKUP, "%s: Full path: %.*s\n", __func__, (int)namelen,
	    name);

	sep = NULL;
	for (;;) {
		while (name[0] == '/' && namelen > 0)
			name++, namelen--;
		if (namelen == 0 || name[0] == '\0') {
			name = NULL;
			break;
		}

		parent = tnp;
		cn.cn_nameptr = sep = name;
		len = namelen;
		do {
			if (*sep == '/')
				break;
			sep++;
		} while (--len > 0);
		if (*sep == '/')
			cn.cn_namelen = sep - name;
		else
			cn.cn_namelen = namelen;
		TARFS_DPF(LOOKUP, "%s: Search: %.*s\n", __func__,
		    (int)cn.cn_namelen, cn.cn_nameptr);

		tnp = tarfs_lookup_node(parent, NULL, &cn);
		if (tnp == NULL)
			break;
		name += cn.cn_namelen;
		namelen -= cn.cn_namelen;
	}

	TARFS_DPF(LOOKUP, "%s: Parent %p, node %p\n", __func__, parent, tnp);

	if (retparent)
		*retparent = parent;
	if (retnode)
		*retnode = tnp;
	if (endp) {
		if (namelen > 0)
			*endp = name;
		else
			*endp = NULL;
	}
	if (sepp)
		*sepp = sep;
}

static void
tarfs_free_mount(struct tarfs_mount *tmp)
{
	struct mount *mp;
	struct tarfs_node *tnp;
	struct thread *td = curthread;

	MPASS(tmp != NULL);

	TARFS_DPF(ALLOC, "%s: Freeing mount structure %p\n", __func__, tmp);

	if (tmp->tfsmnt_vp != NULL) {
		TARFS_DPF(ALLOC, "%s: closing backing file %p\n", __func__,
		    tmp->tfsmnt_vp);
		(void)vn_close(tmp->tfsmnt_vp, FREAD, td->td_ucred, td);
	}

	TARFS_DPF(ALLOC, "%s: freeing tarfs_node structures\n", __func__);
	while (!TAILQ_EMPTY(&tmp->tfsmnt_allnodes)) {
		tnp = TAILQ_FIRST(&tmp->tfsmnt_allnodes);
		TAILQ_REMOVE(&tmp->tfsmnt_allnodes, tnp, tfsnode_entries);
		tarfs_free_node(tnp);
	}

	TARFS_DPF(ALLOC, "%s: deleting unr header\n", __func__);
	delete_unrhdr(tmp->tfsmnt_ino_unr);
	mp = tmp->tfsmnt_vfs;
	mp->mnt_data = NULL;

	TARFS_DPF(ALLOC, "%s: freeing structure\n", __func__);
	free(tmp, M_TARFSMNT);
}

static int
tarfs_alloc_mount(struct mount *mp, struct vnode *vp, time_t root_mtime,
    uid_t root_uid, gid_t root_gid, mode_t root_mode, struct tarfs_mount **tmpp)
{
	char block[TARFS_BLOCKSIZE];
	struct vattr va;
	tarfs_iconv_list_t iconv_list = TAILQ_HEAD_INITIALIZER(iconv_list);
	char *fullpath, *namep, *sep;
	struct ustar_header *hdrp;
	struct tarfs_mount *tmp;
	struct tarfs_node *parent, *root, *tnp;
	size_t linknamelen, namelen, nblks, nfiles, prefixlen, sz;
	enum vtype type;
	dev_t rdev;
	gid_t gid;
	mode_t mode;
	off_t blknum;
	time_t mtime;
	uid_t uid;
	long major, minor;
	int endmarker, error;
	boolean_t ustar_format;

	KASSERT(tmpp != NULL, ("tarfs mount return is NULL"));

	hdrp = (struct ustar_header *)block;

	error = VOP_GETATTR(vp, &va, mp->mnt_cred);
	if (error)
		return error;

	/* Allocate and initialize tarfs mount structure */
	tmp = (struct tarfs_mount *)malloc(sizeof(struct tarfs_mount),
	    M_TARFSMNT, M_WAITOK | M_ZERO);
	TARFS_DPF(ALLOC, "%s: Allocated mount structure\n", __func__);

	mtx_init(&tmp->tfsmnt_allnode_lock, "tarfs allnode lock", NULL,
	    MTX_DEF);
	TAILQ_INIT(&tmp->tfsmnt_allnodes);
	tmp->tfsmnt_ino_unr = new_unrhdr(3, INT_MAX, &tmp->tfsmnt_allnode_lock);
	tmp->tfsmnt_vp = vp;
	tmp->tfsmnt_vfs = mp;
	if (tarfs_iconv)
		tarfs_get_iconv(&iconv_list, "UTF-8");

	error = tarfs_alloc_node(tmp, NULL, 0, VDIR, 0, 0, root_mtime, root_uid,
	    root_gid, root_mode & ALLPERMS, NULL, NODEV, NULL, &root);
	if (error != 0 || root == NULL)
		goto bad;
	tmp->tfsmnt_root = root;
	TARFS_DPF(ALLOC, "%s: root %p\n", __func__, root);

	fullpath = NULL;
	endmarker = 0;
	blknum = 0;
	nfiles = 0;
	for (;;) {
		linknamelen = 0;
		bzero(block, sizeof(block));

		/* Read the next header */
		error = tarfs_read_block(vp, hdrp, &blknum);
		if (error != 0)
			goto bad;
		if (blknum == TAR_EOF)
			break;
		blknum++;

		if (memcchr(block, 0x00, TARFS_BLOCKSIZE) == NULL) {
			if (endmarker++)
				break;
			continue;
		}
		if (endmarker) {
			TARFS_DPF(ALLOC, "%s: Possibly corrupted tar file\n",
			    __func__);
			error = EINVAL;
			goto bad;
		}

		if (!tarfs_checksum(hdrp)) {
			TARFS_DPF(ALLOC,
			    "%s: header block %jd checksum failure\n",
			    __func__, blknum - 1);
			error = EINVAL;
			goto bad;
		}

		ustar_format = (bcmp(hdrp->magic, USTAR_MAGIC,
		    sizeof(USTAR_MAGIC) - 1) == 0 \
		    && bcmp(hdrp->version, USTAR_VERSION,
		    sizeof(USTAR_VERSION) - 1) == 0);

		sz = tarfs_str2int64(hdrp->size, sizeof(hdrp->size));
		nblks = TARFS_SZ2BLKS(sz);
		mtime = tarfs_str2int64(hdrp->mtime, sizeof(hdrp->mtime));
		mode = tarfs_str2int64(hdrp->mode, sizeof(hdrp->mode));
		gid = tarfs_str2int64(hdrp->gid, sizeof(hdrp->gid));
		uid = tarfs_str2int64(hdrp->uid, sizeof(hdrp->uid));
		rdev = NODEV;

		type = tarfs_header_to_vtype(hdrp, ustar_format);
		switch (type) {
		case VLNK:
			sz = strnlen(hdrp->linkname, sizeof(hdrp->linkname));
			break;
		case VBLK:
		case VCHR:
			major = tarfs_str2int64(hdrp->major,
			    sizeof(hdrp->major));
			minor = tarfs_str2int64(hdrp->minor,
			    sizeof(hdrp->minor));
			rdev = makedev(major, minor);
			break;
		default:
			break;
		}

		parent = tnp = NULL;
		sep = NULL;
		namelen = strnlen(hdrp->name, sizeof(hdrp->name));
		if (ustar_format)
			prefixlen = strnlen(hdrp->prefix, sizeof(hdrp->prefix));
		else
			prefixlen = 0;
		if (prefixlen > 0) {
			fullpath = realloc(fullpath, namelen + prefixlen + 2,
			    M_TEMP, M_WAITOK);
			snprintf(fullpath, namelen + prefixlen + 2, "%*s/%*s",
			    (int)prefixlen, hdrp->prefix, (int)namelen,
			    hdrp->name);
			fullpath[namelen + prefixlen + 1] = '\0';
			namelen += prefixlen + 1;
		}

		tarfs_lookup_path(tmp, prefixlen > 0 ? fullpath : hdrp->name,
		    namelen, &namep, &sep, &parent, &tnp);

		/* Allocate a node */
		if (type != VNON && tnp == NULL && namep != NULL &&
		    namep != sep) {
			error = tarfs_alloc_node(tmp, namep, sep - namep,
			    type, blknum * TARFS_BLOCKSIZE, sz, mtime, uid,
			    gid, mode, hdrp->linkname, rdev, parent, &tnp);
			if (error != 0)
				return error;

			TARFS_DPF(ALLOC, "%s: node %p\n", __func__, tnp);
			nfiles++;
		}

		/* Seek to the next header */
		blknum += nblks;
	}

	tmp->tfsmnt_nblocks = blknum;
	tmp->tfsmnt_nfiles = nfiles;

	tarfs_free_iconv(&iconv_list);

	*tmpp = tmp;

	TARFS_DPF(ALLOC, "%s: tfsmnt_root %p\n", __func__, tmp->tfsmnt_root);

#ifdef	TARFS_DEBUG_VERBOSE
	tarfs_dump_tree(tmp->tfsmnt_root);
#endif	/* TARFS_DEBUG_VERBOSE */

	return 0;

bad:
	tarfs_free_mount(tmp);
	tarfs_free_iconv(&iconv_list);
	return error;
}

/*
 * VFS Operations.
 */

static int
tarfs_mount(struct mount *mp)
{
	struct nameidata nd;
	struct vattr va;
	struct tarfs_mount *tmp = NULL;
	struct thread *td = curthread;
	struct vnode *vp;
	char *from;
#ifdef	TARFS_DEBUG
	char *fspath;
#endif	/* TARFS_DEBUG */
	uid_t root_uid;
	gid_t root_gid;
	mode_t root_mode;
	int error, flags, len;
#if VFS_VERSION == VFS_VERSION_00
	int vfslocked;
#endif

	if (mp->mnt_flag & MNT_UPDATE)
		return EOPNOTSUPP;

	if (vfs_filteropt(mp->mnt_optnew, tarfs_opts))
		return EINVAL;

	vn_lock(mp->mnt_vnodecovered, LK_SHARED | LK_RETRY);
	error = VOP_GETATTR(mp->mnt_vnodecovered, &va, mp->mnt_cred);
	VOP_UNLOCK(mp->mnt_vnodecovered, 0);
	if (error)
		return error;

	if (mp->mnt_cred->cr_ruid != 0 ||
	    vfs_scanopt(mp->mnt_optnew, "gid", "%d", &root_gid) != 1)
		root_gid = va.va_gid;
	if (mp->mnt_cred->cr_ruid != 0 ||
	    vfs_scanopt(mp->mnt_optnew, "uid", "%d", &root_uid) != 1)
		root_uid = va.va_uid;
	if (mp->mnt_cred->cr_ruid != 0 ||
	    vfs_scanopt(mp->mnt_optnew, "mode", "%ho", &root_mode) != 1)
		root_mode = va.va_mode;

	error = vfs_getopt(mp->mnt_optnew, "from", (void **)&from, &len);
	if (error != 0 || from[len - 1] != '\0')
		return EINVAL;
#ifdef	TARFS_DEBUG
	error = vfs_getopt(mp->mnt_optnew, "fspath", (void **)&fspath, &len);
	if (error != 0 || fspath[len - 1] != '\0')
		return EINVAL;
	TARFS_DPF(FS, "%s: From = %s\n", __func__, from);
#endif	/* TARFS_DEBUG */

	/* Find the source tar file */
	flags = FREAD;
	NDINIT(&nd, LOOKUP, FOLLOW | LOCKLEAF, UIO_SYSSPACE, from, td);
	error = vn_open(&nd, &flags, 0, NULL);
	if (error != 0)
		return error;
#if VFS_VERSION == VFS_VERSION_00
	vfslocked = NDHASGIANT(&nd);
#endif
	NDFREE(&nd, NDF_ONLY_PNBUF);
	if (nd.ni_vp->v_type != VREG) {
		VOP_UNLOCK(nd.ni_vp, 0);
		(void)vn_close(nd.ni_vp, flags, td->td_ucred, td);
		error = EINVAL;
		goto bad;
	}
	TARFS_DPF(FS, "%s: Opened tar file: %s\n", __func__, from);

	vp = nd.ni_vp;
	VOP_UNLOCK(vp, 0);

	error = tarfs_alloc_mount(mp, vp, va.va_mtime.tv_sec, root_uid,
	    root_gid, root_mode, &tmp);
	if (error != 0)
		goto bad;

	/*
	 * Unconditionally mount as read-only.
	 */
	MNT_ILOCK(mp);
	mp->mnt_flag |= (MNT_LOCAL | MNT_RDONLY);
#if VFS_VERSION == VFS_VERSION_00
	mp->mnt_kern_flag |= MNTK_MPSAFE;
#endif
	MNT_IUNLOCK(mp);
	mp->mnt_data = tmp;

	vfs_getnewfsid(mp);
	vfs_mountedfrom(mp, "tarfs");
#if VFS_VERSION == VFS_VERSION_00
	VFS_UNLOCK_GIANT(vfslocked);
#endif
	TARFS_DPF(FS, "%s: Mounted %s on %s\n", __func__, from, fspath);

	return 0;

bad:
	if (tmp)
		tarfs_free_mount(tmp);
#if VFS_VERSION == VFS_VERSION_00
	VFS_UNLOCK_GIANT(vfslocked);
#endif
	return error;
}

/*
 * unmount system call
 */
static int
tarfs_unmount(struct mount *mp, int mntflags)
{
	struct tarfs_mount *tmp;
	int error;
	int flags = 0;

	TARFS_DPF(FS, "%s: Unmounting %p\n", __func__, mp);

	/* Handle forced unmounts */
	if (mntflags & MNT_FORCE)
		flags |= FORCECLOSE;

	/* Finalize all pending I/O */
	error = vflush(mp, 0, flags, curthread);
	if (error != 0)
		return error;
	tmp = VFS_TO_TARFS(mp);

	MPASS(tmp->tfsmnt_vp != NULL);
	tarfs_free_mount(tmp);

	return 0;
}

/*
 * Return root of a filesystem
 */
static int
tarfs_root(struct mount *mp, int flags, struct vnode **vpp)
{
	struct vnode *nvp;
	int error;

	TARFS_DPF(FS, "%s: Getting root vnode\n", __func__);

	error = VFS_VGET(mp, TARFS_ROOTINO, LK_EXCLUSIVE, &nvp);
	if (error != 0)
		return error;

	*vpp = nvp;
	return 0;
}

/*
 * Get filesystem statistics.
 */
static int
tarfs_statfs(struct mount *mp, struct statfs *sbp)
{
	struct tarfs_mount *tmp;

	tmp = VFS_TO_TARFS(mp);

	sbp->f_bsize = 512;
	sbp->f_iosize = 512;
	sbp->f_blocks = tmp->tfsmnt_nblocks;
	sbp->f_bfree = 0;
	sbp->f_bavail = 0;
	sbp->f_files = tmp->tfsmnt_nfiles;
	sbp->f_ffree = 0;

	return 0;
}

static int
tarfs_vget(struct mount *mp, ino_t ino, int flags, struct vnode **vpp)
{
	struct tarfs_mount *tmp;
	struct tarfs_node *tnp;
	struct thread *td;
	struct vnode *vp;
	int error;

	TARFS_DPF(FS, "%s: mp %p, ino %d, flags %d\n", __func__, mp, ino,
	    flags);

	td = curthread;
	error = vfs_hash_get(mp, ino, flags, td, vpp, NULL, NULL);
	if (error)
		return error;

	if (*vpp != NULL) {
		TARFS_DPF(FS, "%s: found hashed vnode %p\n", __func__, *vpp);
		return error;
	}

	TARFS_DPF(FS, "%s: no hashed vnode for inode %d\n", __func__, ino);

	tmp = VFS_TO_TARFS(mp);
	/* XXX Should use hash instead? */
	TAILQ_FOREACH(tnp, &tmp->tfsmnt_allnodes, tfsnode_entries) {
		if (tnp->tfsnode_ino == ino)
			break;
	}
	TARFS_DPF(FS, "%s: search of all nodes found %p\n", __func__, tnp);
	if (tnp == NULL)
		return ENOENT;

	error = getnewvnode("tarfs", mp, &tarfs_vnodeops, &vp);
	if (error != 0)
		goto bad;
	TARFS_DPF(FS, "%s: allocated vnode\n", __func__);
	vp->v_data = tnp;
	vp->v_type = tnp->tfsnode_type;
	tnp->tfsnode_vnode = vp;

	lockmgr(vp->v_vnlock, LK_EXCLUSIVE, NULL);
	error = insmntque(vp, mp);
	if (error != 0)
		goto bad;
	TARFS_DPF(FS, "%s: inserting entry into VFS hash\n", __func__);
	error = vfs_hash_insert(vp, ino, flags, td, vpp, NULL, NULL);
	if (error || *vpp != NULL)
		return (error);

	*vpp = vp;
	return 0;

bad:
	*vpp = NULLVP;
	return error;
}

static int
tarfs_fhtovp(struct mount *mp, struct fid *fhp, int flags, struct vnode **vpp)
{
	struct tarfs_mount *tmp;
	struct tarfs_node *tnp;
	struct tarfs_fid *tfp;
	struct vnode *nvp;
	int error;

	tfp = (struct tarfs_fid *)fhp;
	tmp = VFS_TO_TARFS(mp);
	if (tfp->tfsfid_ino < TARFS_ROOTINO || tfp->tfsfid_ino > INT_MAX)
		return ESTALE;

	error = VFS_VGET(mp, tfp->tfsfid_ino, LK_EXCLUSIVE, &nvp);
	if (error != 0) {
		*vpp = NULLVP;
		return error;
	}
	tnp = VP_TO_TARFS_NODE(nvp);
	if (tnp->tfsnode_mode == 0 ||
	    tnp->tfsnode_gen != tfp->tfsfid_gen ||
	    tnp->tfsnode_nlink <= 0) {
		vput(nvp);
		*vpp = NULLVP;
		return ESTALE;
	}
	*vpp = nvp;
	return 0;
}

static struct vfsops tarfs_vfsops = {
	.vfs_fhtovp =	tarfs_fhtovp,
	.vfs_mount =	tarfs_mount,
	.vfs_root =	tarfs_root,
	.vfs_statfs =	tarfs_statfs,
	.vfs_unmount =	tarfs_unmount,
	.vfs_vget =	tarfs_vget,
};
VFS_SET(tarfs_vfsops, tarfs, VFCF_READONLY);
MODULE_VERSION(tarfs, 1);

