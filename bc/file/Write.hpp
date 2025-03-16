#ifndef BC_FILE_WRITE_HPP
#define BC_FILE_WRITE_HPP

#include <cstdint>
#include "bc/file/Types.hpp"

namespace Blizzard {
namespace File {

// write
bool Write(StreamRecord* file, const void* data, int32_t count);

// write
bool Write(StreamRecord* file, const void* data, int32_t* count);

// writep
bool Write(StreamRecord* file, const void* data, int64_t offset, int32_t* count);

} // namespace File
} // namespace Blizzard

#endif
