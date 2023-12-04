#include "bc/file/path/Path.hpp"
#include "bc/file/path/Posix.hpp"
#include "bc/String.hpp"
#include "bc/Memory.hpp"
#include "bc/Debug.hpp"

#include <cctype>

namespace Blizzard {
namespace File {
namespace Path {

// Classes

QuickNative::QuickNative(const char* path) {
    this->size = 0;
    this->slowpath = nullptr;
    this->fastpath[0] = '\0';

    if (!path) {
        return;
    }

    // Null byte
    constexpr size_t reserved = 1;

    this->size = String::Length(path) + reserved;

    char* nativePath = nullptr;

    if (this->size < BC_FILE_MAX_PATH) {
        // (fast)
        nativePath = this->fastpath;
        MakeNativePath(path, this->fastpath, BC_FILE_MAX_PATH);
    } else {
        // (slow)
        this->slowpath = reinterpret_cast<char*>(Memory::Allocate(this->size));
        nativePath = this->slowpath;
        MakeNativePath(path, this->slowpath, this->size);
    }

#if defined(WHOA_SYSTEM_LINUX)
    // Linux typically does not involve a case-sensitive filesystem
    // For compatibility, try to find if this path exists in another case
    // This is much slower

    // The size of the resolved path. Maybe be larger due to "./"
    size_t resolvedSize = this->size + 2;

    auto resolvedPath = static_cast<char*>(Memory::Allocate(resolvedSize));
    resolvedPath[resolvedSize-1] = '\0';

    bool wasResolved = ResolvePosixCasePath(nativepath, resolvedPath);
    if (!wasResolved) {
        Memory::Free(resolvedPath);
        return;
    }

    if (this->slowpath != nullptr) {
        Memory::Free(this->slowpath);
    }

    this->slowpath = resolvedPath;
#endif
}

QuickNative::~QuickNative() {
    if (this->slowpath != nullptr) {
        Memory::Free(this->slowpath);
    }
}

const char* QuickNative::Str() {
    if (this->slowpath != nullptr) {
        return this->slowpath;
    }

    return this->fastpath;
}

size_t QuickNative::Size() {
    return this->size;
}

// Functions

void ForceTrailingSeparator(char* buf, size_t bufMax, char sep) {
    if (buf == nullptr) {
        return;
    }

    // If no separator character is provided, infer correct separator based on runes
    if (sep == '\0') {
        auto ptr = buf;

        char ch;

        while (true) {
            ch = *ptr++;
            // Make inference based on what data we have.
            if (ch == '/' || ch == '\\') {
                sep = ch;
                break;
            }

            if (ch == '\0') {
                // Fail safe to the system's path separator.
                sep = BC_FILE_SYSTEM_PATH_SEPARATOR;
                break;
            }
        }
    }

    // Buffer needs at least two characters.
    if (bufMax < 2) {
        return;
    }

    // Slice off the end of the path buffer, so that any existing* trailing separator is discarded.
    auto len = String::Length(buf);
    char ch = '\0';
    if (len > 0) {
        ch = buf[len - 1];
    }
    switch (ch) {
    case '/':
    case '\\':
        len--;
        break;
    default:
        break;
    }

    // Add (back*) trailing separator
    BLIZZARD_ASSERT(len <= bufMax - 2);
    buf[len] = sep;
    buf[len + 1] = '\0'; // pad null
}

// Convert path to DOS-style.
// the function will iterate through the path string, converting all occurrences of forward slashes (/) to backslashes (\)
bool MakeBackslashPath(const char* path, char* result, size_t capacity) {
    size_t i = 0;
    size_t c = capacity - 1;

    while (i <= c) {
        if (path[i] == '\0') {
            result[i] = '\0';
            return true;
        }

        result[i] = path[i] == '/' ? '\\' : path[i];

        i++;
    }

    result[0] = '\0';
    return false;
}

// Make a path string consistent before the last path separator.
bool MakeConsistentPath(const char* path, char* result, size_t capacity) {
    if (!result || (capacity < 1)) {
        return false;
    }

    if (!path || (capacity == 1)) {
        *result = '\0';
        return false;
    }

    for (auto i = capacity - 1; i != -1; i--) {
        auto ch = path[i];

        switch (ch) {
        case '\\':
            return MakeBackslashPath(path, result, capacity);
        case '/':
            return MakeUnivPath(path, result, capacity);
        case '\0':
        default:
            break;
        }
    }

    String::Copy(result, path, capacity);
    return true;
}

// Convert any path string into something that can be used on the current OS.
bool MakeNativePath(const char* path, char* result, size_t capacity) {
#if defined(WHOA_SYSTEM_WIN)
    return MakeWindowsPath(path, result, capacity);
#else
    return MakeUnivPath(path, result, capacity);
#endif
}

// Convert a path string into something UNIX-friendly.
bool MakeUnivPath(const char* path, char* result, size_t capacity) {
    size_t i = 0; // path and result index
    size_t c = capacity - 1; // ceiling

    while (i <= c) {
        if (path[i] == '\0') {
            result[i] = '\0';
            return true;
        }

        result[i] = path[i] == '\\' ? '/' : path[i];

        i++;
    }

    result[0] = '\0';
    return false;
}

bool MakeWindowsPath(const char* path, char* result, size_t capacity) {
    return MakeBackslashPath(path, result, capacity);
}

} // namespace Path
} // namespace File
} // namespace Blizzard
