#pragma once

#include <cstdint>
#include <iostream>
#include <vector>
#include "debug.h"

class Memory {
    private:

    std::vector<uint8_t> data;
    uint32_t size;

    public:

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
