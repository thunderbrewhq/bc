#ifndef BC_MEMORY_STORM_HPP
#define BC_MEMORY_STORM_HPP

#include <cstdint>
#include <cstdlib>
#include <new>
#include <type_traits>

#define ALLOC(n)         SMemAlloc(static_cast<size_t>(n), __FILE__, __LINE__, 0x0)
#define ALLOC_ZERO(n)    SMemAlloc(static_cast<size_t>(n), __FILE__, __LINE__, 0x8)
#define FREE(ptr)        SMemFree(ptr, __FILE__, __LINE__, 0x0)

#define NEW(T, ...)      (new (SMemAlloc(sizeof(T), __FILE__, __LINE__, 0)) T(__VA_ARGS__))
#define NEW_ZERO(T, ...) (new (SMemAlloc(sizeof(T), __FILE__, __LINE__, 0x8)) T(__VA_ARGS__))
#define DEL(ptr)                                                                               \
    do {                                                                                       \
        if (ptr) {                                                                             \
            using __storm_object = std::remove_pointer<std::decay<decltype(ptr)>::type>::type; \
            (ptr)->~__storm_object();                                                          \
            ::operator delete(ptr, __FILE__, __LINE__);                                        \
        }                                                                                      \
    } while (0)

// allocation functions

void* SMemAlloc(size_t bytes, const char* filename, int32_t linenumber, uint32_t flags);

void SMemFree(void* ptr);

void SMemFree(void* ptr, const char* filename, int32_t linenumber, uint32_t flags);

void* SMemReAlloc(void* ptr, size_t bytes, const char* filename, int32_t linenumber, uint32_t flags);

// global operators

void* operator new(size_t bytes);

void* operator new[](size_t bytes);

void operator delete(void* ptr);

inline void operator delete(void* ptr, const char* filename, int32_t linenumber) {
    if (ptr) {
        SMemFree(ptr, "delete", -1, 0);
    }
}

inline void operator delete[](void* ptr, const char* filename, int32_t linenumber) {
    if (ptr) {
        SMemFree(ptr, "delete[]", -1, 0);
    }
}

#endif
