#ifndef BC_FILE_REMOVE_DIRECTORY_HPP
#define BC_FILE_REMOVE_DIRECTORY_HPP

#include <cstdint>

namespace Blizzard {
namespace File {

// rmdir
bool RemoveDirectory(const char* name);

// rmdir (-r)
bool RemoveDirectoryAndContents(const char* name, bool a2);

} // namespace File
} // namespace Blizzard

#endif
