#include "bc/os/file/GetCurrentDirectory.hpp"
#include "bc/file/GetWorkingDirectory.hpp"
#include "bc/os/file/Defines.hpp"

int32_t OsGetCurrentDirectory(uint32_t buffersize, char* buffer) {
    if (!buffer) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::GetWorkingDirectory(buffer, buffersize);
}
