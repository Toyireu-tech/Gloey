#pragma once

#include <cstdint>
#include "cpu_def.hpp"
#include "memory.hpp"
#include <stack>

class CPU {
private:
    Memory *mem;
    uint32_t regs[REGISTER_COUNT] = {0};
    bool running = false;
    std::stack<uint32_t> call_stack;
public:
    uint32_t get_reg(uint8_t index);
    void set_reg(uint8_t index, uint32_t value);

    uint32_t get_pc() {return regs[Register::PC];};
    void set_pc(uint32_t value) {regs[Register::PC] = value;};


    FlagStates get_flag() {return static_cast<FlagStates>(regs[Register::FLAGS]);};
    void set_flag(FlagStates value) {regs[Register::FLAGS] = value;};

    void add(uint8_t target, uint8_t r1, uint8_t r2);
    void sub(uint8_t target, uint8_t r1, uint8_t r2);
    void mult(uint8_t target, uint8_t r1, uint8_t r2);
    void div(uint8_t rQ, uint8_t rR, uint8_t r1, uint8_t r2);

    void _and(uint8_t target, uint8_t r1, uint8_t r2);
    void _or(uint8_t target, uint8_t r1, uint8_t r2);
    void _xor(uint8_t target, uint8_t r1, uint8_t r2);
    void _not(uint8_t target, uint8_t r1);

    void shl(uint8_t target, uint8_t r1, uint8_t r2);
    void shr(uint8_t target, uint8_t r1, uint8_t r2);

    void load(uint8_t target, uint32_t value);

    void memg(uint8_t target, uint8_t addr);
    void mems(uint8_t addr, uint8_t value);
    void memgw(uint8_t target, uint8_t addr);
    void memsw(uint8_t addr, uint8_t value);

    void comp(uint8_t r1, uint8_t r2);
    void jump(uint32_t addr);
    void jeq(uint32_t addr);
    void jneq(uint32_t addr);
    void jsup(uint32_t addr);
    void jinf(uint32_t addr);
    void jump_reg(uint8_t r1);
    void JeqReg(uint8_t r1);
    void JneqReg(uint8_t r1);
    void JsupReg(uint8_t r1);
    void JinfReg(uint8_t r1);

    void call(uint32_t addr);
    void call_reg(uint8_t addr);
    void ret();

    void exec(const uint8_t instr[8]);
    void run(uint32_t start_addr = 0);

    CPU(Memory *mem);
    ~CPU() = default;

};
