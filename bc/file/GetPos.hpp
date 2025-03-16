#ifndef BC_FILE_GET_POS_HPP
#define BC_FILE_GET_POS_HPP

#include <cstdint>
#include "bc/file/Types.hpp"

namespace Blizzard {
namespace File {

// getpos
bool GetPos(StreamRecord* file, int64_t& offset);

} // namespace File
} // namespace Blizzard


#endif
