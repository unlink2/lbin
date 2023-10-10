#ifndef ENV_H_
#define ENV_H_

#define LBIN_ENV_ECHO "LBIN_ECHO"
#define LBIN_ENV_NO_HEADERS "LBIN_NO_HEADERS"
#define LBIN_ENV_IN "LBIN_IN"
#define LBIN_ENV_OUT "LBIN_OUT"
#define LBIN_ENV_BASE_PATH "LBIN_BASEPATH"
#define LBIN_ENV_FILE_PATH "LBIN_FILEPATH"
#define LBIN_ENV_NO_CHK_FILENAME "LBIN_NOFILENAMECHK"

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
