# $FreeBSD$
#

.if ${MK_FICL4} != "no"
FICL_VERSION=	4
.else
FICL_VERSION=	3
.endif
.if !defined(FICLDIR) || empty(FICLDIR)
FICLDIR :=	${.PARSEDIR:H:H}/contrib/ficl${FICL_VERSION}
.endif

.PATH: ${FICLDIR}
.if ${MK_FICL4} != "no"
.PATH: ${FICLDIR}/ficlplatform
.endif
.if defined(FICL32)
.PATH: ${FICLDIR}/${MACHINE_CPUARCH:S/amd64/i386/}
.elif ${MACHINE_ARCH} == "mips64" || ${MACHINE_ARCH} == "mips64el"
.PATH: ${FICLDIR}/mips64
.else
.PATH: ${FICLDIR}/${MACHINE_CPUARCH}
.endif

BASE_SRCS.ficl3= dict.c ficl.c fileaccess.c float.c loader.c math64.c \
		prefix.c search.c stack.c tools.c vm.c words.c
BASE_SRCS.ficl4= bit.c callback.c dictionary.c double.c fileaccess.c \
		float.c hash.c loader.c lzcompress.c lzuncompress.c prefix.c \
		primitives.c search.c stack.c system.c tools.c utility.c vm.c \
		word.c freebsd.c
BASE_SRCS=	${BASE_SRCS.ficl${FICL_VERSION}}
SRCS.ficl3=	sysdep.c
SRCS=		${BASE_SRCS} ${SRCS.ficl${FICL_VERSION}} softcore.c
CLEANFILES=	softcore.c testmain testmain.o
.if ${MK_FICL4} != "no"
CLEANFILES+=	makesoftcore
.endif
.if !make(testmain)
CFLAGS+=	-ffreestanding -fPIC
.endif
.if ${MK_FICL4} != "no"
CFLAGS+=	-DFICL_WANT_LZ_SOFTCORE=0
CFLAGS+=	-DFICL_WANT_FLOAT=0
CFLAGS+=	-DFICL_WANT_FILE=0
.endif
.if ${MACHINE_CPUARCH} == "i386" || \
    (${MACHINE_CPUARCH} == "amd64" && defined(FICL32))
CFLAGS+=	-march=i386
CFLAGS.gcc+=	-mpreferred-stack-boundary=2
.endif
.if ${MACHINE_CPUARCH} == "amd64"
CFLAGS+=	-mno-red-zone
.endif
.if ${MACHINE_CPUARCH} == "i386" || ${MACHINE_CPUARCH} == "amd64"
CFLAGS+=	-mno-mmx -mno-3dnow -mno-sse -mno-sse2 -mno-sse3 -msoft-float
.endif
.if ${MACHINE_CPUARCH} == "powerpc" || ${MACHINE_CPUARCH} == "arm" || ${MACHINE_CPUARCH} == "mips"
CFLAGS+=	-msoft-float
.endif
.if ${MACHINE} == "pc98"
CFLAGS+=	-Os -DPC98
.endif
.if ${MACHINE_CPUARCH} == "mips"
CFLAGS+=	-G0 -fno-pic -mno-abicalls
.endif
.if HAVE_PNP
CFLAGS+=	-DHAVE_PNP
.endif
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
.if ${MK_FICL4} != "no"
.PATH: ${FICLDIR}/softcore
.else
.PATH: ${FICLDIR}/softwords
.endif
SOFTWORDS=	softcore.fr ifbrack.fr
.if ${MK_FICL4} != "no"
SOFTWORDS+=	ficl.fr
.endif
SOFTWORDS+=	jhlocal.fr marker.fr freebsd.fr ficllocal.fr
# Optional OO extension softwords
#SOFTWORDS+=	oo.fr classes.fr

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

.if ${MACHINE_CPUARCH} == "amd64" && defined(FICL32)
FICL_CPUARCH=	i386
.else
FICL_CPUARCH=	${MACHINE_CPUARCH}
.endif

CFLAGS+=	-I${FICLDIR}
CFLAGS+=	-I${FICLDIR}/${FICL_CPUARCH}
CFLAGS+=	-I${FICLDIR:H:H}/boot/common

.if ${MK_FICL4} != "no"
makesoftcore: makesoftcore.c lzcompress.c bit.c
	${CC} ${CFLAGS} ${CPPFLAGS} -DTESTMAIN \
	    -DSOFTCORE_OUT=\"${.OBJDIR}/softcore.c\" -o ${.TARGET} ${.ALLSRC}

softcore.c: ${SOFTWORDS} makesoftcore
	${.OBJDIR}/makesoftcore ${.ALLSRC:Nmakesoftcore}
.else
softcore.c: ${SOFTWORDS} softcore.awk
	(cd ${FICLDIR}/softwords; cat ${SOFTWORDS} \
	    | awk -f softcore.awk -v datestamp="`LC_ALL=C date`") > ${.TARGET}
.endif
