#include "bc/os/Path.hpp"
#include "bc/String.hpp"

// Win32
#if defined(WHOA_SYSTEM_WIN)
#include <windows.h>
#endif

// Darwin
#if defined(WHOA_SYSTEM_MAC)
#include <cstdlib>
#include <mach-o/dyld.h>
#endif

#if defined(WHOA_SYSTEM_LINUX)
#include <unistd.h>
#endif

// Get the full path of the currently running .exe/ELF/Mach-O executable
void OsGetExeName(char* buffer, uint32_t buffersize) {
#if defined(WHOA_SYSTEM_WIN)
    ::GetModuleFileName(nullptr, buffer, buffersize);
#endif

#if defined(WHOA_SYSTEM_MAC)
    char executablepathbuffer[4096];
    uint32_t executablepathbuffersize = sizeof(executablepathbuffer);
    ::_NSGetExecutablePath(executablepathbuffer, &executablepathbuffersize);

    char realpathbuffer[4096];
    ::realpath(executablepathbuffer, realpathbuffer);

    Blizzard::String::Copy(buffer, realpathbuffer, buffersize);
#endif

#if defined(WHOA_SYSTEM_LINUX)
    // procfs
    if (::readlink("/proc/self/exe", buffer, buffersize) == -1) {
        *buffer = '\0';
    }
#endif
}

void OsPathStripFilename(char* name) {
    auto n = name;
    while (*n) {
        n++;
    }
    if (n > name) {
        while (n > name) {
            if (*n == '/' || *n == '\\') {
                break;
            }
            n--;
        }

        *n = '\0';
    }
}

// Get the directory containing the currently running executable
void OsGetExePath(char* buffer, uint32_t buffersize) {
    OsGetExeName(buffer, buffersize);
    OsPathStripFilename(buffer);
}
