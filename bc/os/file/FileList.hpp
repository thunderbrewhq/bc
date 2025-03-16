#ifndef BC_OS_FILE_FILE_LIST_HPP
#define BC_OS_FILE_FILE_LIST_HPP

#include <cstdint>

class OS_FILE_DATA {
    public:
    uint32_t size;
    uint32_t flags;
    char fileName[260];
};

typedef int32_t (*OsFileListCallback)(const OS_FILE_DATA& data, void* param);

int32_t OsFileList(const char* dir, const char* pattern, OsFileListCallback callback, void* param, int32_t flags);

#endif