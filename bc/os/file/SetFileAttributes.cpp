#include "bc/os/file/SetFileAttributes.hpp"
#include "bc/file/SetAttributes.hpp"
#include "bc/os/file/Defines.hpp"

int32_t OsSetFileAttributes(const char* fileName, uint32_t attributes) {
    if (!fileName) {
        // SetLastError(0x57);
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::SetAttributes(fileName, attributes);
}