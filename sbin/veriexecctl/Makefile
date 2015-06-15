# $FreeBSD$

PROG=	veriexecctl
MAN=	veriexecctl.8
SRCS=	veriexecctl_parse.y veriexecctl_conf.l veriexecctl.c

CFLAGS+= -I${.CURDIR:H:H}/sys

WARNS?=	3

LIBADD=	l

.include <bsd.prog.mk>
