#pragma once

#include <cstdint>

inline uint32_t little_endian_to_u32(const uint8_t bytes[4])
{
    return  (uint32_t(bytes[0])      ) |
            (uint32_t(bytes[1]) <<  8) |
            (uint32_t(bytes[2]) << 16) |
            (uint32_t(bytes[3]) << 24);
}