#include "bc/file/GetWorkingDirectory.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/system/file/Types.hpp"

namespace Blizzard {
namespace File {

bool GetWorkingDirectory(char* buffer, int32_t buffersize) {
    System_File::Stacked::FileParms parms;
    parms.op         = offsetof(Filesystem, cwd);
    parms.buffer     = buffer;
    parms.buffersize = buffersize;

    *buffer = '\0';
    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->cwd(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
