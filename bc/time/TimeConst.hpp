#ifndef BC_TIME_TIME_CONST_HPP
#define BC_TIME_TIME_CONST_HPP

#include <cstdint>

namespace TimeConst {

// The number of nanoseconds in a second
constexpr int64_t TimestampsPerSecond = 1000000000ULL;

// amount of win32 filetime units in a second
constexpr int64_t FileTimeUnitsPerSecond = (TimestampsPerSecond / 100LL);

// the FILETIME value needed to move from 1601 epoch to the Year 2000 epoch that Blizzard prefers
constexpr uint64_t FileTimeDelta = 125911584000000000ULL;

} // namespace TimeConst

#endif
