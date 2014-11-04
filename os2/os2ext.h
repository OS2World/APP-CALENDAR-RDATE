#ifndef __OS2EXT_H__
#define __OS2EXT_H__

#include <sys/types.h>
#include <sys/time.h>

typedef long int		int32_t;
typedef unsigned char		u_int8_t;
typedef unsigned long int	u_int32_t;
typedef unsigned long long	u_int64_t;
typedef int32_t fd_mask;
#define NBBY			8				/* number of bits in a byte */
#define NFDBITS			(sizeof(fd_mask) * NBBY)	/* bits per mask */
#define howmany(x, y)   	(((x)+((y)-1))/(y))

/* fakelink.c */
int link(const char*, const char*);

/* From OpenBSD's libc */
extern size_t strlcat(char*, const char*, size_t);
extern size_t strlcpy(char*, const char*, size_t);

/*
 * CCHMAXPATH connot be use because this would mean that we
 * would have to include os2.h in every single source file
 */
#define __CCHMAXPATH	260

void __OS2_SETUP_PATH(void);
extern char __OS2_TZDIR[__CCHMAXPATH+1];
extern char __OS2_TZDEFAULT[__CCHMAXPATH+1];

#endif
