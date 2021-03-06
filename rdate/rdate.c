/*	$OpenBSD: rdate.c,v 1.19 2002/09/08 12:33:42 jakob Exp $	*/
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
 *	Uses the rfc868 time protocol at socket 37.
 *	Time is returned as the number of seconds since
 *	midnight January 1st 1900.
 */
#ifndef lint
#if 0
from: static char rcsid[] = "$NetBSD: rdate.c,v 1.3 1996/02/22 06:59:18 thorpej Exp $";
#else
static const char rcsid[] = "$OpenBSD: rdate.c,v 1.19 2002/09/08 12:33:42 jakob Exp $";
#endif
#endif				/* lint */


#include <sys/param.h>
#include <sys/time.h>
#include <emx/time.h>

#include <stdio.h>
#include <ctype.h>
#include <err.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

/* there are systems without libutil; for portability */
#ifndef NO_UTIL
#include <util.h>
#else
#define logwtmp(a,b,c)
#endif

#ifdef __EMX__
	#include "os2ext.h"
#endif

void rfc868time_client (const char *, struct timeval *, struct timeval *, int);
void ntp_client (const char *, struct timeval *, struct timeval *, int);

extern const char    *__progname;

void
usage()
{
	(void) fprintf(stderr, "Usage: %s [-ncpsa] host\n", __progname);
	(void) fprintf(stderr, "  -n: use SNTP instead of RFC868 time protocol\n");
	(void) fprintf(stderr, "  -c: correct leap second count\n");
	(void) fprintf(stderr, "  -p: just print, don't set\n");
	(void) fprintf(stderr, "  -s: just set, don't print\n");
	(void) fprintf(stderr, "  -v: verbose output\n");
}

int
main(int argc, char **argv)
{
	int             pr = 0, silent = 0, ntp = 0, verbose = 0;
	int		slidetime = 0, corrleaps = 0;
	char           *hname;
	extern int      optind;
	int             c;

	struct timeval new, new_local, adjust;

	if (argc == 0)
		return -1;

	__progname = argv[0];

	while ((c = getopt(argc, argv, "psncv")) != -1)
		switch (c) {
		case 'p':
			pr++;
			break;

		case 's':
			silent++;
			break;

		case 'n':
			ntp++;
			break;

		case 'c':
			corrleaps = 1;
			break;

		case 'v':
			verbose++;
			break;

		case 'a':
			/* Not supported for OS/2 - ignore */
			break;

		default:
			usage();
			return 1;
		}

	if (argc - 1 != optind) {
		usage();
		return 1;
	}
	hname = argv[optind];

	if (ntp)
		ntp_client(hname, &new, &adjust, corrleaps);
	else
		rfc868time_client(hname, &new, &adjust, corrleaps);

	if (!pr) {
		/*
		   Work around a bug (?) in EMX's settimeofday(), which
		   does not take the local timezone into account
		*/
		if (!_tzset_flag) _tzset ();
		new_local = new;
		_gmt2loc(&new_local.tv_sec);

		if (settimeofday(&new_local, NULL) == -1)
			err(1, "Could not set time of day");
	}

	if (!silent) {
		struct tm      *ltm;
		char		buf[80];
		time_t		tim = new.tv_sec;
		double		adjsec;

		ltm = localtime(&tim);
		(void) strftime(buf, sizeof buf, "%a %b %e %H:%M:%S %Z %Y\n", ltm);
		(void) fputs(buf, stdout);

		adjsec  = adjust.tv_sec + adjust.tv_usec / 1.0e6;

		if (slidetime || verbose) {
			if (ntp)
				(void) fprintf(stdout,
				   "%s: adjust local clock by %.6f seconds\n",
				   __progname, adjsec);
			else
				(void) fprintf(stdout,
				   "%s: adjust local clock by %ld seconds\n",
				   __progname, adjust.tv_sec);
		}
	}

	return 0;
}
