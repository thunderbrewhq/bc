#include "bc/file/GetPos.hpp"
#include "bc/file/Defines.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

bool GetPos(StreamRecord* file, int64_t& offset) {
    System_File::Stacked::FileParms parms;
    parms.op     = offsetof(Filesystem, getpos);
    parms.file   = file;
    parms.offset = offset;
    parms.whence = BC_FILE_SEEK_CURRENT;

    auto fs = System_File::Stacked::s_manager;
    if (!fs || !fs->setpos(fs, &parms)) {
        return false;
    }
    offset = parms.offset;
    return true;
}

} // namespace File
} // namespace Blizzard
