/*-
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
 *
 * $FreeBSD$
 */

#ifndef _SYS_NETSTACK_H_
#define _SYS_NETSTACK_H_

#ifdef _KERNEL

#include <sys/kobj.h>
#include <sys/mount.h>
#include <sys/module.h>
#include <sys/protosw.h>

typedef struct netstack *netstack_t;
typedef kobj_class_t netstack_class_t;

struct netstack {
	KOBJ_FIELDS;

	netstack_class_t	ns_class;
};

struct netstack_module_data {
	const char		*nmd_stackname;
	netstack_class_t	nmd_class;
};

extern netstack_t curnetstack;

/* Network stack capabilities */
#define	NETSTACK_CAP_LRO	(1<<0)	/* Large Receive Offload */
#define	NETSTACK_CAP_MQ		(1<<1)	/* Multiqueue */
#define	NETSTACK_CAP_ALL	(NETSTACK_CAP_LRO | NETSTACK_CAP_MQ)

int		netstack_module_handler(struct module *, int, void *);
int		netstack_register(netstack_t);
netstack_t	netstack_create(netstack_class_t);

#include "netstack_if.h"

#define	NETSTACK_MODULE(name, methods, order)				\
	DEFINE_CLASS_0(name, name##_netstack_class, methods,		\
	    sizeof(struct netstack));					\
	static struct netstack_module_data name##_netstack_mod_data = {	\
		#name,							\
		(netstack_class_t) & name##_netstack_class		\
	};								\
	static moduledata_t netstack_##name##_mod = {			\
		"netstack_" #name,					\
		netstack_module_handler,				\
		& name##_netstack_mod_data				\
	};								\
	DECLARE_MODULE(netstack_##name, netstack_##name##_mod,		\
	    SI_SUB_NETSTACK, order)

#endif

#endif	/* _SYS_NETSTACK_H_ */
