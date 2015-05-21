# 1 "float.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 308 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "float.c" 2
# 44 "float.c"
# 1 "/usr/include/stdlib.h" 1 3 4
# 36 "/usr/include/stdlib.h" 3 4
# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 37 "/usr/include/stdlib.h" 2 3 4
# 1 "/usr/include/sys/_null.h" 1 3 4
# 38 "/usr/include/stdlib.h" 2 3 4
# 1 "/usr/include/sys/_types.h" 1 3 4
# 33 "/usr/include/sys/_types.h" 3 4
# 1 "/usr/include/machine/_types.h" 1 3 4





# 1 "/usr/include/x86/_types.h" 1 3 4
# 51 "/usr/include/x86/_types.h" 3 4
typedef signed char __int8_t;
typedef unsigned char __uint8_t;
typedef short __int16_t;
typedef unsigned short __uint16_t;
typedef int __int32_t;
typedef unsigned int __uint32_t;

typedef long __int64_t;
typedef unsigned long __uint64_t;
# 77 "/usr/include/x86/_types.h" 3 4
typedef __int32_t __clock_t;
typedef __int64_t __critical_t;
typedef double __double_t;
typedef float __float_t;
typedef __int64_t __intfptr_t;
typedef __int64_t __intptr_t;
# 91 "/usr/include/x86/_types.h" 3 4
typedef __int64_t __intmax_t;
typedef __int32_t __int_fast8_t;
typedef __int32_t __int_fast16_t;
typedef __int32_t __int_fast32_t;
typedef __int64_t __int_fast64_t;
typedef __int8_t __int_least8_t;
typedef __int16_t __int_least16_t;
typedef __int32_t __int_least32_t;
typedef __int64_t __int_least64_t;

typedef __int64_t __ptrdiff_t;
typedef __int64_t __register_t;
typedef __int64_t __segsz_t;
typedef __uint64_t __size_t;
typedef __int64_t __ssize_t;
typedef __int64_t __time_t;
typedef __uint64_t __uintfptr_t;
typedef __uint64_t __uintptr_t;
# 119 "/usr/include/x86/_types.h" 3 4
typedef __uint64_t __uintmax_t;
typedef __uint32_t __uint_fast8_t;
typedef __uint32_t __uint_fast16_t;
typedef __uint32_t __uint_fast32_t;
typedef __uint64_t __uint_fast64_t;
typedef __uint8_t __uint_least8_t;
typedef __uint16_t __uint_least16_t;
typedef __uint32_t __uint_least32_t;
typedef __uint64_t __uint_least64_t;

typedef __uint64_t __u_register_t;
typedef __uint64_t __vm_offset_t;
typedef __uint64_t __vm_paddr_t;
typedef __uint64_t __vm_size_t;
# 143 "/usr/include/x86/_types.h" 3 4
typedef __int64_t __vm_ooffset_t;
typedef __uint64_t __vm_pindex_t;
typedef int ___wchar_t;
# 154 "/usr/include/x86/_types.h" 3 4
typedef __builtin_va_list __va_list;






typedef __va_list __gnuc_va_list;
# 7 "/usr/include/machine/_types.h" 2 3 4
# 34 "/usr/include/sys/_types.h" 2 3 4




typedef __int32_t __blksize_t;
typedef __int64_t __blkcnt_t;
typedef __int32_t __clockid_t;
typedef __uint32_t __fflags_t;
typedef __uint64_t __fsblkcnt_t;
typedef __uint64_t __fsfilcnt_t;
typedef __uint32_t __gid_t;
typedef __int64_t __id_t;
typedef __uint32_t __ino_t;
typedef long __key_t;
typedef __int32_t __lwpid_t;
typedef __uint16_t __mode_t;
typedef int __accmode_t;
typedef int __nl_item;
typedef __uint16_t __nlink_t;
typedef __int64_t __off_t;
typedef __int32_t __pid_t;
typedef __int64_t __rlim_t;


typedef __uint8_t __sa_family_t;
typedef __uint32_t __socklen_t;
typedef long __suseconds_t;
typedef struct __timer *__timer_t;
typedef struct __mq *__mqd_t;
typedef __uint32_t __uid_t;
typedef unsigned int __useconds_t;
typedef int __cpuwhich_t;
typedef int __cpulevel_t;
typedef int __cpusetid_t;
# 87 "/usr/include/sys/_types.h" 3 4
typedef int __ct_rune_t;
typedef __ct_rune_t __rune_t;
typedef __ct_rune_t __wint_t;



typedef __uint_least16_t __char16_t;
typedef __uint_least32_t __char32_t;







typedef __uint32_t __dev_t;

typedef __uint32_t __fixpt_t;





typedef union {
 char __mbstate8[128];
 __int64_t _mbstateL;
} __mbstate_t;
# 39 "/usr/include/stdlib.h" 2 3 4



typedef __rune_t rune_t;





typedef __size_t size_t;





typedef ___wchar_t wchar_t;




typedef struct {
 int quot;
 int rem;
} div_t;

typedef struct {
 long quot;
 long rem;
} ldiv_t;
# 78 "/usr/include/stdlib.h" 3 4
extern int __mb_cur_max;
extern int ___mb_cur_max(void);


__attribute__((__noreturn__)) void abort(void);
int abs(int) __attribute__((__const__));
int atexit(void (*)(void));
double atof(const char *);
int atoi(const char *);
long atol(const char *);
void *bsearch(const void *, const void *, size_t,
     size_t, int (*)(const void *, const void *));
void *calloc(size_t, size_t) __attribute__((__malloc__));
div_t div(int, int) __attribute__((__const__));
__attribute__((__noreturn__)) void exit(int);
void free(void *);
char *getenv(const char *);
long labs(long) __attribute__((__const__));
ldiv_t ldiv(long, long) __attribute__((__const__));
void *malloc(size_t) __attribute__((__malloc__));
int mblen(const char *, size_t);
size_t mbstowcs(wchar_t * restrict , const char * restrict, size_t);
int mbtowc(wchar_t * restrict, const char * restrict, size_t);
void qsort(void *, size_t, size_t,
     int (*)(const void *, const void *));
int rand(void);
void *realloc(void *, size_t);
void srand(unsigned);
double strtod(const char * restrict, char ** restrict);
float strtof(const char * restrict, char ** restrict);
long strtol(const char * restrict, char ** restrict, int);
long double
  strtold(const char * restrict, char ** restrict);
unsigned long
  strtoul(const char * restrict, char ** restrict, int);
int system(const char *);
int wctomb(char *, wchar_t);
size_t wcstombs(char * restrict, const wchar_t * restrict, size_t);
# 130 "/usr/include/stdlib.h" 3 4
typedef struct {
 long long quot;
 long long rem;
} lldiv_t;


long long
  atoll(const char *);

long long
  llabs(long long) __attribute__((__const__));

lldiv_t lldiv(long long, long long) __attribute__((__const__));

long long
  strtoll(const char * restrict, char ** restrict, int);

unsigned long long
  strtoull(const char * restrict, char ** restrict, int);


__attribute__((__noreturn__)) void _Exit(int);






void * aligned_alloc(size_t, size_t) __attribute__((__malloc__));
int at_quick_exit(void (*)(void));
__attribute__((__noreturn__)) void
 quick_exit(int);





char *realpath(const char * restrict, char * restrict);


int rand_r(unsigned *);


int posix_memalign(void **, size_t, size_t);
int setenv(const char *, const char *, int);
int unsetenv(const char *);



int getsubopt(char **, char *const *, char **);

char *mkdtemp(char *);



int mkstemp(char *);
# 198 "/usr/include/stdlib.h" 3 4
long a64l(const char *);
double drand48(void);

double erand48(unsigned short[3]);


int grantpt(int);
char *initstate(unsigned long , char *, long);
long jrand48(unsigned short[3]);
char *l64a(long);
void lcong48(unsigned short[7]);
long lrand48(void);

char *mktemp(char *);


long mrand48(void);
long nrand48(unsigned short[3]);
int posix_openpt(int);
char *ptsname(int);
int putenv(char *);
long random(void);
unsigned short
 *seed48(unsigned short[3]);

int setkey(const char *);


char *setstate( char *);
void srand48(long);
void srandom(unsigned long);
int unlockpt(int);



extern const char *malloc_conf;
extern void (*malloc_message)(void *, const char *);
# 252 "/usr/include/stdlib.h" 3 4
void abort2(const char *, int, void **) __attribute__((__noreturn__));
__uint32_t
  arc4random(void);
void arc4random_addrandom(unsigned char *, int);
void arc4random_buf(void *, size_t);
void arc4random_stir(void);
__uint32_t
  arc4random_uniform(__uint32_t);





char *getbsize(int *, long *);

char *cgetcap(char *, const char *, int);
int cgetclose(void);
int cgetent(char **, char **, const char *);
int cgetfirst(char **, char **);
int cgetmatch(const char *, const char *);
int cgetnext(char **, char **);
int cgetnum(char *, const char *, long *);
int cgetset(const char *);
int cgetstr(char *, const char *, char **);
int cgetustr(char *, const char *, char **);

int daemon(int, int);
char *devname(__dev_t, __mode_t);
char *devname_r(__dev_t, __mode_t, char *, int);
char *fdevname(int);
char *fdevname_r(int, char *, int);
int getloadavg(double [], int);
const char *
  getprogname(void);

int heapsort(void *, size_t, size_t, int (*)(const void *, const void *));





int l64a_r(long, char *, int);
int mergesort(void *, size_t, size_t, int (*)(const void *, const void *));



int mkostemp(char *, int);
int mkostemps(char *, int, int);
void qsort_r(void *, size_t, size_t, void *,
     int (*)(void *, const void *, const void *));
int radixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void *reallocf(void *, size_t);
int rpmatch(const char *);
void setprogname(const char *);
int sradixsort(const unsigned char **, int, const unsigned char *,
     unsigned);
void sranddev(void);
void srandomdev(void);
long long
 strtonum(const char *, long long, long long, const char **);


__int64_t
  strtoq(const char *, char **, int);
__uint64_t
  strtouq(const char *, char **, int);

extern char *suboptarg;
# 45 "float.c" 2
# 1 "/usr/include/stdio.h" 1 3 4
# 43 "/usr/include/stdio.h" 3 4
typedef __off_t fpos_t;
# 53 "/usr/include/stdio.h" 3 4
typedef __off_t off_t;



typedef __ssize_t ssize_t;





typedef __va_list va_list;
# 77 "/usr/include/stdio.h" 3 4
struct __sbuf {
 unsigned char *_base;
 int _size;
};
# 110 "/usr/include/stdio.h" 3 4
struct __sFILE {
 unsigned char *_p;
 int _r;
 int _w;
 short _flags;
 short _file;
 struct __sbuf _bf;
 int _lbfsize;


 void *_cookie;
 int (*_close)(void *);
 int (*_read)(void *, char *, int);
 fpos_t (*_seek)(void *, fpos_t, int);
 int (*_write)(void *, const char *, int);


 struct __sbuf _ub;
 unsigned char *_up;
 int _ur;


 unsigned char _ubuf[3];
 unsigned char _nbuf[1];


 struct __sbuf _lb;


 int _blksize;
 fpos_t _offset;

 struct pthread_mutex *_fl_mutex;
 struct pthread *_fl_owner;
 int _fl_count;
 int _orientation;
 __mbstate_t _mbstate;
};


typedef struct __sFILE FILE;



extern FILE *__stdinp;
extern FILE *__stdoutp;
extern FILE *__stderrp;
# 234 "/usr/include/stdio.h" 3 4
void clearerr(FILE *);
int fclose(FILE *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE * restrict, fpos_t * restrict);
char *fgets(char * restrict, int, FILE * restrict);
FILE *fopen(const char * restrict, const char * restrict);
int fprintf(FILE * restrict, const char * restrict, ...);
int fputc(int, FILE *);
int fputs(const char * restrict, FILE * restrict);
size_t fread(void * restrict, size_t, size_t, FILE * restrict);
FILE *freopen(const char * restrict, const char * restrict, FILE * restrict);
int fscanf(FILE * restrict, const char * restrict, ...);
int fseek(FILE *, long, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
size_t fwrite(const void * restrict, size_t, size_t, FILE * restrict);
int getc(FILE *);
int getchar(void);
char *gets(char *);
void perror(const char *);
int printf(const char * restrict, ...);
int putc(int, FILE *);
int putchar(int);
int puts(const char *);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
int scanf(const char * restrict, ...);
void setbuf(FILE * restrict, char * restrict);
int setvbuf(FILE * restrict, char * restrict, int, size_t);
int sprintf(char * restrict, const char * restrict, ...);
int sscanf(const char * restrict, const char * restrict, ...);
FILE *tmpfile(void);
char *tmpnam(char *);
int ungetc(int, FILE *);
int vfprintf(FILE * restrict, const char * restrict,
     __va_list);
int vprintf(const char * restrict, __va_list);
int vsprintf(char * restrict, const char * restrict,
     __va_list);


int snprintf(char * restrict, size_t, const char * restrict,
     ...) __attribute__((__format__ (__printf__, 3, 4)));
int vfscanf(FILE * restrict, const char * restrict, __va_list)
     __attribute__((__format__ (__scanf__, 2, 0)));
int vscanf(const char * restrict, __va_list) __attribute__((__format__ (__scanf__, 1, 0)));
int vsnprintf(char * restrict, size_t, const char * restrict,
     __va_list) __attribute__((__format__ (__printf__, 3, 0)));
int vsscanf(const char * restrict, const char * restrict, __va_list)
     __attribute__((__format__ (__scanf__, 2, 0)));
# 300 "/usr/include/stdio.h" 3 4
char *ctermid(char *);
FILE *fdopen(int, const char *);
int fileno(FILE *);



int pclose(FILE *);
FILE *popen(const char *, const char *);



int ftrylockfile(FILE *);
void flockfile(FILE *);
void funlockfile(FILE *);





int getc_unlocked(FILE *);
int getchar_unlocked(void);
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);


void clearerr_unlocked(FILE *);
int feof_unlocked(FILE *);
int ferror_unlocked(FILE *);
int fileno_unlocked(FILE *);



int fseeko(FILE *, __off_t, int);
__off_t ftello(FILE *);



int getw(FILE *);
int putw(int, FILE *);



char *tempnam(const char *, const char *);



FILE *fmemopen(void * restrict, size_t, const char * restrict);
ssize_t getdelim(char ** restrict, size_t * restrict, int,
     FILE * restrict);
FILE *open_memstream(char **, size_t *);
int renameat(int, const char *, int, const char *);
int vdprintf(int, const char * restrict, __va_list);
# 396 "/usr/include/stdio.h" 3 4
int asprintf(char **, const char *, ...) __attribute__((__format__ (__printf__, 2, 3)));
char *ctermid_r(char *);
void fcloseall(void);
char *fgetln(FILE *, size_t *);
const char *fmtcheck(const char *, const char *) __attribute__((__format_arg__ (2)));
int fpurge(FILE *);
void setbuffer(FILE *, char *, int);
int setlinebuf(FILE *);
int vasprintf(char **, const char *, __va_list)
     __attribute__((__format__ (__printf__, 2, 0)));






extern const int sys_nerr;
extern const char * const sys_errlist[];




FILE *funopen(const void *,
     int (*)(void *, char *, int),
     int (*)(void *, const char *, int),
     fpos_t (*)(void *, fpos_t, int),
     int (*)(void *));
# 431 "/usr/include/stdio.h" 3 4
int ftruncate(int, __off_t);



__off_t lseek(int, __off_t, int);



void *mmap(void *, size_t, int, int, int, __off_t);



int truncate(const char *, __off_t);






int __srget(FILE *);
int __swbuf(int, FILE *);







static __inline int __sputc(int _c, FILE *_p) {
 if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
  return (*_p->_p++ = _c);
 else
  return (__swbuf(_c, _p));
}
# 479 "/usr/include/stdio.h" 3 4
extern int __isthreaded;
# 46 "float.c" 2
# 1 "/usr/include/string.h" 1 3 4
# 45 "/usr/include/string.h" 3 4
# 1 "/usr/include/strings.h" 1 3 4
# 42 "/usr/include/strings.h" 3 4
int bcmp(const void *, const void *, size_t) __attribute__((__pure__));
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);


void explicit_bzero(void *, size_t);


int ffs(int) __attribute__((__const__));


int ffsl(long) __attribute__((__const__));
int ffsll(long long) __attribute__((__const__));
int fls(int) __attribute__((__const__));
int flsl(long) __attribute__((__const__));
int flsll(long long) __attribute__((__const__));


char *index(const char *, int) __attribute__((__pure__));
char *rindex(const char *, int) __attribute__((__pure__));

int strcasecmp(const char *, const char *) __attribute__((__pure__));
int strncasecmp(const char *, const char *, size_t) __attribute__((__pure__));



# 1 "/usr/include/xlocale/_strings.h" 1 3 4
# 31 "/usr/include/xlocale/_strings.h" 3 4
typedef struct _xlocale *locale_t;
# 46 "/usr/include/xlocale/_strings.h" 3 4
int strcasecmp_l(const char *, const char *, locale_t);
int strncasecmp_l(const char *, const char *, size_t, locale_t);
# 68 "/usr/include/strings.h" 2 3 4
# 46 "/usr/include/string.h" 2 3 4
# 55 "/usr/include/string.h" 3 4
void *memccpy(void * restrict, const void * restrict, int, size_t);

void *memchr(const void *, int, size_t) __attribute__((__pure__));

void *memrchr(const void *, int, size_t) __attribute__((__pure__));

int memcmp(const void *, const void *, size_t) __attribute__((__pure__));
void *memcpy(void * restrict, const void * restrict, size_t);

void *memmem(const void *, size_t, const void *, size_t) __attribute__((__pure__));

void *memmove(void *, const void *, size_t);
void *memset(void *, int, size_t);

char *stpcpy(char * restrict, const char * restrict);
char *stpncpy(char * restrict, const char * restrict, size_t);


char *strcasestr(const char *, const char *) __attribute__((__pure__));

char *strcat(char * restrict, const char * restrict);
char *strchr(const char *, int) __attribute__((__pure__));

char *strchrnul(const char*, int) __attribute__((__pure__));

int strcmp(const char *, const char *) __attribute__((__pure__));
int strcoll(const char *, const char *);
char *strcpy(char * restrict, const char * restrict);
size_t strcspn(const char *, const char *) __attribute__((__pure__));

char *strdup(const char *) __attribute__((__malloc__));

char *strerror(int);

int strerror_r(int, char *, size_t);


size_t strlcat(char * restrict, const char * restrict, size_t);
size_t strlcpy(char * restrict, const char * restrict, size_t);

size_t strlen(const char *) __attribute__((__pure__));

void strmode(int, char *);

char *strncat(char * restrict, const char * restrict, size_t);
int strncmp(const char *, const char *, size_t) __attribute__((__pure__));
char *strncpy(char * restrict, const char * restrict, size_t);

char *strndup(const char *, size_t) __attribute__((__malloc__));
size_t strnlen(const char *, size_t) __attribute__((__pure__));


char *strnstr(const char *, const char *, size_t) __attribute__((__pure__));

char *strpbrk(const char *, const char *) __attribute__((__pure__));
char *strrchr(const char *, int) __attribute__((__pure__));

char *strsep(char **, const char *);


char *strsignal(int);

size_t strspn(const char *, const char *) __attribute__((__pure__));
char *strstr(const char *, const char *) __attribute__((__pure__));
char *strtok(char * restrict, const char * restrict);

char *strtok_r(char *, const char *, char **);

size_t strxfrm(char * restrict, const char * restrict, size_t);
# 134 "/usr/include/string.h" 3 4
void swab(const void * restrict, void * restrict, ssize_t);






# 1 "/usr/include/xlocale/_string.h" 1 3 4
# 46 "/usr/include/xlocale/_string.h" 3 4
int strcoll_l(const char *, const char *, locale_t);
size_t strxfrm_l(char *, const char *, size_t, locale_t);
# 141 "/usr/include/string.h" 2 3 4
# 47 "float.c" 2
# 1 "/usr/include/ctype.h" 1 3 4
# 46 "/usr/include/ctype.h" 3 4
# 1 "/usr/include/_ctype.h" 1 3 4
# 71 "/usr/include/_ctype.h" 3 4
unsigned long ___runetype(__ct_rune_t) __attribute__((__pure__));
__ct_rune_t ___tolower(__ct_rune_t) __attribute__((__pure__));
__ct_rune_t ___toupper(__ct_rune_t) __attribute__((__pure__));
# 86 "/usr/include/_ctype.h" 3 4
extern int __mb_sb_limit;








# 1 "/usr/include/runetype.h" 1 3 4
# 48 "/usr/include/runetype.h" 3 4
typedef struct {
 __rune_t __min;
 __rune_t __max;
 __rune_t __map;
 unsigned long *__types;
} _RuneEntry;

typedef struct {
 int __nranges;
 _RuneEntry *__ranges;
} _RuneRange;

typedef struct {
 char __magic[8];
 char __encoding[32];

 __rune_t (*__sgetrune)(const char *, __size_t, char const **);
 int (*__sputrune)(__rune_t, char *, __size_t, char **);
 __rune_t __invalid_rune;

 unsigned long __runetype[(1 <<8 )];
 __rune_t __maplower[(1 <<8 )];
 __rune_t __mapupper[(1 <<8 )];






 _RuneRange __runetype_ext;
 _RuneRange __maplower_ext;
 _RuneRange __mapupper_ext;

 void *__variable;
 int __variable_len;
} _RuneLocale;



extern const _RuneLocale _DefaultRuneLocale;
extern const _RuneLocale *_CurrentRuneLocale;



extern _Thread_local const _RuneLocale *_ThreadRuneLocale;
static __inline const _RuneLocale *__getCurrentRuneLocale(void)
{

 if (_ThreadRuneLocale)
  return _ThreadRuneLocale;
 if (_CurrentRuneLocale)
  return _CurrentRuneLocale;
 return &_DefaultRuneLocale;
}
# 95 "/usr/include/_ctype.h" 2 3 4

static __inline int
__maskrune(__ct_rune_t _c, unsigned long _f)
{
 return ((_c < 0 || _c >= (1 <<8 )) ? ___runetype(_c) :
  (__getCurrentRuneLocale())->__runetype[_c]) & _f;
}

static __inline int
__sbmaskrune(__ct_rune_t _c, unsigned long _f)
{
 return (_c < 0 || _c >= __mb_sb_limit) ? 0 :
        (__getCurrentRuneLocale())->__runetype[_c] & _f;
}

static __inline int
__istype(__ct_rune_t _c, unsigned long _f)
{
 return (!!__maskrune(_c, _f));
}

static __inline int
__sbistype(__ct_rune_t _c, unsigned long _f)
{
 return (!!__sbmaskrune(_c, _f));
}

static __inline int
__isctype(__ct_rune_t _c, unsigned long _f)
{
 return (_c < 0 || _c >= 128) ? 0 :
        !!(_DefaultRuneLocale.__runetype[_c] & _f);
}

static __inline __ct_rune_t
__toupper(__ct_rune_t _c)
{
 return (_c < 0 || _c >= (1 <<8 )) ? ___toupper(_c) :
        (__getCurrentRuneLocale())->__mapupper[_c];
}

static __inline __ct_rune_t
__sbtoupper(__ct_rune_t _c)
{
 return (_c < 0 || _c >= __mb_sb_limit) ? _c :
        (__getCurrentRuneLocale())->__mapupper[_c];
}

static __inline __ct_rune_t
__tolower(__ct_rune_t _c)
{
 return (_c < 0 || _c >= (1 <<8 )) ? ___tolower(_c) :
        (__getCurrentRuneLocale())->__maplower[_c];
}

static __inline __ct_rune_t
__sbtolower(__ct_rune_t _c)
{
 return (_c < 0 || _c >= __mb_sb_limit) ? _c :
        (__getCurrentRuneLocale())->__maplower[_c];
}

static __inline int
__wcwidth(__ct_rune_t _c)
{
 unsigned int _x;

 if (_c == 0)
  return (0);
 _x = (unsigned int)__maskrune(_c, 0xe0000000L|0x00040000L);
 if ((_x & 0xe0000000L) != 0)
  return ((_x & 0xe0000000L) >> 30);
 return ((_x & 0x00040000L) != 0 ? 1 : -1);
}
# 47 "/usr/include/ctype.h" 2 3 4


int isalnum(int);
int isalpha(int);
int iscntrl(int);
int isdigit(int);
int isgraph(int);
int islower(int);
int isprint(int);
int ispunct(int);
int isspace(int);
int isupper(int);
int isxdigit(int);
int tolower(int);
int toupper(int);


int isascii(int);
int toascii(int);



int isblank(int);



int digittoint(int);
int ishexnumber(int);
int isideogram(int);
int isnumber(int);
int isphonogram(int);
int isrune(int);
int isspecial(int);




# 1 "/usr/include/xlocale/_ctype.h" 1 3 4
# 49 "/usr/include/xlocale/_ctype.h" 3 4
unsigned long ___runetype_l(__ct_rune_t, locale_t) __attribute__((__pure__));
__ct_rune_t ___tolower_l(__ct_rune_t, locale_t) __attribute__((__pure__));
__ct_rune_t ___toupper_l(__ct_rune_t, locale_t) __attribute__((__pure__));
_RuneLocale *__runes_for_locale(locale_t, int*);
# 96 "/usr/include/xlocale/_ctype.h" 3 4
inline int
__sbmaskrune_l(__ct_rune_t __c, unsigned long __f, locale_t __loc);
inline int
__sbistype_l(__ct_rune_t __c, unsigned long __f, locale_t __loc);

inline int
__sbmaskrune_l(__ct_rune_t __c, unsigned long __f, locale_t __loc)
{
 int __limit;
 _RuneLocale *runes = __runes_for_locale(__loc, &__limit);
 return (__c < 0 || __c >= __limit) ? 0 :
        runes->__runetype[__c] & __f;
}

inline int
__sbistype_l(__ct_rune_t __c, unsigned long __f, locale_t __loc)
{
 return (!!__sbmaskrune_l(__c, __f, __loc));
}







inline int isalnum_l(int, locale_t); inline int isalnum_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000100L|0x00000400L, __l); }
inline int isalpha_l(int, locale_t); inline int isalpha_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000100L, __l); }
inline int isblank_l(int, locale_t); inline int isblank_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00020000L, __l); }
inline int iscntrl_l(int, locale_t); inline int iscntrl_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000200L, __l); }
inline int isdigit_l(int, locale_t); inline int isdigit_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000400L, __l); }
inline int isgraph_l(int, locale_t); inline int isgraph_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000800L, __l); }
inline int ishexnumber_l(int, locale_t); inline int ishexnumber_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00010000L, __l); }
inline int isideogram_l(int, locale_t); inline int isideogram_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00080000L, __l); }
inline int islower_l(int, locale_t); inline int islower_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00001000L, __l); }
inline int isnumber_l(int, locale_t); inline int isnumber_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00000400L, __l); }
inline int isphonogram_l(int, locale_t); inline int isphonogram_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00200000L, __l); }
inline int isprint_l(int, locale_t); inline int isprint_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00040000L, __l); }
inline int ispunct_l(int, locale_t); inline int ispunct_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00002000L, __l); }
inline int isrune_l(int, locale_t); inline int isrune_l(int __c, locale_t __l) { return __sbistype_l(__c, 0xFFFFFF00L, __l); }
inline int isspace_l(int, locale_t); inline int isspace_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00004000L, __l); }
inline int isspecial_l(int, locale_t); inline int isspecial_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00100000L, __l); }
inline int isupper_l(int, locale_t); inline int isupper_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00008000L, __l); }
inline int isxdigit_l(int, locale_t); inline int isxdigit_l(int __c, locale_t __l) { return __sbistype_l(__c, 0x00010000L, __l); }
# 180 "/usr/include/xlocale/_ctype.h" 3 4
inline int digittoint_l(int, locale_t);
inline int tolower_l(int, locale_t);
inline int toupper_l(int, locale_t);

inline int digittoint_l(int __c, locale_t __l)
{ return __sbmaskrune_l((__c), 0xFF, __l); }

inline int tolower_l(int __c, locale_t __l)
{
 int __limit;
 _RuneLocale *__runes = __runes_for_locale(__l, &__limit);
 return (__c < 0 || __c >= __limit) ? __c :
        __runes->__maplower[__c];
}
inline int toupper_l(int __c, locale_t __l)
{
 int __limit;
 _RuneLocale *__runes = __runes_for_locale(__l, &__limit);
 return (__c < 0 || __c >= __limit) ? __c :
        __runes->__mapupper[__c];
}
# 84 "/usr/include/ctype.h" 2 3 4
# 48 "float.c" 2
# 1 "/usr/include/math.h" 1 3 4
# 22 "/usr/include/math.h" 3 4
# 1 "/usr/include/machine/_limits.h" 1 3 4





# 1 "/usr/include/x86/_limits.h" 1 3 4
# 7 "/usr/include/machine/_limits.h" 2 3 4
# 23 "/usr/include/math.h" 2 3 4




extern const union __infinity_un {
 unsigned char __uc[8];
 double __ud;
} __infinity;

extern const union __nan_un {
 unsigned char __uc[sizeof(float)];
 float __uf;
} __nan;
# 136 "/usr/include/math.h" 3 4
typedef __double_t double_t;
typedef __float_t float_t;
# 159 "/usr/include/math.h" 3 4
extern int signgam;
# 180 "/usr/include/math.h" 3 4
int __fpclassifyd(double) __attribute__((__const__));
int __fpclassifyf(float) __attribute__((__const__));
int __fpclassifyl(long double) __attribute__((__const__));
int __isfinitef(float) __attribute__((__const__));
int __isfinite(double) __attribute__((__const__));
int __isfinitel(long double) __attribute__((__const__));
int __isinff(float) __attribute__((__const__));
int __isinf(double) __attribute__((__const__));
int __isinfl(long double) __attribute__((__const__));
int __isnormalf(float) __attribute__((__const__));
int __isnormal(double) __attribute__((__const__));
int __isnormall(long double) __attribute__((__const__));
int __signbit(double) __attribute__((__const__));
int __signbitf(float) __attribute__((__const__));
int __signbitl(long double) __attribute__((__const__));

static __inline int
__inline_isnan(const double __x)
{

 return (__x != __x);
}

static __inline int
__inline_isnanf(const float __x)
{

 return (__x != __x);
}

static __inline int
__inline_isnanl(const long double __x)
{

 return (__x != __x);
}
# 232 "/usr/include/math.h" 3 4
double acos(double);
double asin(double);
double atan(double);
double atan2(double, double);
double cos(double);
double sin(double);
double tan(double);

double cosh(double);
double sinh(double);
double tanh(double);

double exp(double);
double frexp(double, int *);
double ldexp(double, int);
double log(double);
double log10(double);
double modf(double, double *);

double pow(double, double);
double sqrt(double);

double ceil(double);
double fabs(double) __attribute__((__const__));
double floor(double);
double fmod(double, double);





double acosh(double);
double asinh(double);
double atanh(double);
double cbrt(double);
double erf(double);
double erfc(double);
double exp2(double);
double expm1(double);
double fma(double, double, double);
double hypot(double, double);
int ilogb(double) __attribute__((__const__));
double lgamma(double);
long long llrint(double);
long long llround(double);
double log1p(double);
double log2(double);
double logb(double);
long lrint(double);
long lround(double);
double nan(const char *) __attribute__((__const__));
double nextafter(double, double);
double remainder(double, double);
double remquo(double, double, int *);
double rint(double);



double j0(double);
double j1(double);
double jn(int, double);
double y0(double);
double y1(double);
double yn(int, double);


double gamma(double);



double scalb(double, double);




double copysign(double, double) __attribute__((__const__));
double fdim(double, double);
double fmax(double, double) __attribute__((__const__));
double fmin(double, double) __attribute__((__const__));
double nearbyint(double);
double round(double);
double scalbln(double, long);
double scalbn(double, int);
double tgamma(double);
double trunc(double);






double drem(double, double);
int finite(double) __attribute__((__const__));
int isnanf(float) __attribute__((__const__));





double gamma_r(double, int *);
double lgamma_r(double, int *);




double significand(double);




float acosf(float);
float asinf(float);
float atanf(float);
float atan2f(float, float);
float cosf(float);
float sinf(float);
float tanf(float);

float coshf(float);
float sinhf(float);
float tanhf(float);

float exp2f(float);
float expf(float);
float expm1f(float);
float frexpf(float, int *);
int ilogbf(float) __attribute__((__const__));
float ldexpf(float, int);
float log10f(float);
float log1pf(float);
float log2f(float);
float logf(float);
float modff(float, float *);

float powf(float, float);
float sqrtf(float);

float ceilf(float);
float fabsf(float) __attribute__((__const__));
float floorf(float);
float fmodf(float, float);
float roundf(float);

float erff(float);
float erfcf(float);
float hypotf(float, float);
float lgammaf(float);
float tgammaf(float);

float acoshf(float);
float asinhf(float);
float atanhf(float);
float cbrtf(float);
float logbf(float);
float copysignf(float, float) __attribute__((__const__));
long long llrintf(float);
long long llroundf(float);
long lrintf(float);
long lroundf(float);
float nanf(const char *) __attribute__((__const__));
float nearbyintf(float);
float nextafterf(float, float);
float remainderf(float, float);
float remquof(float, float, int *);
float rintf(float);
float scalblnf(float, long);
float scalbnf(float, int);
float truncf(float);

float fdimf(float, float);
float fmaf(float, float, float);
float fmaxf(float, float) __attribute__((__const__));
float fminf(float, float) __attribute__((__const__));






float dremf(float, float);
int finitef(float) __attribute__((__const__));
float gammaf(float);
float j0f(float);
float j1f(float);
float jnf(int, float);
float scalbf(float, float);
float y0f(float);
float y1f(float);
float ynf(int, float);






float gammaf_r(float, int *);
float lgammaf_r(float, int *);




float significandf(float);






long double acoshl(long double);
long double acosl(long double);
long double asinhl(long double);
long double asinl(long double);
long double atan2l(long double, long double);
long double atanhl(long double);
long double atanl(long double);
long double cbrtl(long double);
long double ceill(long double);
long double copysignl(long double, long double) __attribute__((__const__));
long double coshl(long double);
long double cosl(long double);
long double erfcl(long double);
long double erfl(long double);
long double exp2l(long double);
long double expl(long double);
long double expm1l(long double);
long double fabsl(long double) __attribute__((__const__));
long double fdiml(long double, long double);
long double floorl(long double);
long double fmal(long double, long double, long double);
long double fmaxl(long double, long double) __attribute__((__const__));
long double fminl(long double, long double) __attribute__((__const__));
long double fmodl(long double, long double);
long double frexpl(long double value, int *);
long double hypotl(long double, long double);
int ilogbl(long double) __attribute__((__const__));
long double ldexpl(long double, int);
long double lgammal(long double);
long long llrintl(long double);
long long llroundl(long double);
long double log10l(long double);
long double log1pl(long double);
long double log2l(long double);
long double logbl(long double);
long double logl(long double);
long lrintl(long double);
long lroundl(long double);
long double modfl(long double, long double *);
long double nanl(const char *) __attribute__((__const__));
long double nearbyintl(long double);
long double nextafterl(long double, long double);
double nexttoward(double, long double);
float nexttowardf(float, long double);
long double nexttowardl(long double, long double);
long double powl(long double, long double);
long double remainderl(long double, long double);
long double remquol(long double, long double, int *);
long double rintl(long double);
long double roundl(long double);
long double scalblnl(long double, long);
long double scalbnl(long double, int);
long double sinhl(long double);
long double sinl(long double);
long double sqrtl(long double);
long double tanhl(long double);
long double tanl(long double);
long double tgammal(long double);
long double truncl(long double);



long double lgammal_r(long double, int *);
# 49 "float.c" 2
# 1 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h" 1
# 144 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
# 1 "/usr/include/limits.h" 1 3 4
# 138 "/usr/include/limits.h" 3 4
# 1 "/usr/include/sys/limits.h" 1 3 4
# 36 "/usr/include/sys/limits.h" 3 4
# 1 "/usr/include/machine/_limits.h" 1 3 4
# 37 "/usr/include/sys/limits.h" 2 3 4
# 139 "/usr/include/limits.h" 2 3 4


# 1 "/usr/include/sys/syslimits.h" 1 3 4
# 142 "/usr/include/limits.h" 2 3 4
# 145 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h" 2
# 1 "/usr/include/setjmp.h" 1 3 4
# 44 "/usr/include/setjmp.h" 3 4
# 1 "/usr/include/machine/setjmp.h" 1 3 4





# 1 "/usr/include/x86/setjmp.h" 1 3 4
# 45 "/usr/include/x86/setjmp.h" 3 4
typedef struct _sigjmp_buf { long _sjb[12]; } sigjmp_buf[1];


typedef struct _jmp_buf { long _jb[12]; } jmp_buf[1];
# 7 "/usr/include/machine/setjmp.h" 2 3 4
# 45 "/usr/include/setjmp.h" 2 3 4



void _longjmp(jmp_buf, int) __attribute__((__noreturn__));
int _setjmp(jmp_buf) __attribute__((__returns_twice__));

void longjmp(jmp_buf, int) __attribute__((__noreturn__));

void longjmperror(void);

int setjmp(jmp_buf) __attribute__((__returns_twice__));

void siglongjmp(sigjmp_buf, int) __attribute__((__noreturn__));
int sigsetjmp(sigjmp_buf, int) __attribute__((__returns_twice__));
# 146 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h" 2
# 1 "/usr/include/stdarg.h" 1 3 4





# 1 "/usr/include/x86/stdarg.h" 1 3 4
# 7 "/usr/include/stdarg.h" 2 3 4
# 147 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h" 2
# 1 "/usr/include/stddef.h" 1 3 4
# 42 "/usr/include/stddef.h" 3 4
typedef __ptrdiff_t ptrdiff_t;
# 148 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h" 2
# 157 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
# 1 "/usr/home/support/freebsd/sys/boot/ficl4/ficllocal.h" 1
# 158 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h" 2
# 169 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
# 1 "/usr/home/support/freebsd/sys/boot/ficl4/ficlplatform/unix.h" 1
# 1 "/usr/include/stdint.h" 1 3 4
# 35 "/usr/include/stdint.h" 3 4
# 1 "/usr/include/machine/_stdint.h" 1 3 4





# 1 "/usr/include/x86/_stdint.h" 1 3 4
# 7 "/usr/include/machine/_stdint.h" 2 3 4
# 36 "/usr/include/stdint.h" 2 3 4
# 1 "/usr/include/sys/_stdint.h" 1 3 4
# 34 "/usr/include/sys/_stdint.h" 3 4
typedef __int8_t int8_t;




typedef __int16_t int16_t;




typedef __int32_t int32_t;




typedef __int64_t int64_t;




typedef __uint8_t uint8_t;




typedef __uint16_t uint16_t;




typedef __uint32_t uint32_t;




typedef __uint64_t uint64_t;




typedef __intptr_t intptr_t;



typedef __uintptr_t uintptr_t;
# 37 "/usr/include/stdint.h" 2 3 4

typedef __int_least8_t int_least8_t;
typedef __int_least16_t int_least16_t;
typedef __int_least32_t int_least32_t;
typedef __int_least64_t int_least64_t;

typedef __uint_least8_t uint_least8_t;
typedef __uint_least16_t uint_least16_t;
typedef __uint_least32_t uint_least32_t;
typedef __uint_least64_t uint_least64_t;

typedef __int_fast8_t int_fast8_t;
typedef __int_fast16_t int_fast16_t;
typedef __int_fast32_t int_fast32_t;
typedef __int_fast64_t int_fast64_t;

typedef __uint_fast8_t uint_fast8_t;
typedef __uint_fast16_t uint_fast16_t;
typedef __uint_fast32_t uint_fast32_t;
typedef __uint_fast64_t uint_fast64_t;


typedef __intmax_t intmax_t;



typedef __uintmax_t uintmax_t;
# 2 "/usr/home/support/freebsd/sys/boot/ficl4/ficlplatform/unix.h" 2
# 1 "/usr/include/unistd.h" 1 3 4
# 37 "/usr/include/unistd.h" 3 4
# 1 "/usr/include/sys/types.h" 1 3 4
# 44 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/machine/endian.h" 1 3 4





# 1 "/usr/include/x86/endian.h" 1 3 4
# 91 "/usr/include/x86/endian.h" 3 4
static __inline __uint16_t
__bswap16_var(__uint16_t _x)
{

 return ((__uint16_t)((_x) << 8 | (_x) >> 8));
}

static __inline __uint32_t
__bswap32_var(__uint32_t _x)
{


 __asm("bswap %0" : "+r" (_x));
 return (_x);



}

static __inline __uint64_t
__bswap64_var(__uint64_t _x)
{


 __asm("bswap %0" : "+r" (_x));
 return (_x);







}
# 7 "/usr/include/machine/endian.h" 2 3 4
# 45 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/sys/_pthreadtypes.h" 1 3 4
# 44 "/usr/include/sys/_pthreadtypes.h" 3 4
struct pthread;
struct pthread_attr;
struct pthread_cond;
struct pthread_cond_attr;
struct pthread_mutex;
struct pthread_mutex_attr;
struct pthread_once;
struct pthread_rwlock;
struct pthread_rwlockattr;
struct pthread_barrier;
struct pthread_barrier_attr;
struct pthread_spinlock;
# 65 "/usr/include/sys/_pthreadtypes.h" 3 4
typedef struct pthread *pthread_t;


typedef struct pthread_attr *pthread_attr_t;
typedef struct pthread_mutex *pthread_mutex_t;
typedef struct pthread_mutex_attr *pthread_mutexattr_t;
typedef struct pthread_cond *pthread_cond_t;
typedef struct pthread_cond_attr *pthread_condattr_t;
typedef int pthread_key_t;
typedef struct pthread_once pthread_once_t;
typedef struct pthread_rwlock *pthread_rwlock_t;
typedef struct pthread_rwlockattr *pthread_rwlockattr_t;
typedef struct pthread_barrier *pthread_barrier_t;
typedef struct pthread_barrierattr *pthread_barrierattr_t;
typedef struct pthread_spinlock *pthread_spinlock_t;







typedef void *pthread_addr_t;
typedef void *(*pthread_startroutine_t)(void *);




struct pthread_once {
 int state;
 pthread_mutex_t mutex;
};
# 48 "/usr/include/sys/types.h" 2 3 4


typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;

typedef unsigned short ushort;
typedef unsigned int uint;
# 65 "/usr/include/sys/types.h" 3 4
typedef __uint8_t u_int8_t;
typedef __uint16_t u_int16_t;
typedef __uint32_t u_int32_t;
typedef __uint64_t u_int64_t;

typedef __uint64_t u_quad_t;
typedef __int64_t quad_t;
typedef quad_t * qaddr_t;

typedef char * caddr_t;
typedef const char * c_caddr_t;


typedef __blksize_t blksize_t;



typedef __cpuwhich_t cpuwhich_t;
typedef __cpulevel_t cpulevel_t;
typedef __cpusetid_t cpusetid_t;


typedef __blkcnt_t blkcnt_t;




typedef __clock_t clock_t;




typedef __clockid_t clockid_t;



typedef __critical_t critical_t;
typedef __int64_t daddr_t;


typedef __dev_t dev_t;




typedef __fflags_t fflags_t;



typedef __fixpt_t fixpt_t;


typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;




typedef __gid_t gid_t;




typedef __uint32_t in_addr_t;




typedef __uint16_t in_port_t;




typedef __id_t id_t;




typedef __ino_t ino_t;




typedef __key_t key_t;




typedef __lwpid_t lwpid_t;




typedef __mode_t mode_t;




typedef __accmode_t accmode_t;




typedef __nlink_t nlink_t;
# 178 "/usr/include/sys/types.h" 3 4
typedef __pid_t pid_t;



typedef __register_t register_t;


typedef __rlim_t rlim_t;



typedef __int64_t sbintime_t;

typedef __segsz_t segsz_t;
# 204 "/usr/include/sys/types.h" 3 4
typedef __suseconds_t suseconds_t;




typedef __time_t time_t;




typedef __timer_t timer_t;




typedef __mqd_t mqd_t;



typedef __u_register_t u_register_t;


typedef __uid_t uid_t;




typedef __useconds_t useconds_t;





struct cap_rights;

typedef struct cap_rights cap_rights_t;


typedef __vm_offset_t vm_offset_t;
typedef __vm_ooffset_t vm_ooffset_t;
typedef __vm_paddr_t vm_paddr_t;
typedef __vm_pindex_t vm_pindex_t;
typedef __vm_size_t vm_size_t;
# 290 "/usr/include/sys/types.h" 3 4
# 1 "/usr/include/sys/select.h" 1 3 4
# 38 "/usr/include/sys/select.h" 3 4
# 1 "/usr/include/sys/_sigset.h" 1 3 4
# 51 "/usr/include/sys/_sigset.h" 3 4
typedef struct __sigset {
 __uint32_t __bits[4];
} __sigset_t;
# 39 "/usr/include/sys/select.h" 2 3 4
# 1 "/usr/include/sys/_timeval.h" 1 3 4
# 47 "/usr/include/sys/_timeval.h" 3 4
struct timeval {
 time_t tv_sec;
 suseconds_t tv_usec;
};
# 40 "/usr/include/sys/select.h" 2 3 4
# 1 "/usr/include/sys/timespec.h" 1 3 4
# 38 "/usr/include/sys/timespec.h" 3 4
# 1 "/usr/include/sys/_timespec.h" 1 3 4
# 44 "/usr/include/sys/_timespec.h" 3 4
struct timespec {
 time_t tv_sec;
 long tv_nsec;
};
# 39 "/usr/include/sys/timespec.h" 2 3 4
# 58 "/usr/include/sys/timespec.h" 3 4
struct itimerspec {
 struct timespec it_interval;
 struct timespec it_value;
};
# 41 "/usr/include/sys/select.h" 2 3 4

typedef unsigned long __fd_mask;

typedef __fd_mask fd_mask;




typedef __sigset_t sigset_t;
# 71 "/usr/include/sys/select.h" 3 4
typedef struct fd_set {
 __fd_mask __fds_bits[(((1024) + (((sizeof(__fd_mask) * 8)) - 1)) / ((sizeof(__fd_mask) * 8)))];
} fd_set;
# 98 "/usr/include/sys/select.h" 3 4
int pselect(int, fd_set *restrict, fd_set *restrict, fd_set *restrict,
 const struct timespec *restrict, const sigset_t *restrict);



int select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
# 291 "/usr/include/sys/types.h" 2 3 4
# 38 "/usr/include/unistd.h" 2 3 4
# 1 "/usr/include/sys/unistd.h" 1 3 4
# 39 "/usr/include/unistd.h" 2 3 4
# 321 "/usr/include/unistd.h" 3 4
void _exit(int) __attribute__((__noreturn__));
int access(const char *, int);
unsigned int alarm(unsigned int);
int chdir(const char *);
int chown(const char *, uid_t, gid_t);
int close(int);
void closefrom(int);
int dup(int);
int dup2(int, int);
int execl(const char *, const char *, ...);
int execle(const char *, const char *, ...);
int execlp(const char *, const char *, ...);
int execv(const char *, char * const *);
int execve(const char *, char * const *, char * const *);
int execvp(const char *, char * const *);
pid_t fork(void);
long fpathconf(int, int);
char *getcwd(char *, size_t);
gid_t getegid(void);
uid_t geteuid(void);
gid_t getgid(void);
int getgroups(int, gid_t []);
char *getlogin(void);
pid_t getpgrp(void);
pid_t getpid(void);
pid_t getppid(void);
uid_t getuid(void);
int isatty(int);
int link(const char *, const char *);




long pathconf(const char *, int);
int pause(void);
int pipe(int *);
ssize_t read(int, void *, size_t);
int rmdir(const char *);
int setgid(gid_t);
int setpgid(pid_t, pid_t);
pid_t setsid(void);
int setuid(uid_t);
unsigned int sleep(unsigned int);
long sysconf(int);
pid_t tcgetpgrp(int);
int tcsetpgrp(int, pid_t);
char *ttyname(int);
int ttyname_r(int, char *, size_t);
int unlink(const char *);
ssize_t write(int, const void *, size_t);



size_t confstr(int, char *, size_t);


int getopt(int, char * const [], const char *);

extern char *optarg;
extern int optind, opterr, optopt;





int fsync(int);
# 399 "/usr/include/unistd.h" 3 4
int getlogin_r(char *, int);




int fchown(int, uid_t, gid_t);
ssize_t readlink(const char * restrict, char * restrict, size_t);


int gethostname(char *, size_t);
int setegid(gid_t);
int seteuid(uid_t);




int getsid(pid_t _pid);
int fchdir(int);
int getpgid(pid_t _pid);
int lchown(const char *, uid_t, gid_t);
ssize_t pread(int, void *, size_t, off_t);
ssize_t pwrite(int, const void *, size_t, off_t);
# 430 "/usr/include/unistd.h" 3 4
int faccessat(int, const char *, int, int);
int fchownat(int, const char *, uid_t, gid_t, int);
int fexecve(int, char *const [], char *const []);
int linkat(int, const char *, int, const char *, int);
ssize_t readlinkat(int, const char * restrict, char * restrict, size_t);
int symlinkat(const char *, int, const char *);
int unlinkat(int, const char *, int);
# 445 "/usr/include/unistd.h" 3 4
int symlink(const char * restrict, const char * restrict);




char *crypt(const char *, const char *);

int encrypt(char *, int);
long gethostid(void);
int lockf(int, int, off_t);
int nice(int);
int setregid(gid_t, gid_t);
int setreuid(uid_t, uid_t);






void sync(void);




int brk(const void *);
int chroot(const char *);
int getdtablesize(void);
int getpagesize(void) __attribute__((__const__));
char *getpass(const char *);
void *sbrk(intptr_t);



char *getwd(char *);
useconds_t
  ualarm(useconds_t, useconds_t);
int usleep(useconds_t);
pid_t vfork(void) __attribute__((__returns_twice__));



struct timeval;
int acct(const char *);
int async_daemon(void);
int check_utility_compat(const char *);
const char *
  crypt_get_format(void);
int crypt_set_format(const char *);
int des_cipher(const char *, char *, long, int);
int des_setkey(const char *key);
int dup3(int, int, int);
int eaccess(const char *, int);
void endusershell(void);
int exect(const char *, char * const *, char * const *);
int execvP(const char *, const char *, char * const *);
int feature_present(const char *);
char *fflagstostr(u_long);
int getdomainname(char *, int);
int getgrouplist(const char *, gid_t, gid_t *, int *);
int getloginclass(char *, size_t);
mode_t getmode(const void *, mode_t);
int getosreldate(void);
int getpeereid(int, uid_t *, gid_t *);
int getresgid(gid_t *, gid_t *, gid_t *);
int getresuid(uid_t *, uid_t *, uid_t *);
char *getusershell(void);
int initgroups(const char *, gid_t);
int iruserok(unsigned long, int, const char *, const char *);
int iruserok_sa(const void *, int, int, const char *, const char *);
int issetugid(void);
void __FreeBSD_libc_enter_restricted_mode(void);
long lpathconf(const char *, int);





int mknod(const char *, mode_t, dev_t);






int mkstemps(char *, int);




int nfssvc(int, void *);
int nlm_syscall(int, int, int, char **);
int pipe2(int *, int);
int profil(char *, size_t, vm_offset_t, int);
int rcmd(char **, int, const char *, const char *, const char *, int *);
int rcmd_af(char **, int, const char *,
  const char *, const char *, int *, int);
int rcmdsh(char **, int, const char *,
  const char *, const char *, const char *);
char *re_comp(const char *);
int re_exec(const char *);
int reboot(int);
int revoke(const char *);
pid_t rfork(int);
pid_t rfork_thread(int, void *, int (*)(void *), void *);
int rresvport(int *);
int rresvport_af(int *, int);
int ruserok(const char *, int, const char *, const char *);






int setdomainname(const char *, int);
int setgroups(int, const gid_t *);
void sethostid(long);
int sethostname(const char *, int);




int setlogin(const char *);
int setloginclass(const char *);
void *setmode(const char *);
int setpgrp(pid_t, pid_t);
void setproctitle(const char *_fmt, ...) __attribute__((__format__ (__printf0__, 1, 2)));
int setresgid(gid_t, gid_t, gid_t);
int setresuid(uid_t, uid_t, uid_t);
int setrgid(gid_t);
int setruid(uid_t);
void setusershell(void);
int strtofflags(char **, u_long *, u_long *);
int swapon(const char *);
int swapoff(const char *);
int syscall(int, ...);
off_t __syscall(quad_t, ...);
int undelete(const char *);
int unwhiteout(const char *);
void *valloc(size_t);



extern int optreset;
# 3 "/usr/home/support/freebsd/sys/boot/ficl4/ficlplatform/unix.h" 2
# 25 "/usr/home/support/freebsd/sys/boot/ficl4/ficlplatform/unix.h"
typedef int8_t ficlInteger8;
typedef uint8_t ficlUnsigned8;
typedef int16_t ficlInteger16;
typedef uint16_t ficlUnsigned16;
typedef int32_t ficlInteger32;
typedef uint32_t ficlUnsigned32;
typedef int64_t ficlInteger64;
typedef uint64_t ficlUnsigned64;


typedef ficlInteger64 ficlInteger;
typedef ficlUnsigned64 ficlUnsigned;




typedef float ficlFloat;
# 170 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h" 2
# 593 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
struct ficlWord;
typedef struct ficlWord ficlWord;
struct ficlVm;
typedef struct ficlVm ficlVm;
struct ficlDictionary;
typedef struct ficlDictionary ficlDictionary;
struct ficlSystem;
typedef struct ficlSystem ficlSystem;
struct ficlSystemInformation;
typedef struct ficlSystemInformation ficlSystemInformation;
struct ficlCallback;
typedef struct ficlCallback ficlCallback;
struct ficlCountedString;
typedef struct ficlCountedString ficlCountedString;
struct ficlString;
typedef struct ficlString ficlString;
# 622 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern void ficlCallbackDefaultTextOut(ficlCallback *callback, char *text);
extern void *ficlMalloc (size_t size);
extern void ficlFree (void *p);
extern void *ficlRealloc(void *p, size_t size);
# 712 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef struct
{
    ficlUnsigned high;
    ficlUnsigned low;
} ficl2Unsigned;

typedef struct
{
    ficlInteger high;
    ficlInteger low;
} ficl2Integer;
# 739 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern int ficl2IntegerIsNegative(ficl2Integer x);
extern ficl2Integer ficl2IntegerNegate(ficl2Integer x);

extern ficl2Integer ficl2IntegerMultiply(ficlInteger x, ficlInteger y);
extern ficl2Integer ficl2IntegerDecrement(ficl2Integer x);

extern ficl2Unsigned ficl2UnsignedAdd(ficl2Unsigned x, ficl2Unsigned y);
extern ficl2Unsigned ficl2UnsignedSubtract(ficl2Unsigned x, ficl2Unsigned y);
extern ficl2Unsigned ficl2UnsignedMultiply(ficlUnsigned x, ficlUnsigned y);
extern ficl2Unsigned ficl2UnsignedMultiplyAccumulate(ficl2Unsigned u, ficlUnsigned mul, ficlUnsigned add);
extern ficl2Unsigned ficl2UnsignedArithmeticShiftLeft( ficl2Unsigned x );
extern ficl2Unsigned ficl2UnsignedArithmeticShiftRight( ficl2Unsigned x );
extern int ficl2UnsignedCompare(ficl2Unsigned x, ficl2Unsigned y);
extern ficl2Unsigned ficl2UnsignedOr( ficl2Unsigned x, ficl2Unsigned y );



extern ficl2Integer ficl2IntegerAbsoluteValue(ficl2Integer x);




typedef struct
{
    ficl2Unsigned quotient;
    ficlUnsigned remainder;
} ficl2UnsignedQR;

typedef struct
{
    ficl2Integer quotient;
    ficlInteger remainder;
} ficl2IntegerQR;
# 784 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern ficl2IntegerQR ficl2IntegerDivideFloored(ficl2Integer num, ficlInteger den);
extern ficl2IntegerQR ficl2IntegerDivideSymmetric(ficl2Integer num, ficlInteger den);

extern ficl2UnsignedQR ficl2UnsignedDivide(ficl2Unsigned q, ficlUnsigned y);
# 804 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef union ficlCell
{
    ficlInteger i;
    ficlUnsigned u;

    ficlFloat f;

    void *p;
    void (*fn)(void);
} ficlCell;
# 865 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
struct ficlCountedString
{
    ficlUnsigned8 length;
    char text[1];
};







struct ficlString
{
    ficlUnsigned length;
    char *text;
};
# 910 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef struct
{
    ficlInteger index;
    char *end;
    char *text;
} ficlTIB;
# 927 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef struct ficlStack
{
    ficlUnsigned size;
    ficlCell *frame;
    ficlCell *top;
    ficlVm *vm;
    char *name;
    ficlCell base[1];
} ficlStack;




extern ficlStack *ficlStackCreate (ficlVm *vm, char *name, unsigned nCells);
extern void ficlStackDestroy (ficlStack *stack);
extern int ficlStackDepth (ficlStack *stack);
extern void ficlStackDrop (ficlStack *stack, int n);
extern ficlCell ficlStackFetch (ficlStack *stack, int n);
extern ficlCell ficlStackGetTop (ficlStack *stack);
extern void ficlStackPick (ficlStack *stack, int n);
extern ficlCell ficlStackPop (ficlStack *stack);
extern void ficlStackPush (ficlStack *stack, ficlCell c);
extern void ficlStackReset (ficlStack *stack);
extern void ficlStackRoll (ficlStack *stack, int n);
extern void ficlStackSetTop (ficlStack *stack, ficlCell c);
extern void ficlStackStore (ficlStack *stack, int n, ficlCell c);


extern void ficlStackLink (ficlStack *stack, int nCells);
extern void ficlStackUnlink (ficlStack *stack);


extern void *ficlStackPopPointer (ficlStack *stack);
extern ficlUnsigned ficlStackPopUnsigned (ficlStack *stack);
extern ficlInteger ficlStackPopInteger (ficlStack *stack);
extern void ficlStackPushPointer (ficlStack *stack, void *ptr);
extern void ficlStackPushUnsigned (ficlStack *stack, ficlUnsigned u);
extern void ficlStackPushInteger (ficlStack *stack, ficlInteger i);


extern ficlFloat ficlStackPopFloat (ficlStack *stack);
extern void ficlStackPushFloat (ficlStack *stack, ficlFloat f);


extern void ficlStackPush2Integer (ficlStack *stack, ficl2Integer i64);
extern ficl2Integer ficlStackPop2Integer (ficlStack *stack);
extern void ficlStackPush2Unsigned(ficlStack *stack, ficl2Unsigned u64);
extern ficl2Unsigned ficlStackPop2Unsigned (ficlStack *stack);



extern void ficlStackCheck (ficlStack *stack, int popCells, int pushCells);





typedef ficlInteger (*ficlStackWalkFunction)(void *constant, ficlCell *cell);
extern void ficlStackWalk(ficlStack *stack, ficlStackWalkFunction callback, void *context, ficlInteger bottomToTop);
extern void ficlStackDisplay(ficlStack *stack, ficlStackWalkFunction callback, void *context);


typedef ficlWord **ficlIp;
typedef void (*ficlPrimitive)(ficlVm *vm);
typedef void (*ficlOutputFunction)(ficlCallback *callback, char *text);
# 1004 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
struct ficlCallback
{
    void *context;
    ficlOutputFunction textOut;
    ficlOutputFunction errorOut;
    ficlSystem *system;
    ficlVm *vm;
};

extern void ficlCallbackTextOut(ficlCallback *callback, char *text);
extern void ficlCallbackErrorOut(ficlCallback *callback, char *text);




typedef void (*ficlCompatibilityOutputFunction)(ficlVm *vm, char *text, int newline);
extern void ficlCompatibilityTextOutCallback(ficlCallback *callback, char *text, ficlCompatibilityOutputFunction oldFunction);
# 1030 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
enum ficlInstruction
{



# 1 "/usr/home/support/freebsd/sys/boot/ficl4/ficltokens.h" 1
ficlInstructionInvalid,
ficlInstruction1,
ficlInstruction2,
ficlInstruction3,
ficlInstruction4,
ficlInstruction5,
ficlInstruction6,
ficlInstruction7,
ficlInstruction8,
ficlInstruction9,
ficlInstruction10,
ficlInstruction11,
ficlInstruction12,
ficlInstruction13,
ficlInstruction14,
ficlInstruction15,
ficlInstruction16,
ficlInstruction0,
ficlInstructionNeg1,
ficlInstructionNeg2,
ficlInstructionNeg3,
ficlInstructionNeg4,
ficlInstructionNeg5,
ficlInstructionNeg6,
ficlInstructionNeg7,
ficlInstructionNeg8,
ficlInstructionNeg9,
ficlInstructionNeg10,
ficlInstructionNeg11,
ficlInstructionNeg12,
ficlInstructionNeg13,
ficlInstructionNeg14,
ficlInstructionNeg15,
ficlInstructionNeg16,

ficlInstructionF0,
ficlInstructionF1,
ficlInstructionFNeg1,

ficlInstructionPlus,
ficlInstructionMinus,
ficlInstruction1Plus,
ficlInstruction1Minus,
ficlInstruction2Plus,
ficlInstruction2Minus,
ficlInstructionSemiParen,
ficlInstructionExitParen,
ficlInstructionDup,
ficlInstructionSwap,
ficlInstructionGreaterThan,
ficlInstructionBranchParenWithCheck,
ficlInstructionBranchParen,
ficlInstructionBranch0ParenWithCheck,
ficlInstructionBranch0Paren,
ficlInstructionLiteralParen,
ficlInstructionLoopParen,
ficlInstructionOfParen,
ficlInstructionPlusLoopParen,
ficlInstructionFetch,
ficlInstructionStore,
ficlInstructionComma,
ficlInstructionCComma,
ficlInstructionCells,
ficlInstructionCellPlus,
ficlInstructionNegate,
ficlInstructionStar,
ficlInstructionSlash,
ficlInstructionStarSlash,
ficlInstructionSlashMod,
ficlInstructionStarSlashMod,
ficlInstruction2Star,
ficlInstruction2Slash,

ficlInstructionColonParen,
ficlInstructionVariableParen,
ficlInstructionConstantParen,
ficlInstruction2ConstantParen,
ficlInstruction2LiteralParen,
ficlInstructionDoDoes,
ficlInstructionDoParen,
ficlInstructionDoesParen,
ficlInstructionQDoParen,
ficlInstructionCreateParen,
ficlInstructionStringLiteralParen,
ficlInstructionCStringLiteralParen,

ficlInstructionPlusStore,
ficlInstruction0Less,
ficlInstruction0Greater,
ficlInstruction0Equals,
ficlInstruction2Store,
ficlInstruction2Fetch,
ficlInstructionOver,
ficlInstructionRot,
ficlInstruction2Drop,
ficlInstruction2Dup,
ficlInstruction2Over,
ficlInstruction2Swap,
ficlInstructionFromRStack,
ficlInstructionFetchRStack,
ficlInstruction2ToR,
ficlInstruction2RFrom,
ficlInstruction2RFetch,
ficlInstructionLess,
ficlInstructionEquals,
ficlInstructionToRStack,
ficlInstructionQuestionDup,
ficlInstructionAnd,
ficlInstructionCStore,
ficlInstructionCFetch,
ficlInstructionDrop,
ficlInstructionPick,
ficlInstructionRoll,
ficlInstructionMinusRoll,
ficlInstructionMinusRot,
ficlInstructionFill,
ficlInstructionSToD,
ficlInstructionULess,

ficlInstructionQuadFetch,
ficlInstructionQuadStore,
ficlInstructionWFetch,
ficlInstructionWStore,

ficlInstructionInvert,
ficlInstructionLShift,
ficlInstructionMax,
ficlInstructionMin,
ficlInstructionMove,
ficlInstructionOr,
ficlInstructionRShift,
ficlInstructionXor,

ficlInstructionI,
ficlInstructionJ,
ficlInstructionK,

ficlInstructionCompare,
ficlInstructionCompareInsensitive,
ficlInstructionRandom,
ficlInstructionSeedRandom,

ficlInstructionLeave,
ficlInstructionUnloop,


ficlInstructionUserParen,



ficlInstructionLinkParen,
ficlInstructionUnlinkParen,

ficlInstructionGetLocalParen,
ficlInstructionGet2LocalParen,
ficlInstructionToLocalParen,
ficlInstructionTo2LocalParen,

ficlInstructionGetLocal0,
ficlInstructionGet2Local0,
ficlInstructionToLocal0,
ficlInstructionTo2Local0,

ficlInstructionGetLocal1,
ficlInstructionToLocal1,


ficlInstructionGetFLocalParen,
ficlInstructionGetF2LocalParen,
ficlInstructionToFLocalParen,
ficlInstructionToF2LocalParen,





ficlInstructionFLiteralParen,
ficlInstructionFConstantParen,
ficlInstructionF2ConstantParen,

ficlInstructionFPlus,
ficlInstructionFMinus,
ficlInstructionFStar,
ficlInstructionFSlash,
ficlInstructionFNegate,
ficlInstructionFPlusI,
ficlInstructionFMinusI,
ficlInstructionFStarI,
ficlInstructionFSlashI,
ficlInstructionIMinusF,
ficlInstructionISlashF,

ficlInstructionFFrom,
ficlInstructionToF,
ficlInstructionIntToFloat,
ficlInstructionFloatToInt,

ficlInstructionFFetch,
ficlInstructionFStore,
ficlInstructionF2Fetch,
ficlInstructionF2Store,
ficlInstructionFPlusStore,

ficlInstructionFDrop,
ficlInstructionF2Drop,
ficlInstructionFDup,
ficlInstructionF2Dup,
ficlInstructionFMinusRoll,
ficlInstructionFMinusRot,
ficlInstructionFQuestionDup,
ficlInstructionFOver,
ficlInstructionF2Over,
ficlInstructionFPick,
ficlInstructionFRoll,
ficlInstructionFRot,
ficlInstructionFSwap,
ficlInstructionF2Swap,

ficlInstructionF0Less,
ficlInstructionFLess,
ficlInstructionF0Equals,
ficlInstructionFEquals,
ficlInstructionF0Greater,
ficlInstructionFGreater,



ficlInstructionExitInnerLoop,
# 1035 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h" 2



    ficlInstructionLast,

    ficlInstructionFourByteTrick = 0x10000000
};
typedef intptr_t ficlInstruction;
# 1058 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
struct ficlVm
{
    ficlCallback callback;
    ficlVm *link;
    jmp_buf *exceptionHandler;
    short restart;
    ficlIp ip;
    ficlWord *runningWord;
    ficlUnsigned state;
    ficlUnsigned base;
    ficlStack *dataStack;
    ficlStack *returnStack;

    ficlStack *floatStack;

    ficlCell sourceId;
    ficlTIB tib;

    ficlCell user[(16)];

    char pad[(256)];



};
# 1111 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern void ficlVmBranchRelative(ficlVm *vm, int offset);
extern ficlVm * ficlVmCreate (ficlVm *vm, unsigned nPStack, unsigned nRStack);
extern void ficlVmDestroy (ficlVm *vm);
extern ficlDictionary *ficlVmGetDictionary(ficlVm *vm);
extern char * ficlVmGetString (ficlVm *vm, ficlCountedString *spDest, char delimiter);
extern ficlString ficlVmGetWord (ficlVm *vm);
extern ficlString ficlVmGetWord0 (ficlVm *vm);
extern int ficlVmGetWordToPad (ficlVm *vm);
extern void ficlVmInnerLoop (ficlVm *vm, ficlWord *word);
extern ficlString ficlVmParseString (ficlVm *vm, char delimiter);
extern ficlString ficlVmParseStringEx(ficlVm *vm, char delimiter, char fSkipLeading);
extern ficlCell ficlVmPop (ficlVm *vm);
extern void ficlVmPush (ficlVm *vm, ficlCell c);
extern void ficlVmPopIP (ficlVm *vm);
extern void ficlVmPushIP (ficlVm *vm, ficlIp newIP);
extern void ficlVmQuit (ficlVm *vm);
extern void ficlVmReset (ficlVm *vm);
extern void ficlVmSetTextOut (ficlVm *vm, ficlOutputFunction textOut);
extern void ficlVmThrow (ficlVm *vm, int except);
extern void ficlVmThrowError (ficlVm *vm, char *fmt, ...);
extern void ficlVmThrowErrorVararg(ficlVm *vm, char *fmt, va_list list);
extern void ficlVmTextOut (ficlVm *vm, char *text);
extern void ficlVmErrorOut (ficlVm *vm, char *text);







extern void ficlVmDisplayDataStack(ficlVm *vm);
extern void ficlVmDisplayDataStackSimple(ficlVm *vm);
extern void ficlVmDisplayReturnStack(ficlVm *vm);

extern void ficlVmDisplayFloatStack(ficlVm *vm);
# 1156 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern int ficlVmEvaluate(ficlVm *vm, char *s);
# 1184 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern int ficlVmExecuteString(ficlVm *vm, ficlString s);
extern int ficlVmExecuteXT(ficlVm *vm, ficlWord *pWord);
extern void ficlVmExecuteInstruction(ficlVm *vm, ficlInstruction i);
extern void ficlVmExecuteWord(ficlVm *vm, ficlWord *pWord);

extern void ficlVmDictionaryAllot(ficlVm *vm, ficlDictionary *dictionary, int n);
extern void ficlVmDictionaryAllotCells(ficlVm *vm, ficlDictionary *dictionary, int cells);

extern int ficlVmParseWord(ficlVm *vm, ficlString s);
# 1206 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern void ficlVmPushTib (ficlVm *vm, char *text, ficlInteger nChars, ficlTIB *pSaveTib);
extern void ficlVmPopTib (ficlVm *vm, ficlTIB *pTib);
# 1216 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
    extern void ficlVmDictionaryCheck(ficlVm *vm, ficlDictionary *dictionary, int n);
    extern void ficlVmDictionarySimpleCheck(ficlVm *vm, ficlDictionary *dictionary, int n);
# 1227 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern void ficlPrimitiveLiteralIm(ficlVm *vm);
# 1250 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
struct ficlWord
{
    struct ficlWord *link;
    ficlUnsigned16 hash;
    ficlUnsigned8 flags;
    ficlUnsigned8 length;
    char *name;
    ficlPrimitive code;
    ficlInstruction semiParen;
    ficlCell param[1];
};
# 1324 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern int ficlWordIsImmediate(ficlWord *word);
extern int ficlWordIsCompileOnly(ficlWord *word);





 extern void ficlCallbackAssert(ficlCallback *callback, int expression, char *expressionString, char *filename, int line);
# 1347 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern int ficlIsPowerOfTwo(ficlUnsigned u);

extern char *ficlLtoa(ficlInteger value, char *string, int radix );
extern char *ficlUltoa(ficlUnsigned value, char *string, int radix );
extern char ficlDigitToCharacter(int value);
extern char *ficlStringReverse( char *string );
extern char *ficlStringSkipSpace(char *s, char *end);
extern char *ficlStringCaseFold(char *s);
extern int ficlStrincmp(char *s1, char *s2, ficlUnsigned length);
extern void *ficlAlignPointer(void *ptr);
# 1367 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef struct ficlHash
{
    struct ficlHash *link;
    char *name;
    unsigned size;
    ficlWord *table[1];
} ficlHash;

extern void ficlHashForget (ficlHash *hash, void *where);
extern ficlUnsigned16 ficlHashCode (ficlString s);
extern void ficlHashInsertWord(ficlHash *hash, ficlWord *word);
extern ficlWord *ficlHashLookup (ficlHash *hash, ficlString name, ficlUnsigned16 hashCode);
extern void ficlHashReset (ficlHash *hash);
# 1412 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
struct ficlDictionary
{
    ficlCell *here;
 void *context;
    ficlWord *smudge;
    ficlHash *forthWordlist;
    ficlHash *compilationWordlist;
    ficlHash *wordlists[(16)];
    int wordlistCount;
    unsigned size;
 ficlSystem *system;
    ficlCell base[1];
};

extern void ficlDictionaryAbortDefinition(ficlDictionary *dictionary);
extern void ficlDictionaryAlign (ficlDictionary *dictionary);
extern void ficlDictionaryAllot (ficlDictionary *dictionary, int n);
extern void ficlDictionaryAllotCells (ficlDictionary *dictionary, int nCells);
extern void ficlDictionaryAppendCell (ficlDictionary *dictionary, ficlCell c);
extern void ficlDictionaryAppendCharacter(ficlDictionary *dictionary, char c);
extern void ficlDictionaryAppendUnsigned(ficlDictionary *dictionary, ficlUnsigned u);
extern void *ficlDictionaryAppendData(ficlDictionary *dictionary, void *data, ficlInteger length);
extern char *ficlDictionaryAppendString(ficlDictionary *dictionary, ficlString s);
extern ficlWord *ficlDictionaryAppendWord(ficlDictionary *dictionary,
                           ficlString name,
                           ficlPrimitive pCode,
                           ficlUnsigned8 flags);
extern ficlWord *ficlDictionaryAppendPrimitive(ficlDictionary *dictionary,
                           char *name,
                           ficlPrimitive pCode,
                           ficlUnsigned8 flags);
extern ficlWord *ficlDictionaryAppendInstruction(ficlDictionary *dictionary,
       char *name,
       ficlInstruction i,
       ficlUnsigned8 flags);

extern ficlWord *ficlDictionaryAppendConstantInstruction(ficlDictionary *dictionary, ficlString name, ficlInstruction instruction, ficlInteger value);
extern ficlWord *ficlDictionaryAppend2ConstantInstruction(ficlDictionary *dictionary, ficlString name, ficlInstruction instruction, ficl2Integer value);

extern ficlWord *ficlDictionaryAppendConstant(ficlDictionary *dictionary, char *name, ficlInteger value);
extern ficlWord *ficlDictionaryAppend2Constant(ficlDictionary *dictionary, char *name, ficl2Integer value);



extern ficlWord *ficlDictionaryAppendFConstant(ficlDictionary *dictionary, char *name, float value);
extern ficlWord *ficlDictionaryAppendF2Constant(ficlDictionary *dictionary, char *name, double value);



extern ficlWord *ficlDictionarySetConstantInstruction(ficlDictionary *dictionary, ficlString name, ficlInstruction instruction, ficlInteger value);
extern ficlWord *ficlDictionarySet2ConstantInstruction(ficlDictionary *dictionary, ficlString name, ficlInstruction instruction, ficl2Integer value);

extern ficlWord *ficlDictionarySetConstant(ficlDictionary *dictionary, char *name, ficlInteger value);


extern ficlWord *ficlDictionarySet2Constant(ficlDictionary *dictionary, char *name, ficl2Integer value);
extern ficlWord *ficlDictionarySetConstantString(ficlDictionary *dictionary, char *name, char *value);
extern ficlWord *ficlDictionarySetPrimitive(ficlDictionary *dictionary,
                          char *name,
                          ficlPrimitive code,
                          ficlUnsigned8 flags);
extern ficlWord *ficlDictionarySetInstruction(ficlDictionary *dictionary,
                          char *name,
                          ficlInstruction i,
                          ficlUnsigned8 flags);

extern ficlWord *ficlDictionarySetFConstant(ficlDictionary *dictionary, char *name, float value);
extern ficlWord *ficlDictionarySetF2Constant(ficlDictionary *dictionary, char *name, double value);


extern int ficlDictionaryCellsAvailable (ficlDictionary *dictionary);
extern int ficlDictionaryCellsUsed (ficlDictionary *dictionary);
extern ficlDictionary *ficlDictionaryCreate(ficlSystem *system, unsigned nCELLS);
extern ficlDictionary *ficlDictionaryCreateHashed(ficlSystem *system, unsigned nCells, unsigned nHash);
extern ficlHash *ficlDictionaryCreateWordlist(ficlDictionary *dictionary, int nBuckets);
extern void ficlDictionaryDestroy (ficlDictionary *dictionary);
extern void ficlDictionaryEmpty (ficlDictionary *dictionary, unsigned nHash);
extern int ficlDictionaryIncludes (ficlDictionary *dictionary, void *p);
extern ficlWord *ficlDictionaryLookup (ficlDictionary *dictionary, ficlString name);
extern void ficlDictionaryResetSearchOrder(ficlDictionary *dictionary);
extern void ficlDictionarySetFlags (ficlDictionary *dictionary, ficlUnsigned8 set);
extern void ficlDictionaryClearFlags(ficlDictionary *dictionary, ficlUnsigned8 clear);
extern void ficlDictionarySetImmediate(ficlDictionary *dictionary);
extern void ficlDictionaryUnsmudge (ficlDictionary *dictionary);
extern ficlCell *ficlDictionaryWhere (ficlDictionary *dictionary);

extern int ficlDictionaryIsAWord(ficlDictionary *dictionary, ficlWord *word);
extern void ficlDictionarySee(ficlDictionary *dictionary, ficlWord *word, ficlCallback *callback);
extern ficlWord *ficlDictionaryFindEnclosingWord(ficlDictionary *dictionary, ficlCell *cell);
# 1545 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef int (*ficlParseStep)(ficlVm *vm, ficlString s);
# 1557 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef struct ficlBreakpoint
{
    void *address;
    ficlWord *oldXT;
} ficlBreakpoint;
# 1573 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
struct ficlSystemInformation
{
    int size;
    void *context;
    int dictionarySize;
    int stackSize;
    ficlOutputFunction textOut;
    ficlOutputFunction errorOut;
    int environmentSize;
};







struct ficlSystem
{
    ficlCallback callback;
    ficlSystem *link;
    ficlVm *vmList;
    ficlDictionary *dictionary;
    ficlDictionary *environment;

    ficlWord *interpreterLoop[3];
    ficlWord *parseList[(8)];

    ficlWord *exitInnerWord;
    ficlWord *interpretWord;


    ficlDictionary *locals;
    ficlInteger localsCount;
    ficlCell *localsFixup;


    ficlInteger stackSize;

    ficlBreakpoint breakpoint;



};
# 1638 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern ficlSystem *ficlSystemCreate(ficlSystemInformation *fsi);







extern void ficlSystemDestroy(ficlSystem *system);







extern ficlVm *ficlSystemCreateVm(ficlSystem *system);
# 1663 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern void ficlSystemDestroyVm(ficlVm *vm);







extern ficlWord *ficlSystemLookup(ficlSystem *system, char *name);






ficlDictionary *ficlSystemGetDictionary(ficlSystem *system);
ficlDictionary *ficlSystemGetEnvironment(ficlSystem *system);

ficlDictionary *ficlSystemGetLocals(ficlSystem *system);







extern void ficlSystemCompileCore(ficlSystem *system);
extern void ficlSystemCompilePrefix(ficlSystem *system);
extern void ficlSystemCompileSearch(ficlSystem *system);
extern void ficlSystemCompileSoftCore(ficlSystem *system);
extern void ficlSystemCompileTools(ficlSystem *system);
extern void ficlSystemCompileFile(ficlSystem *system);

extern void ficlSystemCompileFloat(ficlSystem *system);
extern int ficlVmParseFloatNumber(ficlVm *vm, ficlString s);


extern void ficlSystemCompilePlatform(ficlSystem *system);

extern void ficlSystemCompileExtras(ficlSystem *system);


extern int ficlVmParsePrefix(ficlVm *vm, ficlString s);


extern ficlWord *ficlSystemLookupLocal(ficlSystem *system, ficlString name);





extern int ficlVmParseNumber(ficlVm *vm, ficlString s);
extern void ficlPrimitiveTick(ficlVm *vm);
extern void ficlPrimitiveParseStepParen(ficlVm *vm);

extern void ficlLocalParen(ficlVm *vm, int isDouble, int isFloat);
# 1729 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern int ficlSystemAddParseStep(ficlSystem *system, ficlWord *word);
extern void ficlSystemAddPrimitiveParseStep(ficlSystem *system, char *name, ficlParseStep pStep);
# 1740 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef enum
{
    FICL_WORDKIND_BRANCH,
    FICL_WORDKIND_BRANCH0,
    FICL_WORDKIND_COLON,
    FICL_WORDKIND_CONSTANT,
    FICL_WORDKIND_2CONSTANT,
    FICL_WORDKIND_CREATE,
    FICL_WORDKIND_DO,
    FICL_WORDKIND_DOES,
    FICL_WORDKIND_LITERAL,
    FICL_WORDKIND_2LITERAL,

    FICL_WORDKIND_FLITERAL,

    FICL_WORDKIND_LOOP,
    FICL_WORDKIND_OF,
    FICL_WORDKIND_PLOOP,
    FICL_WORDKIND_PRIMITIVE,
    FICL_WORDKIND_QDO,
    FICL_WORDKIND_STRING_LITERAL,
    FICL_WORDKIND_CSTRING_LITERAL,

    FICL_WORDKIND_USER,

    FICL_WORDKIND_VARIABLE,
    FICL_WORDKIND_INSTRUCTION,
    FICL_WORDKIND_INSTRUCTION_WORD,
    FICL_WORDKIND_INSTRUCTION_WITH_ARGUMENT,
} ficlWordKind;

ficlWordKind ficlWordClassify(ficlWord *word);
# 1787 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
typedef struct ficlFile
{
    FILE *f;
    char filename[256];
} ficlFile;



extern int ficlFileTruncate(ficlFile *ff, ficlUnsigned size);


extern int ficlFileStatus(char *filename, int *status);
extern long ficlFileSize(ficlFile *ff);
# 1840 "/usr/home/support/freebsd/sys/boot/ficl4/ficl.h"
extern int ficlBitGet(const unsigned char *bits, size_t index);
extern void ficlBitSet(unsigned char *bits, size_t size_t, int value);
extern void ficlBitGetString(unsigned char *destination, const unsigned char *source, int offset, int count, int destAlignment);

extern ficlUnsigned16 ficlNetworkUnsigned16(ficlUnsigned16 number);
extern ficlUnsigned32 ficlNetworkUnsigned32(ficlUnsigned32 number);


extern int ficlLzCompress(const unsigned char *uncompressed, size_t uncompressedSize, unsigned char **compressed, size_t *compressedSize);
extern int ficlLzUncompress(const unsigned char *compressed, unsigned char **uncompressed, size_t *uncompressedSize);
# 50 "float.c" 2








static void ficlPrimitiveFConstant(ficlVm *vm)
{
    ficlDictionary *dictionary = ficlVmGetDictionary(vm);
    ficlString name = ficlVmGetWord(vm);

    ficlStackCheck(vm->floatStack, 1, 0);

    ficlDictionaryAppendWord(dictionary, name, (ficlPrimitive)ficlInstructionFConstantParen, ( 0));
    ficlDictionaryAppendCell(dictionary, ficlStackPop(vm->floatStack));
}


ficlWord *ficlDictionaryAppendFConstant(ficlDictionary *dictionary, char *name, float value)
{
 ficlString s;
 {(s).text = (name); (s).length = strlen(name);};
 return ficlDictionaryAppendConstantInstruction(dictionary, s, ficlInstructionFConstantParen, *(ficlInteger *)(&value));
}


ficlWord *ficlDictionarySetFConstant(ficlDictionary *dictionary, char *name, float value)
{
    ficlString s;
    {(s).text = (name); (s).length = strlen(name);};
    return ficlDictionarySetConstantInstruction(dictionary, s, ficlInstructionFConstantParen, *(ficlInteger *)(&value));
}




static void ficlPrimitiveF2Constant(ficlVm *vm)
{
    ficlDictionary *dictionary = ficlVmGetDictionary(vm);
    ficlString name = ficlVmGetWord(vm);

    ficlStackCheck(vm->floatStack, 2, 0);

    ficlDictionaryAppendWord(dictionary, name, (ficlPrimitive)ficlInstructionF2ConstantParen, ( 0));
    ficlDictionaryAppendCell(dictionary, ficlStackPop(vm->floatStack));
    ficlDictionaryAppendCell(dictionary, ficlStackPop(vm->floatStack));
}


ficlWord *ficlDictionaryAppendF2Constant(ficlDictionary *dictionary, char *name, double value)
{
    ficlString s;
    {(s).text = (name); (s).length = strlen(name);};
    return ficlDictionaryAppend2ConstantInstruction(dictionary, s, ficlInstructionF2ConstantParen, *(ficl2Integer *)(&value));
}


ficlWord *ficlDictionarySetF2Constant(ficlDictionary *dictionary, char *name, double value)
{
    ficlString s;
    {(s).text = (name); (s).length = strlen(name);};
    return ficlDictionarySet2ConstantInstruction(dictionary, s, ficlInstructionF2ConstantParen, *(ficl2Integer *)(&value));
}






static void ficlPrimitiveFDot(ficlVm *vm)
{
    float f;

    ficlStackCheck(vm->floatStack, 1, 0);

    f = ficlStackPopFloat(vm->floatStack);
    sprintf(vm->pad,"%#f ",f);
    ficlVmTextOut(vm, vm->pad);
}





static void ficlPrimitiveEDot(ficlVm *vm)
{
    float f;

    ficlStackCheck(vm->floatStack, 1, 0);

    f = ficlStackPopFloat(vm->floatStack);
    sprintf(vm->pad,"%#e ",f);
    ficlVmTextOut(vm, vm->pad);
}






struct stackContext
{
    ficlVm *vm;
    int count;
};

static ficlInteger ficlFloatStackDisplayCallback(void *c, ficlCell *cell)
{
    struct stackContext *context = (struct stackContext *)c;
    char buffer[64];
    sprintf(buffer, "[0x%08x %3d] %16f (0x%08x)\n", cell, context->count++, (double)(cell->f), cell->i);
    ficlVmTextOut(context->vm, buffer);
 return ((unsigned long)~(0L));
}



void ficlVmDisplayFloatStack(ficlVm *vm)
{
    struct stackContext context;
 context.vm = vm;
 context.count = 0;
    ficlStackDisplay(vm->floatStack, ficlFloatStackDisplayCallback, &context);
    return;
}







static void ficlPrimitiveFDepth(ficlVm *vm)
{
    int i;

    ficlStackCheck(vm->dataStack, 0, 1);

    i = ficlStackDepth(vm->floatStack);
    ficlStackPushInteger(vm->dataStack, i);
}




static void ficlPrimitiveFLiteralImmediate(ficlVm *vm)
{
    ficlDictionary *dictionary = ficlVmGetDictionary(vm);
 ficlCell cell;


    ficlStackCheck(vm->floatStack, 1, 0);


 cell = ficlStackPop(vm->floatStack);
 if (cell.f == 1.0f)
 {
  ficlDictionaryAppendUnsigned(dictionary, ficlInstructionF1);
 }
 else if (cell.f == 0.0f)
 {
  ficlDictionaryAppendUnsigned(dictionary, ficlInstructionF0);
 }
 else if (cell.f == -1.0f)
 {
  ficlDictionaryAppendUnsigned(dictionary, ficlInstructionFNeg1);
 }
 else
 {
  ficlDictionaryAppendUnsigned(dictionary, ficlInstructionFLiteralParen);
  ficlDictionaryAppendCell(dictionary, cell);
 }
}
# 234 "float.c"
typedef enum _floatParseState
{
    FPS_START,
    FPS_ININT,
    FPS_INMANT,
    FPS_STARTEXP,
    FPS_INEXP
} FloatParseState;







int ficlVmParseFloatNumber( ficlVm *vm, ficlString s)
{
    unsigned char c;
 unsigned char digit;
    char *trace;
    ficlUnsigned length;
    float power;
    float accum = 0.0f;
    float mant = 0.1f;
    ficlInteger exponent = 0;
    char flag = 0;
    FloatParseState estate = FPS_START;


    ficlStackCheck(vm->floatStack, 0, 1);





    if (vm->base != 10)
        return(0);


    trace = ((s).text);
    length = ((s).length);


    while ((length--) && ((c = *trace++) != 0))
    {
        switch (estate)
        {

            case FPS_START:
            {
                estate = FPS_ININT;
                if (c == '-')
                {
                    flag |= 1;
                    break;
                }
                if (c == '+')
                {
                    break;
                }
            }




            case FPS_ININT:
            {
                if (c == '.')
                {
                    estate = FPS_INMANT;
                }
                else if ((c == 'e') || (c == 'E'))
                {
                    estate = FPS_STARTEXP;
                }
                else
                {
                    digit = (unsigned char)(c - '0');
                    if (digit > 9)
                        return(0);

                    accum = accum * 10 + digit;

                }
                break;
            }




            case FPS_INMANT:
            {
                if ((c == 'e') || (c == 'E'))
                {
                    estate = FPS_STARTEXP;
                }
                else
                {
                    digit = (unsigned char)(c - '0');
                    if (digit > 9)
                        return(0);

                    accum += digit * mant;
                    mant *= 0.1f;
                }
                break;
            }


            case FPS_STARTEXP:
            {
                estate = FPS_INEXP;

                if (c == '-')
                {
                    flag |= 2;
                    break;
                }
                else if (c == '+')
                {
                    break;
                }
            }




            case FPS_INEXP:
            {
                digit = (unsigned char)(c - '0');
                if (digit > 9)
                    return(0);

                exponent = exponent * 10 + digit;

                break;
            }
        }
    }


    if (estate < FPS_STARTEXP)
        return(0);


    if (flag & 1)
        accum = -accum;


    if (exponent != 0)
    {

        if (flag & 2)
        {
            exponent = -exponent;
        }

        power = (float)pow(10.0, exponent);
        accum *= power;
    }

    ficlStackPushFloat(vm->floatStack, accum);
    if (vm->state == (1))
        ficlPrimitiveFLiteralImmediate(vm);

    return(1);
}




static void ficlPrimitiveFLocalParen(ficlVm *vm)
{
   ficlLocalParen(vm, 0, 1);
}

static void ficlPrimitiveF2LocalParen(ficlVm *vm)
{
   ficlLocalParen(vm, 1, 1);
}
# 423 "float.c"
void ficlSystemCompileFloat(ficlSystem *system)
{

    ficlDictionary *dictionary = ficlSystemGetDictionary(system);
    ficlDictionary *environment = ficlSystemGetEnvironment(system);

    (ficlCallbackAssert(((ficlCallback *)(system)), ((dictionary)) != 0, "(dictionary)", "float.c", 429));
    (ficlCallbackAssert(((ficlCallback *)(system)), ((environment)) != 0, "(environment)", "float.c", 430));

    ficlDictionarySetPrimitive(dictionary, "fconstant", ficlPrimitiveFConstant, ( 0));
    ficlDictionarySetPrimitive(dictionary, "fvalue", ficlPrimitiveFConstant, ( 0));
    ficlDictionarySetPrimitive(dictionary, "f2constant", ficlPrimitiveF2Constant, ( 0));
    ficlDictionarySetPrimitive(dictionary, "f2value", ficlPrimitiveF2Constant, ( 0));
    ficlDictionarySetPrimitive(dictionary, "fdepth", ficlPrimitiveFDepth, ( 0));
    ficlDictionarySetPrimitive(dictionary, "fliteral", ficlPrimitiveFLiteralImmediate, ( 1));
    ficlDictionarySetPrimitive(dictionary, "f.", ficlPrimitiveFDot, ( 0));
    ficlDictionarySetPrimitive(dictionary, "f.s", ficlVmDisplayFloatStack, ( 0));
    ficlDictionarySetPrimitive(dictionary, "fe.", ficlPrimitiveEDot, ( 0));


    ficlDictionarySetPrimitive(dictionary, "(flocal)", ficlPrimitiveFLocalParen, ( 2));
    ficlDictionarySetPrimitive(dictionary, "(f2local)", ficlPrimitiveF2LocalParen, ( 2));
# 461 "float.c"
    ficlDictionarySetConstant(environment, "floating", (0));
    ficlDictionarySetConstant(environment, "floating-ext", (0));
    ficlDictionarySetConstant(environment, "floating-stack", system->stackSize);




    return;
}
