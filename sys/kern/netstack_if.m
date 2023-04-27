#-
# SPDX-License-Identifier: BSD-2-Clause-FreeBSD
#
# Copyright (c) 2012-2023 Juniper Networks, Inc.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
# IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
# OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
# IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
# NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

#include <sys/mount.h>
#include <sys/netstack.h>
#include <sys/protosw.h>
#include <sys/proc.h>
#include <sys/socket.h>
#include <sys/sockopt.h>

INTERFACE netstack;

#
# Default implementations of some methods.
#
CODE {
	static int
	null_get_capabilities(netstack_t nstack)
	{
		return (0);
	}

	static bool
	null_soopt_validfib(netstack_t nstack, struct sockopt *sopt, int fibnum)
	{
		return (fibnum == 0);
	}
};

METHOD int get_capabilities {
    netstack_t		nstack;
} DEFAULT null_get_capabilities;

METHOD void socreate {
    netstack_t		nstack;
    struct socket*	so;
    struct protosw*	prp;
    struct ucred*	cred;
    struct thread*	td;
};

METHOD bool soopt_validfib {
    netstack_t		nstack;
    struct sockopt*	sopt;
    int			fibnum;
};

METHOD int vfs_export {
    netstack_t		nstack;
    struct mount*	mp;
    struct export_args*	argp;
};

METHOD void vfs_free_exports {
    netstack_t		nstack;
    struct mount*	mp;
};

METHOD int vfs_stdcheckexp {
    netstack_t		nstack;
    struct mount*	mp;
    struct sockaddr*	nam;
    uint64_t*		extflagsp;
    struct ucred**	credanonp;
    int*		numsecflavors;
    int*		secflavors;
};
