
#include "bc/file/Defines.hpp"
#if defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_MAC)

#include "bc/system/file/Stacked.hpp"
#include "bc/Debug.hpp"
#include "bc/File.hpp"
#include "bc/Memory.hpp"
#include "bc/String.hpp"
#include "bc/system/file/posix/Support.hpp"
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <fcntl.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/vfs.h>
#include <unistd.h>

#define PATH(name) Blizzard::String::QuickNativePath<BC_FILE_MAX_PATH>(name).ToString()

namespace System_File {
namespace Stacked {

// cd
bool SetWorkingDirectory(FileParms* parms) {
    if (!parms->name) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    char name[BC_FILE_MAX_PATH];
    Blizzard::String::MakeNativePath(parms->name, name, BC_FILE_MAX_PATH);

    return ::chdir(name) == 0;
}

// close
bool Close(FileParms* parms) {
    auto file = parms->file;
    BLIZZARD_ASSERT(file != nullptr);

    ::close(file->filefd);
    Blizzard::Memory::Free(file);

    return true;
}

// ??
bool Create(FileParms* parms) {
    BC_FILE_SET_ERROR(BC_FILE_ERROR_UNIMPLEMENTED);
    return false;
}

// cwd
bool GetWorkingDirectory(FileParms* parms) {
    if (!parms->buffer || parms->buffersize <= 0) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    *parms->buffer = '\0';
    return ::getcwd(parms->buffer, parms->buffersize) != 0;
}

// dirwalk
bool ProcessDirFast(FileParms* parms) {
    char name[BC_FILE_MAX_PATH];
    if (parms->name) {
        Blizzard::String::MakeNativePath(parms->name, name, BC_FILE_MAX_PATH);
    } else {
        name[0] = '\0';
    }

    auto dir = ::opendir(name);
    if (!dir) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    Blizzard::File::ProcessDirParms dirwalkparms;
    dirwalkparms.dir   = name;
    dirwalkparms.param = parms->dirwalkparam;

    auto ignorelinks = parms->mode != 0;

    char link[BC_FILE_MAX_PATH];
    char linkname[BC_FILE_MAX_PATH];
    struct dirent* item;

    while ((item = ::readdir(dir))) {
        auto n = item->d_name;
        if (!(n[0] == '.' && n[1] == '\0') && !(n[0] == '.' && n[1] == '.')) {
            dirwalkparms.item = item->d_name;

            if (item->d_type == DT_REG) {
                dirwalkparms.isdir = false;
            } else if (item->d_type == DT_DIR) {
                dirwalkparms.isdir = true;
            } else if (item->d_type == DT_LNK) {
                if (ignorelinks) {
                    dirwalkparms.isdir = false;
                } else {
                    Blizzard::String::Copy(link, name, BC_FILE_MAX_PATH);
                    Blizzard::String::Append(link, "/", BC_FILE_MAX_PATH);
                    Blizzard::String::Append(link, item->d_name, BC_FILE_MAX_PATH);
                    if (!::realpath(link, linkname)) {
                        continue;
                    }
                    auto filetype = Blizzard::File::Exists(linkname);
                    if (filetype == 1) {
                        dirwalkparms.isdir = false;
                    } else if (filetype == 2) {
                        dirwalkparms.isdir = 1;
                    } else {
                        if (filetype == 0) {
                            continue;
                        }
                    }
                }
            }

            if (!parms->dirwalkcallback(dirwalkparms)) {
                break;
            }
        }
    }

    ::closedir(dir);
    return true;
}

// exists
bool Exists(FileParms* parms) {
    struct stat info;
    if (::stat(PATH(parms->name), &info) == -1) {
        parms->info->filetype = 0;
    } else {
        parms->info->filetype = S_ISDIR(info.st_mode) ? 2 : 1;
    }
    return true;
}

// flush
bool Flush(FileParms* parms) {
    auto file = parms->file;
    BLIZZARD_ASSERT(file != nullptr);

#if defined(WHOA_SYSTEM_MAC)
    // from BSD syscall manual:
    // "Note that while fsync() will flush all data from the host to the drive
    //  (i.e. the "permanent storage device"), the drive itself may not physi-
    //  cally write the data to the platters for quite some time and it may be
    //  written in an out-of-order sequence."

    // "The F_FULLFSYNC fcntl asks the drive to flush all buffered data to permanent storage."
    auto status = ::fcntl(file->filefd, F_FULLFSYNC, 0);

    if (status == 0) {
        return true;
    }
#endif
    // Attempts to transfer all writes in the cache buffer to the underlying storage device.
    // fsync() does not return until the transfer is complete, or until an error is detected.
    return ::fsync(file->filefd) == 0;
}

// getfileinfo
bool GetFileInfo(FileParms* parms) {
    struct stat info;
    char name[BC_FILE_MAX_PATH];

    if (!parms->name) {
        auto file = parms->file;

        if (file) {
            if (!file->haveinfo) {
                if (::fstat(file->filefd, &info) == -1) {
                    file->info.filetype = 0;
                } else {
                    static_assert(sizeof(info.st_size) >= sizeof(int64_t), "stat size should hold at least 64 bits");
                    if (file->unk48 == nullptr || *file->unk48 == 0) {
                        file->info.size = info.st_size;
                    }

                    auto modtime = Blizzard::Time::FromUnixTime(info.st_mtim.tv_sec);

                    file->info.createtime = modtime;
                    file->info.writetime  = modtime;
                    file->info.filetype   = S_ISDIR(info.st_mode) ? 2 : 0;

                    file->haveinfo = true;
                }
            }
            parms->info = &file->info;
            if (!file->info.name) {
                file->info.name = file->name;
            }
            return file->haveinfo;
        }
    } else {
        Blizzard::String::MakeNativePath(parms->name, name, BC_FILE_MAX_PATH);

        if (::stat(name, &info) != -1) {
            auto infoptr        = parms->info;
            infoptr->size       = static_cast<uint64_t>(info.st_size);
            auto modtime        = Blizzard::Time::FromUnixTime(info.st_mtim.tv_sec);
            infoptr->createtime = modtime;
            infoptr->writetime  = modtime;
            infoptr->name       = nullptr;
            infoptr->normal     = S_ISREG(info.st_mode) ? 1 : 0;
            infoptr->attributes = S_ISDIR(info.st_mode) ? BC_FILE_ATTRIBUTE_DIRECTORY : BC_FILE_ATTRIBUTE_NORMAL;
            infoptr->filetype   = S_ISDIR(info.st_mode) ? 2 : 1;
            return false;
        }

        parms->info->filetype = 0;
    }

    BC_FILE_SET_ERROR(8);
    return false;
}

// getfreespace
bool GetFreeSpace(FileParms* parms) {
    if (parms->name && *parms->name) {
        char name[BC_FILE_MAX_PATH];
        Blizzard::String::MakeNativePath(parms->name, name, BC_FILE_MAX_PATH);

        struct statfs fsinfo;

        auto terminator = name;
        while (name[0] && ::statfs(name, &fsinfo)) {
            terminator = Blizzard::String::FindFilename(name);
            if (name == terminator) {
                goto fail;
            }

            terminator[-1] = '\0';
        }

        parms->offset = static_cast<int64_t>(fsinfo.f_bavail) * static_cast<int64_t>(fsinfo.f_bsize);
        return true;
    }

fail:
    BC_FILE_SET_ERROR(8);
    return false;
}

// getpos
bool GetPos(FileParms* parms) {
    auto offset = ::lseek(parms->file->filefd, 0, SEEK_CUR);
    if (offset == -1) {
        // FileError(8)
        BC_FILE_SET_ERROR(8);
        return false;
    }

    parms->offset = static_cast<int64_t>(offset);

    return true;
}

// getrootchars
bool GetRootChars(FileParms* parms) {
    auto name = parms->name;

    if (*name == '/' || *name == '\\') {
        parms->offset = 1;

#if defined(WHOA_SYSTEM_MAC)
        if (Blizzard::String::EqualI(name + 1, "Volumes", 7) && (name[8] == '/' || name[8] == '\\')) {
            auto volume = name + 9;

            while (*volume) {
                volume++;
                if (*volume == '/' || *volume == '\\') {
                    break;
                }
            }

            parms->offset = volume - name;
        }
#endif
    } else {
        parms->offset = 0;
    }

    return true;
}

// isabspath
bool IsAbsolutePath(FileParms* parms) {
    return *parms->name == '/' || *parms->name == '\\';
}

// isreadonly
bool IsReadOnly(FileParms* parms) {
    if (!Blizzard::File::IsFile(parms->name)) {
        return false;
    }
    Blizzard::File::StreamRecord* file;
    if (!Blizzard::File::Open(parms->name, Blizzard::File::Mode::write | Blizzard::File::Mode::sharewrite | Blizzard::File::Mode::mustexist, file)) {
        return true;
    }
    Blizzard::File::Close(file);

    return false;
}

// makeabspath
bool MakeAbsolutePath(FileParms* parms) {
    char namebuffer[BC_FILE_MAX_PATH];
    auto name = parms->buffersize > BC_FILE_MAX_PATH
                    ? static_cast<char*>(Blizzard::Memory::Allocate(parms->buffersize))
                    : namebuffer;

    *name = '\0';
    if (!Blizzard::File::IsAbsolutePath(parms->name)) {
        Blizzard::File::GetWorkingDirectory(name, parms->buffersize);
        Blizzard::String::ForceTrailingSeparator(name, parms->buffersize, '/');
    }
    Blizzard::String::Append(name, parms->name, parms->buffersize);
    char resultbuffer[BC_FILE_MAX_PATH];
    auto result = parms->buffersize > BC_FILE_MAX_PATH
                      ? static_cast<char*>(Blizzard::Memory::Allocate(parms->buffersize))
                      : resultbuffer;
    Blizzard::String::MakeUnivPath(name, result, parms->buffersize);

    auto cur = result + 1;
    auto dst = result;

    char* slash;
    // TODO: deduplicate
    while ((slash = Blizzard::String::Find(cur, '/', parms->buffersize))) {
        if (slash[1] != '.') {
            if (dst > result && cur == slash) {
                Blizzard::String::Copy(slash, slash + 1, parms->buffersize);
            } else {
                dst = slash;
                cur = slash + 1;
            }
        } else if (slash[2] == '.') {
            if (slash[3] == '/') {
                Blizzard::String::Copy(cur, slash + 4, parms->buffersize);
            } else {
                if (dst > result && cur == slash) {
                    Blizzard::String::Copy(slash, slash + 1, parms->buffersize);
                } else {
                    dst = slash;
                    cur = slash + 1;
                }
            }
        } else {
            if (slash[2] == '/') {
                Blizzard::String::Copy(slash + 1, slash + 3, parms->buffersize);
            } else {
                if (dst > result && cur == slash) {
                    Blizzard::String::Copy(slash, slash + 1, parms->buffersize);
                } else {
                    dst = slash;
                    cur = slash + 1;
                }
            }
        }
    }

    auto length = Blizzard::String::Length(result);
    if (length > 2) {
        if (result[length - 1] == '.' && result[length - 2] == '/') {
            result[length - 1] = '\0';
        }
    }

    if (parms->canonicalize) {
        char canonicalbuffer[BC_FILE_MAX_PATH];
        char resolvedbuffer[BC_FILE_MAX_PATH];
        auto canonical = parms->buffersize > BC_FILE_MAX_PATH
                             ? static_cast<char*>(Blizzard::Memory::Allocate(parms->buffersize))
                             : canonicalbuffer;
        auto cur       = canonical;

        auto src   = result;
        auto begin = src;
        while (*src) {
            while (*src && *src != '/') {
                src++;
            }

            auto v15 = (src + 1) - begin;
            Blizzard::String::Copy(cur, begin, v15 + 1);

            auto resolved = parms->buffersize > BC_FILE_MAX_PATH ? static_cast<char*>(Blizzard::Memory::Allocate(parms->buffersize)) : resolvedbuffer;
            if (::realpath(canonical, resolved)) {
                Blizzard::String::Copy(canonical, resolved, parms->buffersize);
                if (src[0] == '/' || src[0] == '\0' && src[-1] == '/') {
                    Blizzard::String::ForceTrailingSeparator(canonical, parms->buffersize, '/');
                }
                cur = canonical + Blizzard::String::Length(canonical);
            } else {
                cur += v15;
            }

            if (*src) {
                src   = src + 1;
                begin = src;
            }

            if (resolved != resolvedbuffer) {
                Blizzard::Memory::Free(resolved);
            }
        }

        Blizzard::String::Copy(result, canonical, parms->buffersize);
        if (canonical != canonicalbuffer) {
            Blizzard::Memory::Free(canonical);
        }
    }

    Blizzard::String::Copy(parms->buffer, result, parms->buffersize);

    if (name != namebuffer) {
        Blizzard::Memory::Free(name);
    }
    if (result != resultbuffer) {
        Blizzard::Memory::Free(result);
    }

    return *parms->buffer != '\0';
}

// mkdir
bool CreateDirectory(FileParms* parms) {
    if (!parms->name) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    char name[BC_FILE_MAX_PATH];
    char dir[BC_FILE_MAX_PATH];
    Blizzard::String::MakeNativePath(parms->name, name, BC_FILE_MAX_PATH);

    if (parms->recurse) {
        auto path = name;
        while (*path) {
            while (*path && *path != '/') {
                path++;
            }

            auto length = path - name;
            Blizzard::String::Copy(dir, name, length < BC_FILE_MAX_PATH ? length : BC_FILE_MAX_PATH);

            if (!Blizzard::File::IsDirectory(dir)) {
                if (::mkdir(parms->name, 0777) != 0) {
                    return false;
                }
            }

            if (*path) {
                path++;
            }
        }
        return true;
    }

    if (!Blizzard::File::IsDirectory(parms->name)) {
        return ::mkdir(parms->name, 0777) == 0;
    }

    return true;
}

// move
bool Move(FileParms* parms) {
    struct stat info;

    char name[BC_FILE_MAX_PATH];
    char newname[BC_FILE_MAX_PATH];

    Blizzard::String::MakeNativePath(parms->name, name, BC_FILE_MAX_PATH);
    Blizzard::String::MakeNativePath(parms->newname, newname, BC_FILE_MAX_PATH);

    if (::stat(newname, &info) == 0) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    // if on the same device, just rename the file
    if (::rename(name, newname) == 0) {
        return true;
    }
    if (errno != EXDEV) {
        // there's some other reason why rename wouldn't work, besides being on different devices
        BC_FILE_SET_ERROR(8);
        return false;
    }

    if (Blizzard::File::Copy(name, newname, false)) {
        Blizzard::File::Delete(name);
    }
    return true;
}

// copy
bool Copy(FileParms* parms) {
    char name[BC_FILE_MAX_PATH];
    char newname[BC_FILE_MAX_PATH];
    Blizzard::String::MakeNativePath(parms->name, name, BC_FILE_MAX_PATH);
    Blizzard::String::MakeNativePath(parms->newname, newname, BC_FILE_MAX_PATH);

    Blizzard::File::StreamRecord* src;
    Blizzard::File::StreamRecord* dst;

    if (!Blizzard::File::Open(name, Blizzard::File::Mode::read | Blizzard::File::Mode::shareread | Blizzard::File::Mode::mustexist, src)) {
        BC_FILE_SET_ERROR(2); // TODO: find out what 2 is
        return false;
    }

    if (!Blizzard::File::Open(name, Blizzard::File::Mode::write | (parms->overwrite ? Blizzard::File::Mode::create : Blizzard::File::Mode::create | Blizzard::File::Mode::mustnotexist), dst)) {
        BC_FILE_SET_ERROR(4); // TODO: find out what 4 is
        return false;
    }

    auto size           = Blizzard::File::GetFileInfo(src)->size;
    auto copybuffersize = size > BC_SYSTEM_FILE_COPYBUFFER_SIZE ? BC_SYSTEM_FILE_COPYBUFFER_SIZE : size;
    auto copybuffer     = Blizzard::Memory::Allocate(copybuffersize);

    int64_t offset = 0;

    bool result = true;

    while (offset < size) {
        int32_t count = size - offset >= BC_SYSTEM_FILE_COPYBUFFER_SIZE ? BC_SYSTEM_FILE_COPYBUFFER_SIZE : size - offset;
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
    if (result == false) {
        Blizzard::File::Delete(newname);
    }
    return result;
}

bool LockFileDescriptorForWrite(int filefd) {
    struct flock lock = {};
    lock.l_type       = F_WRLCK;

    if (::fcntl(filefd, F_SETLK, &lock) == 0) {
        return true;
    }

    if (errno == ENOTSUP) {
        if (::flock(filefd, LOCK_EX | LOCK_NB) == 0) {
            return true;
        }
    }

    BC_FILE_SET_ERROR_MSG(4, "Posix Open");
    ::close(filefd);
    return false;
}

// open
bool Open(FileParms* parms) {
    Blizzard::String::QuickNativePath<BC_FILE_MAX_PATH> name(parms->name);

    auto read  = (parms->mode & Blizzard::File::Mode::read) != 0;
    auto write = (parms->mode & Blizzard::File::Mode::write) != 0;

    BLIZZARD_ASSERT(read || write);
    if (!read && !write) {
        BC_FILE_SET_ERROR_MSG(8, "Posix Open - Bad file mode - %s", name.ToString());
        return false;
    }

    int flags;
    if (!read || !write) {
        flags = read ? O_RDONLY : O_WRONLY;
    } else {
        flags = O_RDWR;
    }

#if defined(WHOA_SYSTEM_LINUX)
    flags |= (parms->mode & Blizzard::File::Mode::nocache) ? O_DIRECT : 0;
#endif

    flags |= ((parms->mode & Blizzard::File::Mode::truncate) != 0 ? O_TRUNC : 0) | ((parms->mode & Blizzard::File::Mode::mustexist) == 0 ? O_CREAT : 0) | ((parms->mode & Blizzard::File::Mode::mustnotexist) != 0 ? O_EXCL : 0) | ((parms->mode & Blizzard::File::Mode::create) != 0 ? O_CREAT | O_TRUNC : 0);

    auto filefd = flags & O_CREAT ? ::open(name.ToString(), flags, 0777) : ::open(name.ToString(), flags);
    if (filefd == -1) {
        printf("opened? '%s'\n", name.ToString());
        BC_FILE_SET_ERROR_MSG(8, "Posix Open - %s", name.ToString());
        return false;
    }

    if (System_File::s_EnableFileLocks) {
        if ((parms->mode & Blizzard::File::Mode::shareread) == 0 && (parms->mode & Blizzard::File::Mode::write) != 0) {
                if (!LockFileDescriptorForWrite(filefd)) {
                    return false;
                }
            }
    }

    if (parms->mode & Blizzard::File::Mode::append) {
        ::lseek(filefd, 0, SEEK_END);
    }

    struct stat info;
    if (::fstat(filefd, &info) != 0 || S_ISDIR(info.st_mode)) {
        BC_FILE_SET_ERROR_MSG(2, "Posix Read - File not found - %s", name.ToString());
        ::close(filefd);
        return false;
    }

#if defined(WHOA_SYSTEM_MAC)
    if (parms->mode & Blizzard::File::Mode::nocache) {
        ::fcntl(filefd, F_NOCACHE, 1);
    }
#endif

    if (parms->mode & Blizzard::File::Mode::temporary) {
        ::unlink(name.ToString());
    }

    auto file = parms->file;

    if (!file) {
        auto namelength = Blizzard::String::Length(name.ToString()) + 1;
        file            = reinterpret_cast<Blizzard::File::StreamRecord*>(Blizzard::Memory::Allocate(sizeof(Blizzard::File::StreamRecord) + namelength));
        auto filename   = reinterpret_cast<char*>(file) + sizeof(Blizzard::File::StreamRecord);
        file->name      = filename;
        Blizzard::String::Copy(filename, name.ToString(), namelength);

    }

    file->filefd   = filefd;
    file->haveinfo = false;
    file->mode     = parms->mode;

    // if (!file->unk48 || !*file->unk48) {
    file->info.size = info.st_size;
    // }

    auto modtime = Blizzard::Time::FromUnixTime(info.st_mtim.tv_sec);

    file->info.createtime = modtime;
    file->info.writetime  = modtime;
    file->info.filetype   = S_ISDIR(info.st_mode) ? 2 : 0;

    file->haveinfo = true;

    parms->file = file;

    return true;
}

// read
// readp

// rmdir
bool RemoveDirectory(FileParms* parms) {
    char name[BC_FILE_MAX_PATH];
    if (!parms->name) {
        BC_FILE_SET_ERROR(8);
        return false;
    }
    Blizzard::String::MakeNativePath(parms->name, name, BC_FILE_MAX_PATH);
    return ::rmdir(name) == 0;
}

// setcachemode
bool SetCacheMode(FileParms* parms) {
#if defined(WHOA_SYSTEM_MAC)
    auto mode = parms->mode;
    BLIZZARD_ASSERT(0 == (mode & ~Blizzard::File::Mode::nocache));
    Blizzard::File::Flush(parms->file);
    return 0 == fcntl(parms->file->filefd, F_NOCACHE, mode & Blizzard::File::Mode::nocache);
#endif

#if defined(WHOA_SYSTEM_LINUX)
    // TODO: implement equivalent fcntl
    BC_FILE_SET_ERROR(8);
    return false;
#endif
}

// seteof
bool SetEOF(FileParms* parms) {
    if (!Blizzard::File::SetPos(parms->file, parms->offset, parms->whence)) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    int64_t offset;
    if (!Blizzard::File::GetPos(parms->file, offset)) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    parms->offset = offset;
    if (::ftruncate(parms->file->filefd, offset) == -1) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    return true;
}

// setfileinfo
bool SetAttributes(FileParms* parms) {
    auto file = parms->file;

    if (parms->setinfo & BC_SYSTEM_FILE_INFO_TIMES) {
        struct timeval times[2];

        times[0].tv_sec  = Blizzard::Time::ToUnixTime(file->info.writetime);
        times[0].tv_usec = 0;

        times[1].tv_sec  = times[0].tv_sec;
        times[1].tv_usec = 0;

        if (::futimes(file->filefd, times) != 0) {
            BC_FILE_SET_ERROR(8);
            return false;
        }

        parms->setinfo &= ~(BC_SYSTEM_FILE_INFO_TIMES);
    }

    if (parms->setinfo & BC_SYSTEM_FILE_INFO_ATTRIBUTES) {
        struct stat info;
        if (::stat(PATH(parms->name), &info) == -1) {
            BC_FILE_SET_ERROR(8);
            return false;
        }

        auto mode = parms->info->attributes & BC_FILE_ATTRIBUTE_READONLY
            ? info.st_mode & (S_IFSOCK | S_IFLNK | S_IFIFO | S_ISUID | S_ISGID | S_ISVTX | 0555)
            : info.st_mode | 0222;

        if (::chmod(PATH(parms->name), mode) != 0) {
            BC_FILE_SET_ERROR(8);
            return false;
        }

        parms->setinfo &= ~(BC_SYSTEM_FILE_INFO_ATTRIBUTES);
    }

    return parms->setinfo == 0;
}

// setpos
bool SetPos(FileParms* parms) {
    auto file   = parms->file;
    auto offset = static_cast<off_t>(parms->offset);
    auto whence = static_cast<int>(parms->whence);
    if (whence != 0) {
        whence = (whence != 1) + 1;
    }

    if (::lseek(file->filefd, offset, whence) == -1) {
        BC_FILE_SET_ERROR(8);
        return false;
    }

    return true;
}

// unlink
bool Delete(FileParms* parms) {
    return ::unlink(PATH(parms->name)) != -1;
}

// write
// writep

// shutdown
bool Shutdown(FileParms* parms) {
    // ?
    return true;
}

} // namespace Stacked
} // namespace System_File

#endif
