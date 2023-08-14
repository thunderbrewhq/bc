#ifndef BC_OS_FILE_HPP
#define BC_OS_FILE_HPP

#include "bc/file/Types.hpp"

#include <cstdint>

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

enum EOSFileFlagsAndAttributes {
    OS_FILE_ATTRIBUTE_READONLY      = 0x1,
    OS_FILE_ATTRIBUTE_HIDDEN        = 0x2,
    OS_FILE_ATTRIBUTE_SYSTEM        = 0x4,
    OS_FILE_ATTRIBUTE_DIRECTORY     = 0x10,
    OS_FILE_ATTRIBUTE_ARCHIVE       = 0x20,
    OS_FILE_ATTRIBUTE_NORMAL        = 0x80,
    OS_FILE_ATTRIBUTE_TEMPORARY     = 0x100,
    OS_FILE_ATTRIBUTE_OFFLINE       = 0x1000,
    OS_FILE_ATTRIBUTE_ENCRYPTED     = 0x4000,

    OS_FILE_FLAG_OPEN_NO_RECALL	    = 0x00100000,
    OS_FILE_FLAG_OPEN_REPARSE_POINT	= 0x00200000,
    OS_FILE_FLAG_POSIX_SEMANTICS    = 0x01000000,
    OS_FILE_FLAG_BACKUP_SEMANTICS   = 0x02000000,
    OS_FILE_FLAG_DELETE_ON_CLOSE    = 0x04000000,
    OS_FILE_FLAG_SEQUENTIAL_SCAN    = 0x08000000,
    OS_FILE_FLAG_RANDOM_ACCESS      = 0x10000000,
    OS_FILE_FLAG_NO_BUFFERING       = 0x20000000,
    OS_FILE_FLAG_OVERLAPPED	        = 0x40000000,
    OS_FILE_FLAG_WRITE_THROUGH      = 0x80000000
};

typedef Blizzard::File::StreamRecord* HOSFILE;

HOSFILE  OsCreateFile(const char* fileName, uint32_t desiredAccess, uint32_t shareMode, uint32_t createDisposition, uint32_t flagsAndAttributes, uint32_t extendedFileType);

int32_t  OsSetFileAttributes(const char* fileName, uint32_t attributes);

uint64_t OsGetFileSize(HOSFILE fileHandle);

int32_t  OsWriteFile(HOSFILE fileHandle, void* buffer, size_t bytesToWrite, size_t* bytesWritten);

int32_t  OsReadFile(HOSFILE fileHandle, void* buffer, size_t bytesToRead, size_t* bytesRead);

void     OsCloseFile(HOSFILE fileHandle);

int64_t  OsSetFilePointer(HOSFILE fileHandle, int64_t distanceToMove, uint32_t moveMethod);

#endif
