# $FreeBSD$

LIBFDTDIR=	${.CURDIR}/../../contrib/dtc/libfdt

LIB=	fdt

.PATH:	${LIBFDTDIR}

SRCS=	fdt.c			\
	fdt_empty_tree.c	\
	fdt_ro.c		\
	fdt_rw.c		\
	fdt_strerror.c		\
	fdt_sw.c		\
	fdt_wip.c

INCS=	fdt.h			\
	libfdt.h		\
	libfdt_env.h

CFLAGS+=	-I${LIBFDTDIR}

WARNS?=	2

MAN=

.include <bsd.lib.mk>
