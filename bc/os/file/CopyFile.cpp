#include "bc/os/file/CopyFile.hpp"
#include "bc/os/file/Defines.hpp"
#include "bc/file/Copy.hpp"

int32_t OsCopyFile(const char* existingFileName, const char* newFileName, int32_t failIfExists) {
    if (!existingFileName || !newFileName) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::Copy(existingFileName, newFileName, failIfExists == 0);
}