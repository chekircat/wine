/*
 * Wine porting definitions
 *
 */

#ifndef __WINE_WINE_PORT_H
#define __WINE_WINE_PORT_H

#include "config.h"
#include "winnt.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>

/* Types */

#ifndef HAVE_GETRLIMIT
#define RLIMIT_STACK 3
typedef int rlim_t;
struct rlimit
{
    rlim_t rlim_cur;
    rlim_t rlim_max;
};
int getrlimit (int resource, struct rlimit *rlim);
#endif /* HAVE_GETRLIMIT */

#if !defined(HAVE_GETNETBYADDR) && !defined(HAVE_GETNETBYNAME)
struct netent {
  char         *n_name;
  char        **n_aliases;
  int           n_addrtype;
  unsigned long n_net;
};
#endif /* !defined(HAVE_GETNETBYADDR) && !defined(HAVE_GETNETBYNAME) */

#if !defined(HAVE_GETPROTOBYNAME) && !defined(HAVE_GETPROTOBYNUMBER)
struct  protoent {
  char  *p_name;
  char **p_aliases;
  int    p_proto;
};
#endif /* !defined(HAVE_GETPROTOBYNAME) && !defined(HAVE_GETPROTOBYNUMBER) */

#ifndef HAVE_STATFS
# ifdef __BEOS__
#  define STATFS_HAS_BFREE
struct statfs {
  long   f_bsize;  /* block_size */
  long   f_blocks; /* total_blocks */
  long   f_bfree;  /* free_blocks */
};
# else /* defined(__BEOS__) */
struct statfs;
# endif /* defined(__BEOS__) */
#endif /* !defined(HAVE_STATFS) */

/* Functions */

#if !defined(HAVE_CLONE) && defined(linux)
int clone(int (*fn)(void *arg), void *stack, int flags, void *arg);
#endif /* !defined(HAVE_CLONE) && defined(linux) */

#ifndef HAVE_GETNETBYADDR
struct netent *getnetbyaddr(unsigned long net, int type);
#endif /* defined(HAVE_GETNETBYNAME) */

#ifndef HAVE_GETNETBYNAME
struct netent *getnetbyname(const char *name);
#endif /* defined(HAVE_GETNETBYNAME) */

#ifndef HAVE_GETPROTOBYNAME
struct protoent *getprotobyname(const char *name);
#endif /* !defined(HAVE_GETPROTOBYNAME) */

#ifndef HAVE_GETPROTOBYNUMBER
struct protoent *getprotobynumber(int proto);
#endif /* !defined(HAVE_GETPROTOBYNUMBER) */

#ifndef HAVE_GETSERVBYPORT
struct servent *getservbyport(int port, const char *proto);
#endif /* !defined(HAVE_GETSERVBYPORT) */

#ifndef HAVE_GETSOCKOPT
int getsockopt(int socket, int level, int option_name, void *option_value, size_t *option_len);
#endif /* !defined(HAVE_GETSOCKOPT) */

#ifndef HAVE_MEMMOVE
void *memmove(void *dest, const void *src, unsigned int len);
#endif /* !defined(HAVE_MEMMOVE) */

#ifndef HAVE_GETPAGESIZE
size_t getpagesize(void);
#endif  /* HAVE_GETPAGESIZE */

#ifndef HAVE_INET_NETWORK
unsigned long inet_network(const char *cp);
#endif /* !defined(HAVE_INET_NETWORK) */

#ifndef HAVE_SETTIMEOFDAY
int settimeofday(struct timeval *tp, void *reserved);
#endif /* !defined(HAVE_SETTIMEOFDAY) */

#ifndef HAVE_STATFS
int statfs(const char *name, struct statfs *info);
#endif /* !defined(HAVE_STATFS) */

#ifndef HAVE_STRNCASECMP
int strncasecmp(const char *str1, const char *str2, size_t n);
#endif /* !defined(HAVE_STRNCASECMP) */

#ifndef HAVE_STRERROR
const char *strerror(int err);
#endif /* !defined(HAVE_STRERROR) */

#ifndef HAVE_STRCASECMP
int strcasecmp(const char *str1, const char *str2);
#endif /* !defined(HAVE_STRCASECMP) */

#ifndef HAVE_USLEEP
int usleep (unsigned int useconds);
#endif /* !defined(HAVE_USLEEP) */

#ifndef HAVE_LSTAT
int lstat(const char *file_name, struct stat *buf);
#endif /* HAVE_LSTAT */

#ifndef S_ISLNK
#define S_ISLNK(mod) (0)
#endif /* S_ISLNK */

/* So we open files in 64 bit access mode on Linux */
#ifndef O_LARGEFILE
# define O_LARGEFILE 0
#endif

extern void *wine_dlopen( const char *filename, int flag, char *error, int errorsize );
extern void *wine_dlsym( void *handle, const char *symbol, char *error, int errorsize );
extern int wine_dlclose( void *handle, char *error, int errorsize );

#ifdef HAVE_DL_API
#include <dlfcn.h>
#else
#define RTLD_LAZY	0x001
#define RTLD_NOW	0x002
#define RTLD_GLOBAL	0x100
#endif


/* Macros to access unaligned or wrong-endian WORDs and DWORDs. */

#define PUT_WORD(ptr, w)  (*(WORD *)(ptr) = (w))
#define GET_WORD(ptr)     (*(WORD *)(ptr))
#define PUT_DWORD(ptr, d) (*(DWORD *)(ptr) = (d))
#define GET_DWORD(ptr)    (*(DWORD *)(ptr))

#define PUT_LE_WORD(ptr, w) \
        do { ((BYTE *)(ptr))[0] = LOBYTE(w); \
             ((BYTE *)(ptr))[1] = HIBYTE(w); } while (0)
#define GET_LE_WORD(ptr) \
        MAKEWORD( ((BYTE *)(ptr))[0], \
                  ((BYTE *)(ptr))[1] )
#define PUT_LE_DWORD(ptr, d) \
        do { PUT_LE_WORD(&((WORD *)(ptr))[0], LOWORD(d)); \
             PUT_LE_WORD(&((WORD *)(ptr))[1], HIWORD(d)); } while (0)
#define GET_LE_DWORD(ptr) \
        ((DWORD)MAKELONG( GET_LE_WORD(&((WORD *)(ptr))[0]), \
                          GET_LE_WORD(&((WORD *)(ptr))[1]) ))

#define PUT_BE_WORD(ptr, w) \
        do { ((BYTE *)(ptr))[1] = LOBYTE(w); \
             ((BYTE *)(ptr))[0] = HIBYTE(w); } while (0)
#define GET_BE_WORD(ptr) \
        MAKEWORD( ((BYTE *)(ptr))[1], \
                  ((BYTE *)(ptr))[0] )
#define PUT_BE_DWORD(ptr, d) \
        do { PUT_BE_WORD(&((WORD *)(ptr))[1], LOWORD(d)); \
             PUT_BE_WORD(&((WORD *)(ptr))[0], HIWORD(d)); } while (0)
#define GET_BE_DWORD(ptr) \
        ((DWORD)MAKELONG( GET_BE_WORD(&((WORD *)(ptr))[1]), \
                          GET_BE_WORD(&((WORD *)(ptr))[0]) ))

#if defined(ALLOW_UNALIGNED_ACCESS)
#define PUT_UA_WORD(ptr, w)  PUT_WORD(ptr, w)
#define GET_UA_WORD(ptr)     GET_WORD(ptr)
#define PUT_UA_DWORD(ptr, d) PUT_DWORD(ptr, d)
#define GET_UA_DWORD(ptr)    GET_DWORD(ptr)
#elif defined(WORDS_BIGENDIAN)
#define PUT_UA_WORD(ptr, w)  PUT_BE_WORD(ptr, w)
#define GET_UA_WORD(ptr)     GET_BE_WORD(ptr)
#define PUT_UA_DWORD(ptr, d) PUT_BE_DWORD(ptr, d)
#define GET_UA_DWORD(ptr)    GET_BE_DWORD(ptr)
#else
#define PUT_UA_WORD(ptr, w)  PUT_LE_WORD(ptr, w)
#define GET_UA_WORD(ptr)     GET_LE_WORD(ptr)
#define PUT_UA_DWORD(ptr, d) PUT_LE_DWORD(ptr, d)
#define GET_UA_DWORD(ptr)    GET_LE_DWORD(ptr)
#endif

#endif /* !defined(__WINE_WINE_PORT_H) */
