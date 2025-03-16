#ifndef BC_OS_FILE_TYPES_HPP
#define BC_OS_FILE_TYPES_HPP

#include <cstdint>

#define OS_FILE_ATTRIBUTE_NORMAL 0x80

// this is passed into last argument of OsCreateFile
#define OS_FILE_TYPE_DEFAULT 0x3F3F3F3F

enum EOSFileDisposition {
    OS_CREATE_NEW        = 1,
    OS_CREATE_ALWAYS     = 2,
    OS_OPEN_EXISTING     = 3,
    OS_OPEN_ALWAYS       = 4,
    OS_TRUNCATE_EXISTING = 5
};

enum EOSFileAccess {
    OS_GENERIC_ALL     = 0x10000000,
    OS_GENERIC_EXECUTE = 0x20000000,
    OS_GENERIC_WRITE   = 0x40000000,
    OS_GENERIC_READ    = 0x80000000
};

enum EOSFileShare {
    OS_FILE_SHARE_READ  = 0x00000001,
    OS_FILE_SHARE_WRITE = 0x00000002
};

// TODO: generate a proper handle here
struct HOSFILE__ {
    int32_t unused;
};

typedef HOSFILE__* HOSFILE;

constexpr HOSFILE HOSFILE_INVALID = nullptr;

#endif