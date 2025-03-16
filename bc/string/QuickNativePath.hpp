#ifndef BC_STRING_QUICK_NATIVE_PATH_HPP
#define BC_STRING_QUICK_NATIVE_PATH_HPP

#include "bc/String.hpp"
#include "bc/Memory.hpp"
#include <cstdint>

namespace Blizzard {
namespace String {

template <uint32_t N>
class QuickNativePath {
    private:
        // string length of path, including NULL character
        uint32_t length;
        char* path;
        char buffer[N];

    public:
        QuickNativePath(const char* name) {
            this->length = String::Length(name) + 1;

            if (this->length > N) {
                this->path = reinterpret_cast<char*>(Memory::Allocate(this->length));
                MakeNativePath(name, this->path, this->length);
            } else {
                this->path = this->buffer;
                MakeNativePath(name, this->path, N);
            }
        }

        ~QuickNativePath() {
            if (this->path != this->buffer) {
                Memory::Free(this->path);
            }
        }

        const char* ToString() {
            return this->path;
        }
};

} // namespace String
} // namespace Blizzard

#endif
