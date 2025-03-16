#include "bc/file/GetFileInfo.hpp"
#include "bc/file/Types.hpp"
#include "bc/system/file/Stacked.hpp"

namespace Blizzard {
namespace File {

bool GetFileInfo(const char* name, FileInfo* info) {
    System_File::Stacked::FileParms parms;
    parms.op      = offsetof(Filesystem, getfileinfo);
    parms.name    = name;
    parms.file    = nullptr;
    parms.setinfo = 0;
    parms.getinfo = 0xFFFFFFFF;
    parms.info    = &parms.noinfo;

    auto fs = System_File::Stacked::s_manager;
    if (fs && fs->getfileinfo(fs, &parms)) {
        *info = *parms.info;
        return true;
    }

    return false;
}

FileInfo* GetFileInfo(StreamRecord* file) {
    System_File::Stacked::FileParms parms;
    parms.op      = offsetof(Filesystem, getfileinfo);
    parms.name    = nullptr;
    parms.file    = file;
    parms.setinfo = 0;
    parms.getinfo = 0xFFFFFFFF;
    parms.info    = nullptr;

    auto fs = System_File::Stacked::s_manager;
    if (fs && fs->getfileinfo(fs, &parms)) {
        return parms.info;
    }

    static FileInfo s_noinfo = {};
    return &s_noinfo;
}

bool GetFileInfo(StreamRecord* file, FileInfo* info) {
    System_File::Stacked::FileParms parms;
    parms.op   = offsetof(Filesystem, getfileinfo);
    parms.name = nullptr;
    parms.file = file;
    parms.info = nullptr;

    auto fs = System_File::Stacked::s_manager;
    if (fs && fs->getfileinfo(fs, &parms)) {
        *info = *parms.info;
        return true;
    }

    return false;
}

} // namespace File
} // namespace Blizzard
