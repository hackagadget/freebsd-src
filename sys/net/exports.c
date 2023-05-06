/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California. All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All rights reserved.
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

#include "opt_inet.h"
#include "opt_inet6.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/dirent.h>
#include <sys/jail.h>
#include <sys/kernel.h>
#include <sys/malloc.h>
#include <sys/mbuf.h>
#include <sys/mount.h>
#include <sys/mutex.h>
#include <sys/vnode.h>
#include <sys/rmlock.h>
#include <sys/socket.h>
#include <sys/ucred.h>

#include <netinet/in.h>
#include <net/exports.h>
#include <net/radix.h>

#include <rpc/types.h>
#include <rpc/auth.h>

static MALLOC_DEFINE(M_NETADDR, "export_host", "Export host address structure");

#if defined(INET) || defined(INET6)
static struct radix_node_head *exports_create_addrlist_af(
		    struct radix_node_head **prnh, int off);
#endif

/*
 * Network address lookup element
 */
struct netcred {
	struct	radix_node netc_rnodes[2];
	int	netc_exflags;
	struct	ucred *netc_anon;
	int	netc_numsecflavors;
	int	netc_secflavors[MAXSECFLAVORS];
};

/*
 * Network export information
 */
struct netexport {
	struct	netcred ne_defexported;		      /* Default export */
	struct	radix_node_head *ne4;
	struct	radix_node_head *ne6;
};

/*
 * Build hash lists of net addresses and hang them off the mount point.
 * Called by export_set_options() to set up the lists of export addresses.
 */
static int
exports_hang_addrlist(struct mount *mp, struct netexport *nep,
    struct export_args *argp)
{
	struct netcred *np;
	struct radix_node_head *rnh;
	int i;
	struct radix_node *rn;
	struct sockaddr *saddr, *smask = NULL;
#if defined(INET6) || defined(INET)
	int off;
#endif
	int error;

	KASSERT(argp->ex_numsecflavors > 0,
	    ("%s: numsecflavors <= 0", __func__));
	KASSERT(argp->ex_numsecflavors < MAXSECFLAVORS,
	    ("%s: numsecflavors >= MAXSECFLAVORS", __func__));

	/*
	 * XXX: This routine converts from a uid plus gid list
	 * to a `struct ucred' (np->netc_anon).  This
	 * operation is questionable; for example, what should be done
	 * with fields like cr_uidinfo and cr_prison?  Currently, this
	 * routine does not touch them (leaves them as NULL).
	 */
	if (argp->ex_addrlen == 0) {
		if (mp->mnt_flag & MNT_DEFEXPORTED) {
			vfs_mount_error(mp,
			    "MNT_DEFEXPORTED already set for mount %p", mp);
			return (EPERM);
		}
		np = &nep->ne_defexported;
		np->netc_exflags = argp->ex_flags;
		np->netc_anon = crget();
		np->netc_anon->cr_uid = argp->ex_uid;
		crsetgroups(np->netc_anon, argp->ex_ngroups,
		    argp->ex_groups);
		np->netc_anon->cr_prison = &prison0;
		prison_hold(np->netc_anon->cr_prison);
		np->netc_numsecflavors = argp->ex_numsecflavors;
		bcopy(argp->ex_secflavors, np->netc_secflavors,
		    sizeof(np->netc_secflavors));
		MNT_ILOCK(mp);
		mp->mnt_flag |= MNT_DEFEXPORTED;
		MNT_IUNLOCK(mp);
		return (0);
	}

#if MSIZE <= 256
	if (argp->ex_addrlen > MLEN) {
		vfs_mount_error(mp, "ex_addrlen %d is greater than %d",
		    argp->ex_addrlen, MLEN);
		return (EINVAL);
	}
#endif

	i = sizeof(struct netcred) + argp->ex_addrlen + argp->ex_masklen;
	np = (struct netcred *) malloc(i, M_NETADDR, M_WAITOK | M_ZERO);
	saddr = (struct sockaddr *) (np + 1);
	if ((error = copyin(argp->ex_addr, saddr, argp->ex_addrlen)))
		goto out;
	if (saddr->sa_family == AF_UNSPEC || saddr->sa_family > AF_MAX) {
		error = EINVAL;
		vfs_mount_error(mp, "Invalid saddr->sa_family: %d");
		goto out;
	}
	if (saddr->sa_len > argp->ex_addrlen)
		saddr->sa_len = argp->ex_addrlen;
	if (argp->ex_masklen) {
		smask = (struct sockaddr *)((caddr_t)saddr + argp->ex_addrlen);
		error = copyin(argp->ex_mask, smask, argp->ex_masklen);
		if (error)
			goto out;
		if (smask->sa_len > argp->ex_masklen)
			smask->sa_len = argp->ex_masklen;
	}
	rnh = NULL;
	switch (saddr->sa_family) {
#ifdef INET
	case AF_INET:
		if ((rnh = nep->ne4) == NULL) {
			off = offsetof(struct sockaddr_in, sin_addr) << 3;
			rnh = exports_create_addrlist_af(&nep->ne4, off);
		}
		break;
#endif
#ifdef INET6
	case AF_INET6:
		if ((rnh = nep->ne6) == NULL) {
			off = offsetof(struct sockaddr_in6, sin6_addr) << 3;
			rnh = exports_create_addrlist_af(&nep->ne6, off);
		}
		break;
#endif
	}
	if (rnh == NULL) {
		error = ENOBUFS;
		vfs_mount_error(mp, "%s %s %d",
		    "Unable to initialize radix node head ",
		    "for address family", saddr->sa_family);
		goto out;
	}
	RADIX_NODE_HEAD_LOCK(rnh);
	rn = (*rnh->rnh_addaddr)(saddr, smask, &rnh->rh, np->netc_rnodes);
	RADIX_NODE_HEAD_UNLOCK(rnh);
	if (rn == NULL || np != (struct netcred *)rn) {	/* already exists */
		error = EPERM;
		vfs_mount_error(mp,
		    "netcred already exists for given addr/mask");
		goto out;
	}
	np->netc_exflags = argp->ex_flags;
	np->netc_anon = crget();
	np->netc_anon->cr_uid = argp->ex_uid;
	crsetgroups(np->netc_anon, argp->ex_ngroups,
	    argp->ex_groups);
	np->netc_anon->cr_prison = &prison0;
	prison_hold(np->netc_anon->cr_prison);
	np->netc_numsecflavors = argp->ex_numsecflavors;
	bcopy(argp->ex_secflavors, np->netc_secflavors,
	    sizeof(np->netc_secflavors));
	return (0);
out:
	free(np, M_NETADDR);
	return (error);
}

/* Helper for exports_free_addrlist. */
/* ARGSUSED */
static int
exports_free_netcred(struct radix_node *rn, void *w)
{
	struct radix_node_head *rnh = (struct radix_node_head *) w;
	struct ucred *cred;

	(*rnh->rnh_deladdr) (rn->rn_key, rn->rn_mask, &rnh->rh);
	cred = ((struct netcred *)rn)->netc_anon;
	if (cred != NULL)
		crfree(cred);
	free(rn, M_NETADDR);
	return (0);
}

#if defined(INET) || defined(INET6)
static struct radix_node_head *
exports_create_addrlist_af(struct radix_node_head **prnh, int off)
{

	if (rn_inithead((void **)prnh, off) == 0)
		return (NULL);
	RADIX_NODE_HEAD_LOCK_INIT(*prnh);
	return (*prnh);
}
#endif

static void
exports_free_addrlist_af(struct radix_node_head **prnh)
{
	struct radix_node_head *rnh;

	rnh = *prnh;
	RADIX_NODE_HEAD_LOCK(rnh);
	(*rnh->rnh_walktree)(&rnh->rh, exports_free_netcred, rnh);
	RADIX_NODE_HEAD_UNLOCK(rnh);
	RADIX_NODE_HEAD_DESTROY(rnh);
	rn_detachhead((void **)prnh);
	prnh = NULL;
}

/*
 * Free the net address hash lists that are hanging off the mount points.
 */
void
exports_free_addrlist(struct netexport *nep)
{
	struct ucred *cred;

	if (nep->ne4 != NULL)
		exports_free_addrlist_af(&nep->ne4);
	if (nep->ne6 != NULL)
		exports_free_addrlist_af(&nep->ne6);

	cred = nep->ne_defexported.netc_anon;
	if (cred != NULL) {
		crfree(cred);
		nep->ne_defexported.netc_anon = NULL;
	}
}

void
exports_free(struct mount *mp)
{

	if (mp->mnt_export == NULL)
		return;

	exports_free_addrlist(mp->mnt_export);
	free(mp->mnt_export, M_MOUNT);
	mp->mnt_export = NULL;
}

/*
 * Used by the filesystems to determine if a given network address
 * (passed in 'nam') is present in their exports list, returns a pointer
 * to struct netcred so that the filesystem can examine it for
 * access rights (read/write/etc).
 */
struct netcred *
exports_lookup(struct mount *mp, struct sockaddr *nam)
{
	RADIX_NODE_HEAD_RLOCK_TRACKER;
	struct netexport *nep;
	struct netcred *np = NULL;
	struct radix_node_head *rnh;
	struct sockaddr *saddr;

	nep = mp->mnt_export;
	if (nep == NULL)
		return (NULL);
	if ((mp->mnt_flag & MNT_EXPORTED) == 0)
		return (NULL);

	/*
	 * Lookup in the export list
	 */
	if (nam != NULL) {
		saddr = nam;
		rnh = NULL;
		switch (saddr->sa_family) {
		case AF_INET:
			rnh = nep->ne4;
			break;
		case AF_INET6:
			rnh = nep->ne6;
			break;
		}
		if (rnh != NULL) {
			RADIX_NODE_HEAD_RLOCK(rnh);
			np = (struct netcred *) (*rnh->rnh_matchaddr)(saddr, &rnh->rh);
			RADIX_NODE_HEAD_RUNLOCK(rnh);
			if (np != NULL && (np->netc_rnodes->rn_flags & RNF_ROOT) != 0)
				return (NULL);
		}
	}

	/*
	 * If no address match, use the default if it exists.
	 */
	if (np == NULL && mp->mnt_flag & MNT_DEFEXPORTED)
		return (&nep->ne_defexported);

	return (np);
}

/*
 * High level function to manipulate export options on a mount point
 * and the passed in netexport.
 * Struct export_args *argp is the variable used to twiddle options,
 * the structure is described in sys/mount.h
 */
int
exports_set_options(struct mount *mp, struct export_args *argp)
{

	struct netexport *nep;
	int error;

	if ((argp->ex_flags & (MNT_DELEXPORT | MNT_EXPORTED)) == 0)
		return (EINVAL);

	if ((argp->ex_flags & MNT_EXPORTED) != 0 &&
	    (argp->ex_numsecflavors < 0
	    || argp->ex_numsecflavors >= MAXSECFLAVORS))
		return (EINVAL);

	error = 0;
	lockmgr(&mp->mnt_explock, LK_EXCLUSIVE, NULL);
	nep = mp->mnt_export;
	if (argp->ex_flags & MNT_DELEXPORT) {
		if (nep == NULL) {
			error = ENOENT;
			goto out;
		}
		if (mp->mnt_flag & MNT_EXPUBLIC) {
			vfs_setpublicfs(NULL, NULL, NULL);
			MNT_ILOCK(mp);
			mp->mnt_flag &= ~MNT_EXPUBLIC;
			MNT_IUNLOCK(mp);
		}
		exports_free_addrlist(nep);
		mp->mnt_export = NULL;
		free(nep, M_MOUNT);
		nep = NULL;
		MNT_ILOCK(mp);
		mp->mnt_flag &= ~(MNT_EXPORTED | MNT_DEFEXPORTED);
		MNT_IUNLOCK(mp);
	}
	if (argp->ex_flags & MNT_EXPORTED) {
		if (nep == NULL) {
			nep = malloc(sizeof(struct netexport), M_MOUNT, M_WAITOK | M_ZERO);
			mp->mnt_export = nep;
		}
		if (argp->ex_flags & MNT_EXPUBLIC) {
			if ((error = vfs_setpublicfs(mp, nep, argp)) != 0)
				goto out;
			MNT_ILOCK(mp);
			mp->mnt_flag |= MNT_EXPUBLIC;
			MNT_IUNLOCK(mp);
		}
		if (argp->ex_numsecflavors == 0) {
			argp->ex_numsecflavors = 1;
			argp->ex_secflavors[0] = AUTH_SYS;
		}
		if ((error = exports_hang_addrlist(mp, nep, argp)))
			goto out;
		MNT_ILOCK(mp);
		mp->mnt_flag |= MNT_EXPORTED;
		MNT_IUNLOCK(mp);
	}

out:
	lockmgr(&mp->mnt_explock, LK_RELEASE, NULL);
	/*
	 * Once we have executed the exports_set_options() command, we do
	 * not want to keep the "export" option around in the
	 * options list, since that will cause subsequent MNT_UPDATE
	 * calls to fail.  The export information is saved in
	 * mp->mnt_export, so we can safely delete the "export" mount option
	 * here.
	 */
	vfs_deleteopt(mp->mnt_optnew, "export");
	vfs_deleteopt(mp->mnt_opt, "export");
	return (error);
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
exports_check(struct mount *mp, struct sockaddr *nam, uint64_t *extflagsp,
    struct ucred **credanonp, int *numsecflavors, int *secflavors)
{

	struct netcred *np;

	lockmgr(&mp->mnt_explock, LK_SHARED, NULL);
	np = exports_lookup(mp, nam);
	if (np == NULL) {
		lockmgr(&mp->mnt_explock, LK_RELEASE, NULL);
		*credanonp = NULL;
		return (EACCES);
	}
	*extflagsp = np->netc_exflags;
	if ((*credanonp = np->netc_anon) != NULL)
		crhold(*credanonp);
	if (numsecflavors) {
		*numsecflavors = np->netc_numsecflavors;
		KASSERT(*numsecflavors > 0,
		    ("%s: numsecflavors <= 0", __func__));
		KASSERT(*numsecflavors < MAXSECFLAVORS,
		    ("%s: numsecflavors >= MAXSECFLAVORS", __func__));
	}
	if (secflavors && np->netc_numsecflavors > 0)
		memcpy(secflavors, np->netc_secflavors, np->netc_numsecflavors *
		    sizeof(int));
	lockmgr(&mp->mnt_explock, LK_RELEASE, NULL);
	return (0);
}
