#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#define INCL_DOSMODULEMGR
#define INCL_LONGLONG
#include <os2.h>

#include "os2ext.h"

char __OS2_TZDIR[__CCHMAXPATH+1];
char __OS2_TZDEFAULT[__CCHMAXPATH+1];

void __OS2_SETUP_PATH(void) {
	APIRET		rc;
	PIB*		my_pib;
	char*		cut_pos;
	char*		env_unixroot;
	char*		env_home;

	memset(&__OS2_TZDIR, 0, sizeof(__OS2_TZDIR));
	memset(&__OS2_TZDEFAULT, 0, sizeof(__OS2_TZDEFAULT));

	env_unixroot = getenv("UNIXROOT");

	if (env_unixroot != NULL) {
		strlcpy((char*)&__OS2_TZDIR, env_unixroot, __CCHMAXPATH-30);
		strlcpy((char*)&__OS2_TZDEFAULT, env_unixroot, __CCHMAXPATH-30);
	}

	strcat((char*)&__OS2_TZDIR, "/usr/share/zoneinfo");
	strcat((char*)&__OS2_TZDEFAULT, "/etc/localtime");
}

