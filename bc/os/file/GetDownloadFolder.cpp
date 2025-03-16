#include "bc/os/file/GetDownloadFolder.hpp"
#include "bc/file/GetWorkingDirectory.hpp"

const char* OsFileGetDownloadFolder() {
    static char s_downloadfolder[260] = { 0 };
    if (s_downloadfolder[0] == '\0') {
        Blizzard::File::GetWorkingDirectory(s_downloadfolder, 260);
    }
    return s_downloadfolder;
}