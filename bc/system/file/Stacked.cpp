
#include "bc/file/Filesystem.hpp"
#include "bc/system/file/System_File.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/String.hpp"
#include "bc/Memory.hpp"

namespace System_File {
namespace Stacked {

bool null_cd(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_close(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_create(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_cwd(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_dirwalk(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_exists(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_flush(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_getfileinfo(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_getfreespace(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_getpos(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_getrootchars(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_isabspath(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_isreadonly(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_makeabspath(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_mkdir(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_move(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_copy(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_open(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_read(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_readp(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_rmdir(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_setcachemode(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_seteof(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_setfileinfo(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_setpos(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_unlink(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_write(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_writep(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}
bool null_shutdown(Blizzard::File::Filesystem* fs, FileParms* parms) {
    return false;
}

// null stack: does absolutely nothing
// only purpose is to store fallback functions
static Blizzard::File::Filesystem s_nullstack = {
    &s_nullstack,

    nullptr,

    null_cd,
    null_close,
    null_create,
    null_cwd,
    null_dirwalk,
    null_exists,
    null_flush,
    null_getfileinfo,
    null_getfreespace,
    null_getpos,
    null_getrootchars,
    null_isabspath,
    null_isreadonly,
    null_makeabspath,
    null_mkdir,
    null_move,
    null_copy,
    null_open,
    null_read,
    null_readp,
    null_rmdir,
    null_setcachemode,
    null_seteof,
    null_setfileinfo,
    null_setpos,
    null_unlink,
    null_write,
    null_writep,
    null_shutdown
};

// base stack: contains the base System_File functions.
static Blizzard::File::Filesystem s_basestack = {
    &s_basestack,

    nullptr,

    System_File::SetWorkingDirectory,
    System_File::Close,
    System_File::Create,
    System_File::GetWorkingDirectory,
    System_File::ProcessDirFast,
    System_File::Exists,
    System_File::Flush,
    System_File::GetFileInfo,
    System_File::GetFreeSpace,
    System_File::GetPos,
    System_File::GetRootChars,
    System_File::IsAbsolutePath,
    System_File::IsReadOnly,
    System_File::MakeAbsolutePath,
    System_File::CreateDirectory,
    System_File::Move,
    System_File::Copy,
    System_File::Open,
    System_File::Read,
    System_File::ReadP,
    System_File::RemoveDirectory,
    System_File::SetCacheMode,
    System_File::SetEOF,
    System_File::SetAttributes,
    System_File::SetPos,
    System_File::Delete,
    System_File::Write,
    System_File::WriteP,
    System_File::Shutdown
};

// File initialization stack: this is the default
static Blizzard::File::Filesystem s_fileinit = {
    &s_fileinit,

    nullptr,

    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init,
    file_init
};

// manager will be initialized upon first use
Blizzard::File::Filesystem* s_manager = &s_fileinit;

// push a Filesystem onto the stack
void Push(Blizzard::File::Filesystem* fs) {
    auto head = reinterpret_cast<Blizzard::File::Filesystem*>(Blizzard::Memory::Allocate(sizeof(Blizzard::File::Filesystem)));
    Blizzard::String::MemFill(head, sizeof(Blizzard::File::Filesystem), 0);
    Blizzard::String::MemCopy(head, fs, sizeof(Blizzard::File::Filesystem));

    head->next = s_manager;
    s_manager = head;

    HoistAll();
}

// file_init is part of the default filesystem stack
// the first use of s_manager will call this function only once; it is a fallback function in case s_basestack's funcs are not yet hoisted.
bool file_init(Blizzard::File::Filesystem* fs, FileParms* parms) {
    // allocate a null stack, replacing what was previously a pointer to s_fileinit
    s_manager = reinterpret_cast<Blizzard::File::Filesystem*>(Blizzard::Memory::Allocate(sizeof(Blizzard::File::Filesystem)));
    if (s_manager == nullptr) {
        return false;
    }

    // add null fs calls to stack
    // after Push/HoistAll, if a filesystem call is unimplemented, the func from s_nullstack gets called instead.
    Blizzard::String::MemCopy(s_manager, &s_nullstack, sizeof(Blizzard::File::Filesystem));

    // add basic file functions
    Push(&s_basestack);

    if (s_manager == nullptr) {
        return false;
    }

    // do the original operation
    auto op = *reinterpret_cast<Blizzard::File::Operation*>(reinterpret_cast<uintptr_t>(s_manager) + parms->op);
    return op(fs, parms);
}

// Hoist all functions in the filesystem stack to the top, i.e. the manager
void HoistAll() {
    #define HOIST_OP(op) if (s_manager->op == nullptr) { \
        auto cur = s_manager; \
        while (cur->next) { \
            cur->op = cur->next->op; \
            cur = cur->next; \
        } \
    }

    HOIST_OP(cd);
    HOIST_OP(close);
    HOIST_OP(create);
    HOIST_OP(cwd);
    HOIST_OP(dirwalk);
    HOIST_OP(exists);
    HOIST_OP(flush);
    HOIST_OP(getfileinfo);
    HOIST_OP(getfreespace);
    HOIST_OP(getpos);
    HOIST_OP(getrootchars);
    HOIST_OP(isabspath);
    HOIST_OP(isreadonly);
    HOIST_OP(makeabspath);
    HOIST_OP(mkdir);
    HOIST_OP(move);
    HOIST_OP(copy);
    HOIST_OP(open);
    HOIST_OP(read);
    HOIST_OP(readp);
    HOIST_OP(rmdir);
    HOIST_OP(setcachemode);
    HOIST_OP(seteof);
    HOIST_OP(setfileinfo);
    HOIST_OP(setpos);
    HOIST_OP(unlink);
    HOIST_OP(write);
    HOIST_OP(writep);
    HOIST_OP(shutdown);

    #undef HOIST_OP
}

} // namespace Stacked
} // namespace System_File
