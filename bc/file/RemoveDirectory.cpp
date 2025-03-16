#include "bc/file/RemoveDirectory.hpp"
#include "bc/file/ProcessDirFast.hpp"
#include "bc/file/Defines.hpp"
#include "bc/file/Filesystem.hpp"
#include "bc/file/SetAttributes.hpp"
#include "bc/file/Exists.hpp"
#include "bc/file/Delete.hpp"
#include "bc/system/file/Stacked.hpp"
#include "bc/String.hpp"

namespace Blizzard {
namespace File {

bool RemoveDirectory(const char* name) {
    System_File::Stacked::FileParms parms;
    parms.op      = offsetof(Filesystem, rmdir);
    parms.name    = name;
    parms.recurse = false;

    auto fs = System_File::Stacked::s_manager;
    return fs ? fs->rmdir(fs, &parms) : false;
}

bool RemoveDirectoryAndContents(const char* name, bool noreadonly) {
    class Internal {
        public:
        struct RemoveDirectoryRecurseData {
            bool unk00;
            bool noreadonly;
        };

        static bool RemoveDirectoryRecurse(RemoveDirectoryRecurseData* data, const char* name, bool noreadonly) {
            data->unk00      = true;
            data->noreadonly = noreadonly;

            return File::ProcessDirFast(name, static_cast<void*>(data), Callback, false) && data->unk00;
        }

        static bool Callback(const ProcessDirParms& dirwalkparms) {
            char name[BC_FILE_MAX_PATH];

            auto rmdirdata = reinterpret_cast<RemoveDirectoryRecurseData*>(dirwalkparms.param);

            bool removeditem = false;

            if (dirwalkparms.isdir) {
                String::Format(name, BC_FILE_MAX_PATH, "%s%s/", dirwalkparms.dir, dirwalkparms.item);
                removeditem = File::RemoveDirectoryAndContents(name, true);
            } else {
                String::Format(name, BC_FILE_MAX_PATH, "%s%s", dirwalkparms.dir, dirwalkparms.item);

                if (rmdirdata->noreadonly) {
                    File::SetAttributes(name, BC_FILE_ATTRIBUTE_NORMAL);
                }

                // TODO: ???
                // if (sub_44EFB0(name)) {
                //     return true;
                // }

                removeditem = File::Delete(name);
            }

            if (!removeditem) {
                rmdirdata->unk00 = false;
            }

            return true;
        }
    };

    if (File::Exists(name) != 2) {
        return false;
    }

    char path[BC_FILE_MAX_PATH];
    String::Copy(path, name, BC_FILE_MAX_PATH);
    String::ForceTrailingSeparator(path, BC_FILE_MAX_PATH, '\0');

    Internal::RemoveDirectoryRecurseData rmdirdata;
    if (!Internal::RemoveDirectoryRecurse(&rmdirdata, path, noreadonly)) {
        return false;
    }

    return File::RemoveDirectory(name);
}

} // namespace File
} // namespace Blizzard

