#include "cpu.hpp"
#include "cpu_def.hpp"
#include "memory.hpp"
#include <cstdint>
#include <cstring>
#include <string>

#include "debug.h"
#include "opcode.hpp"
#include "utils.h"

uint32_t CPU::get_reg(uint8_t index) {
    DEBUG_CHECK(index >= REGISTER_COUNT, "Bad register get");
    return regs[index];
}

void CPU::set_reg(uint8_t index, uint32_t value) {
    DEBUG_CHECK(index >= REGISTER_COUNT, "Bad register \"" + std::to_string(index) + "\"");
    regs[index] = value;
}

void CPU::add(uint8_t target, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(target, a + b);
}

void CPU::sub(uint8_t target, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(target, a - b);
}

void CPU::mult(uint8_t target, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(target, a * b);
}

void CPU::div(uint8_t rQ, uint8_t rR, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(rQ, a / b);
    set_reg(rR, a % b);
}

void CPU::_and(uint8_t target, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(target, a & b);
}

void CPU::_or(uint8_t target, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(target, a | b);
}

void CPU::_xor(uint8_t target, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(target, a ^ b);
}

void CPU::_not(uint8_t target, uint8_t r1) {
    const uint32_t a = get_reg(r1);
    set_reg(target, ~a);
}

void CPU::shl(uint8_t target, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(target, a << b);
}

void CPU::shr(uint8_t target, uint8_t r1, uint8_t r2) {
    const uint32_t a = get_reg(r1);
    const uint32_t b = get_reg(r2);
    set_reg(target, a >> b);
}

void CPU::load(uint8_t target, uint32_t value) {
    set_reg(target, value);
}

void CPU::memg(uint8_t target, uint8_t addr) {
    const uint32_t addr_val = get_reg(addr);
    set_reg(target, mem->get(addr_val));
}

void CPU::mems(uint8_t addr, uint8_t value) {
    const uint32_t addr_val = get_reg(addr);
    const uint32_t val = get_reg(value);
    mem->set(addr_val, val);
}


void CPU::comp(uint8_t r1, uint8_t r2) {
    const uint32_t r1_val = get_reg(r1);
    const uint32_t r2_val = get_reg(r2);

    if(r1_val == r2_val) set_flag(FlagStates::Equal); return;
    if(r1_val != r2_val) set_flag(FlagStates::Different); return;
    if(r1_val > r2_val) set_flag(FlagStates::Superior); return;
    if(r1_val < r2_val) set_flag(FlagStates::Inferior); return;
}

void CPU::jump(uint32_t addr) {
    set_pc(addr);
}

void CPU::jeq(uint32_t addr)  {
    if (get_flag() == FlagStates::Equal) set_pc(addr);
}

void CPU::jneq(uint32_t addr) {
    if (get_flag() == FlagStates::Different) set_pc(addr);
}

void CPU::jsup(uint32_t addr) {
    if (get_flag() == FlagStates::Superior) set_pc(addr);
}

void CPU::jinf(uint32_t addr) {
    if (get_flag() == FlagStates::Inferior) set_pc(addr);
}

void CPU::jump_reg(uint8_t r1) {
    set_pc(get_reg(r1));
}

void CPU::jeq_reg(uint8_t r1) {
    if (get_flag() == FlagStates::Equal) set_pc(get_reg(r1));
}

void CPU::jneq_reg(uint8_t r1) {
    if (get_flag() == FlagStates::Different) set_pc(get_reg(r1));
}


void CPU::jsup_reg(uint8_t r1) {
    if (get_flag() == FlagStates::Superior) set_pc(get_reg(r1));
}


void CPU::jinf_reg(uint8_t r1) {
    if (get_flag() == FlagStates::Inferior) set_pc(get_reg(r1));
}


// [opcode] [arg 1] [arg 2] [arg 3] [imediate]
void CPU::exec(const uint8_t instr[8]) {

    const uint8_t opcode = instr[0];
    const uint8_t arg1 = instr[1];
    const uint8_t arg2 = instr[2];
    const uint8_t arg3 = instr[3];
    const uint8_t arg4 = instr[4]; // only used for 4 arguments instructiions like div

    uint8_t imm[4] = {0};
    std::memcpy(imm, instr + 4, 4);

    uint32_t imm_value = little_endian_to_u32(imm);

    switch (opcode) {
        case OpCode::Nop:
            break;
        case OpCode::Add:
            add(arg1, arg2, arg3);
            break;
        case OpCode::Sub:
            sub(arg1, arg2, arg3);
            break;
        case OpCode::Mul:
            mult(arg1, arg2, arg3);
            break;
        case OpCode::Div:
            div(arg1, arg2, arg3, arg4);
            break;
        case OpCode::Shl:
            shl(arg1, arg2, arg3);
            break;
        case OpCode::Shr:
            shr(arg1, arg2, arg3);
            break;
        case OpCode::Load:
            load(arg1, imm_value);
            break;
        case OpCode::Memg:
            memg(arg1, arg2);
            break;
        case OpCode::Mems:
            mems(arg1, arg2);
            break;
        case OpCode::Comp:
            comp(arg1, arg2);
            break;
        case OpCode::Jump:
            jump(imm_value);
            break;
        case OpCode::Jeq:
            jeq(imm_value);
            break;
        case OpCode::Jneq:
            jneq(imm_value);
            break;
        case OpCode::Jsup:
            jsup(imm_value);
            break;
        case OpCode::Jinf:
            jinf(imm_value);
            break;
        case OpCode::Jreg:
            jump_reg(arg1);
            break;
        case OpCode::Jeq_reg:
            jeq_reg(arg1);
            break;
        case OpCode::Jneq_reg:
            jneq_reg(arg1);
            break;
        case OpCode::Jsup_reg:
            jsup_reg(arg1);
            break;
        case OpCode::Jinf_reg:
            jinf_reg(arg1);
            break;
        case OpCode::Halt:
            running = false;
            break;
        default:
            break;
    }
    return;
}




CPU::CPU(Memory *mem): mem(mem) {
    regs[Register::FLAGS] = FlagStates::None;
}
