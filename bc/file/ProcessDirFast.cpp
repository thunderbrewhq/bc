#include "bc/file/ProcessDirFast.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

bool ProcessDirFast(const char* name, void* param, ProcessDirCallback callback, bool nofollow) {
    System_File::Stacked::FileParms parms;
    parms.op              = offsetof(Filesystem, dirwalk);
    parms.name            = name;
    parms.dirwalkparam    = param;
    parms.dirwalkcallback = callback;
    // parms.unk88           = false;
    parms.mode            = nofollow;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->dirwalk(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
