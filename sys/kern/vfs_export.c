/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)vfs_subr.c	8.31 (Berkeley) 5/26/95
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD$");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dirent.h>
#include <sys/jail.h>
#include <sys/kernel.h>
#include <sys/kobj.h>
#include <sys/lock.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/mount.h>
#include <sys/netstack.h>
#include <sys/socket.h>
#include <sys/vnode.h>

#include "netstack_if.h"

/*
 * High level function to manipulate export options on a mount point
 * and the passed in netexport.
 * Struct export_args *argp is the variable used to twiddle options,
 * the structure is described in sys/mount.h
 * The do_exjail argument should be true if *mp is in the mountlist
 * and false if not.  It is not in the mountlist for the NFSv4 rootfs
 * fake mount point just used for exports.
 */
int
vfs_export(struct mount *mp, struct export_args *argp, bool do_exjail)
{

	return (NETSTACK_VFS_EXPORT(curnetstack, mp, argp));
}

void
vfs_free_exports(struct mount *mp)
{

	NETSTACK_VFS_FREE_EXPORTS(curnetstack, mp);
}

/*
 * Get rid of credential references for this prison.
 */
void
vfs_exjail_delete(struct prison *pr)
{
	struct mount *mp;
	struct ucred *cr;
	int error, i;

	/*
	 * Since this function is called from prison_cleanup() after
	 * all processes in the prison have exited, the value of
	 * pr_exportcnt can no longer increase.  It is possible for
	 * a dismount of a file system exported within this prison
	 * to be in progress.  In this case, the file system is no
	 * longer in the mountlist and the mnt_exjail will be free'd
	 * by vfs_mount_destroy() at some time.  As such, pr_exportcnt
	 * and, therefore "i", is the upper bound on the number of
	 * mnt_exjail entries to be found by this function.
	 */
	i = atomic_load_int(&pr->pr_exportcnt);
	KASSERT(i >= 0, ("vfs_exjail_delete: pr_exportcnt negative"));
	if (i == 0)
		return;
	mtx_lock(&mountlist_mtx);
tryagain:
	TAILQ_FOREACH(mp, &mountlist, mnt_list) {
		MNT_ILOCK(mp);
		if (mp->mnt_exjail != NULL &&
		    mp->mnt_exjail->cr_prison == pr) {
			MNT_IUNLOCK(mp);
			error = vfs_busy(mp, MBF_MNTLSTLOCK | MBF_NOWAIT);
			if (error != 0) {
				/*
				 * If the vfs_busy() fails, we still want to
				 * get rid of mnt_exjail for two reasons:
				 * - a credential reference will result in
				 *   a prison not being removed
				 * - setting mnt_exjail NULL indicates that
				 *   the exports are no longer valid
				 * The now invalid exports will be deleted
				 * when the file system is dismounted or
				 * the file system is re-exported by mountd.
				 */
				cr = NULL;
				MNT_ILOCK(mp);
				if (mp->mnt_exjail != NULL &&
				    mp->mnt_exjail->cr_prison == pr) {
					cr = mp->mnt_exjail;
					mp->mnt_exjail = NULL;
				}
				MNT_IUNLOCK(mp);
				if (cr != NULL) {
					crfree(cr);
					i--;
				}
				if (i == 0)
					break;
				continue;
			}
			cr = NULL;
			lockmgr(&mp->mnt_explock, LK_EXCLUSIVE, NULL);
			MNT_ILOCK(mp);
			if (mp->mnt_exjail != NULL &&
			    mp->mnt_exjail->cr_prison == pr) {
				cr = mp->mnt_exjail;
				mp->mnt_exjail = NULL;
				mp->mnt_flag &= ~(MNT_EXPORTED | MNT_DEFEXPORTED);
				MNT_IUNLOCK(mp);
				vfs_free_exports(mp);
			} else
				MNT_IUNLOCK(mp);
			lockmgr(&mp->mnt_explock, LK_RELEASE, NULL);
			if (cr != NULL) {
				crfree(cr);
				i--;
			}
			mtx_lock(&mountlist_mtx);
			vfs_unbusy(mp);
			if (i == 0)
				break;
			goto tryagain;
		}
		MNT_IUNLOCK(mp);
	}
	mtx_unlock(&mountlist_mtx);
}

/*
 * Set the publicly exported filesystem (WebNFS). Currently, only
 * one public filesystem is possible in the spec (RFC 2054 and 2055)
 */
int
vfs_setpublicfs(struct mount *mp, struct netexport *nep,
    struct export_args *argp)
{
	int error;
	struct vnode *rvp;
	char *cp;

	/*
	 * mp == NULL -> invalidate the current info, the FS is
	 * no longer exported. May be called from either vfs_export
	 * or unmount, so check if it hasn't already been done.
	 */
	if (mp == NULL) {
		if (nfs_pub.np_valid) {
			nfs_pub.np_valid = 0;
			if (nfs_pub.np_index != NULL) {
				free(nfs_pub.np_index, M_TEMP);
				nfs_pub.np_index = NULL;
			}
		}
		return (0);
	}

	/*
	 * Only one allowed at a time.
	 */
	if (nfs_pub.np_valid != 0 && mp != nfs_pub.np_mount)
		return (EBUSY);

	/*
	 * Get real filehandle for root of exported FS.
	 */
	bzero(&nfs_pub.np_handle, sizeof(nfs_pub.np_handle));
	nfs_pub.np_handle.fh_fsid = mp->mnt_stat.f_fsid;

	if ((error = VFS_ROOT(mp, LK_EXCLUSIVE, &rvp)))
		return (error);

	if ((error = VOP_VPTOFH(rvp, &nfs_pub.np_handle.fh_fid)))
		return (error);

	vput(rvp);

	/*
	 * If an indexfile was specified, pull it in.
	 */
	if (argp->ex_indexfile != NULL) {
		if (nfs_pub.np_index == NULL)
			nfs_pub.np_index = malloc(MAXNAMLEN + 1, M_TEMP,
			    M_WAITOK);
		error = copyinstr(argp->ex_indexfile, nfs_pub.np_index,
		    MAXNAMLEN, (size_t *)0);
		if (!error) {
			/*
			 * Check for illegal filenames.
			 */
			for (cp = nfs_pub.np_index; *cp; cp++) {
				if (*cp == '/') {
					error = EINVAL;
					break;
				}
			}
		}
		if (error) {
			free(nfs_pub.np_index, M_TEMP);
			nfs_pub.np_index = NULL;
			return (error);
		}
	}

	nfs_pub.np_mount = mp;
	nfs_pub.np_valid = 1;
	return (0);
}

/*
 * XXX: This comment comes from the deprecated ufs_check_export()
 * XXX: and may not entirely apply, but lacking something better:
 * This is the generic part of fhtovp called after the underlying
 * filesystem has validated the file handle.
 *
 * Verify that a host should have access to a filesystem.
 */

int
vfs_stdcheckexp(struct mount *mp, struct sockaddr *nam, uint64_t *extflagsp,
    struct ucred **credanonp, int *numsecflavors, int *secflavors)
{

	return (NETSTACK_VFS_STDCHECKEXP(curnetstack, mp, nam, extflagsp,
	    credanonp, numsecflavors, secflavors));
}
