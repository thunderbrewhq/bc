#ifndef BC_FILE_PROCESS_DIR_FAST_HPP
#define BC_FILE_PROCESS_DIR_FAST_HPP

#include "bc/file/Types.hpp"

namespace Blizzard {
namespace File {

// dirwalk
bool ProcessDirFast(const char* name, void* param, ProcessDirCallback callback, bool flag);

} // namespace File
} // namespace Blizzard

#endif
