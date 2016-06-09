# $FreeBSD$

TARFS_DEBUG = yes

.PATH: ${.CURDIR}/../../fs/tarfs

KMOD=	tarfs
SRCS=	vnode_if.h \
	tarfs_subr.c \
	tarfs_vnops.c \
	tarfs_vfsops.c

CFLAGS+=	-DINVARIANT_SUPPORT
CFLAGS+=	-DINVARIANTS
.ifdef TARFS_DEBUG
CFLAGS+=	-DTARFS_DEBUG
.endif

.include <bsd.kmod.mk>

