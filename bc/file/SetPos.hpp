#ifndef BC_FILE_SET_POS_HPP
#define BC_FILE_SET_POS_HPP

#include <cstdint>
#include "bc/file/Types.hpp"

namespace Blizzard {
namespace File {

// setpos
bool SetPos(StreamRecord* file, int64_t offset, int32_t whence);

} // namespace File
} // namespace Blizzard


#endif
