#pragma once

#include <cstdint>
#include <iostream>
#include <vector>

class Memory {
    private:

    std::vector<uint8_t> data;
    uint32_t size;

    public:

    void set(uint32_t addr, uint8_t value) {
        if (addr >= size || addr < 0) std::cerr << "Bad Address writted, ignored" << "\n"; return;
        data[addr] = value;
    }

    uint8_t get(uint32_t addr) {
        if (addr >= size || addr < 0) std::cerr << "Bad Address readed, ignored" << "\n"; return 0;
        return data[addr];
    }
    
    Memory(uint32_t byte_count) {
        data.resize(byte_count);
        size = byte_count;
    }

    ~Memory() = default;  
};