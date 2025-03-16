#include "bc/file/Write.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/file/Types.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/system/file/Types.hpp"

namespace Blizzard {
namespace File {

bool Write(StreamRecord* file, const void* data, int32_t count) {
    System_File::Stacked::FileParms parms;
    parms.op     = offsetof(Filesystem, write);
    parms.file   = file;
    parms.data   = const_cast<void*>(data);
    parms.offset = -1LL;
    parms.count  = count;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->write(fs, &parms) : false;
}

bool Write(StreamRecord* file, const void* data, int32_t* count) {
    System_File::Stacked::FileParms parms;
    parms.op     = offsetof(Filesystem, write);
    parms.file   = file;
    parms.data   = const_cast<void*>(data);
    parms.offset = -1LL;
    parms.count  = *count;

    auto fs = System_File::Stacked::s_manager;
    if (!fs || fs->write(fs, &parms)) {
        return false;
    }

    *count = parms.count;
    return true;
}

bool Write(StreamRecord* file, const void* data, int64_t offset, int32_t* count) {
    System_File::Stacked::FileParms parms;
    parms.op     = offsetof(Filesystem, writep);
    parms.file   = file;
    parms.data   = const_cast<void*>(data);
    parms.offset = offset;
    parms.count  = *count;

    auto fs = System_File::Stacked::s_manager;
    if (!fs || fs->writep(fs, &parms)) {
        return false;
    }

    *count = parms.count;
    return true;
}

} // namespace File
} // namespace Blizzard
