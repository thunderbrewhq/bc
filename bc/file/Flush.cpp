#include "bc/file/Flush.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

// flush
bool Flush(StreamRecord* file) {
    System_File::Stacked::FileParms parms;
    parms.op   = offsetof(Filesystem, flush);
    parms.file = file;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->flush(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard