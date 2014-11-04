/*-
 * Copyright (c) 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)err.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _ERR_H_
#define	_ERR_H_

#include <stdarg.h>

#if defined (__cplusplus)
extern "C" {
#endif

extern __const__ char *__progname; /* crt0 stores name of app to this global */

extern void err(int,__const__ char*,...)__attribute__((noreturn,format(printf,2,3)));
extern void verr(int,__const__ char*,va_list)__attribute__((noreturn,format(printf,2,0)));
extern void errx(int,__const__ char*,...)__attribute__((noreturn,format(printf,2,3)));
extern void verrx(int,__const__ char*,va_list)__attribute__((noreturn,format(printf,2,0)));
extern void warn(__const__ char*,...)__attribute__((format(printf,1,2)));
extern void vwarn(__const__ char*,va_list)__attribute__((format(printf,1,0)));
extern void warnx(__const__ char*,...)__attribute__((format(printf,1,2)));
extern void vwarnx(__const__ char*,va_list)__attribute__((format(printf,1,0)));

/*
* The _* versions are for use in library functions so user-defined
 * versions of err*,warn* do not get used.
 */
#ifndef _USE_APPLICATION_ERR_

extern void _err(int,__const__ char*,...)__attribute__((noreturn,format(printf,2,3)));
extern void _verr(int,__const__ char*,va_list)__attribute__((noreturn,format(printf,2,0)));
extern void _errx(int,__const__ char*,...)__attribute__((noreturn,format(printf,2,3)));
extern void _verrx(int,__const__ char*,va_list)__attribute__((noreturn,format(printf,2,0)));
extern void _warn(__const__ char*,...)__attribute__((format(printf,1,2)));
extern void _vwarn(__const__ char*,va_list)__attribute__((format(printf,1,0)));
extern void _warnx(__const__ char*,...)__attribute__((format(printf,1,2)));
extern void _vwarnx(__const__ char*,va_list)__attribute__((format(printf,1,0)));

#define err _err
#define verr _verr
#define errx _errx
#define verrx _verrx
#define warn _warn
#define vwarn _vwarn
#define warnx _warnx
#define vwarnx _vwarnx

#endif

#if defined (__cplusplus)
}
#endif

#endif /* !_ERR_H_ */
