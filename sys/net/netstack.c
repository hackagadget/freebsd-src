/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2008 Cisco Systems, All rights reserved
 * Copyright (c) 2012-2023 Juniper Networks, Inc.
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
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "opt_inet.h"
#include "opt_inet6.h"

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mount.h>
#include <sys/netstack.h>
#include <sys/proc.h>
#include <sys/protosw.h>
#include <sys/socket.h>

#include <net/exports.h>

#include "netstack_if.h"

static int
netstack_vfs_export(netstack_t nstack, struct mount *mp,
    struct export_args *argp)
{

	return (exports_set_options(mp, argp));
}

static void
netstack_vfs_free_exports(netstack_t nstack __unused, struct mount *mp)
{

	return (exports_free(mp));
}

static int
netstack_vfs_stdcheckexp(netstack_t nstack __unused, struct mount *mp,
    struct sockaddr *nam, uint64_t *extflagsp, struct ucred **credanonp,
    int *numsecflavors, int *secflavors)
{

	return (exports_check(mp, nam, extflagsp, credanonp, numsecflavors,
	    secflavors));
}

static void
netstack_socreate(netstack_t nstack __unused, struct socket *so,
    struct protosw *prp, struct ucred *cred __unused, struct thread *td)
{

	if ((prp->pr_domain->dom_family == PF_INET) ||
	    (prp->pr_domain->dom_family == PF_INET6) ||
	    (prp->pr_domain->dom_family == PF_ROUTE))
		so->so_fibnum = td->td_proc->p_fibnum;
	else
		so->so_fibnum = 0;
}

static int
netstack_get_capabilities(netstack_t nstack __unused)
{

	return (NETSTACK_CAP_ALL);
}

static kobj_method_t netstack_methods[] = {
	KOBJMETHOD(netstack_get_capabilities,	netstack_get_capabilities),
	KOBJMETHOD(netstack_vfs_free_exports,	netstack_vfs_free_exports),
	KOBJMETHOD(netstack_vfs_export,		netstack_vfs_export),
	KOBJMETHOD(netstack_vfs_stdcheckexp,	netstack_vfs_stdcheckexp),

	KOBJMETHOD(netstack_socreate,		netstack_socreate),
	KOBJMETHOD_END
};
NETSTACK_MODULE(freebsd, netstack_methods, SI_ORDER_ANY);
MODULE_VERSION(netstack, 1);
