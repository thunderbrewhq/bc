#if defined(WHOA_SYSTEM_WIN)

#include "bc/string/QuickFormat.hpp"
#include "bc/File.hpp"
#include "bc/Debug.hpp"
#include "bc/Memory.hpp"
#include "bc/String.hpp"
#include "bc/Unicode.hpp"
#include "bc/File.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/time/Time.hpp"

#include <algorithm>

#include <windows.h>
#include <sddl.h>

#define PATH(name) Blizzard::String::QuickNativePath<300UL>(name).ToString()

#define MAKEU64(high, low) static_cast<uint64_t>(high) << 32ULL | static_cast<uint64_t>(low)

#define BREAKU64(qw, high, low)             \
    high = static_cast<uint32_t>(qw >> 32); \
    low  = static_cast<uint32_t>(qw)

#define BREAKFILEPOINTER(qw, high, low)  \
     high = static_cast<LONG>(qw >> 32); \
     low  = static_cast<LONG>(qw)

namespace System_File {

// converts file name from local Windows format into UTF-8
void FromNativeName(char* buffer, int32_t buffersize, const char* name) {
    uint16_t widenamebuffer[300];
    auto len      = name ? Blizzard::String::Length(name) : 0;
    auto widename = len + 1 > 300
                        ? static_cast<uint16_t*>(Blizzard::Memory::Allocate(2 * (len + 1)))
                        : widenamebuffer;

    widename[MultiByteToWideChar(CP_ACP, 0, name, len, reinterpret_cast<LPWSTR>(widename), len + 1)] = 0;

    Blizzard::Unicode::ConvertUTF16to8(reinterpret_cast<uint8_t*>(buffer), buffersize, widenamebuffer, 0xFFFFFFFF, nullptr, nullptr);

    if (widenamebuffer != widename) {
        Blizzard::Memory::Free(widename);
    }
}

// translate Win32 file bits into Blizzard file bits
uint32_t FromNativeAttributes(DWORD a) {
    uint32_t b = 0;
    if (a & FILE_ATTRIBUTE_READONLY) {
        b |= BC_FILE_ATTRIBUTE_READONLY;
    }

    if (a & FILE_ATTRIBUTE_HIDDEN) {
        b |= BC_FILE_ATTRIBUTE_HIDDEN;
    }

    if (a & FILE_ATTRIBUTE_SYSTEM) {
        b |= BC_FILE_ATTRIBUTE_SYSTEM;
    }

    if (a & FILE_ATTRIBUTE_ARCHIVE) {
        b |= BC_FILE_ATTRIBUTE_ARCHIVE;
    }

    if (a & FILE_ATTRIBUTE_TEMPORARY) {
        b |= BC_FILE_ATTRIBUTE_TEMPORARY;
    }

    if (a & FILE_ATTRIBUTE_DIRECTORY) {
        b |= BC_FILE_ATTRIBUTE_DIRECTORY;
    }

    if ((a & FILE_ATTRIBUTE_NORMAL) != 0 || (a & FILE_ATTRIBUTE_DIRECTORY) == 0) {
        b |= BC_FILE_ATTRIBUTE_NORMAL;
    }

    return b;
}

// translate Blizzard file attribute bits into Win32 attribute bits.
DWORD ToNativeAttributes(uint32_t a) {
    DWORD b = 0;

    if (a & BC_FILE_ATTRIBUTE_READONLY) {
        b |= FILE_ATTRIBUTE_READONLY;
    }

    if (a & BC_FILE_ATTRIBUTE_HIDDEN) {
        b |= FILE_ATTRIBUTE_HIDDEN;
    }

    if (a & BC_FILE_ATTRIBUTE_SYSTEM) {
        b |= FILE_ATTRIBUTE_SYSTEM;
    }

    if (a & BC_FILE_ATTRIBUTE_ARCHIVE) {
        b |= FILE_ATTRIBUTE_ARCHIVE;
    }

    if (a & BC_FILE_ATTRIBUTE_TEMPORARY) {
        b |= FILE_ATTRIBUTE_TEMPORARY;
    }

    if (a & BC_FILE_ATTRIBUTE_NORMAL) {
        b |= FILE_ATTRIBUTE_NORMAL;
    }

    if (a & BC_FILE_ATTRIBUTE_DIRECTORY) {
        b |= FILE_ATTRIBUTE_DIRECTORY;
    }

    return b;
}

// convert FILETIME (two 32-bit values) into 64-bit unsigned integer
inline uint64_t MakeFileTime(FILETIME ft) {
    return MAKEU64(ft.dwHighDateTime, ft.dwLowDateTime);
}

// convert 64-bit unsigned integer into FILETIME (two 32-bit values)
inline FILETIME BreakFileTime(uint64_t qw) {
    FILETIME ft;
    BREAKU64(qw, ft.dwHighDateTime, ft.dwLowDateTime);
    return ft;
}

void GetFileInfoByFile(Blizzard::File::StreamRecord* file) {
    if (!file->haveinfo) {
        file->haveinfo = false;

        DWORD filesizehigh;
        auto filesize = static_cast<uint64_t>(GetFileSize(file->filehandle, &filesizehigh));

        if (filesize == INVALID_FILE_SIZE && GetLastError()) {
            return;
        }

        auto info = &file->info;

        // TODO: ??????
        if (!file->unk48 || !*file->unk48) {
            info->size = MAKEU64(filesizehigh, filesize);
        }

        WIN32_FILE_ATTRIBUTE_DATA fileinfo;
        if (::GetFileAttributesEx(PATH(file->name), GetFileExInfoStandard, &fileinfo)) {

            info->createtime   = Blizzard::Time::FromFileTime(MakeFileTime(fileinfo.ftCreationTime));
            info->writetime  = Blizzard::Time::FromFileTime(MakeFileTime(fileinfo.ftLastWriteTime));
            info->accesstime = Blizzard::Time::FromFileTime(MakeFileTime(fileinfo.ftLastAccessTime));

            info->attributes = FromNativeAttributes(fileinfo.dwFileAttributes);

            info->normal = info->attributes & BC_FILE_ATTRIBUTE_NORMAL != 0;

            if (fileinfo.dwFileAttributes == 0xFFFFFFFF) {
                info->filetype = 0;
            } else {
                info->filetype = ((info->attributes & BC_FILE_ATTRIBUTE_DIRECTORY) != 0) + 1;
            }
        }
    }

    file->haveinfo = true;
}

bool ToCreateFlags(int32_t mode, DWORD& sharemode, DWORD& desiredaccess, DWORD& creationdisposition, DWORD& flagsandattributes) {
    sharemode           = (mode >> 2) & 3;
    desiredaccess       = ((mode << 2) | mode & Blizzard::File::Mode::write) << 29;
    creationdisposition = 0;

    if ((mode & Blizzard::File::Mode::truncate) == 0 && (mode & Blizzard::File::Mode::create) == 0) {
        if ((mode & Blizzard::File::Mode::mustnotexist) == 0) {
            creationdisposition = mode & Blizzard::File::Mode::mustexist ? OPEN_EXISTING : OPEN_ALWAYS;
        } else {
            creationdisposition = CREATE_NEW;
        }
    } else if (mode & Blizzard::File::Mode::mustnotexist) {
        creationdisposition = CREATE_NEW;
    } else {
        creationdisposition = CREATE_ALWAYS;
    }

    flagsandattributes = FILE_ATTRIBUTE_NORMAL | (mode & Blizzard::File::Mode::temporary ? FILE_ATTRIBUTE_TEMPORARY : 0) | (mode & Blizzard::File::Mode::nocache ? FILE_FLAG_WRITE_THROUGH | FILE_FLAG_NO_BUFFERING : 0);

    return true;
}

namespace Stacked {

// begin stacked file functions

bool (SetWorkingDirectory)(FileParms* parms) {
    BLIZZARD_ASSERT(parms->name);

    return ::SetCurrentDirectory(PATH(parms->name)) != 0;
}

bool (Close)(FileParms* parms) {
    auto file = parms->file;
    if (file->filehandle != INVALID_HANDLE_VALUE) {
        ::CloseHandle(file->filehandle);
    }

    Blizzard::Memory::Free(file);

    return true;
}

bool (GetWorkingDirectory)(FileParms* parms) {
    if (!parms->buffer || !parms->buffersize) {
        // System_File::FileError(8)
        BC_FILE_SET_ERROR(8);
        return false;
    }

    char cwd[BC_FILE_MAX_PATH];
    ::GetCurrentDirectory(BC_FILE_MAX_PATH, cwd);

    FromNativeName(parms->buffer, parms->buffersize, cwd);

    return true;
}

bool (ProcessDirFast)(FileParms* parms) {
    char dirfindpattern[512];
    Blizzard::String::Format(dirfindpattern, 512, "%s\\*", parms->name);

    WIN32_FIND_DATA findData;
    auto hFindFile = ::FindFirstFile(PATH(dirfindpattern), &findData);
    if (hFindFile == INVALID_HANDLE_VALUE) {
        BC_FILE_SET_ERROR_MSG(8, "failed to open %s\n", parms->name);
        return false;
    }

    Blizzard::File::ProcessDirParms dirwalkparms;
    dirwalkparms.dir   = parms->name;
    dirwalkparms.param = parms->dirwalkparam;

    char item[BC_FILE_MAX_PATH];

    while (true) {
        // ignore .. and .
        if (findData.cFileName[0] != '.' || findData.cFileName[1] && (findData.cFileName[1] != '.' || findData.cFileName[2])) {
            FromNativeName(item, BC_FILE_MAX_PATH, findData.cFileName);
            dirwalkparms.item  = item;
            dirwalkparms.isdir = (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

            if (!parms->dirwalkcallback(dirwalkparms)) {
                ::FindClose(hFindFile);
                return true;
            }
        }

        if (!::FindNextFile(hFindFile, &findData)) {
            break;
        }
    }

    ::FindClose(hFindFile);
    return false;
}

bool (Exists)(FileParms* parms) {
    char name[BC_FILE_MAX_PATH];
    if (parms->name) {
        Blizzard::String::MakeBackslashPath(parms->name, name, sizeof(name));
    } else {
        name[0] = '\0';
    }
    auto dwFileAttributes = ::GetFileAttributes(static_cast<LPCSTR>(PATH(name)));

    if (dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
        parms->info->filetype = 0;
        return true;
    }

    parms->info->filetype = dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ? 2 : 1;

    return true;
}

bool (Flush)(FileParms* parms) {
    auto file = parms->file;

    if (file->filehandle == INVALID_HANDLE_VALUE) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    return ::FlushFileBuffers(file->filehandle) != 0;
}

bool (GetFileInfo)(FileParms* parms) {
    if (parms->name) {
        WIN32_FILE_ATTRIBUTE_DATA fileinfo;
        if (!::GetFileAttributesEx(PATH(parms->name), GetFileExInfoStandard, &fileinfo)) {
            BC_FILE_SET_ERROR(8);
            return false;
        }

        auto info = parms->info;

        info->size = MAKEU64(fileinfo.nFileSizeHigh, fileinfo.nFileSizeLow);

        info->createtime   = Blizzard::Time::FromFileTime(MakeFileTime(fileinfo.ftCreationTime));
        info->writetime  = Blizzard::Time::FromFileTime(MakeFileTime(fileinfo.ftLastWriteTime));
        info->accesstime = Blizzard::Time::FromFileTime(MakeFileTime(fileinfo.ftLastAccessTime));

        info->name = nullptr;

        info->attributes = FromNativeAttributes(fileinfo.dwFileAttributes);

        info->normal = info->attributes & BC_FILE_ATTRIBUTE_NORMAL != 0;

        if (fileinfo.dwFileAttributes == 0xFFFFFFFF) {
            info->filetype = 0;
        } else {
            info->filetype = ((info->attributes & BC_FILE_ATTRIBUTE_DIRECTORY) != 0) + 1;
        }
        return true;
    } else if (parms->file) {
        auto file = parms->file;
        GetFileInfoByFile(file);
        parms->info = &file->info;
        if (!file->info.name) {
            file->info.name = file->name;
        }
        return true;
    }

    BC_FILE_SET_ERROR(8);
    return false;
}

bool (GetFreeSpace)(FileParms* parms) {
    auto name = parms->name;
    if (name == nullptr || *name == '\0') {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    auto pathchars = (Blizzard::String::FindFilename(name) + 1) - name;
    if (pathchars > 260) {
        pathchars = 260;
    }

    char path[260];
    char shortpath[260];
    Blizzard::String::Copy(path, name, pathchars);

    ULARGE_INTEGER freebytesavailable;
    ULARGE_INTEGER totalbytesavailable;

    auto shortpathchars = ::GetShortPathName(PATH(path), shortpath, 260);
    if (shortpathchars && shortpathchars < 260) {
        if (!GetDiskFreeSpaceEx(shortpath, &freebytesavailable, &totalbytesavailable, nullptr)) {
            BC_FILE_SET_ERROR(8);
            return false;
        }
    } else {
        if (!GetDiskFreeSpaceEx(PATH(path), &freebytesavailable, &totalbytesavailable, nullptr)) {
            BC_FILE_SET_ERROR(8);
            return false;
        }
    }

    parms->offset = static_cast<int64_t>(freebytesavailable.QuadPart);

    return true;
}

bool (GetPos)(FileParms* parms) {
    auto file = parms->file;

    if (file == nullptr || file->filehandle == INVALID_HANDLE_VALUE) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    LONG high = 0;
    auto low  = ::SetFilePointer(file->filehandle, 0, &high, FILE_CURRENT);
    if (low == INVALID_SET_FILE_POINTER && GetLastError()) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    parms->offset = MAKEU64(high, low);
    return true;
}

bool (GetRootChars)(FileParms* parms) {
    char pathbuffer[256];
    auto pathsize = Blizzard::String::Length(parms->name) + 1;
    auto path     = pathsize > 256 ? reinterpret_cast<char*>(Blizzard::Memory::Allocate(pathsize)) : pathbuffer;

    if (pathsize > 2) {
        Blizzard::String::MakeUnivPath(parms->name, path, pathsize);
        if (path[1] == ':') {
            parms->offset = (path[2] == '/') + 2;
        } else if (path[0] == '/' && path[1] == '/') {
            uint32_t i = 0;
            auto cur   = path + 1;
            for (uint32_t i = 0; i < 2; i++) {
                cur = strchr(cur + 1, '/');
                if (!cur) {
                    parms->offset = pathsize;
                    if (path != pathbuffer) {
                        Blizzard::Memory::Free(path);
                    }
                    return true;
                }
            }
            parms->offset = (cur - path) + 1;
        } else {
            parms->offset = 0LL;
        }
    }

    if (path != pathbuffer) {
        Blizzard::Memory::Free(path);
    }
    return true;
}

bool (IsAbsolutePath)(FileParms* parms) {
    auto path = parms->name;

    auto first = path[0];
    // UNC
    if (((first == '\\' || first == '/') && ((path[1] == '\\' || path[1] == '/'))) && path[2] != '\0') {
        return true;
    }

    // DOS canonical
    if (isalpha(first) && (path[1] == ':') && (((path[2] == '\\') || (path[2] == '/')) && path[3] != '\0')) {
        return true;
    }

    BC_FILE_SET_ERROR(8);
    return false;
}

bool (IsReadOnly)(FileParms* parms) {
    if (!parms->name) {
        BC_FILE_SET_ERROR(8);
        return false;
    }
    auto dwFileAttributes = ::GetFileAttributes(PATH(parms->name));
    if (dwFileAttributes == INVALID_FILE_ATTRIBUTES) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    return dwFileAttributes & FILE_ATTRIBUTE_READONLY;
}

bool (MakeAbsolutePath)(FileParms* parms) {
    if (!parms->name) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    char full[BC_FILE_MAX_PATH];
    ::_fullpath(full, PATH(parms->name), BC_FILE_MAX_PATH);
    FromNativeName(parms->buffer, parms->buffersize, full);

    Blizzard::String::ForceTrailingSeparator(parms->buffer, parms->buffersize, '\0');

    return true;
}

// avoid clobbering
//   System_File::Stacked::CreateDirectory
// into
//    System_File::Stacked::CreateDirectoryA
#undef CreateDirectory
bool (CreateDirectory)(FileParms* parms) {
#define CreateDirectory CreateDirectoryA
    if (!parms->name) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    char pathbuffer[260];
    auto pathsize = Blizzard::String::Length(parms->name) + 1;
    auto path     = pathsize > 260 ? reinterpret_cast<char*>(Blizzard::Memory::Allocate(pathsize)) : pathbuffer;

    Blizzard::String::MakeBackslashPath(parms->name, path, pathsize);

    SECURITY_ATTRIBUTES security_attr = {};
    security_attr.nLength             = sizeof(SECURITY_ATTRIBUTES);
    security_attr.bInheritHandle      = FALSE;

    if (parms->set_acl) {
        ConvertStringSecurityDescriptorToSecurityDescriptorW(L"D:(A;OICI;GA;;;BU)", OWNER_SECURITY_INFORMATION, &security_attr.lpSecurityDescriptor, nullptr);
    }

    if (parms->recurse) {
        char leadingpath[260];

        for (auto s = path; *s && s < (s + Blizzard::String::Length(s)); s++) {
            while (*s && *s != '\\') {
                s++;
            }

            auto leadingpathsize = (s - path) + 2;
            if (leadingpathsize > 260) {
                leadingpathsize = 260;
            }
            if (Blizzard::String::Copy(leadingpath, path, leadingpathsize)) {
                BC_FILE_SET_ERROR(8);
                if (path != pathbuffer) {
                    Blizzard::Memory::Free(path);
                }
                return false;
            }

            Blizzard::String::QuickNativePath<300UL> directorypath(leadingpath);

            if (::CreateDirectory(directorypath.ToString(), parms->set_acl ? &security_attr : nullptr)) {
                if (*s == '\0') {
                    break;
                }
            } else {
                auto last_err = ::GetLastError();

                if ((last_err != ERROR_ALREADY_EXISTS && last_err != ERROR_ACCESS_DENIED)) {
                    if (path != pathbuffer) {
                        Blizzard::Memory::Free(path);
                    }
                    BC_FILE_SET_ERROR(8);
                    return false;
                }

                auto directoryattributes = GetFileAttributes(directorypath.ToString());
                if (directoryattributes == INVALID_FILE_ATTRIBUTES || (directoryattributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
                    if (path != pathbuffer) {
                        Blizzard::Memory::Free(path);
                    }
                    BC_FILE_SET_ERROR(8);
                    return false;
                }
            }
        }
    } else {
        if (!::CreateDirectory(PATH(path), parms->set_acl ? &security_attr : nullptr)) {
            auto last_err = ::GetLastError();
            bool result   = last_err == ERROR_ALREADY_EXISTS || last_err == ERROR_ACCESS_DENIED;
            if (path != pathbuffer) {
                Blizzard::Memory::Free(path);
            }
            return result;
        }
    }

    if (path != pathbuffer) {
        Blizzard::Memory::Free(path);
    }
    return true;
}

bool (Move)(FileParms* parms) {
    char src[BC_FILE_MAX_PATH];
    char dst[BC_FILE_MAX_PATH];
    if (parms->name) {
        Blizzard::String::MakeBackslashPath(parms->name, src, BC_FILE_MAX_PATH);
    } else {
        src[0] = '\0';
    }
    if (parms->newname) {
        Blizzard::String::MakeBackslashPath(parms->newname, dst, BC_FILE_MAX_PATH);
    } else {
        dst[0] = '\0';
    }
    return ::MoveFile(PATH(src), PATH(dst)) != 0;
}

bool (Copy)(FileParms* parms) {
    char srcname[BC_FILE_MAX_PATH];
    char dstname[BC_FILE_MAX_PATH];
    if (parms->name) {
        Blizzard::String::MakeBackslashPath(parms->name, srcname, BC_FILE_MAX_PATH);
    } else {
        srcname[0] = '\0';
    }
    if (parms->newname) {
        Blizzard::String::MakeBackslashPath(parms->newname, dstname, BC_FILE_MAX_PATH);
    } else {
        dstname[0] = '\0';
    }

    Blizzard::File::StreamRecord* src;
    Blizzard::File::StreamRecord* dst;

    if (!Blizzard::File::Open(srcname, Blizzard::File::Mode::read | Blizzard::File::Mode::shareread | Blizzard::File::Mode::mustexist, src)) {
        BC_FILE_SET_ERROR(2); // TODO: find out what 2 is
        return false;
    }

    if (!Blizzard::File::Open(dstname, Blizzard::File::Mode::write | (parms->overwrite ? Blizzard::File::Mode::create : Blizzard::File::Mode::create | Blizzard::File::Mode::mustnotexist), dst)) {
        BC_FILE_SET_ERROR(4); // TODO: find out what 4 is
        return false;
    }

    auto size           = Blizzard::File::GetFileInfo(src)->size;
    auto copybuffersize = size > BC_SYSTEM_FILE_COPYBUFFER_SIZE ? BC_SYSTEM_FILE_COPYBUFFER_SIZE : size;
    auto copybuffer     = Blizzard::Memory::Allocate(copybuffersize);

    int64_t offset = 0;

    bool result = true;

    while (offset < size) {
        auto count = static_cast<int32_t>(size - offset >= BC_SYSTEM_FILE_COPYBUFFER_SIZE ? BC_SYSTEM_FILE_COPYBUFFER_SIZE : size - offset);
        if (!Blizzard::File::Read(src, copybuffer, offset, &count)) {
            result = false;
            break;
        }

        if (!Blizzard::File::Write(dst, copybuffer, offset, &count)) {
            result = false;
            break;
        }

        offset += static_cast<int64_t>(count);
    }

    Blizzard::Memory::Free(copybuffer);
    Blizzard::File::Close(src);
    Blizzard::File::Close(dst);
    if (!result) {
        Blizzard::File::Delete(dstname);
    }
    return result;
}

bool (Open)(FileParms* parms) {
    char name[BC_FILE_MAX_PATH];
    if (parms->name) {
        Blizzard::String::MakeBackslashPath(parms->name, name, BC_FILE_MAX_PATH);
    } else {
        name[0] = '\0';
    }

    DWORD sharemode;
    DWORD desiredaccess;
    DWORD creationdisposition;
    DWORD flagsandattributes;
    if (!ToCreateFlags(parms->mode, sharemode, desiredaccess, creationdisposition, flagsandattributes)) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    auto filehandle = CreateFile(
        PATH(name),
        desiredaccess,
        sharemode,
        nullptr,
        creationdisposition,
        flagsandattributes,
        nullptr);
    if (filehandle == INVALID_HANDLE_VALUE) {
        BC_FILE_SET_ERROR_MSG(4, "Win32 Open - %s", name);
        return false;
    }

    auto file = parms->file;

    if (file == nullptr) {
        // alloc(sizeof(StreamRecord) + len(name) + 1)
        // block of memory holds file as well as C string of the filename
        auto namesize = Blizzard::String::Length(name);
        file          = reinterpret_cast<Blizzard::File::StreamRecord*>(Blizzard::Memory::Allocate(sizeof(Blizzard::File::StreamRecord) + 1 + namesize));
        // set the name pointer inside of StreamRecord to point to the end of StreamRecord (i.e. the start of the name cstring in the memory block)
        auto filename = reinterpret_cast<char*>(file) + sizeof(Blizzard::File::StreamRecord);
        file->name    = filename;
        // copy name into memory block
        Blizzard::String::Copy(filename, name, namesize + 1);
    }

    file->filehandle = filehandle;
    file->mode       = parms->mode;
    file->haveinfo   = false;
    file->info.name  = file->name;
    GetFileInfoByFile(file);

    if (parms->mode & Blizzard::File::Mode::append) {
        Blizzard::File::SetPos(file, file->info.size, BC_FILE_SEEK_START);
    }

    parms->file = file;
    return true;
}

#undef RemoveDirectory
bool (RemoveDirectory)(FileParms* parms) {
#define RemoveDirectory RemoveDirectoryA
    if (parms->recurse) {
        return Blizzard::File::RemoveDirectoryAndContents(parms->name, false);
    }
    if (!parms->name) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    char namebuffer[260];
    auto namesize = Blizzard::String::Length(parms->name);
    auto name     = namesize > 260 ? reinterpret_cast<char*>(Blizzard::Memory::Allocate(namesize)) : namebuffer;
    Blizzard::String::MakeBackslashPath(parms->name, name, 260);

    auto removed = ::RemoveDirectory(PATH(name));

    if (name != namebuffer) {
        Blizzard::Memory::Free(name);
    }

    return removed != 0;
}

bool (SetCacheMode)(FileParms* parms) {
    auto file = parms->file;

    parms->mode |= ~(Blizzard::File::Mode::nocache);

    DWORD sharemode;
    DWORD desiredaccess;
    DWORD creationdisposition;
    DWORD flagsandattributes;
    if (!ToCreateFlags(parms->mode, sharemode, desiredaccess, creationdisposition, flagsandattributes)) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    ::CloseHandle(file->filehandle);

    file->filehandle = CreateFile(PATH(file->info.name), desiredaccess, sharemode, nullptr, creationdisposition, flagsandattributes, nullptr);
    if (file->filehandle == INVALID_HANDLE_VALUE) {
        BC_FILE_SET_ERROR_MSG(4, "Win32 Close");
        return false;
    }

    return true;
}

bool (SetEOF)(FileParms* parms) {
    auto file = parms->file;

    int64_t offset;
    if (!Blizzard::File::GetPos(file, offset) || !Blizzard::File::SetPos(file, parms->offset, parms->whence)) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    auto status = ::SetEndOfFile(file->filehandle);
    Blizzard::File::SetPos(file, offset, BC_FILE_SEEK_START);

    return status != 0;
}

bool (SetAttributes)(FileParms* parms) {
    if (parms->setinfo & BC_SYSTEM_FILE_INFO_TIMES) {
        auto info = parms->info;

        auto createtime     = BreakFileTime(Blizzard::Time::ToFileTime(info->createtime));
        auto lastaccesstime = BreakFileTime(Blizzard::Time::ToFileTime(info->accesstime));
        auto lastwritetime  = BreakFileTime(Blizzard::Time::ToFileTime(info->writetime));

        if (!::SetFileTime(parms->file->filehandle, &createtime, &lastaccesstime, &lastwritetime)) {
            BC_FILE_SET_ERROR(8);
            return false;
        }

        parms->setinfo &= ~(BC_SYSTEM_FILE_INFO_TIMES);
    }

    if (parms->setinfo & BC_SYSTEM_FILE_INFO_ATTRIBUTES) {
        if (!::SetFileAttributes(PATH(parms->file->name), ToNativeAttributes(parms->info->attributes))) {
            BC_FILE_SET_ERROR(8);
            return false;
        }

        parms->setinfo &= ~(BC_SYSTEM_FILE_INFO_ATTRIBUTES);
    }

    return parms->setinfo == 0;
}

bool (SetPos)(FileParms* parms) {
    auto file = parms->file;

    if (file->filehandle == INVALID_HANDLE_VALUE) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    LONG distancetomovelow;
    LONG distancetomovehigh;
    BREAKFILEPOINTER(parms->offset, distancetomovehigh, distancetomovelow);

    auto movemethod = static_cast<DWORD>(parms->whence);
    if (movemethod > FILE_END) {
        movemethod = FILE_END;
    }

    if (::SetFilePointer(file->filehandle, distancetomovelow, &distancetomovehigh, movemethod) == INVALID_SET_FILE_POINTER) {
        if (GetLastError()) {
            return false;
        }
    }

    return true;
}

bool (Delete)(FileParms* parms) {
    char name[BC_FILE_MAX_PATH];
    if (parms->name) {
        Blizzard::String::MakeBackslashPath(parms->name, name, BC_FILE_MAX_PATH);
    } else {
        name[0] = '\0';
    }
    return ::DeleteFile(PATH(name)) != 0;
}

// shutdown
bool (Shutdown)(FileParms* parms) {
    // ?
    return true;
}

} // namespace Stacked
} // namespace System_File

#endif
