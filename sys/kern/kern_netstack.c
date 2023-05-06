/*-
 * SPDX-License-Identifier: BSD-2-Clause-FreeBSD
 *
 * Copyright (c) 2012-2023, Juniper Networks, Inc.
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

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/kobj.h>
#include <sys/malloc.h>
#include <sys/mount.h>
#include <sys/netstack.h>
#include <sys/proc.h>
#include <sys/protosw.h>
#include <sys/socket.h>
#include <sys/systm.h>

#include "netstack_if.h"

static kobj_method_t netstack_null_methods[] = {
	KOBJMETHOD_END
};
DEFINE_CLASS_0(netstack_null, netstack_null_class, netstack_null_methods, 0);
struct netstack netstack_null;

MALLOC_DEFINE(M_NETSTACK, "netstack", "network stack");

netstack_t curnetstack = &netstack_null;

static void	netstack_init(void *);

int
netstack_register(netstack_t stack)
{

	if (stack == NULL)
		return (EINVAL);

	if (curnetstack != &netstack_null) {
#ifdef INVARIANTS
		printf("%s: stack %s already registered, cannot "
		    "register another\n", __func__,
		    curnetstack->ns_class->name);
#endif
		return (EEXIST);
	}

	curnetstack = stack;
	return (0);
}

netstack_t
netstack_create(netstack_class_t nc)
{
	netstack_t nstack;

	nstack = (netstack_t) kobj_create((kobj_class_t)nc, M_NETSTACK,
	    M_WAITOK);
	if (nstack == NULL)
		return (NULL);
	nstack->ns_class = nc;
	return (nstack);
}

int
netstack_module_handler(module_t mod, int what, void *arg)
{
	struct netstack_module_data *nmd;
	netstack_class_t netstack_class;
	struct netstack *nstack;
	int error;

	nmd = (struct netstack_module_data *)arg;

	switch (what) {
	case MOD_LOAD:
		/* Compile the netstack's methods */
		netstack_class = nmd->nmd_class;
		kobj_class_compile((kobj_class_t) netstack_class);

		/* Create an instance of the netstack and register it */
		nstack = netstack_create(netstack_class);
		if (nstack == NULL) {
			error = ENOMEM;
			break;
		}
		error = netstack_register(nstack);
		break;

	default:
		error = EOPNOTSUPP;
		break;
	}

	return (error);
}

static void
netstack_init(void *arg __unused)
{

	kobj_init((kobj_t) &netstack_null, &netstack_null_class);
}
SYSINIT(netstack, SI_SUB_NETSTACK_PRELINK, SI_ORDER_FIRST, netstack_init, NULL);
