#ifndef BC_FILE_ERROR_HPP
#define BC_FILE_ERROR_HPP

#include <cstdint>

namespace Blizzard {
namespace File {

// Functions
void SetLastError(int32_t errorcode);

void AddToLastErrorStack(int32_t errorcode, const char* msg, int32_t param_3);

} // namespace File
} // namespace Blizzard

#endif
