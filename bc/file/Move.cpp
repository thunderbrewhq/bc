#include "bc/file/Move.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

bool Move(const char* src, const char* dst) {
    System_File::Stacked::FileParms parms;
    parms.op        = offsetof(Filesystem, move);
    parms.name      = src;
    parms.newname   = dst;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->move(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
