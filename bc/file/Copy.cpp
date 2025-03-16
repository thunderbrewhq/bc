#include "bc/file/Copy.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

bool Copy(const char* src, const char* dst, bool overwrite) {
    System_File::Stacked::FileParms parms;
    parms.op        = offsetof(Filesystem, copy);
    parms.name      = src;
    parms.newname   = dst;
    parms.overwrite = overwrite;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->copy(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
