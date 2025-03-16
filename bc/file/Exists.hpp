#ifndef BC_FILE_EXISTS_HPP
#define BC_FILE_EXISTS_HPP

#include <cstdint>

namespace Blizzard {
namespace File {

// exists
uint32_t Exists(const char* name);

// exists
bool IsFile(const char* name);

// exists
bool IsDirectory(const char* name);

} // namespace File
} // namespace Blizzard

#endif
