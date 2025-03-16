#include "bc/file/MakeAbsolutePath.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/system/file/Types.hpp"

namespace Blizzard {
namespace File {

bool MakeAbsolutePath(const char* name, char* buffer, int32_t buffersize, bool canonicalize) {
    System_File::Stacked::FileParms parms;
    parms.op           = offsetof(Filesystem, makeabspath);
    parms.name         = name;
    parms.buffer       = buffer;
    parms.buffersize   = buffersize;
    parms.canonicalize = canonicalize;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->makeabspath(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard