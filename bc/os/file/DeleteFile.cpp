#include "bc/os/file/DeleteFile.hpp"
#include "bc/os/file/Defines.hpp"
#include "bc/file/Delete.hpp"

int32_t OsDeleteFile(const char* fileName) {
    if (!fileName) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::Delete(fileName);
}