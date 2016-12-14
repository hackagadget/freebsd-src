# $FreeBSD$

# Common flags to build FICL related files

.include "defs.mk"

.if ${MK_FICL4} != "no"
FICL_VERSION=   4
.else
FICL_VERSION=   3
.endif
FICLSRC?=	${SRCTOP}/sys/contrib/ficl${FICL_VERSION}

.if ${MACHINE_CPUARCH} == "amd64" && defined(FICL32)
FICL_CPUARCH=	i386
.elif ${MACHINE_ARCH:Mmips64*} != ""
FICL_CPUARCH=	mips64
.else
FICL_CPUARCH=	${MACHINE_CPUARCH}
.endif

.PATH: ${FICLSRC} ${FICLSRC}/${FICL_CPUARCH}

.if ${MK_FICL4} != "no"
CFLAGS+=        -DFICL_WANT_LZ_SOFTCORE=0
CFLAGS+=        -DFICL_WANT_FLOAT=0
CFLAGS+=	-D_STANDALONE
.endif

.if ${MACHINE_CPUARCH} == "amd64"
.if defined(FICL32)
CFLAGS+=	-m32 -I.
.else
CFLAGS+=	-fPIC
.endif
.endif

.if ${MACHINE_ARCH} == "powerpc64"
CFLAGS+=	-m32 -mcpu=powerpc -I.
.endif

CFLAGS+=	-I${FICLSRC} -I${FICLSRC}/${FICL_CPUARCH} -I${LDRSRC}
CFLAGS+=	-DBOOT_FORTH
CFLAGS+=	-DBF_DICTSIZE=15000

.if ${MACHINE_CPUARCH} == "amd64" && defined(FICL32)
.if !exists(machine)
${SRCS:M*.c:R:S/$/.o/g}: machine

beforedepend ${OBJS}: machine
.endif

machine: .NOMETA
	ln -sf ${.CURDIR}/../../i386/include machine

CLEANFILES+=	machine
.endif
