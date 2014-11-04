/*	$OpenBSD: rfc868time.c,v 1.5 2002/09/08 12:33:42 jakob Exp $	*/
/*	$NetBSD: rdate.c,v 1.4 1996/03/16 12:37:45 pk Exp $	*/

/*
 * Copyright (c) 1994 Christos Zoulas
 * All rights reserved.
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
 *	This product includes software developed by Christos Zoulas.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * rdate.c: Set the date from the specified host
 *
 *	Uses the rfc868 time protocol at socket 37 (tcp).
 *	Time is returned as the number of seconds since
 *	midnight January 1st 1900.
 */

#ifndef lint
#if 0
from: static char rcsid[] = "$NetBSD: rdate.c,v 1.3 1996/02/22 06:59:18 thorpej Exp $";
#else
static const char rcsid[] = "$OpenBSD: rfc868time.c,v 1.5 2002/09/08 12:33:42 jakob Exp $";
#endif
#endif				/* lint */

#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#include <stdio.h>
#include <ctype.h>
#include <err.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>

#ifdef __EMX__
	#include "os2ext.h"
#endif

/* Obviously it is not just for SNTP clients... */
#include "ntpleaps.h"

/* seconds from midnight Jan 1900 - 1970 */
#define DIFFERENCE 2208988800UL


void
rfc868time_client (const char *hostname, struct timeval *new,
    struct timeval *adjust, int leapflag)
{
	struct hostent *hp;
	struct servent *sp, ssp;
	struct protoent *pp, ppp;
	struct sockaddr_in sa;
	struct timeval old;
	u_int32_t tim;	/* RFC 868 states clearly this is an uint32 */
	int s;
	u_int64_t td;

	if ((hp = gethostbyname(hostname)) == NULL) {
		err(1, "%s: rc = %u\n", hostname, h_errno);
	}

	if ((sp = getservbyname("time", "tcp")) == NULL) {
		sp = &ssp;
		sp->s_port = 37;
		sp->s_proto = "tcp";
	}
	if ((pp = getprotobyname(sp->s_proto)) == NULL) {
		pp = &ppp;
		pp->p_proto = 6;
	}
	if ((s = socket(AF_INET, SOCK_STREAM, pp->p_proto)) == -1) {
		err(1, "Could not create socket\n");
	}

/*	bzero(&sa, sizeof sa); */
	memset(&sa, 0, sizeof sa);
	sa.sin_family = AF_INET;
	sa.sin_port = sp->s_port;

	(void) memcpy(&(sa.sin_addr.s_addr), hp->h_addr, hp->h_length);

	if (connect(s, (struct sockaddr *) & sa, sizeof(sa)) == -1) {
		err(1, "Could not connect socket\n");
	}

	if (read(s, &tim, sizeof(tim)) != sizeof(tim))
		err(1, "Could not read data");

	(void) close(s);
	tim = ntohl(tim) - DIFFERENCE;

	if (gettimeofday(&old, NULL) == -1)
		err(1, "Could not get local time of day");

	td = SEC_TO_TAI64(old.tv_sec);
	if (leapflag)
		ntpleaps_sub(&td);

	adjust->tv_sec = tim - TAI64_TO_SEC(td);
	adjust->tv_usec = 0;

	new->tv_sec = old.tv_sec + adjust->tv_sec;
	new->tv_usec = 0;
}
