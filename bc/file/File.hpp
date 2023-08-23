#ifndef BC_FILE_FILE_HPP
#define BC_FILE_FILE_HPP

#include "bc/Time.hpp"
#include "bc/file/Defines.hpp"
#include "bc/file/Types.hpp"
#include "bc/system/file/System_File.hpp"

namespace Blizzard {
namespace File {

// Functions
void SetLastError(int32_t errorcode);

void AddToLastErrorStack(int32_t errorcode, const char* msg, int32_t param_3);

bool Copy(const char* source, const char* destination, bool overwrite);

bool CreateDirectory(const char* path, bool recursive);

bool Delete(const char* path);

bool Exists(const char* path);

bool Flush(StreamRecord* stream);

bool IsFile(const char* path);

bool IsDirectory(const char* path);

bool IsAbsolutePath(const char* path);

bool      GetFileInfo(const char* path, FileInfo* info);
bool      GetFileInfo(StreamRecord* stream, FileInfo* info);
FileInfo* GetFileInfo(StreamRecord* stream);

bool GetWorkingDirectory(char* path, size_t capacity);

bool SetWorkingDirectory(const char* path);

bool MakeAbsolutePath(const char* rel, char* result, int32_t capacity, bool unkflag);

bool Open(const char* filename, uint32_t flags, StreamRecord*& stream);

bool Close(StreamRecord* stream);

bool SetAttributes(const char* filename, uint32_t attributes);

bool Read(StreamRecord* stream, void* buffer, size_t bytesToRead, size_t* bytesRead);

bool Write(StreamRecord* stream, void* buffer, size_t bytesToWrite, size_t* bytesWritten);

bool SetPos(StreamRecord* stream, int64_t pos, int32_t whence);

bool GetPos(StreamRecord* stream, int64_t& pos);

} // namespace File
} // namespace Blizzard

#endif
