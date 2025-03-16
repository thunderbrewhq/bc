#include "bc/os/file/CreateDirectory.hpp"
#include "bc/file/CreateDirectory.hpp"
#include "bc/os/file/Defines.hpp"

int32_t OsCreateDirectory(const char* pathName, int32_t recursive) {
    if (pathName == nullptr) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::CreateDirectory(pathName, recursive != 0);
}