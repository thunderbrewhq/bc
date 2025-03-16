#include "bc/file/Exists.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

uint32_t Exists(const char* name) {
    System_File::Stacked::FileParms parms;

    parms.op   = offsetof(Filesystem, exists);
    parms.name = name;
    parms.info = &parms.noinfo;

    auto fs = System_File::Stacked::s_manager;

    if (name && fs && fs->exists(fs, &parms)) {
        return parms.info->filetype;
    } else {
        return 0;
    }
}

bool IsFile(const char* name) {
    return Exists(name) == 1;
}

bool IsDirectory(const char* name) {
    return Exists(name) == 2;
}

} // namespace File
} // namespace Blizzard


