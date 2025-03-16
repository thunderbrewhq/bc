#include "bc/os/file/GetFileAttributes.hpp"
#include "bc/file/GetFileInfo.hpp"
#include "bc/os/file/Defines.hpp"

int32_t OsGetFileAttributes(const char* fileName) {
    if (!fileName) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    Blizzard::File::FileInfo info;
    if (!Blizzard::File::GetFileInfo(fileName, &info)) {
        return -1;
    }

    return info.attributes;
}