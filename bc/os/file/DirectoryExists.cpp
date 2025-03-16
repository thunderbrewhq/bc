#include "bc/os/file/DirectoryExists.hpp"
#include "bc/file/Exists.hpp"

int32_t OsDirectoryExists(const char* dirName) {
    return Blizzard::File::IsDirectory(dirName);
}