#include "bc/file/SetWorkingDirectory.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/system/file/Types.hpp"

namespace Blizzard {
namespace File {

bool SetWorkingDirectory(const char* name) {
    System_File::Stacked::FileParms parms;
    parms.op   = offsetof(Filesystem, cd);
    parms.name = name;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->cd(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
