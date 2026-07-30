#pragma once

#include <cstdint>

constexpr int REGISTER_COUNT = 20;

enum Register : uint8_t {
        R0 = 0,
        R1 = 1,
        R2 = 2,
        R3 = 3,
        R4 = 4,
        R5 = 5,
        R6 = 6,
        R7 = 7,
        R8 = 8,
        R9 = 9,
        R10 = 10,
        R11 = 11,
        R12 = 12,
        R13 = 13,
        R14 = 14,
        R15 = 15,

        SP = 16,
        RA = 17,
        FLAGS = 18,
        PC = 19
};

enum FlagStates: uint8_t {
    None = 0x00,
    Equal = 0x01,
    //Different = 0x02,
    Superior = 0x03,
    Inferior = 0x04
};

