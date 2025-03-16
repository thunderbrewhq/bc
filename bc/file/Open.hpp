#ifndef BC_FILE_OPEN_HPP
#define BC_FILE_OPEN_HPP

#include <cstdint>
#include "bc/file/Types.hpp"

namespace Blizzard {
namespace File {

// open
bool Open(const char* name, int32_t mode, StreamRecord*& file);

} // namespace File
} // namespace Blizzard

#endif
