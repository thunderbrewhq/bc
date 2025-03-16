#ifndef BC_SYSTEM_FILE_SYSTEM_FILE_HPP
#define BC_SYSTEM_FILE_SYSTEM_FILE_HPP

#include "bc/Debug.hpp"
#include "bc/system/file/Types.hpp"
#include "bc/file/Filesystem.hpp"
#include <cstdint>

#if defined(WHOA_SYSTEM_WIN)
#include "bc/system/file/win/Support.hpp"
#endif

namespace System_File {

extern bool s_EnableFileLocks;

// Fs calls
bool SetWorkingDirectory(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Close(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Create(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool GetWorkingDirectory(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool ProcessDirFast(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Exists(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Flush(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool GetFileInfo(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool GetFreeSpace(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool GetPos(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool GetRootChars(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool IsAbsolutePath(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool IsReadOnly(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool MakeAbsolutePath(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool CreateDirectory(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Move(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Copy(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Open(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Read(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool ReadP(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool RemoveDirectory(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool SetCacheMode(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool SetEOF(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool SetAttributes(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool SetPos(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Delete(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Write(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool WriteP(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);
bool Shutdown(Blizzard::File::Filesystem* fs, Stacked::FileParms* parms);

} // namespace System_File

#endif
