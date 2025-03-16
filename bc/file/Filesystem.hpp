#ifndef BC_FILE_FILESYSTEM_HPP
#define BC_FILE_FILESYSTEM_HPP

#include "bc/system/file/Types.hpp"
#include <cstdint>

namespace Blizzard {
namespace File {

class Filesystem;

typedef bool (*Operation)(Filesystem* fs, System_File::Stacked::FileParms* parms);

// Filesystem describes the layout of a virtual filesystem.
// It is linked to other Filesystems in a list structure.
class Filesystem {
    public:
        // The source filesystem (where it was copied from)
        // You can tell if base == this, that it is the original stack and not part of the filesystem manager
        Filesystem* base;
        // The next filesystem (towards lower filesystems)
        Filesystem* next;

        Operation cd;
        Operation close;
        Operation create;
        Operation cwd;
        Operation dirwalk;
        Operation exists;
        Operation flush;
        Operation getfileinfo;
        Operation getfreespace;
        Operation getpos;
        Operation getrootchars;
        Operation isabspath;
        Operation isreadonly;
        Operation makeabspath;
        Operation mkdir;
        Operation move;
        Operation copy;
        Operation open;
        Operation read;
        Operation readp;
        Operation rmdir;
        Operation setcachemode;
        Operation seteof;
        Operation setfileinfo;
        Operation setpos;
        Operation unlink;
        Operation write;
        Operation writep;
        Operation shutdown;
};

} // namespace File
} // namespace Blizzard

#endif
