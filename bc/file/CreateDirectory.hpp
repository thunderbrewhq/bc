#ifndef BC_FILE_CREATE_DIRECTORY_HPP
#define BC_FILE_CREATE_DIRECTORY_HPP

#include <cstdint>

namespace Blizzard {
namespace File {

// mkdir
bool CreateDirectory(const char* name, bool recurse);

} // namespace File
} // namespace Blizzard

#endif
