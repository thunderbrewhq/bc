#ifndef BC_FILE_GET_FILE_INFO_HPP
#define BC_FILE_GET_FILE_INFO_HPP

#include "bc/file/Types.hpp"

namespace Blizzard {
namespace File {

// getfileinfo
bool GetFileInfo(const char* name, FileInfo* info);

// getfileinfo
FileInfo* GetFileInfo(StreamRecord* file);

// getfileinfo
bool GetFileInfo(StreamRecord* file, FileInfo* info);

} // namespace File
} // namespace Blizzard


#endif
