#pragma once

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include "debug.h"
#include <array>

class Memory {
    private:

    std::vector<uint8_t> data;
    uint32_t size;

    public:
    uint32_t get_size() {return size;}

    uint32_t add_instr(uint32_t addr, uint8_t opcode,
                        uint8_t op0 = 0, uint8_t op1 = 0, uint8_t op2 = 0,
                        uint8_t op3 = 0, uint8_t op4 = 0, uint8_t op5 = 0,
                        uint8_t op6 = 0) {
        set(addr,     opcode);
        set(addr + 1, op0);
        set(addr + 2, op1);
        set(addr + 3, op2);
        set(addr + 4, op3);
        set(addr + 5, op4);
        set(addr + 6, op5);
        set(addr + 7, op6);

        return addr + 8;
    }

    const uint8_t* get_slice8(uint32_t offset) const
    {
        DEBUG_CHECK(offset > size - 8, "Bad Address readed, ignored");
        return data.data() + offset;
    }

    const uint8_t* get_slice4(uint32_t offset) const
    {
        DEBUG_CHECK(offset > size - 4, "Bad Address readed, ignored");
        return data.data() + offset;
    }

    void set(uint32_t addr, uint8_t value) {
        DEBUG_CHECK(addr >= size, "Bad Address writted, ignored");
        data[addr] = value;
    }

    uint8_t get(uint32_t addr) {
        DEBUG_CHECK(addr >= size, "Bad Address readed, ignored");
        return data[addr];
    }
    
    Memory(uint32_t byte_count) {
        data.resize(byte_count);
        size = byte_count;
    }

    ~Memory() = default;  
};
