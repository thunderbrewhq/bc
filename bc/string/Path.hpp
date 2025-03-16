#ifndef BC_STRING_PATH_HPP
#define BC_STRING_PATH_HPP

#include <cstdint>

namespace Blizzard {
namespace String {

const char* FindFilename(const char* str);

char* FindFilename(char* str);

char FirstPathSeparator(const char* str);

void ForceTrailingSeparator(char* buf, int32_t bufMax, char sep);

void JoinPath(char* dst, int32_t count, const char* str1, const char* str2);

char* MakeBackslashPath(const char* src, char* buffer, int32_t buffersize);

char* MakeConsistentPath(const char* src, char* buffer, int32_t buffersize);

char* MakeNativePath(const char* src, char* buffer, int32_t buffersize);

char* MakeUnivPath(const char* src, char* buffer, int32_t buffersize);

} // namespace String
} // namespace Blizzard

#endif
