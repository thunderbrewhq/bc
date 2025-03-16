#include "bc/os/file/CloseFile.hpp"
#include "bc/file/Close.hpp"

void OsCloseFile(HOSFILE fileHandle) {
    Blizzard::File::Close(reinterpret_cast<Blizzard::File::StreamRecord*>(fileHandle));
}