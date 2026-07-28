#pragma once

#include <cstdint>

enum class OpCode: std::uint8_t {
    Nop = 0x00,
    Add = 0x01,
    Sub = 0x02,
    Mul = 0x03,
    Div = 0x04,
    Shl = 0x05,
    Shr = 0x06,
    Load = 0x07,
    Memg = 0x08,
    Mems = 0x09,
    //Add = 0x0A,
    Halt = 0xFF,
};