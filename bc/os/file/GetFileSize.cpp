#include "bc/os/file/GetFileSize.hpp"
#include "bc/file/GetFileInfo.hpp"
#include <limits>

uint64_t OsGetFileSize(HOSFILE fileHandle) {
    auto info = Blizzard::File::GetFileInfo(reinterpret_cast<Blizzard::File::StreamRecord*>(fileHandle));
    return info ? info->size : std::numeric_limits<uint64_t>::max();
}