#pragma once

#include <cstdint>
#include "cpu_def.hpp"
#include "memory.hpp"
#include <stack>
#include <sys/types.h>
#include "display.hpp"
#include <chrono>

inline uint32_t getTicksMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

inline uint32_t g_last_ascii_code = 0;


// Callback appelée à chaque saisie de caractère
inline void char_input_callback(struct mfb_window *window, unsigned int code_point) {
    g_last_ascii_code = code_point;
}

class CPU {
private:
    Memory *mem;
    uint32_t regs[REGISTER_COUNT] = {0};
    Display *display;
    
    std::stack<uint32_t> call_stack;
    std::vector<std::array<uint32_t, 15>> data_stack;
    uint32_t io_ports[64] = {0}; // 0: keyboard   1: clock
    uint32_t clock_begin = getTicksMs();
    
public:
    bool running = false;
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

    void memgi(uint8_t target, uint8_t addr);
    void memsi(uint8_t addr, uint8_t value);
    void memgwi(uint8_t target, uint8_t addr);
    void memswi(uint8_t addr, uint8_t value);

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

    void copy(uint8_t target, uint8_t src);

    void call(uint32_t addr);
    void call_reg(uint8_t addr);
    void ret();

    void addi(uint8_t target, uint8_t r1, uint32_t imm_v);
    void subi(uint8_t target, uint8_t r1, uint32_t imm_v);
    void multi(uint8_t target, uint8_t r1, uint32_t imm_v);

    void _andi(uint8_t target, uint8_t r1, uint32_t imm_v);
    void _ori(uint8_t target, uint8_t r1, uint32_t imm_v);
    void _xori(uint8_t target, uint8_t r1, uint32_t imm_v);
    void compi(uint8_t r1, uint32_t imm_v);

    void shli(uint8_t target, uint8_t r1, uint32_t imm_v);
    void shri(uint8_t target, uint8_t r1, uint32_t imm_v);

    void setpix(uint8_t rX, uint8_t rY, uint8_t rColor);

    void push();
    void pop();

    void in(uint8_t r1,uint32_t port);
    void out(uint8_t target, uint32_t port);

    void exec(const uint8_t instr[8]);
    void run(uint32_t start_addr = 0);

    CPU(Memory *mem, Display *dp = nullptr);
    ~CPU() = default;

};
