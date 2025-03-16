#include "bc/file/CreateDirectory.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

bool CreateDirectory(const char* name, bool recurse) {
    System_File::Stacked::FileParms parms;
    parms.op      = offsetof(Filesystem, mkdir);
    parms.name    = name;
    parms.recurse = recurse;
    parms.set_acl = 0;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->mkdir(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
