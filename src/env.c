#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int lbin_srand(void) {
  int urand = open("/dev/urandom", O_RDONLY | O_CLOEXEC);
  if (urand == -1) {
    fprintf(stderr, "%s\n", strerror(errno));
    return -1;
  }

  unsigned int r = 0;
  read(urand, &r, sizeof(unsigned int));
  assert(r);
  fprintf(stderr, "%d\n", r);
  srand(r);

  close(urand);

  return 0;
}

#else
void lbin_srand(void) { srand(time(NULL)); }
#endif

#endif
