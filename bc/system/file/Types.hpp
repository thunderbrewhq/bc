#ifndef BC_FILE_SYSTEM_TYPES_HPP
#define BC_FILE_SYSTEM_TYPES_HPP

#include "bc/Debug.hpp"
#include "bc/file/Types.hpp"
#include <cstdint>

namespace System_File {

class FileError {
    public:
    // Error codes can be any value, even negative.
    // Look for common error codes in Defines.hpp
    // on POSIX, errorcode could be a POSIX error offset +100.
    int32_t errorcode;
    Blizzard::Debug::ErrorStackRecord* stack;
};

namespace Stacked {

// FileParms stores parameters useful to all filesystem interactions
// so all fs calls can use one signature
class FileParms {
    public:
    uint32_t op;
    const char* name;
    const char* newname;
    Blizzard::File::StreamRecord* file;
    Blizzard::File::FileInfo* info;
    Blizzard::File::FileInfo noinfo;
    uint32_t setinfo;
    uint32_t getinfo;
    int32_t mode;
    void* data;
    int32_t count;
    int64_t offset;
    int32_t whence;
    char* buffer;
    int32_t buffersize;
    bool recurse;
    bool canonicalize;
    void* dirwalkparam;
    Blizzard::File::ProcessDirCallback dirwalkcallback;
    bool overwrite;
    bool set_acl;
};

} // namespace Stacked

} // namespace System_File

#endif
