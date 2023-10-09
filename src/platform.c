#include "platform.h"

#ifdef __OpenBSD__

#define LBIN_PLEDGE "stdio"

#include <unistd.h>

int lbin_pledge(void) { return pledge(LBIN_PLEDGE, NULL); }

#else

int lbin_pledge(void) { return 0; }

#endif
