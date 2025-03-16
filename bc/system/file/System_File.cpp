#include "bc/system/file/Stacked.hpp"
#include "bc/system/file/System_File.hpp"
#include "bc/Lock.hpp"
#include "bc/File.hpp"
#include "bc/String.hpp"
#include "bc/system/file/Types.hpp"

#if defined(WHOA_SYSTEM_WIN)
#include <handleapi.h>
#include <minwinbase.h>
#include <winnt.h>

#include "bc/system/file/win/Support.hpp"
#endif

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)
#include <cerrno>
#include <unistd.h>
#endif

namespace System_File {

bool s_EnableFileLocks = false;

bool SetWorkingDirectory(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::SetWorkingDirectory(parms);
}

bool Close(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Close(parms);
}

bool Create(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Create(parms);
}

bool GetWorkingDirectory(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::GetWorkingDirectory(parms);
}

bool ProcessDirFast(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::ProcessDirFast(parms);
}

bool Exists(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Exists(parms);
}

bool Flush(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Flush(parms);
}

bool GetFileInfo(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::GetFileInfo(parms);
}

bool GetFreeSpace(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::GetFreeSpace(parms);
}

bool GetPos(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::GetPos(parms);
}

bool GetRootChars(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::GetRootChars(parms);
}

bool IsAbsolutePath(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::IsAbsolutePath(parms);
}

bool IsReadOnly(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::IsReadOnly(parms);
}

bool MakeAbsolutePath(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::MakeAbsolutePath(parms);
}

bool CreateDirectory(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::CreateDirectory(parms);
}

bool Move(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Move(parms);
}

bool Copy(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Copy(parms);
}

bool Open(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Open(parms);
}

bool RemoveDirectory(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::RemoveDirectory(parms);
}

bool SetCacheMode(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::SetCacheMode(parms);
}

bool SetEOF(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::SetEOF(parms);
}

bool SetAttributes(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::SetAttributes(parms);
}

bool SetPos(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::SetPos(parms);
}

bool Delete(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Delete(parms);
}

#if defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)

bool Read(Blizzard::File::StreamRecord* file, void* data, int64_t offset, int32_t* count) {
    // file must be open to read
    BLIZZARD_ASSERT(file != nullptr && file->filefd != -1);

    if (count == nullptr || *count == 0) {
        return true;
    }

    auto n = *count;
    auto c = n;

    while (n > 0) {
        auto m = offset < 0 ? ::read(file->filefd, data, n) : ::pread(file->filefd, data, n, offset);
        if (m == 0) {
            BC_FILE_SET_ERROR_MSG(7, "Posix Read - End of File - %s", file->name);
            return false;
        }
        if (m == -1) {
            if (errno == EINTR) {
                continue;
            }
            BC_FILE_SET_ERROR_MSG(100 + errno, "Posix Read - %s", file->name);
            return false;
        }
        data = static_cast<uint8_t*>(data) + m;
        n -= m;
    }

    *count = c - n;

    return n == 0;
}

bool Read(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return System_File::Read(parms->file, parms->data, -1, &parms->count);
}

bool ReadP(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return System_File::Read(parms->file, parms->data, parms->offset, &parms->count);
}

bool Write(Blizzard::File::StreamRecord* file, const void* data, int64_t offset, int32_t* count) {
    // file descriptor must be initialized
    BLIZZARD_ASSERT(file != nullptr && file->filefd != -1);

    if (count == nullptr || *count == 0) {
        return true;
    }

    auto c = *count;
    auto n = c;

    while (n > 0) {
        auto m = offset < 0 ? ::write(file->filefd, data, n) : ::pwrite(file->filefd, data, n, offset);
        if (m < 0) {
            BC_FILE_SET_ERROR_MSG(100 + errno, "Posix Write - %s", file->name);
            return false;
        }

        if (offset > -1) {
            offset += m;
        }

        n -= m;
        data = static_cast<const uint8_t*>(data) + m;
    }

    *count = c - n;

    return n == 0;
}

bool Write(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return System_File::Write(parms->file, parms->data, -1LL, &parms->count);
}

bool WriteP(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return System_File::Write(parms->file, parms->data, parms->offset, &parms->count);
}

#endif // defined(WHOA_SYSTEM_MAC) || defined(WHOA_SYSTEM_LINUX)

#if defined(WHOA_SYSTEM_WIN)

bool read_init(Blizzard::File::StreamRecord*, void*, LPOVERLAPPED, LPDWORD);
bool write_init(Blizzard::File::StreamRecord*, const void*, LPOVERLAPPED, LPDWORD);

bool (*read_func)(Blizzard::File::StreamRecord*, void*, LPOVERLAPPED, LPDWORD)  = read_init;
bool (*write_func)(Blizzard::File::StreamRecord*, const void*, LPOVERLAPPED, LPDWORD) = write_init;

static Blizzard::Lock::DoOnceData s_read_init;
static Blizzard::Lock::DoOnceData s_write_init;

static Blizzard::Lock::Mutex s_io_lock;

bool read_overlapped(Blizzard::File::StreamRecord* file, void* data, LPOVERLAPPED overlapped, LPDWORD count) {
    if (file->filehandle == INVALID_HANDLE_VALUE) {
        BC_FILE_SET_ERROR(BC_FILE_ERROR_INVALID_ARGUMENT);
        return false;
    }

    if (*count == 0) {
        return true;
    }

    if (!::ReadFile(file->filehandle, data, *count, count, overlapped)) {
        BC_FILE_SET_ERROR_MSG(BC_FILE_ERROR_READ, "Win32 Read - %s", file->name);
        return false;
    }

    return true;
}

bool read_locked(Blizzard::File::StreamRecord* file, void* data, LPOVERLAPPED overlapped, LPDWORD count) {
    Blizzard::Lock::MutexEnter(s_io_lock);
    if (!Blizzard::File::SetPos(
            file,
            (static_cast<int64_t>(overlapped->OffsetHigh) << 32) | static_cast<int64_t>(overlapped->Offset),
            BC_FILE_SEEK_START)) {
        BC_FILE_SET_ERROR(9);
        Blizzard::Lock::MutexLeave(s_io_lock);
        return false;
    }
    auto ok = read_overlapped(file, data, nullptr, count);
    Blizzard::Lock::MutexLeave(s_io_lock);
    return ok;
}

bool write_overlapped(Blizzard::File::StreamRecord* file, const void* data, LPOVERLAPPED overlapped, LPDWORD count) {
    if (file->filehandle == nullptr) {
        BC_FILE_SET_ERROR(BC_FILE_ERROR_INVALID_ARGUMENT);
        return false;
    }

    if (*count == 0) {
        return true;
    }

    if (!::WriteFile(file->filehandle, data, *count, count, overlapped)) {
        BC_FILE_SET_ERROR_MSG(BC_FILE_ERROR_WRITE, "Win32 Write - %s", file->name);
        return false;
    }

    return true;
}

bool write_locked(Blizzard::File::StreamRecord* file, const void* data, LPOVERLAPPED overlapped, LPDWORD count) {
    Blizzard::Lock::MutexEnter(s_io_lock);
    if (!Blizzard::File::SetPos(
        file,
        (static_cast<int64_t>(overlapped->OffsetHigh) << 32) | static_cast<int64_t>(overlapped->Offset),
        BC_FILE_SEEK_START)) {
        BC_FILE_SET_ERROR(9);
        Blizzard::Lock::MutexLeave(s_io_lock);
        return false;
    }
    auto ok = write_overlapped(file, data, nullptr, count);
    Blizzard::Lock::MutexLeave(s_io_lock);
    return ok;
}

void detect_io_mode(void*) {
    // if Windows is >= 5.0
    //  read using the overlapped IO method
    //    if Windows < 5.0
    //  each ReadP/WriteP call is locked and calls File::SetPos
    OSVERSIONINFO version_info       = {};
    version_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    if (::GetVersionEx(&version_info) && version_info.dwMajorVersion >= 5) {
        read_func  = read_overlapped;
        write_func = write_overlapped;
    } else {
        Blizzard::Lock::MutexCreate(s_io_lock);
        read_func  = read_locked;
        write_func = write_locked;
    }
}

bool read_init(Blizzard::File::StreamRecord* file, void* data, LPOVERLAPPED overlapped, LPDWORD count) {
    Blizzard::Lock::DoOnce(s_read_init, detect_io_mode, nullptr);
    return read_func(file, data, overlapped, count);
}

bool write_init(Blizzard::File::StreamRecord* file, void* data, LPOVERLAPPED overlapped, LPDWORD count) {
    Blizzard::Lock::DoOnce(s_write_init, detect_io_mode, nullptr);
    return write_func(file, data, overlapped, count);
}

bool Read(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return read_func(parms->file, parms->data, nullptr, reinterpret_cast<LPDWORD>(&parms->count));
}

bool ReadP(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    OVERLAPPED overlapped;
    overlapped.Offset       = static_cast<DWORD>(parms->offset & 0xFFFFFFFF);
    overlapped.OffsetHigh   = static_cast<DWORD>((parms->offset >> 32) & 0xFFFFFFFF);
    overlapped.Internal     = 0;
    overlapped.InternalHigh = 0;
    overlapped.hEvent       = nullptr;
    return read_func(parms->file, parms->data, &overlapped, reinterpret_cast<LPDWORD>(&parms->count));
}

bool Write(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return write_func(parms->file, parms->data, nullptr, reinterpret_cast<LPDWORD>(&parms->count));
}

bool WriteP(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    OVERLAPPED overlapped;
    overlapped.Offset       = static_cast<DWORD>(parms->offset & 0xFFFFFFFF);
    overlapped.OffsetHigh   = static_cast<DWORD>((parms->offset >> 32) & 0xFFFFFFFF);
    overlapped.Internal     = 0;
    overlapped.InternalHigh = 0;
    overlapped.hEvent       = nullptr;

    return write_func(parms->file, parms->data, &overlapped, reinterpret_cast<LPDWORD>(&parms->count));
}

#endif // defined(WHOA_SYSTEM_WIN)

bool Shutdown(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms) {
    return Stacked::Shutdown(parms);
}

} // namespace System_File
