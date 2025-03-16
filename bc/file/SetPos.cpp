#include "bc/file/SetPos.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

bool SetPos(StreamRecord* file, int64_t offset, int32_t whence) {
    System_File::Stacked::FileParms parms;
    parms.op     = offsetof(Filesystem, setpos);
    parms.file   = file;
    parms.offset = offset;
    parms.whence = whence;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->setpos(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
