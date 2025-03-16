#include "bc/os/file/WriteFile.hpp"
#include "bc/file/Write.hpp"
#include "bc/os/file/Defines.hpp"

int32_t OsWriteFile(HOSFILE fileHandle, void* buffer, uint32_t bytesToWrite, uint32_t* bytesWritten) {
    if (!buffer || !bytesWritten) {
        OS_FILE_SET_LAST_ERROR(OS_FILE_ERROR_INVALID_PARAMETER);
        return 0;
    }

    *bytesWritten = bytesToWrite;
    return Blizzard::File::Write(reinterpret_cast<Blizzard::File::StreamRecord*>(fileHandle), buffer, reinterpret_cast<int32_t*>(bytesWritten));
}