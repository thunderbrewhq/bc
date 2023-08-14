#include "bc/os/File.hpp"
#include "bc/file/Defines.hpp"
#include "bc/file/File.hpp"
#include "bc/Debug.hpp"

HOSFILE OsCreateFile(const char* fileName, uint32_t desiredAccess, uint32_t shareMode, uint32_t createDisposition, uint32_t flagsAndAttributes, uint32_t extendedFileType) {
    // Ensure sanity
    BLIZZARD_VALIDATE(fileName, "invalid filename", nullptr);
    BLIZZARD_VALIDATE(desiredAccess != 0, "invalid desired access", nullptr);
    BLIZZARD_VALIDATE(createDisposition <= OS_TRUNCATE_EXISTING, "invalid create disposition", nullptr);

    uint32_t flags;

    // Read/write flags
    if (desiredAccess & OS_GENERIC_READ) {
        flags |= BC_FILE_OPEN_READ;
    }
    if (desiredAccess & OS_GENERIC_WRITE) {
        flags |= BC_FILE_OPEN_WRITE;
    }

    // Allow other users to access the file in read and/or write mode
    if (shareMode & OS_FILE_SHARE_READ) {
        flags |= BC_FILE_OPEN_SHARE_READ;
    }
    if (shareMode & OS_FILE_SHARE_WRITE) {
        flags |= BC_FILE_OPEN_SHARE_WRITE;
    }

    // Convert createDisposition into BC flags
    switch (createDisposition) {
    case OS_CREATE_NEW:
        // flags |= 0xC00;
        flags |= BC_FILE_OPEN_CREATE | BC_FILE_OPEN_MUST_NOT_EXIST;
        break;
    case OS_CREATE_ALWAYS:
        // flags |= 0x400;
        flags |= BC_FILE_OPEN_CREATE;
        break;
    case OS_OPEN_EXISTING:
        // flags |= 0x1000
        flags |= BC_FILE_OPEN_MUST_EXIST;
        break;
    case OS_OPEN_ALWAYS:
        // flags |= 0x200;
        flags |= BC_FILE_OPEN_ALWAYS;
        break;
    case OS_TRUNCATE_EXISTING:
        // flags |= 0x100;
        flags |= BC_FILE_OPEN_TRUNCATE;
        break;
    }

    // Open file
    Blizzard::File::StreamRecord* stream;
    bool success = Blizzard::File::Open(fileName, flags, stream);
    if (!success) {
        return nullptr;
    }

    // Set attributes
    OsSetFileAttributes(fileName, flagsAndAttributes);
    return stream;
}

int32_t OsSetFileAttributes(const char* fileName, uint32_t attributes) {
    BLIZZARD_ASSERT(fileName);

    // Translate OS file attribute bits into BlizzardCore attribute bits.
    uint32_t flags = 0;

    if (attributes & OS_FILE_ATTRIBUTE_READONLY) {
        // flags |= 1;
        flags |= BC_FILE_ATTRIBUTE_READONLY;
    }
    if (attributes & OS_FILE_ATTRIBUTE_HIDDEN) {
        // flags |= 2;
        flags |= BC_FILE_ATTRIBUTE_HIDDEN;
    }
    if (attributes & OS_FILE_ATTRIBUTE_SYSTEM) {
        // flags |= 4
        flags |= BC_FILE_ATTRIBUTE_SYSTEM;
    }
    if (attributes & OS_FILE_ATTRIBUTE_ARCHIVE) {
        // flags |= 8;
        flags |= BC_FILE_ATTRIBUTE_ARCHIVE;
    }
    if (attributes & OS_FILE_ATTRIBUTE_TEMPORARY) {
        // flags |= 0x10;
        flags |= BC_FILE_ATTRIBUTE_TEMPORARY;
    }
    if (attributes & OS_FILE_ATTRIBUTE_NORMAL) {
        // flags |= 0x20;
        flags |= BC_FILE_ATTRIBUTE_NORMAL;
    }
    if (attributes & OS_FILE_ATTRIBUTE_DIRECTORY) {
        // flags |= 0x40;
        flags |= BC_FILE_ATTRIBUTE_DIRECTORY;
    }

    return Blizzard::File::SetAttributes(fileName, flags);
}

uint64_t OsGetFileSize(HOSFILE fileHandle) {
    auto info = Blizzard::File::GetFileInfo(reinterpret_cast<Blizzard::File::StreamRecord*>(fileHandle));
    return info->size;
}

int32_t OsReadFile(HOSFILE fileHandle, void* buffer, size_t bytesToRead, size_t* bytesRead) {
    BLIZZARD_ASSERT(buffer);
    BLIZZARD_ASSERT(bytesToRead);

    return Blizzard::File::Read(fileHandle, buffer, bytesToRead, bytesRead);
}

int32_t OsWriteFile(HOSFILE fileHandle, void* buffer, size_t bytesToWrite, size_t* bytesWritten) {
    if (buffer != nullptr && bytesToWrite != 0) {
        return Blizzard::File::Write(fileHandle, buffer, bytesToWrite, bytesWritten);
    }

    return 0;
}

int64_t OsSetFilePointer(HOSFILE fileHandle, int64_t distanceToMove, uint32_t moveMethod) {
    BLIZZARD_ASSERT(moveMethod <= 2);

    int64_t position;

    if (moveMethod != 0 && !Blizzard::File::GetPos(fileHandle, position)) {
        return -1;
    }

    uint32_t seeks[3] = {
        BC_FILE_SEEK_START,
        BC_FILE_SEEK_CURRENT,
        BC_FILE_SEEK_END
    };

    if (!Blizzard::File::SetPos(fileHandle, position, seeks[moveMethod])) {
        return -1;
    }

    return position + distanceToMove;
}

void OsCloseFile(HOSFILE fileHandle) {
    Blizzard::File::Close(static_cast<Blizzard::File::StreamRecord*>(fileHandle));
}
