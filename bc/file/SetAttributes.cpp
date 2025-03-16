#include "bc/file/SetAttributes.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/file/Defines.hpp"
#include "bc/system/file/Stacked.hpp"
#include <cstddef>

namespace Blizzard {
namespace File {

bool SetAttributes(const char* name, int32_t attributes) {
    System_File::Stacked::FileParms parms;
    parms.op                = offsetof(Filesystem, setfileinfo);
    parms.name              = name;
    parms.setinfo           = BC_SYSTEM_FILE_INFO_ATTRIBUTES;
    parms.noinfo.attributes = attributes;
    parms.info              = &parms.noinfo;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->setfileinfo(fs, &parms) : false;
}

} // namespace File
} // namespace Blizzard
