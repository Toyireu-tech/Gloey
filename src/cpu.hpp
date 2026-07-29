#pragma once

#include <cstdint>
#include "cpu_def.hpp"
#include "memory.hpp"


class CPU {
private:
    Memory *mem;
    uint32_t regs[REGISTER_COUNT] = {0};
public:
    uint32_t get_reg(uint8_t index);
    void set_reg(uint8_t index, uint32_t value);
    uint32_t get_pc() {return regs[Register::PC];};
    void set_pc(uint32_t value) {regs[Register::PC] = value;};

    void add(uint8_t target, uint8_t r1, uint8_t r2);
    void sub(uint8_t target, uint8_t r1, uint8_t r2);
    void mult(uint8_t target, uint8_t r1, uint8_t r2);
    void div(uint8_t rQ, uint8_t rR, uint8_t r1, uint8_t r2);
    void shl(uint8_t target, uint8_t r1, uint8_t r2);
    void shr(uint8_t target, uint8_t r1, uint8_t r2);

    void load(uint8_t target, uint32_t value);
    void memg(uint8_t target, uint8_t addr);
    void mems(uint8_t addr, uint8_t value);

    void exec(const uint8_t instr[8]);

    CPU(Memory *mem);
    ~CPU() = default;

};
