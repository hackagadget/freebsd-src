# $FreeBSD$
#

FICLDIR?=	${.CURDIR}

.PATH: ${FICLDIR}/ficlplatform
BASE_SRCS=	bit.c callback.c dictionary.c double.c extras.c fileaccess.c \
		float.c hash.c loader.c lzcompress.c lzuncompress.c prefix.c \
		primitives.c search.c stack.c system.c tools.c utility.c vm.c \
		word.c unix.c

SRCS=		${BASE_SRCS} softcore.c
CLEANFILES=	softcore.c testmain testmain.o
CFLAGS+=	-ffreestanding
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
.PATH: ${FICLDIR}/softcore
SOFTWORDS=	softcore.fr ifbrack.fr ficl.fr jhlocal.fr marker.fr freebsd.fr \
		ficllocal.fr
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

CFLAGS+=	-I${FICLDIR} -I${FICLDIR}/${FICL_CPUARCH} \
		-I${FICLDIR}/../common

makesoftcore: makesoftcore.c lzcompress.c bit.c
	${CC} ${CFLAGS} ${CPPFLAGS} -DSOFTCORE_OUT=\"${.CURDIR}/softcore.c\" \
	    -o ${.TARGET} ${.ALLSRC}

softcore.c: ${SOFTWORDS} makesoftcore
	${.CURDIR}/makesoftcore ${.ALLSRC:Nmakesoftcore}

.if ${MACHINE_CPUARCH} == "amd64" && defined(FICL32)
.if !exists(machine)
${SRCS:M*.c:R:S/$/.o/g}: machine

beforedepend ${OBJS}: machine
.endif

machine:
	ln -sf ${.CURDIR}/../../i386/include machine

CLEANFILES+=	machine
.endif
