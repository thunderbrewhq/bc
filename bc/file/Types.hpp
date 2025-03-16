#ifndef BC_FILE_TYPES_HPP
#define BC_FILE_TYPES_HPP

#include "bc/system/file/Defines.hpp"
#include "bc/time/Time.hpp"

#if defined(WHOA_SYSTEM_WIN)
typedef void* HANDLE;
#endif

#include <cstddef>

namespace Blizzard {
namespace File {

// Types

enum Mode {
    read         = 0x0001,
    write        = 0x0002,
    shareread    = 0x0004,
    sharewrite   = 0x0008,
    nocache      = 0x0040,
    temporary    = 0x0080,
    truncate     = 0x0100,
    append       = 0x0200,
    create       = 0x0400,
    mustnotexist = 0x0800,
    mustexist    = 0x1000
};

class FileInfo {
    public:
    const char* name;
    int32_t unk04;
    uint64_t size;
    int32_t attributes;
    Time::Timestamp createtime;
    Time::Timestamp writetime;
    Time::Timestamp accesstime;
    int32_t filetype;
    int32_t normal;
};

class ProcessDirParms {
    public:
    const char* dir;
    const char* item;
    void* param;
    bool isdir;
};

typedef bool (*ProcessDirCallback)(const ProcessDirParms&);

class StreamRecord {
    public:
#if defined(WHOA_SYSTEM_WIN)
    HANDLE filehandle;
#endif
#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
    int filefd;
#endif
    int32_t mode;
    bool haveinfo;
    uint32_t unk0C;
    Blizzard::File::FileInfo info;
    int32_t* unk48;
    const char* name;
};

} // namespace File
} // namespace Blizzard

#endif
