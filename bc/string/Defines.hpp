#ifndef BC_STRING_DEFINES_HPP
#define BC_STRING_DEFINES_HPP

// How many bytes to when translating stacked filesystem names to large, native file paths
#define BC_STRING_MAX_PATH 1024

#if defined(WHOA_SYSTEM_WIN)
#define BC_STRING_PATH_SEPARATOR '\\'
#else
#define BC_STRING_PATH_SEPARATOR '/'
#endif

#endif
