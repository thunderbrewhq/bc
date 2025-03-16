#include "bc/file/Read.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

bool Read(StreamRecord* file, void* buffer, int32_t* count) {
    System_File::Stacked::FileParms parms;
    parms.op    = offsetof(Filesystem, read);
    parms.file  = file;
    parms.data  = buffer;
    parms.count = *count;
    *count      = 0;

    auto fs = System_File::Stacked::s_manager;
    if (!fs || !fs->read(fs, &parms)) {
        return false;
    }
    *count = parms.count;
    return true;
}

bool Read(StreamRecord* file, void* buffer, int64_t offset, int32_t* count) {
    System_File::Stacked::FileParms parms;
    parms.op     = offsetof(Filesystem, readp);
    parms.file   = file;
    parms.data   = buffer;
    parms.offset = offset;
    parms.count  = *count;
    *count       = 0;

    auto fs = System_File::Stacked::s_manager;
    if (!fs || !fs->readp(fs, &parms)) {
        return false;
    }
    *count = parms.count;
    return true;
}

} // namespace File
} // namespace Blizzard
