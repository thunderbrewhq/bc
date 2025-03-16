#include "bc/Unicode.hpp"
#include "test/Test.hpp"

TEST_CASE("Blizzard::Unicode::ConvertUTF16to8", "[unicode]") {
    SECTION("convert UTF-16 汉字 to UTF-8") {
        uint16_t widechars[] = { 0x6C49, 0x5B57 };
        uint8_t chars[] = { 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
        uint32_t srcchars;
        uint32_t dstchars;
        auto result = Blizzard::Unicode::ConvertUTF16to8(chars, 6, widechars, 2, &dstchars, &srcchars);
        REQUIRE(srcchars == 2);
        REQUIRE(dstchars == 6);
        REQUIRE(result == -1);

        REQUIRE(chars[0] == 0xE6);
        REQUIRE(chars[1] == 0xB1);
        REQUIRE(chars[2] == 0x89);
        REQUIRE(chars[3] == 0xE5);
        REQUIRE(chars[4] == 0xAD);
        REQUIRE(chars[5] == 0x97);
    }
}

TEST_CASE("Blizzard::Unicode::ConvertUTF8to16", "[unicode]") {
    SECTION("convert UTF-8 汉字 to UTF-16") {
        uint16_t widechars[] = { 0x1111, 0x2222, 0x3333 };
        uint8_t chars[] = { 0xE6, 0xB1, 0x89, 0xE5, 0xAD, 0x97, 0x00 };
        uint32_t srcchars;
        uint32_t dstchars;
        auto result = Blizzard::Unicode::ConvertUTF8to16(widechars, 3, chars, 7, &dstchars, &srcchars);
        REQUIRE(result == 0);
        REQUIRE(dstchars == 3);
        REQUIRE(srcchars == 6);

        REQUIRE(widechars[0] == 0x6C49);
        REQUIRE(widechars[1] == 0x5B57);
        REQUIRE(widechars[2] == 0x0000);
    }
}
