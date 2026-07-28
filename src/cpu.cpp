#include "cpu.hpp"
#include "cpu_def.hpp"
#include "memory.hpp"
#include <cstdint>
#include "debug.h"


uint32_t CPU::get_reg(uint8_t index) {
    DEBUG_CHECK(index >= REGISTER_COUNT, "Bad register get");
    return regs[index];
}

void CPU::set_reg(uint8_t index, uint32_t value) {
    DEBUG_CHECK(index >= REGISTER_COUNT, "Bad register set");
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

uint32_t CPU::memg(uint8_t addr) {
    const uint32_t addr_val = get_reg(addr);
    return mem->get(addr_val);
}

void CPU::mems(uint8_t addr, uint8_t value) {
    const uint32_t addr_val = get_reg(addr);
    const uint32_t val = get_reg(value);
    mem->set(addr_val, val);
}


CPU::CPU(Memory *mem): mem(mem) {

}