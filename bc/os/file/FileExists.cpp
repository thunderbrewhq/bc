#include "bc/os/file/FileExists.hpp"
#include "bc/file/Exists.hpp"

int32_t OsFileExists(const char* path) {
    return Blizzard::File::IsFile(path);
}