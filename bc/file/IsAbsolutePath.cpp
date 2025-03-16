#include "bc/file/IsAbsolutePath.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/system/file/Types.hpp"

namespace Blizzard {
namespace File {

bool IsAbsolutePath(const char* name) {
    System_File::Stacked::FileParms parms;
    parms.op   = offsetof(Filesystem, isabspath);
    parms.name = name;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->isabspath(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard