# $FreeBSD$
#

.include <src.opts.mk>

.include "Makefile.inc"

.if ${MACHINE_CPUARCH} == "amd64" && defined(FICL32)
.if !exists(machine)
${SRCS:M*.c:R:S/$/.o/g}: machine

beforedepend ${OBJS}: machine
.endif

machine:
	ln -sf ${.CURDIR:H:H}/i386/include machine

CLEANFILES+=	machine
.endif
