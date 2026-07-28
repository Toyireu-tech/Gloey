#pragma once

#include <cstdint>
#include "cpu_def.hpp"
#include "memory.hpp"


class CPU {
private:
    Memory *mem;
    uint32_t regs[REGISTER_COUNT];
public:
    CPU();
    ~CPU();

};