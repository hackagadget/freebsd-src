#if !defined(__FICL_FREEBSD_H__)
#define __FICL_FREEBSD_H__

#ifdef _STANDALONE
#include <stand.h>
#else
#include <stdint.h>
#include <unistd.h>
#endif

#define FICL_ANSI (1)
#ifdef _STANDALONE
#define FICL_WANT_FILE (0)
#else
#define FICL_WANT_EXTRAS (1)
#endif
#define FICL_WANT_PLATFORM (1)

#define FICL_PLATFORM_OS              "FreeBSD"
#if defined(__i386__)
#define FICL_PLATFORM_ARCHITECTURE    "i386"
#elif defined(__amd64__)
#define FICL_PLATFORM_ARCHITECTURE    "amd64"
#elif defined(__arm__)
#define FICL_PLATFORM_ARCHITECTURE    "arm"
#else
#define FICL_PLATFORM_ARCHITECTURE    "unknown"
#endif

#define FICL_PLATFORM_BASIC_TYPES     (1)
#if defined(__amd64__)
#define FICL_PLATFORM_ALIGNMENT       (8)
#else
#define FICL_PLATFORM_ALIGNMENT       (4)
#endif
#define FICL_PLATFORM_INLINE          inline

#define FICL_PLATFORM_HAS_FTRUNCATE   (1)
#if defined(__amd64__)
#define FICL_PLATFORM_HAS_2INTEGER    (0)
#else
#define FICL_PLATFORM_HAS_2INTEGER    (1)
#endif

typedef int8_t ficlInteger8;
typedef uint8_t ficlUnsigned8;
typedef int16_t ficlInteger16;
typedef uint16_t ficlUnsigned16;
typedef int32_t ficlInteger32;
typedef uint32_t ficlUnsigned32;
typedef int64_t ficlInteger64;
typedef uint64_t ficlUnsigned64;

#if defined(__amd64__)
typedef ficlInteger64 ficlInteger;
typedef ficlUnsigned64 ficlUnsigned;
#else /* default */
typedef intptr_t ficlInteger;
typedef uintptr_t ficlUnsigned;
#endif
typedef float ficlFloat;

#if defined(FICL_PLATFORM_HAS_2INTEGER) && FICL_PLATFORM_HAS_2INTEGER
typedef ficlInteger64 ficl2Integer;
typedef ficlUnsigned64 ficl2Unsigned;
#endif

#endif /* !__FICL_FREEBSD_H__ */
