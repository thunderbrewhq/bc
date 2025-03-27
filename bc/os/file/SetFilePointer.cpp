#include "bc/os/file/SetFilePointer.hpp"
#include "bc/Debug.hpp"
#include "bc/file/GetPos.hpp"
#include "bc/file/SetPos.hpp"
#include "bc/file/Defines.hpp"

int64_t OsSetFilePointer(HOSFILE fileHandle, int64_t distanceToMove, uint32_t moveMethod) {
    BC_ASSERT(moveMethod <= 2);

    auto file = reinterpret_cast<Blizzard::File::StreamRecord*>(fileHandle);

    int64_t offset;
    if (moveMethod != 0 && !Blizzard::File::GetPos(file, offset)) {
        return -1LL;
    }

    uint32_t whence[] = {
        BC_FILE_SEEK_START,
        BC_FILE_SEEK_CURRENT,
        BC_FILE_SEEK_END
    };

    if (!Blizzard::File::SetPos(file, offset, whence[moveMethod])) {
        return -1LL;
    }

    return offset + distanceToMove;
}
