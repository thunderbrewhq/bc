#ifndef BC_FILE_GET_WORKING_DIRECTORY_HPP
#define BC_FILE_GET_WORKING_DIRECTORY_HPP

#include <cstdint>

namespace Blizzard {
namespace File {

// cwd
bool GetWorkingDirectory(char* buffer, int32_t buffersize);

} // namespace File
} // namespace Blizzard

#endif
