#include "bc/file/Open.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/system/file/Types.hpp"

namespace Blizzard {
namespace File {

bool Open(const char* name, int32_t mode, StreamRecord*& file) {
    System_File::Stacked::FileParms parms;
    parms.op    = offsetof(Filesystem, open);
    parms.name  = name;
    parms.mode  = mode;
    parms.file  = nullptr;

    auto fs = System_File::Stacked::s_manager;
    if (!fs || !fs->open(fs, &parms)) {
        return false;
    }
    file = parms.file;
    return true;
}

} // namespace File
} // namespace Blizzard
