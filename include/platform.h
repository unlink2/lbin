#ifndef PLATFORM_H_
#define PLATFORM_H_

#ifdef _WIN32
#define LBIN_PATH_SEPARATOR "\\"
#elif __unix__
#define LBIN_PATH_SEPARATOR "/"
#else
#error "Unable to determine path separator"
#endif

int lbin_pledge(void);

#endif
