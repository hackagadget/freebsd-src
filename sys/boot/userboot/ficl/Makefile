# $FreeBSD$
#
.include <bsd.own.mk>
MK_SSP=		no

FICL_VERSION?=	4

.PATH: ${.CURDIR:H:H}/ficl${FICL_VERSION}
.PATH: ${.CURDIR:H:H}/ficl${FICL_VERSION}/${MACHINE_CPUARCH}
.if ${FICL_VERSION} == 4
.PATH: ${.CURDIR:H:H}/ficl${FICL_VERSION}/ficlplatform

CFLAGS+=	-DFICL_WANT_LZ_SOFTCORE=0
.endif

BASE_SRCS.ficl3= dict.c ficl.c fileaccess.c float.c loader.c math64.c \
		prefix.c search.c stack.c tools.c vm.c words.c
BASE_SRCS.ficl4= bit.c callback.c dictionary.c double.c extras.c fileaccess.c \
		float.c hash.c loader.c lzcompress.c lzuncompress.c prefix.c \
		primitives.c search.c stack.c system.c tools.c utility.c vm.c \
		word.c unix.c
BASE_SRCS=	${BASE_SRCS.ficl${FICL_VERSION}}
SRCS.ficl3=	sysdep.c
SRCS=		${BASE_SRCS} ${SRCS.ficl${FICL_VERSION}} softcore.c
CLEANFILES=	softcore.c testmain testmain.o

CWARNFLAGS.loader.c.c += -Wno-implicit-function-declaration

.if HAVE_PNP
CFLAGS+=	-DHAVE_PNP
.endif
.include <bsd.stand.mk>
.ifmake testmain
CFLAGS+=	-DTESTMAIN -D_TESTMAIN
SRCS+=		testmain.c
PROG=		testmain
.include <bsd.prog.mk>
.else
LIB=		ficl
INTERNALLIB=
.include <bsd.lib.mk>
.endif

# Standard softwords
.if ${FICL_VERSION} == 4
.PATH: ${.CURDIR:H:H}/ficl${FICL_VERSION}/softcore
.else
.PATH: ${.CURDIR:H:H}/ficl${FICL_VERSION}/softwords
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

CFLAGS+=	-I${.CURDIR:H:H}/ficl${FICL_VERSION}
CFLAGS+=	-I${.CURDIR:H:H}/ficl${FICL_VERSION}/${MACHINE_CPUARCH}
CFLAGS+=	-I${.CURDIR:H:H}/common

.if ${FICL_VERSION} == 4
makesoftcore: makesoftcore.c lzcompress.c bit.c
	${CC} ${CFLAGS} ${CPPFLAGS} -DSOFTCORE_OUT=\"${.CURDIR}/softcore.c\" \
	    -o ${.TARGET} ${.ALLSRC}

softcore.c: ${SOFTWORDS} makesoftcore
	${.CURDIR}/makesoftcore ${.ALLSRC:Nmakesoftcore}
.else
softcore.c: ${SOFTWORDS} softcore.awk
	(cd ${.CURDIR:H:H}/ficl${FICL_VERSION}/softwords; cat ${SOFTWORDS} \
	    | awk -f softcore.awk -v datestamp="`LC_ALL=C date`") > ${.TARGET}
.endif

#.if ${MACHINE_CPUARCH} == "amd64"
#${SRCS:M*.c:R:S/$/.o/g}: machine
#
#beforedepend ${OBJS}: machine
#
<<<<<<< df497bd192877e5ef5fe9f9cb453927ae5a7cd1f
#machine: .NOMETA
#	ln -sf ${.CURDIR}/../../i386/include machine
=======
#machine:
#	ln -sf ${.CURDIR:H:H}/i386/include machine
>>>>>>> Move ficl 3.x sources to ficl3 directory.
#
#CLEANFILES+=	machine
#.endif
