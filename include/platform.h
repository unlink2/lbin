#ifndef PLATFORM_H_
#define PLATFORM_H_

/**
 * Path separator
 */
#ifdef _WIN32
#define LBIN_PATH_SEPARATOR "\\"
#elif __unix__
#define LBIN_PATH_SEPARATOR "/"
#else
#error "Unable to determine path separator"
#endif

/**
 * PATH_MAX
 */
#ifdef __linux__
#include <linux/limits.h>
#else
#define PATH_MAX 4096
#endif

int lbin_pledge(void);

void lbin_srand(void);

#endif
