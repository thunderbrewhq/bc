#include "bc/Memory.hpp"
#include "test/Test.hpp"

#include <cstdio>

struct TestObject {
    int m = 1;
    TestObject() {
        // printf("TestObject created\n");
    }

    ~TestObject() {
        // printf("TestObject destroyed\n");
    }
};

TEST_CASE("Blizzard::Memory::Allocate", "[memory]") {
    SECTION("allocates memory and returns pointer") {
        auto ptr = Blizzard::Memory::Allocate(100);

        REQUIRE(ptr);

        Blizzard::Memory::Free(ptr);
    }

    SECTION("allocates memory and returns pointer using overload with flags") {
        auto ptr = Blizzard::Memory::Allocate(100, 0x0, __FILE__, __LINE__, nullptr);

        REQUIRE(ptr);

        Blizzard::Memory::Free(ptr);
    }
}

TEST_CASE("helper macros", "[memory]") {
    SECTION("ALLOCATE memory and return pointer") {
        auto m = ALLOC(128);
        REQUIRE(m);
        FREE(m);
    }

    SECTION("NEW object and DELETE it") {
        auto m = NEW(TestObject);
        DEL(m);
    }
}

TEST_CASE("operator new", "[memory]") {
    SECTION("allocate memory through the new operator") {
        auto m = new uint8_t[0xCAFE];
        delete m;
    }
}
