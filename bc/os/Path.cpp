#include "bc/os/Path.hpp"
#include "bc/String.hpp"
#include "bc/file/Path.hpp"

// Win32
#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

// Darwin
#if defined(WHOA_SYSTEM_MAC)
#include <mach-o/dyld.h>
#endif

// procfs
#if defined(WHOA_SYSTEM_LINUX)
#include <unistd.h>
#endif

// Get the full path of the currently running .exe/ELF/Mach-O executable
void OsGetExeName(char* buffer, size_t chars) {
#if defined(WHOA_SYSTEM_WIN)
    // Win32
    GetModuleFileName(nullptr, buffer, chars);
#endif

#if defined(WHOA_SYSTEM_MAC)
    // Darwin
    char path[1024] = {0};
    _NSGetExecutablePath(path, 1024);

    char actualPath[1024] = {0};
    realpath(path, actualPath);

    Blizzard::File::Path::MakeBackslashPath(actualPath, buffer, chars);
#endif

#if defined(WHOA_SYSTEM_LINUX)
    // procfs
    char actualPath[4096] = {0};
    readlink("/proc/self/exe", actualPath, chars);
    Blizzard::File::Path::MakeBackslashPath(actualPath, buffer, chars);
#endif
}

void OsPathStripFilename(char* path) {
    auto length = Blizzard::String::Length(path);

    char* head  = &path[length-1];

    while ((head != (path-1)) && (*head != '\0')) {
        if (*head == '\\') {
            *(head + 1) = '\0';
            break;
        }
        head--;
    }
}

// Get the directory containing the currently running executable
void OsGetExePath(char* dest, size_t chars) {
    OsGetExeName(dest, chars);
    OsPathStripFilename(dest);
}
