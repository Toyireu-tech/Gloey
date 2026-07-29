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

// [opcode] [arg 1] [arg 2] [arg 3] [imediate]
void CPU::exec(const uint8_t instr[8]) {

    const uint8_t opcode = instr[0];
    const uint8_t arg1 = instr[1];
    const uint8_t arg2 = instr[2];
    const uint8_t arg3 = instr[3];
    const uint8_t arg4 = instr[4]; // only used for 4 arguments instructiions like div

    uint8_t imm[4] = {0};
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
            std::memcpy(imm, instr + 4, 4);
            load(arg1, little_endian_to_u32(imm));
            break;
        case OpCode::Memg:
            memg(arg1, arg2);
            break;
        case OpCode::Mems:
            mems(arg1, arg2);
            break;
        default:
            break;
    }
    return;
}




CPU::CPU(Memory *mem): mem(mem) {

}
