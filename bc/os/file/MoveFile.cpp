#include "bc/os/file/MoveFile.hpp"
#include "bc/os/file/Defines.hpp"
#include "bc/file/Move.hpp"

int32_t OsMoveFile(const char* existingFileName, const char* newFileName) {
    if (!existingFileName || !newFileName) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    return Blizzard::File::Move(existingFileName, newFileName);
}