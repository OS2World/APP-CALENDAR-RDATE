#include "os2ext.h"

#define INCL_DOSERRORS
#define INCL_DOSFILEMGR
#include <os2.h>

int link(const char * fromname, const char * toname) {
	APIRET		rc;

	rc = DosCopy(fromname, toname, DCPY_EXISTING);
	if (rc != NO_ERROR)
		return -1;

	return 0;
}

