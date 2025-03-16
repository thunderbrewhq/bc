#include "test/Test.hpp"
#include "bc/File.hpp"
#include "bc/String.hpp"
#include "bc/file/Types.hpp"
#if defined(WHOA_SYSTEM_LINUX) || defined(WHOA_SYSTEM_MAC)

TEST_CASE("Blizzard::File::Open", "[file]") {
    SECTION("open /dev/zero") {
        Blizzard::File::StreamRecord* file;
        REQUIRE(Blizzard::File::Open("/dev/zero", Blizzard::File::Mode::mustexist|Blizzard::File::Mode::read, file));
        REQUIRE(Blizzard::File::Close(file));
    }

    SECTION("read data from /dev/zero") {
        Blizzard::File::StreamRecord* file;
        REQUIRE(Blizzard::File::Open("/dev/zero", Blizzard::File::Mode::mustexist|Blizzard::File::Mode::read, file));

        char zero[1024];
        char max[1024];
        Blizzard::String::MemFill(zero, sizeof(zero), 0);
        Blizzard::String::MemFill(max, sizeof(max), 255);
        int32_t count = sizeof(zero);
        REQUIRE(Blizzard::File::Read(file, max, &count));
        REQUIRE(Blizzard::File::Close(file));
        REQUIRE(count == sizeof(zero));
        REQUIRE(Blizzard::String::MemCompare(zero, max, sizeof(zero)) == 0);
    }
}

#endif