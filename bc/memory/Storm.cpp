#include "bc/memory/Storm.hpp"

constexpr size_t ALIGNMENT = 8;

void* operator new(size_t bytes) {
    return SMemAlloc(bytes, "new", -1, 0x0);
}

void* operator new[](size_t bytes) {
    return SMemAlloc(bytes, "new", -1, 0x0);
}

void operator delete(void* ptr) {
    if (ptr) {
        SMemFree(ptr, "delete", -1, 0x0);
    }
}

void* SMemAlloc(size_t bytes, const char* filename, int32_t linenumber, uint32_t flags) {
    size_t alignedBytes = (bytes + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

    void* result;

    if (flags & 0x8) {
        result = calloc(1, alignedBytes);
    } else {
        result = malloc(alignedBytes);
    }

    if (result) {
        return result;
    } else {
        // TODO handle errors
        return nullptr;
    }
}

void SMemFree(void* ptr) {
    if (ptr) {
        free(ptr);
    }
}

void SMemFree(void* ptr, const char* filename, int32_t linenumber, uint32_t flags) {
    if (ptr) {
        free(ptr);
    }
}

void* SMemReAlloc(void* ptr, size_t bytes, const char* filename, int32_t linenumber, uint32_t flags) {
    if (flags == 0xB00BEEE5) {
        return nullptr;
    }

    if (!ptr) {
        return SMemAlloc(bytes, filename, linenumber, flags);
    }

    if (flags & 0x10) {
        return nullptr;
    }

    size_t alignedBytes = (bytes + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);

    void* result = realloc(ptr, alignedBytes);

    if (result) {
        if (flags & 0x8) {
            // TODO zero out expanded portion
        }

        return result;
    } else {
        if (alignedBytes) {
            // TODO handle errors
        }

        return nullptr;
    }
}
