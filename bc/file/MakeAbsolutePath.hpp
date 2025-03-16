#ifndef BC_FILE_MAKE_ABSOLUTE_PATH_HPP
#define BC_FILE_MAKE_ABSOLUTE_PATH_HPP

#include <cstdint>

namespace Blizzard {
namespace File {

// makeabspath
bool MakeAbsolutePath(const char* name, char* buffer, int32_t buffersize, bool canonicalize);

} // namespace File
} // namespace Blizzard

#endif
