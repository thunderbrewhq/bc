#ifndef BC_FILE_SET_ATTRIBUTES_HPP
#define BC_FILE_SET_ATTRIBUTES_HPP

#include <cstdint>

namespace Blizzard {
namespace File {

// setfileinfo
bool SetAttributes(const char* name, int32_t attributes);

} // namespace File
} // namespace Blizzard


#endif
