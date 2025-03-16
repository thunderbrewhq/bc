#ifndef BC_FILE_READ_HPP
#define BC_FILE_READ_HPP

#include <cstdint>
#include "bc/file/Types.hpp"

namespace Blizzard {
namespace File {

// read
bool Read(StreamRecord* file, void* buffer, int32_t* count);

// readp
bool Read(StreamRecord* file, void* buffer, int64_t offset, int32_t* count);

} // namespace File
} // namespace Blizzard

#endif
