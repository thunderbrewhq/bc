#include "bc/file/Close.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/system/file/Types.hpp"

namespace Blizzard {
namespace File {

bool Close(StreamRecord* file) {
    System_File::Stacked::FileParms parms;
    parms.op   = offsetof(Filesystem, close);
    parms.file = file;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->close(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
