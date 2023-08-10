#include "bc/file/Defines.hpp"
#include "bc/file/system/win/Utils.hpp"

#include <windows.h>

namespace Blizzard {
namespace WinFile {

// Translate Win32 file bits into Blizzard file bits.
uint32_t AttributesToBC(DWORD dwFileAttributes) {
    uint32_t fileAttributes = 0;
    if (dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
        fileAttributes |= BC_FILE_ATTRIBUTE_READONLY;
    }

    if (dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) {
        fileAttributes |= BC_FILE_ATTRIBUTE_HIDDEN;
    }

    if (dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) {
        fileAttributes |= BC_FILE_ATTRIBUTE_SYSTEM;
    }

    if (dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY) {
        fileAttributes |= BC_FILE_ATTRIBUTE_TEMPORARY;
    }

    if (dwFileAttributes & FILE_ATTRIBUTE_NORMAL) {
       fileAttributes |= BC_FILE_ATTRIBUTE_NORMAL;
    }

    if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
        fileAttributes |= BC_FILE_ATTRIBUTE_DIRECTORY;
    }

    return fileAttributes;
}

// Translate Blizzard file attribute bits into Win32 attribute bits.
DWORD AttributesToWin(uint32_t fileAttributes) {
    DWORD dwFileAttributes = 0;

    if (fileAttributes & BC_FILE_ATTRIBUTE_READONLY) {
        dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
    }

    if (fileAttributes & BC_FILE_ATTRIBUTE_HIDDEN) {
        dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
    }

    if (fileAttributes & BC_FILE_ATTRIBUTE_SYSTEM) {
        dwFileAttributes |= FILE_ATTRIBUTE_SYSTEM;
    }

    if (fileAttributes & BC_FILE_ATTRIBUTE_ARCHIVE) {
        dwFileAttributes |= FILE_ATTRIBUTE_ARCHIVE;
    }

    if (fileAttributes & BC_FILE_ATTRIBUTE_TEMPORARY) {
        dwFileAttributes |= FILE_ATTRIBUTE_TEMPORARY;
    }

    if (fileAttributes & BC_FILE_ATTRIBUTE_NORMAL) {
        dwFileAttributes |= FILE_ATTRIBUTE_NORMAL;
    }

    if (fileAttributes & BC_FILE_ATTRIBUTE_DIRECTORY) {
        dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
    }

    return dwFileAttributes;
}

// Convert FILETIME (two 32-bit values) into 64-bit unsigned integer.
uint64_t UnpackTime(FILETIME ft) {
    ULARGE_INTEGER ul = {};
    ul.HighPart = ft.dwHighDateTime;
    ul.LowPart  = ft.dwLowDateTime;
    return static_cast<uint64_t>(ul.QuadPart);
}

// Convert 64-bit unsigned integer into FILETIME (two 32-bit values)
FILETIME PackTime(uint64_t qw) {
    ULARGE_INTEGER ul = {};
    ul.QuadPart = qw;
    FILETIME ft = {};
    ft.dwHighDateTime = ul.HighPart;
    ft.dwLowDateTime  = ul.LowPart;
    return ft;
}

// Returns true if BY_HANDLE_FILE_INFORMATION is successfully converted to File::FileInfo.
bool HandleFileInfoToBC(
    LPBY_HANDLE_FILE_INFORMATION  winInfo,
    Blizzard::File::FileInfo*     info) {
    if (!info) {
        return false;
    }

    info->attributes = AttributesToBC(winInfo->dwFileAttributes);
    info->device     = static_cast<uint32_t>(winInfo->dwVolumeSerialNumber);
    info->size       = (static_cast<uint64_t>(winInfo->nFileSizeHigh) << 32ULL) | static_cast<uint64_t>(winInfo->nFileSizeLow);

    info->accessTime                = Blizzard::Time::FromWinFiletime(UnpackTime(winInfo->ftLastAccessTime));
    info->modificationTime          = Blizzard::Time::FromWinFiletime(UnpackTime(winInfo->ftLastWriteTime));
    info->attributeModificationTime = Blizzard::Time::FromWinFiletime(UnpackTime(winInfo->ftCreationTime));

    return true;
}

// Returns true if WIN32_FILE_ATTRIBUTE_DATA is successfully converted to File::FileInfo.
bool AttributeFileInfoToBC(
    LPWIN32_FILE_ATTRIBUTE_DATA winInfo,
    Blizzard::File::FileInfo*     info) {

    if (!info) {
        return false;
    }

    info->attributes = AttributesToBC(winInfo->dwFileAttributes);
    info->device     = 0;
    info->size       = (static_cast<uint64_t>(winInfo->nFileSizeHigh) << 32ULL) | static_cast<uint64_t>(winInfo->nFileSizeLow);

    info->accessTime                = Blizzard::Time::FromWinFiletime(UnpackTime(winInfo->ftLastAccessTime));
    info->modificationTime          = Blizzard::Time::FromWinFiletime(UnpackTime(winInfo->ftLastWriteTime));
    info->attributeModificationTime = Blizzard::Time::FromWinFiletime(UnpackTime(winInfo->ftCreationTime));

    return true;
}

// Open a HANDLE to a file, using BlizzardCore flags.
HANDLE Open(const char* systemPath, uint32_t flags, bool nocache) {
    // expand flags
    bool read         = flags & BC_FILE_OPEN_READ;
    bool write        = flags & BC_FILE_OPEN_WRITE;
    bool shareRead    = flags & BC_FILE_OPEN_SHARE_READ;
    bool shareRead    = flags & BC_FILE_OPEN_SHARE_WRITE;
    bool write        = flags & BC_FILE_OPEN_WRITE;
    bool mustNotExist = flags & BC_FILE_OPEN_MUST_NOT_EXIST;
    bool mustExist    = flags & BC_FILE_OPEN_MUST_EXIST;
    bool create       = flags & BC_FILE_OPEN_CREATE;
    bool truncate     = flags & BC_FILE_OPEN_TRUNCATE;

    // BLIZZARD_ASSERT(read || write);

    // Start building arguments to CreateFile()
    DWORD desiredAccess      = 0;
    DWORD shareMode          = 0;
    DWORD createDisposition  = 0;
    DWORD flagsAndAttributes = 0;

    if (nocache) {
        flagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;
    }

    // Setup desired access
    if (read) {
        desiredAccess |= GENERIC_READ;
    }
    if (write) {
        desiredAccess |= GENERIC_WRITE;
    }

    // Setup create disposition
    if (create && mustNotExist) {
        createDisposition = CREATE_NEW;
    } else if (create && !mustNotExist) {
        createDisposition = CREATE_ALWAYS;
    } else if (truncate) {
        createDisposition = TRUNCATE_EXISTING;
    } else if (mustExist) {
        createDisposition = OPEN_EXISTING;
    } else {
        createDisposition = OPEN_ALWAYS;
    }

    return CreateFileA(systemPath, desiredAccess, shareMode, nullptr, createDisposition, flagsAndAttributes, 0);
}

} // namespace WinFile
} // namespace Blizzard
