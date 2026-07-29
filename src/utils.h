#pragma once

#include <cstdint>

inline uint32_t little_endian_to_u32(const uint8_t bytes[4])
{
    return  (uint32_t(bytes[0])      ) |
            (uint32_t(bytes[1]) <<  8) |
            (uint32_t(bytes[2]) << 16) |
            (uint32_t(bytes[3]) << 24);
}

inline void u32_to_little_endian(uint32_t value, uint8_t bytes[4])
{
    bytes[0] = static_cast<uint8_t>(value);
    bytes[1] = static_cast<uint8_t>(value >> 8);
    bytes[2] = static_cast<uint8_t>(value >> 16);
    bytes[3] = static_cast<uint8_t>(value >> 24);
}