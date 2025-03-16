#include "bc/os/file/FileList.hpp"
#include "bc/file/Defines.hpp"
#include "bc/file/GetFileInfo.hpp"
#include "bc/file/MakeAbsolutePath.hpp"
#include "bc/file/ProcessDirFast.hpp"
#include "bc/file/SimpleGlob.hpp"
#include "bc/file/Types.hpp"
#include "bc/string/Copy.hpp"
#include "bc/string/Path.hpp"

int32_t OsFileList(const char* dir, const char* pattern, OsFileListCallback callback, void* param, int32_t flags) {
    class Internal {
        public:
        struct FileListParms {
            OsFileListCallback callback;
            void* param;
            uint32_t flags;
            const char* pattern;
        };

        static bool Callback(const Blizzard::File::ProcessDirParms& parms) {
            char path[260];
            OS_FILE_DATA data;
            Blizzard::String::Copy(data.fileName, parms.item, sizeof(data.fileName));
            Blizzard::String::JoinPath(path, sizeof(path), parms.dir, parms.item);

            auto list = static_cast<FileListParms*>(parms.param);

            if (Blizzard::File::SimpleGlob(parms.item, list->pattern)) {
                Blizzard::File::FileInfo info;
                if (Blizzard::File::GetFileInfo(path, &info)) {
                    // TODO: consider making this 64 bits
                    data.size = static_cast<uint32_t>(info.size);
                    data.flags = 0;

                    if (info.attributes & BC_FILE_ATTRIBUTE_DIRECTORY) {
                        data.flags |= 0x10;
                    }
                    if (info.attributes & BC_FILE_ATTRIBUTE_READONLY) {
                        data.flags |= 0x01;
                    }
                    if (info.attributes & BC_FILE_ATTRIBUTE_HIDDEN) {
                        data.flags |= 0x02;
                    }

                    if (list->flags && ((info.attributes & BC_FILE_ATTRIBUTE_HIDDEN) == 0)) {
                        if (list->callback(data, list->param)) {
                            return false;
                        }
                    }
                }
            }

            return true;
        }
    };

    char finddir[260];
    char findpath[260];
    Blizzard::String::Copy(finddir, dir, 260);

    if (!Blizzard::File::MakeAbsolutePath(finddir, findpath, 260, false)) {
        return false;
    }

    Internal::FileListParms parms;
    parms.param    = param;
    parms.flags    = flags;
    parms.callback = callback;
    parms.pattern  = pattern;

    return Blizzard::File::ProcessDirFast(findpath, static_cast<void*>(&parms), Internal::Callback, false);
}