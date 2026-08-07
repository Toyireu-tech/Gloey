#pragma once

#include <cstdint>
#include <vector>
#include "debug.h"

class Memory {
    private:

    
    uint32_t size;

    public:
    std::vector<uint8_t> data; 
    uint32_t get_size() {return size;}
    auto get_data() {return &data;}

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
        DEBUG_CHECK(offset > size - 8, "Bad Address readed (slice): " + std::to_string(offset));
        return data.data() + offset;
    }

    const uint8_t* get_slice4(uint32_t offset) const
    {
        DEBUG_CHECK(offset > size - 4, "Bad Address readed (slice): " + std::to_string(offset));
        return data.data() + offset;
    }

    void set(uint32_t addr, uint8_t value) {
        DEBUG_CHECK(addr >= size, "Bad Address writted: " + std::to_string(addr));
        data[addr] = value;
    }

    uint8_t get(uint32_t addr) {
        DEBUG_CHECK(addr >= size, "Bad Address readed: " + std::to_string(addr));
        return data[addr];
    }
    
    Memory(uint32_t byte_count) {
        data.resize(byte_count);
        size = byte_count;
    }

    ~Memory() = default;  
};
