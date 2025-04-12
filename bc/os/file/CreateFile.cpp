#include "bc/os/file/CreateFile.hpp"
#include "bc/os/file/SetFileAttributes.hpp"
#include "bc/Debug.hpp"
#include "bc/file/Open.hpp"
#include "bc/os/file/Types.hpp"

int32_t OsCreateFileMode(uint32_t desiredAccess, uint32_t shareMode, uint32_t createDisposition) {
    using Mode = Blizzard::File::Mode;

    int32_t mode = 0;

    if (desiredAccess & OS_GENERIC_READ) {
        mode |= Mode::read;
    }
    if (desiredAccess & OS_GENERIC_WRITE) {
        mode |= Mode::write;
    }

    if (shareMode & OS_FILE_SHARE_READ) {
        mode |= Mode::shareread;
    }
    if (shareMode & OS_FILE_SHARE_WRITE) {
        mode |= Mode::sharewrite;
    }

    switch (createDisposition) {
    case OS_CREATE_NEW:
        // mode |= 0xC00;
        mode |= (Mode::create|Mode::mustnotexist);
        break;
    case OS_CREATE_ALWAYS:
        // mode |= 0x400;
        mode |= Mode::create;
        break;
    case OS_OPEN_EXISTING:
        // mode |= 0x1000
        mode |= Mode::mustexist;
        break;
    case OS_OPEN_ALWAYS:
        // mode |= 0x200;
        mode |= Mode::append;
        break;
    case OS_TRUNCATE_EXISTING:
        // mode |= 0x100;
        mode |= Mode::truncate;
        break;
    }

    return mode;
}

HOSFILE OsCreateFile(const char* fileName, uint32_t desiredAccess, uint32_t shareMode, uint32_t createDisposition, uint32_t flagsAndAttributes, uint32_t extendedFileType) {
    BC_VALIDATE(fileName, "invalid filename", HOSFILE_INVALID);
    BC_VALIDATE(desiredAccess != 0, "invalid desired access", HOSFILE_INVALID);
    BC_VALIDATE(createDisposition <= OS_TRUNCATE_EXISTING, "invalid create disposition", HOSFILE_INVALID);

    Blizzard::File::StreamRecord* file;
    if (!Blizzard::File::Open(fileName, OsCreateFileMode(desiredAccess, shareMode, createDisposition), file)) {
        return HOSFILE_INVALID;
    }

    if ((flagsAndAttributes & OS_FILE_ATTRIBUTE_NORMAL) == 0) {
        OsSetFileAttributes(fileName, flagsAndAttributes);
    }

    return reinterpret_cast<HOSFILE>(file);
}
