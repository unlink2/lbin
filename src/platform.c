#include "platform.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __OpenBSD__

#define LBIN_PLEDGE "stdio"
#include <unistd.h>
int lbin_pledge(void) { return pledge(LBIN_PLEDGE, NULL); }

#else

int lbin_pledge(void) { return 0; }

#ifdef __unix__

#include <unistd.h>
#include <fcntl.h>

void lbin_srand(void) {
  int urand = open("/dev/urandom", O_RDONLY | O_CLOEXEC);

  unsigned int rand = 0;
  read(urand, &rand, sizeof(unsigned int));
  assert(rand);
  srand(rand);

  close(urand);
}

#else
void lbin_srand(void) { srand(time(NULL)); }
#endif

#endif
