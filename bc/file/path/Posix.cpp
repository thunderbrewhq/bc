#include "bc/file/path/Posix.hpp"

#if defined(WHOA_SYSTEM_LINUX)
#include <cstring>
#include <dirent.h>
#include <alloca.h>
#include <cstdio>
#include <cstdint>

// adapted from https://github.com/OneSadCookie/fcaseopen/blob/master/fcaseopen.c
bool ResolvePosixCasePath(const char* path, char* r) {
    auto l = strlen(path);
    auto p = static_cast<char*>(alloca(l + 1));
    strcpy(p, path);
    size_t rl = 0;

    DIR* d;
    if (p[0] == '/') {
        d = opendir("/");
        p = p + 1;
    } else {
        d = opendir(".");
        r[0] = '.';
        r[1] = 0;
        rl = 1;
    }

    int32_t last = 0;
    auto c = strsep(&p, "/");
    while (c) {
        if (!d) {
            return false;
        }

        if (last) {
            closedir(d);
            return false;
        }

        r[rl] = '/';
        rl += 1;
        r[rl] = 0;

        struct dirent* e = readdir(d);
        while (e) {
            if (strcasecmp(c, e->d_name) == 0) {
                strcpy(r + rl, e->d_name);
                rl += strlen(e->d_name);

                closedir(d);
                d = opendir(r);

                break;
            }

            e = readdir(d);
        }

        if (!e) {
            strcpy(r + rl, c);
            rl += strlen(c);
            last = 1;
        }

        c = strsep(&p, "/");
    }

    if (d) {
        closedir(d);
    }

    return true;
}

#endif
