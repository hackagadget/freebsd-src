/*-
 * Copyright (c) 2013, Juniper Networks, Inc.
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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/dirent.h>
#include <sys/fcntl.h>
#include <sys/namei.h>
#include <sys/proc.h>
#include <sys/vnode.h>

#include <fs/tarfs/tarfs.h>
#include <fs/tarfs/tarfs_dbg.h>

static int
tarfs_open(struct vop_open_args *ap)
{
	struct tarfs_node *tnp;
	struct vnode *vp;
	int mode;

	vp = ap->a_vp;
	mode = ap->a_mode;

	MPASS(VOP_ISLOCKED(vp));
	tnp = VP_TO_TARFS_NODE(vp);

	TARFS_DPF(VNODE, "%s: vnode %p, tarfs_node %p\n", __func__, vp, tnp);

	if ((mode & FWRITE) == FWRITE)
		return EPERM;

	vnode_create_vobject(vp, tnp->tfsnode_size, ap->a_td);
	return 0;
}

static int
tarfs_close(struct vop_close_args *ap)
{
	struct tarfs_node *tnp;
	struct vnode *vp;

	vp = ap->a_vp;

	MPASS(VOP_ISLOCKED(vp));
	tnp = VP_TO_TARFS_NODE(vp);

	TARFS_DPF(VNODE, "%s: vnode %p, tarfs_node %p\n", __func__, vp, tnp);

	return 0;
}

static int
tarfs_access(struct vop_access_args *ap)
{
	struct tarfs_node *tnp;
	struct vnode *vp;
	accmode_t accmode;
	struct ucred *cred;
	int error;

	vp = ap->a_vp;
	accmode = ap->a_accmode;
	cred = ap->a_cred;

	MPASS(VOP_ISLOCKED(vp));
	tnp = VP_TO_TARFS_NODE(vp);

	TARFS_DPF(VNODE, "%s: vnode %p, tarfs_node %p\n", __func__, vp, tnp);

	switch (vp->v_type) {
	case VDIR:
	case VLNK:
	case VREG:
		if ((accmode & VWRITE) != 0)
			return EROFS;
		break;
	case VBLK:
	case VCHR:
	case VFIFO:
		break;
	default:
		return EINVAL;
	}

	if ((accmode & VWRITE) != 0)
		return EPERM;

	error = vaccess(vp->v_type, tnp->tfsnode_mode, tnp->tfsnode_uid,
	    tnp->tfsnode_gid, accmode, cred, NULL);
	return error;
}

static int
tarfs_getattr(struct vop_getattr_args *ap)
{
	struct tarfs_node *tnp;
	struct vnode *vp;
	struct vattr *vap;

	vp = ap->a_vp;
	vap = ap->a_vap;
	tnp = VP_TO_TARFS_NODE(vp);

	TARFS_DPF(VNODE,
	    "%s: tarfs_node %p, node type %d, tarfs_node type %d\n",
	    __func__, tnp, vp->v_type, tnp->tfsnode_type);

	vap->va_type = vp->v_type;
	vap->va_mode = tnp->tfsnode_mode;
	vap->va_nlink = tnp->tfsnode_nlink;
	vap->va_gid = tnp->tfsnode_gid;
	vap->va_uid = tnp->tfsnode_uid;
	vap->va_fsid = vp->v_mount->mnt_stat.f_fsid.val[0];
	vap->va_fileid = tnp->tfsnode_ino;
	vap->va_size = tnp->tfsnode_size;
	vap->va_blocksize = TARFS_BLOCKSIZE;
	vap->va_atime = tnp->tfsnode_atime;
	vap->va_ctime = tnp->tfsnode_ctime;
	vap->va_mtime = tnp->tfsnode_mtime;
	vap->va_birthtime = tnp->tfsnode_birthtime;
	vap->va_gen = tnp->tfsnode_gen;
	vap->va_flags = tnp->tfsnode_flags;
	vap->va_rdev = (vp->v_type == VBLK || vp->v_type == VCHR) ?
	    tnp->tfsnode_rdev : NODEV;
	vap->va_bytes = round_page(tnp->tfsnode_size);
	vap->va_filerev = 0;

	return 0;
}

static int
tarfs_lookup(struct vop_cachedlookup_args *ap)
{
	struct tarfs_mount *tmp;
	struct tarfs_node *dirnode, *parent, *tnp;
	struct componentname *cnp;
	struct mount *mp;
	struct vnode *dvp;
	struct vnode **vpp;
	int error;

	dvp = ap->a_dvp;
	vpp = ap->a_vpp;
	cnp = ap->a_cnp;

	*vpp = NULLVP;
	dirnode = VP_TO_TARFS_NODE(dvp);
	parent = dirnode->tfsnode_parent;
	tmp = dirnode->tfsnode_tmp;
	mp = tmp->tfsmnt_vfs;
	tnp = NULL;

	TARFS_DPF(LOOKUP, "%s: tarfs_node %p, parent %p\n", __func__,
	    dirnode, parent);
	TARFS_DPF(LOOKUP, "\tname: %.*s\n", (int)cnp->cn_namelen,
	    cnp->cn_nameptr);

	error = VOP_ACCESS(dvp, VEXEC, cnp->cn_cred, cnp->cn_thread);
	if (error != 0)
		return error;

	if (cnp->cn_flags & ISDOTDOT) {
		int locktype;

		/* Do not allow .. on the root node */
		if (parent == NULL || parent == dirnode)
			return ENOENT;

		locktype = VOP_ISLOCKED(dvp);
		vhold(dvp);
		VOP_UNLOCK(dvp, 0);
		/* Allocate a new vnode on the matching entry */
		error = VFS_VGET(mp, parent->tfsnode_ino, cnp->cn_lkflags,
		    vpp);
		vn_lock(dvp, locktype | LK_RETRY);
		vdrop(dvp);
		if (error)
			return error;
	} else if (cnp->cn_namelen == 1 && cnp->cn_nameptr[0] == '.') {
		VREF(dvp);
		*vpp = dvp;
	} else {
		tnp = tarfs_lookup_node(dirnode, NULL, cnp);
		if (tnp == NULL)
			return ENOENT;

		if ((cnp->cn_flags & ISLASTCN) == 0 &&
		    (tnp->tfsnode_type != VDIR && tnp->tfsnode_type != VLNK))
			return ENOTDIR;

		error = VFS_VGET(mp, tnp->tfsnode_ino, cnp->cn_lkflags, vpp);
		if (error)
			return error;
	}

#ifdef	TARFS_DEBUG
	if (tnp == NULL)
		tnp = VP_TO_TARFS_NODE(*vpp);
	TARFS_DPF(LOOKUP, "%s: found vnode %p, tarfs_node %p\n", __func__,
	    *vpp, tnp);
#endif	/* TARFS_DEBUG */

	/* Store the result the the cache if MAKEENTRY is specified in flags */
	if ((cnp->cn_flags & MAKEENTRY) != 0 && cnp->cn_nameiop != CREATE)
		cache_enter(dvp, *vpp, cnp);

	return error;
}

static int
tarfs_readdir(struct vop_readdir_args *ap)
{
	struct dirent cde;
	struct tarfs_node *current, *tnp;
	struct vnode *vp;
	struct uio *uio;
	int *eofflag;
	u_long **cookies;
	int *ncookies;
	off_t ndirents, off;
	int error;

	vp = ap->a_vp;
	uio = ap->a_uio;
	eofflag = ap->a_eofflag;
	cookies = ap->a_cookies;
	ncookies = ap->a_ncookies;

	if (vp->v_type != VDIR)
		return ENOTDIR;

	tnp = VP_TO_TARFS_NODE(vp);
	off = uio->uio_offset;
	current = NULL;
	ndirents = 0;

	TARFS_DPF(VNODE, "%s: tarfs_node %p, vp %p, off %jd\n", __func__, tnp,
	    vp, off);

	if (uio->uio_offset == TARFS_COOKIE_EOF)
		return ENOENT;

	if (uio->uio_offset == TARFS_COOKIE_DOT) {
		TARFS_DPF(VNODE, "%s: Generating . entry\n", __func__);
		/* fake . entry */
		cde.d_fileno = tnp->tfsnode_ino;
		cde.d_type = DT_DIR;
		cde.d_namlen = 1;
		cde.d_name[0] = '.';
		cde.d_name[1] = '\0';
		cde.d_reclen = GENERIC_DIRSIZ(&cde);
		error = uiomove(&cde, cde.d_reclen, uio);
		if (error)
			return error;

		uio->uio_offset = TARFS_COOKIE_DOTDOT;
		ndirents++;
	}
	if (uio->uio_offset == TARFS_COOKIE_DOTDOT) {
		TARFS_DPF(VNODE, "%s: Generating .. entry\n", __func__);
		/* fake .. entry */
		MPASS(tnp->tfsnode_parent != NULL);
		TARFS_NODE_LOCK(tnp->tfsnode_parent);
		cde.d_fileno = tnp->tfsnode_parent->tfsnode_ino;
		TARFS_NODE_UNLOCK(tnp->tfsnode_parent);
		cde.d_type = DT_DIR;
		cde.d_namlen = 2;
		cde.d_name[0] = '.';
		cde.d_name[1] = '.';
		cde.d_name[2] = '\0';
		cde.d_reclen = GENERIC_DIRSIZ(&cde);
		error = uiomove(&cde, cde.d_reclen, uio);
		if (error)
			return error;

		current = TAILQ_FIRST(&tnp->tfsnode_dir.dirhead);
		if (current == NULL)
			goto done;
		else {
			uio->uio_offset = current->tfsnode_ino;
			TARFS_DPF(VNODE,
			    "%s: Setting current node to %p, ino %d\n",
			    __func__, current, current->tfsnode_ino);
			if (current->tfsnode_name != NULL)
				TARFS_DPF(VNODE, "%s: name: %s\n", __func__,
				    current->tfsnode_name);
		}
		ndirents++;
	}

	if (current == NULL) {
		current = tarfs_lookup_dir(tnp, uio->uio_offset);
		TARFS_DPF(VNODE, "%s: [%jd] Setting current node to %p\n",
		    __func__, ndirents, current);
		if (current == NULL) {
			if (ndirents == 0)
				error = EINVAL;
			goto done;
		}
	}

	do {
		cde.d_fileno = current->tfsnode_ino;
		switch (current->tfsnode_type) {
		case VBLK:
			cde.d_type = DT_BLK;
			break;
		case VCHR:
			cde.d_type = DT_CHR;
			break;
		case VDIR:
			cde.d_type = DT_DIR;
			break;
		case VFIFO:
			cde.d_type = DT_FIFO;
			break;
		case VLNK:
			cde.d_type = DT_LNK;
			break;
		case VREG:
			cde.d_type = DT_REG;
			break;
		default:
			panic("%s: tarfs_node %p, type %d\n", __func__,
			    current, current->tfsnode_type);
		}
		cde.d_namlen = current->tfsnode_namelen;
		MPASS(tnp->tfsnode_namelen < sizeof(cde.d_name));
		(void)memcpy(cde.d_name, current->tfsnode_name,
		    current->tfsnode_namelen);
		cde.d_name[current->tfsnode_namelen] = '\0';
		cde.d_reclen = GENERIC_DIRSIZ(&cde);

		if (cde.d_reclen > uio->uio_resid) {
			TARFS_DPF(VNODE, "%s: out of space, returning\n",
			    __func__);
			error = (ndirents == 0) ? EINVAL : 0;
			goto done;
		}

		error = uiomove(&cde, cde.d_reclen, uio);
		if (error == 0) {
			ndirents++;
			current = TAILQ_NEXT(current, tfsnode_dirents);
			TARFS_DPF(VNODE,
			    "%s: [%jd] Setting current node to %p\n",
			    __func__, ndirents, current);
		}
	} while (error == 0 && uio->uio_resid > 0 && current != NULL);

done:
	TARFS_DPF(VNODE, "%s: %jd entries written\n", __func__, ndirents);
	TARFS_DPF(VNODE, "%s: Saving cache information\n", __func__);
	if (current == NULL) {
		uio->uio_offset = TARFS_COOKIE_EOF;
		tnp->tfsnode_dir.lastcookie = 0;
		tnp->tfsnode_dir.lasttnp = NULL;
	} else {
		tnp->tfsnode_dir.lastcookie = uio->uio_offset =
		    current->tfsnode_ino;
		tnp->tfsnode_dir.lasttnp = current;
	}

	if (eofflag != NULL) {
		TARFS_DPF(VNODE, "%s: Setting EOF flag\n", __func__);
		*eofflag = (error == 0 && current == NULL);
	}

	/* Update for NFS */
	if (error == 0 && cookies != NULL && ncookies != NULL) {
		off_t idx;

		TARFS_DPF(VNODE, "%s: Updating NFS cookies\n", __func__);
		current = NULL;
		*cookies = malloc(ndirents * sizeof(off_t), M_TEMP, M_WAITOK);
		*ncookies = ndirents;
		for (idx = 0; idx < ndirents; idx++) {
			if (off == TARFS_COOKIE_DOT)
				off = TARFS_COOKIE_DOTDOT;
			else {
				if (off == TARFS_COOKIE_DOTDOT) {
					current = TAILQ_FIRST(
					    &tnp->tfsnode_dir.dirhead);
				} else if (current != NULL) {
					current = TAILQ_NEXT(current,
					    tfsnode_dirents);
				} else {
					current = tarfs_lookup_dir(tnp,
					    off);
					current = TAILQ_NEXT(current,
					    tfsnode_dirents);
				}
				if (current == NULL)
					off = TARFS_COOKIE_EOF;
				else
					off = current->tfsnode_ino;
			}

			TARFS_DPF(VNODE, "%s: [%jd] offset %jd\n", __func__,
			    idx, off);
			(*cookies)[idx] = off;
		}
		MPASS(uio->uio_offset == off);
	}

	return error;
}

static int
tarfs_read(struct vop_read_args *ap)
{
	struct tarfs_mount *tmp;
	struct tarfs_node *tnp;
	struct uio *uiop;
	struct vnode *vp;
	size_t len;
	off_t resid;
	int error;

	uiop = ap->a_uio;
	vp = ap->a_vp;

	if (vp->v_type != VREG)
		return EISDIR;

	if (uiop->uio_offset < 0)
		return EINVAL;

	tnp = VP_TO_TARFS_NODE(vp);
	tmp = tnp->tfsnode_tmp;
	error = 0;

	while ((resid = uiop->uio_resid) > 0) {
		if (tnp->tfsnode_size <= uiop->uio_offset)
			break;
		len = MIN(tnp->tfsnode_size - uiop->uio_offset, resid);
		if (len == 0)
			break;

		error = tarfs_read_file(tmp, tnp, len, uiop);
		if (error != 0 || resid == uiop->uio_resid)
			break;
	}

	return error;
}

static int
tarfs_readlink(struct vop_readlink_args *ap)
{
	struct tarfs_node *tnp;
	struct uio *uiop;
	struct vnode *vp;
	int error;

	TARFS_DPF(VNODE, "%s\n", __func__);

	uiop = ap->a_uio;
	vp = ap->a_vp;

	MPASS(uiop->uio_offset == 0);
	MPASS(vp->v_type == VLNK);

	tnp = VP_TO_TARFS_NODE(vp);
	error = uiomove(tnp->tfsnode_link.name,
	    MIN(tnp->tfsnode_size, uiop->uio_resid), uiop);

	return error;
}

static int
tarfs_reclaim(struct vop_reclaim_args *ap)
{
	struct tarfs_node *tnp;
	struct vnode *vp;

	vp = ap->a_vp;
	tnp = VP_TO_TARFS_NODE(vp);

	vfs_hash_remove(vp);
	vnode_destroy_vobject(vp);
	cache_purge(vp);

	TARFS_NODE_LOCK(tnp);
	tnp->tfsnode_vnode = NULLVP;
	vp->v_data = NULL;
	TARFS_NODE_UNLOCK(tnp);

	return 0;
}

static int
tarfs_print(struct vop_print_args *ap)
{
	struct tarfs_node *tnp;
	struct vnode *vp;

	vp = ap->a_vp;
	tnp = VP_TO_TARFS_NODE(vp);

	printf("tag tarfs, tarfs_node %p, links %d\n",
	    tnp, tnp->tfsnode_nlink);
	printf("\tmode 0%o, owner %d, group %d, size %zd\n",
	    tnp->tfsnode_mode, tnp->tfsnode_uid, tnp->tfsnode_gid,
	    tnp->tfsnode_size);

	if (vp->v_type == VFIFO)
		fifo_printinfo(vp);

	printf("\n");

	return 0;
}

static int
tarfs_vptofh(struct vop_vptofh_args *ap)
{
	struct tarfs_fid *tfp;
	struct tarfs_node *tnp;

	tfp = (struct tarfs_fid *)ap->a_fhp;
	tnp = VP_TO_TARFS_NODE(ap->a_vp);

	tfp->tfsfid_len = sizeof(struct tarfs_fid);
	tfp->tfsfid_ino = tnp->tfsnode_ino;
	tfp->tfsfid_gen = tnp->tfsnode_gen;

	return 0;
}

struct vop_vector tarfs_vnodeops = {
	.vop_default =		&default_vnodeops,
	.vop_lookup =		vfs_cache_lookup,
	.vop_cachedlookup =	tarfs_lookup,
	.vop_access =		tarfs_access,
	.vop_close =		tarfs_close,
	.vop_getattr =		tarfs_getattr,
	.vop_open =		tarfs_open,
	.vop_print =		tarfs_print,
	.vop_read =		tarfs_read,
	.vop_readdir =		tarfs_readdir,
	.vop_readlink =		tarfs_readlink,
	.vop_reclaim =		tarfs_reclaim,
	.vop_vptofh =		tarfs_vptofh,
};

