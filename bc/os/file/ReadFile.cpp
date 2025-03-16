#include "bc/os/file/ReadFile.hpp"
#include "bc/os/file/Defines.hpp"
#include "bc/file/Read.hpp"

int32_t OsReadFile(HOSFILE fileHandle, void* buffer, uint32_t bytesToRead, uint32_t* bytesRead) {
    if (!buffer || !bytesRead) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    *bytesRead = bytesToRead;
    return Blizzard::File::Read(reinterpret_cast<Blizzard::File::StreamRecord*>(fileHandle), buffer, reinterpret_cast<int32_t*>(bytesRead));
}