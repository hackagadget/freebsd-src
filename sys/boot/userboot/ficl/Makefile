# $FreeBSD$
#
.include <src.opts.mk>
MK_SSP=		no

FICL_VERSION?=	4
FICL_SRCDIR=	${.CURDIR:H:H:H}/contrib/ficl${FICL_VERSION}

.PATH: ${FICL_SRCDIR}
.PATH: ${FICL_SRCDIR}/${MACHINE_CPUARCH}
.if ${FICL_VERSION} == 4
.PATH: ${FICL_SRCDIR}/ficlplatform

CFLAGS+=	-DFICL_WANT_LZ_SOFTCORE=0
.endif

BASE_SRCS.ficl3= dict.c ficl.c fileaccess.c float.c loader.c math64.c \
	prefix.c search.c stack.c tools.c vm.c words.c
BASE_SRCS.ficl4= bit.c callback.c dictionary.c double.c extras.c \
	fileaccess.c float.c freebsd.c hash.c loader.c lzcompress.c \
	lzuncompress.c prefix.c primitives.c search.c stack.c system.c \
	tools.c utility.c vm.c word.c
BASE_SRCS=	${BASE_SRCS.ficl${FICL_VERSION}}
SRCS.ficl3=	sysdep.c
SRCS=		${BASE_SRCS} ${SRCS.ficl${FICL_VERSION}} softcore.c
CLEANFILES=	makesoftcore softcore.c \
		testmain testmain.debug testmain.full testmain.o

CWARNFLAGS.loader.c.c += -Wno-implicit-function-declaration

.if HAVE_PNP
CFLAGS+=	-DHAVE_PNP
.endif
.ifmake testmain
CFLAGS+=	-DTESTMAIN -D_TESTMAIN
SRCS+=		testmain.c
PROG=		testmain
LIBADD=	m
.include <bsd.prog.mk>
.else
LIB=		ficl
INTERNALLIB=
.include <bsd.stand.mk>
.include <bsd.lib.mk>
.endif

# Standard softwords
.if ${FICL_VERSION} == 4
.PATH: ${FICL_SRCDIR}/softcore
.else
.PATH: ${FICL_SRCDIR}/softwords
.endif
SOFTWORDS=	softcore.fr ifbrack.fr
.if ${FICL_VERSION} == 4
SOFTWORDS+=	ficl.fr
.endif
SOFTWORDS+=	jhlocal.fr marker.fr freebsd.fr ficllocal.fr
# Optional OO extension softwords
#SOFTWORDS+=	oo.fr classes.fr

#.if ${MACHINE_CPUARCH} == "amd64"
#CFLAGS+=	-m32 -I.
#.endif

.if ${MACHINE_ARCH} == "powerpc64"
CFLAGS+=	-m32 -mcpu=powerpc -I.
.endif

CFLAGS+=	-I${FICL_SRCDIR}
CFLAGS+=	-I${FICL_SRCDIR}/${MACHINE_CPUARCH}
CFLAGS+=	-I${.CURDIR:H:H}/common
.if ${FICL_VERSION} == 4
makesoftcore: makesoftcore.c lzcompress.c bit.c
	${CC} ${CFLAGS:N-D_STANDALONE} ${CPPFLAGS} \
	    -DSOFTCORE_OUT=\"${.CURDIR}/softcore.c\" -o ${.TARGET} ${.ALLSRC}

softcore.c: ${SOFTWORDS} makesoftcore
	${.CURDIR}/makesoftcore ${.ALLSRC:Nmakesoftcore}
.else
softcore.c: ${SOFTWORDS} softcore.awk
	(cd ${FICL_SRCDIR}/softwords; cat ${SOFTWORDS} \
	    | awk -f softcore.awk -v datestamp="`LC_ALL=C date`") > ${.TARGET}
.endif

#.if ${MACHINE_CPUARCH} == "amd64"
#${SRCS:M*.c:R:S/$/.o/g}: machine
#
#beforedepend ${OBJS}: machine
#
#machine: .NOMETA
#	ln -sf ${.CURDIR}/../../i386/include machine
#
#CLEANFILES+=	machine
#.endif
