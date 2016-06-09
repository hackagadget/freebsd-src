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

#ifndef	_FS_TARFS_TARFS_H_
#define	_FS_TARFS_TARFS_H_

#include <sys/types.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mount.h>
#include <sys/mutex.h>
#include <sys/queue.h>

#ifndef _KERNEL
#error Should only be included by kernel
#endif

MALLOC_DECLARE(M_TARFSMNT);
MALLOC_DECLARE(M_TARFSNODE);
MALLOC_DECLARE(M_TARFSNAME);

struct componentname;
struct mount;
struct vnode;

/*
 * Internal representation of a tarfs file system node.
 */
struct tarfs_node {
	TAILQ_ENTRY(tarfs_node)	tfsnode_entries;
	TAILQ_ENTRY(tarfs_node)	tfsnode_dirents;

	struct mtx		tfsnode_lock;

	struct vnode *		tfsnode_vnode;
	struct tarfs_mount *	tfsnode_tmp;
	enum vtype		tfsnode_type;
	ino_t			tfsnode_ino;
	off_t			tfsnode_offset;
	size_t			tfsnode_size;
	size_t			tfsnode_nblocks;
	char *			tfsnode_name;
	size_t			tfsnode_namelen;

	/* Node attributes */
	uid_t			tfsnode_uid;
	gid_t			tfsnode_gid;
	mode_t			tfsnode_mode;
	int			tfsnode_flags;
	nlink_t			tfsnode_nlink;
	struct timespec		tfsnode_atime;
	struct timespec		tfsnode_mtime;
	struct timespec		tfsnode_ctime;
	struct timespec		tfsnode_birthtime;
	unsigned long		tfsnode_gen;

	struct tarfs_node *	tfsnode_parent;
	union {
		/* VDIR */
		struct {
			TAILQ_HEAD(, tarfs_node)	dirhead;
			off_t				lastcookie;
			struct tarfs_node *		lasttnp;
		} tfsnode_dir;

		/* VLNK */
		struct {
			char *				name;
			size_t				namelen;
		} tfsnode_link;

		/* VBLK or VCHR */
		dev_t		tfsnode_rdev;
	} tfsnode_spec;
};

#define	tfsnode_dir	tfsnode_spec.tfsnode_dir
#define	tfsnode_link	tfsnode_spec.tfsnode_link
#define	tfsnode_rdev	tfsnode_spec.tfsnode_rdev

/*
 * Internal representation of a tarfs mount point.
 */
struct tarfs_mount {
	TAILQ_HEAD(, tarfs_node) tfsmnt_allnodes;
	struct mtx		tfsmnt_allnode_lock;

	struct tarfs_node *	tfsmnt_root;
	struct vnode *		tfsmnt_vp;
	struct mount *		tfsmnt_vfs;
	ino_t			tfsmnt_ino;
	struct unrhdr *		tfsmnt_ino_unr;
	size_t			tfsmnt_nblocks;
	size_t			tfsmnt_nfiles;

	void *			tfsmnt_utf8toa;
};

struct tarfs_fid {
	u_short			tfsfid_len;	/* length of data in bytes */
	u_short			tfsfid_data0;	/* force alignment */
	ino_t			tfsfid_ino;
	unsigned long		tfsfid_gen;
};

#define	TARFS_NODE_LOCK(tnp) \
	mtx_lock(&(tnp)->tfsnode_lock)
#define	TARFS_NODE_UNLOCK(tnp) \
	mtx_unlock(&(tnp)->tfsnode_lock)
#define	TARFS_ALLNODES_LOCK(tnp) \
	mtx_lock(&(tmp)->tfsmnt_allnode_lock)
#define	TARFS_ALLNODES_UNLOCK(tnp) \
	mtx_unlock(&(tmp)->tfsmnt_allnode_lock)

#define	TARFS_BLOCKSIZE	512
#define	TARFS_SZ2BLKS(sz)	(((sz) + TARFS_BLOCKSIZE - 1) / TARFS_BLOCKSIZE)
#define	TARFS_ROOTINO		((ino_t)3)

#define	TARFS_COOKIE_DOT	0
#define	TARFS_COOKIE_DOTDOT	1
#define	TARFS_COOKIE_EOF	2

extern struct vop_vector tarfs_vnodeops;

static inline
struct tarfs_mount *
VFS_TO_TARFS(struct mount *mp)
{
	struct tarfs_mount *tmp;

	MPASS(mp != NULL && mp->mnt_data != NULL);
	tmp = (struct tarfs_mount *) mp->mnt_data;
	return tmp;
}

static inline
struct tarfs_node *
VP_TO_TARFS_NODE(struct vnode *vp)
{
	struct tarfs_node *node;

	MPASS(vp != NULL && vp->v_data != NULL);
	node = (struct tarfs_node *)vp->v_data;
	return node;
}

int	tarfs_alloc_node(struct tarfs_mount *tmp, const char *name,
	    size_t namelen, enum vtype type, off_t off, size_t sz,
	    time_t mtime, uid_t uid, gid_t gid, mode_t mode,
	    const char *linkname, dev_t rdev, struct tarfs_node *parent,
	    struct tarfs_node **node);
void	tarfs_dump_tree(struct tarfs_node *tnp);
void	tarfs_free_node(struct tarfs_node *tnp);
struct tarfs_node *
	tarfs_lookup_dir(struct tarfs_node *tnp, off_t cookie);
struct tarfs_node *
	tarfs_lookup_node(struct tarfs_node *tnp, struct tarfs_node *f,
	    struct componentname *cnp);
void	tarfs_print_node(struct tarfs_node *tnp);
int	tarfs_read_file(struct tarfs_mount *tmp, struct tarfs_node *tnp,
	    size_t len, struct uio *uiop);

#endif	/* _FS_TARFS_TARFS_H_ */

