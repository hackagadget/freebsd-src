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

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/fcntl.h>
#include <sys/libkern.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mount.h>
#include <sys/namei.h>
#include <sys/proc.h>
#include <sys/queue.h>
#include <sys/sysctl.h>
#include <sys/vnode.h>

#include <fs/tarfs/tarfs.h>
#include <fs/tarfs/tarfs_dbg.h>

MALLOC_DEFINE(M_TARFSNAME, "tarfs name", "tarfs file names");

SYSCTL_NODE(_vfs, OID_AUTO, tarfs, CTLFLAG_RW, 0, "Tar filesystem");

#ifdef	TARFS_DEBUG
int tarfs_debug;
SYSCTL_INT(_vfs_tarfs, OID_AUTO, debug, CTLFLAG_RW, &tarfs_debug, 0,
    "Tar filesystem debug category mask");
#endif	/* TARFS_DEBUG */

static void
tarfs_dump_tree_internal(struct tarfs_node *tnp, int indent)
{
	struct tarfs_node *current;
	const char *name;

	if (tnp->tfsnode_type != VDIR)
		return;

	TAILQ_FOREACH(current, &tnp->tfsnode_dir.dirhead, tfsnode_dirents) {
		if (current->tfsnode_name == NULL)
			name = "<<root>>";
		else
			name = current->tfsnode_name;
		printf("%*s%s\n", indent * 4, "", name);
		if (current->tfsnode_type == VDIR)
			tarfs_dump_tree_internal(current, indent + 1);
	}
}

void
tarfs_dump_tree(struct tarfs_node *tnp)
{
	const char *name;

	if (tnp == NULL)
		return;

	if (tnp->tfsnode_name == NULL)
		name = "<<root>>";
	else
		name = tnp->tfsnode_name;
	printf("%s\n", name);

	tarfs_dump_tree_internal(tnp, 1);
}

void
tarfs_print_node(struct tarfs_node *tnp)
{

	if (tnp == NULL)
		return;

	printf("%s: node %p\n", __func__, tnp);
	printf("\ttfsnode_vnode %p\n", tnp->tfsnode_vnode);
	printf("\ttfsnode_tmp %p\n", tnp->tfsnode_tmp);
	printf("\ttfsnode_type %d\n", tnp->tfsnode_type);
	printf("\ttfsnode_ino %ju\n", (uintmax_t)tnp->tfsnode_ino);
	printf("\ttfsnode_size %zu\n", tnp->tfsnode_size);
	printf("\ttfsnode_nblocks %zu\n", tnp->tfsnode_nblocks);
	printf("\ttfsnode_name %s\n",
	    (tnp->tfsnode_name == NULL) ? "<<root>>" : tnp->tfsnode_name);
	printf("\ttfsnode_namelen %zu\n", tnp->tfsnode_namelen);
	printf("\ttfsnode_uid %d\n", tnp->tfsnode_uid);
	printf("\ttfsnode_gid %d\n", tnp->tfsnode_gid);
	printf("\ttfsnode_mode o%o\n", tnp->tfsnode_mode);
	printf("\ttfsnode_flags %d\n", tnp->tfsnode_flags);
	printf("\ttfsnode_nlink %ju\n", (uintmax_t)tnp->tfsnode_nlink);
	printf("\ttfsnode_atime %d\n", (int)tnp->tfsnode_atime.tv_sec);
	printf("\ttfsnode_mtime %d\n", (int)tnp->tfsnode_mtime.tv_sec);
	printf("\ttfsnode_ctime %d\n", (int)tnp->tfsnode_ctime.tv_sec);
	printf("\ttfsnode_birthtime %d\n", (int)tnp->tfsnode_birthtime.tv_sec);
	printf("\ttfsnode_gen %lu\n", tnp->tfsnode_gen);
	printf("\ttfsnode_parent %p\n", tnp->tfsnode_parent);

	switch (tnp->tfsnode_type) {
	case VDIR:
		printf("\ttfsnode_dir.lastcookie %jd\n",
		    tnp->tfsnode_dir.lastcookie);
		printf("\ttfsnode_dir.lasttnp %p\n", tnp->tfsnode_dir.lasttnp);
		break;
	case VBLK:
	case VCHR:
		printf("\ttfsnode_rdev %ju\n", (uintmax_t)tnp->tfsnode_rdev);
		break;
	default:
		break;
	}
}

struct tarfs_node *
tarfs_lookup_node(struct tarfs_node *tnp, struct tarfs_node *f,
    struct componentname *cnp)
{
	boolean_t found;
	struct tarfs_node *entry;

	TARFS_DPF(LOOKUP, "%s: name: %.*s\n", __func__, (int)cnp->cn_namelen,
	    cnp->cn_nameptr);

	found = false;
	TAILQ_FOREACH(entry, &tnp->tfsnode_dir.dirhead, tfsnode_dirents) {
		if (f != NULL && entry != f)
			continue;

		if (entry->tfsnode_namelen == cnp->cn_namelen &&
		    bcmp(entry->tfsnode_name, cnp->cn_nameptr,
		    entry->tfsnode_namelen) == 0) {
			found = 1;
			break;
		}
	}

	TARFS_DPF_IFF(LOOKUP, found, "%s: found tarfs_node %p\n", __func__,
	    entry);
	TARFS_DPF_IFF(LOOKUP, !found, "%s: no match found\n", __func__);

	return found ? entry : NULL;
}

struct tarfs_node *
tarfs_lookup_dir(struct tarfs_node *tnp, off_t cookie)
{
	struct tarfs_node *current;

	TARFS_DPF(LOOKUP, "%s: tarfs_node %p, cookie %jd\n", __func__, tnp,
	    cookie);
	TARFS_DPF(LOOKUP, "%s: name: %s\n", __func__,
	    (tnp->tfsnode_name == NULL) ? "<<root>>" : tnp->tfsnode_name);

	if (cookie == tnp->tfsnode_dir.lastcookie &&
	    tnp->tfsnode_dir.lasttnp != NULL) {
		TARFS_DPF(LOOKUP, "%s: Using cached entry: tarfs_node %p, "
		    "cookie %jd\n", __func__, tnp->tfsnode_dir.lasttnp,
		    tnp->tfsnode_dir.lastcookie);
		return tnp->tfsnode_dir.lasttnp;
	}

	TAILQ_FOREACH(current, &tnp->tfsnode_dir.dirhead, tfsnode_dirents) {
		TARFS_DPF(LOOKUP, "%s: tarfs_node %p, current %p, ino %ju\n",
		    __func__, tnp, current,
		    (uintmax_t)current->tfsnode_ino);
		TARFS_DPF_IFF(LOOKUP, current->tfsnode_name != NULL,
		    "%s: name: %s\n", __func__, current->tfsnode_name);
		if (current->tfsnode_ino == cookie) {
			TARFS_DPF(LOOKUP, "%s: Found entry: tarfs_node %p, "
			    "cookie %ju\n", __func__, current,
			    (uintmax_t)current->tfsnode_ino);
			break;
		}
	}

	return current;
}

int
tarfs_alloc_node(struct tarfs_mount *tmp, const char *name, size_t namelen,
    enum vtype type, off_t off, size_t sz, time_t mtime, uid_t uid, gid_t gid,
    mode_t mode, const char *linkname, dev_t rdev, struct tarfs_node *parent,
    struct tarfs_node **retnode)
{
	struct tarfs_node *tnp;

	TARFS_DPF(ALLOC, "%s: %.*s\n", __func__, (int)namelen, name);

	tnp = malloc(sizeof(struct tarfs_node), M_TARFSNODE, M_WAITOK | M_ZERO);
	mtx_init(&tnp->tfsnode_lock, "tarfs node lock", NULL, MTX_DEF);
	tnp->tfsnode_gen = arc4random();
	tnp->tfsnode_tmp = tmp;
	if (namelen > 0) {
		tnp->tfsnode_name = malloc(namelen + 1, M_TARFSNAME, M_WAITOK);
		tnp->tfsnode_namelen = namelen;
		memcpy(tnp->tfsnode_name, name, namelen);
		tnp->tfsnode_name[namelen] = '\0';
	}
	tnp->tfsnode_type = type;
	tnp->tfsnode_uid = uid;
	tnp->tfsnode_gid = gid;
	tnp->tfsnode_mode = mode;
	vfs_timestamp(&tnp->tfsnode_atime);
	tnp->tfsnode_mtime.tv_sec = mtime;
	tnp->tfsnode_birthtime = tnp->tfsnode_atime;
	tnp->tfsnode_ctime = tnp->tfsnode_mtime;
	tnp->tfsnode_ino = alloc_unr(tmp->tfsmnt_ino_unr);
	tnp->tfsnode_offset = off;
	tnp->tfsnode_size = sz;
	tnp->tfsnode_nblocks = TARFS_SZ2BLKS(sz);
	switch (type) {
	case VDIR:
		MPASS(parent != tnp);
		MPASS(parent != NULL || tmp->tfsmnt_root == NULL);
		TAILQ_INIT(&tnp->tfsnode_dir.dirhead);
		tnp->tfsnode_nlink++;
		if (parent == NULL) {
			tnp->tfsnode_ino = TARFS_ROOTINO;
		} else {
			TARFS_NODE_LOCK(parent);
			parent->tfsnode_nlink++;
			TARFS_NODE_UNLOCK(parent);
		}
		break;
	case VLNK:
		tnp->tfsnode_link.name = malloc(sz + 1, M_TARFSNAME,
		    M_WAITOK);
		tnp->tfsnode_link.namelen = sz;
		memcpy(tnp->tfsnode_link.name, linkname, sz);
		tnp->tfsnode_link.name[sz] = '\0';
		break;
	case VREG:
	case VFIFO:
		/* Nothing extra to do */
		break;
	case VBLK:
	case VCHR:
		tnp->tfsnode_rdev = rdev;
		break;
	default:
		panic("%s: type %d not allowed", __func__, type);
	}
	if (parent) {
		MPASS(parent->tfsnode_type == VDIR);
		TARFS_NODE_LOCK(parent);
		TAILQ_INSERT_TAIL(&parent->tfsnode_dir.dirhead, tnp,
		    tfsnode_dirents);
		parent->tfsnode_size += sizeof(struct tarfs_node);
		tnp->tfsnode_parent = parent;
		TARFS_NODE_UNLOCK(parent);
	} else {
		tnp->tfsnode_parent = tnp;
	}

	TARFS_ALLNODES_LOCK(tmp);
	TAILQ_INSERT_TAIL(&tmp->tfsmnt_allnodes, tnp, tfsnode_entries);
	TARFS_ALLNODES_UNLOCK(tmp);

	*retnode = tnp;
	return 0;
}

void
tarfs_free_node(struct tarfs_node *tnp)
{
	struct tarfs_mount *tmp;

	MPASS(tnp != NULL);
	tmp = tnp->tfsnode_tmp;

	switch (tnp->tfsnode_type) {
	case VLNK:
		if (tnp->tfsnode_link.name)
			free(tnp->tfsnode_link.name, M_TARFSNAME);
		break;
	default:
		break;
	}
	if (tnp->tfsnode_name != NULL)
		free(tnp->tfsnode_name, M_TARFSNAME);
	free_unr(tmp->tfsmnt_ino_unr, tnp->tfsnode_ino);
	free(tnp, M_TARFSNODE);
}

int
tarfs_read_file(struct tarfs_mount *tmp, struct tarfs_node *tnp, size_t len,
    struct uio *uiop)
{
	struct uio auio;
	int error;

	memcpy(&auio, uiop, sizeof(struct uio));
	auio.uio_offset = tnp->tfsnode_offset + uiop->uio_offset;
	auio.uio_resid = len;
	error = VOP_READ(tmp->tfsmnt_vp, &auio, IO_DIRECT, curthread->td_ucred);
	if (error == 0) {
		uiop->uio_offset += (len - auio.uio_resid);
		uiop->uio_resid -= (len - auio.uio_resid);
	}

	return error;
}

