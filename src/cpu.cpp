#include "cpu.hpp"
#include "cpu_def.hpp"
#include "display.hpp"
#include "memory.hpp"
#include <cstdint>
#include <span>

#include "debug.h"
#include "opcode.hpp"
#include "utils.hpp"

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

void CPU::memgw(uint8_t target, uint8_t addr) {
    const uint32_t addr_val = get_reg(addr);

    const uint8_t* val = mem->get_slice4(addr_val);
    set_reg(target, little_endian_to_u32(val));
}

void CPU::memsw(uint8_t addr, uint8_t value) {
    const uint32_t addr_val = get_reg(addr);
    uint8_t val[4];
    u32_to_little_endian(get_reg(value), val);
    mem->set(addr_val, val[0]);
    mem->set(addr_val + 1, val[1]);
    mem->set(addr_val + 2, val[2]);
    mem->set(addr_val + 3, val[3]);
}


void CPU::comp(uint8_t r1, uint8_t r2) {
    const uint32_t r1_val = get_reg(r1);
    const uint32_t r2_val = get_reg(r2);


    if (r1_val == r2_val) set_flag(FlagStates::Equal);
    else if (r1_val > r2_val) set_flag(FlagStates::Superior);
    else set_flag(FlagStates::Inferior);
}

void CPU::jump(uint32_t addr) {
    set_pc(addr - 8);
}

void CPU::jeq(uint32_t addr)  {
    if (get_flag() == FlagStates::Equal) set_pc(addr - 8);
}

void CPU::jneq(uint32_t addr) {
    if (get_flag() != FlagStates::Equal) set_pc(addr - 8);
}

void CPU::jsup(uint32_t addr) {
    if (get_flag() == FlagStates::Superior) set_pc(addr - 8);
}

void CPU::jinf(uint32_t addr) {
    if (get_flag() == FlagStates::Inferior) set_pc(addr - 8);
}

void CPU::jump_reg(uint8_t r1) {
    set_pc(get_reg(r1) - 8);
}

void CPU::JeqReg(uint8_t r1) {
    if (get_flag() == FlagStates::Equal) set_pc(get_reg(r1) - 8);
}

void CPU::JneqReg(uint8_t r1) {
    if (get_flag() != FlagStates::Equal) set_pc(get_reg(r1) - 8);
}


void CPU::JsupReg(uint8_t r1) {
    if (get_flag() == FlagStates::Superior) set_pc(get_reg(r1) - 8);
}


void CPU::JinfReg(uint8_t r1) {
    if (get_flag() == FlagStates::Inferior) set_pc(get_reg(r1) - 8);
}

void CPU::copy(uint8_t target, uint8_t src) {
    set_reg(target, get_reg(src));
}

void CPU::call(uint32_t addr) {
    call_stack.push(regs[Register::PC] + 8);
    set_pc(addr - 8);
}

void CPU::call_reg(uint8_t addr) {
    uint32_t addr_val = get_reg(addr);
    call_stack.push(regs[Register::PC] + 8);
    set_pc(addr_val - 8);
}

void CPU::ret() {
    DEBUG_CHECK(call_stack.empty(), "Call stack underflow");
    set_pc(call_stack.top() - 8);
    call_stack.pop();
}
   



void CPU::addi(uint8_t target, uint8_t r1, uint32_t imm_v) {
    const uint32_t a = get_reg(r1);
    set_reg(target, a + imm_v);
}

void CPU::subi(uint8_t target, uint8_t r1, uint32_t imm_v) {
    const uint32_t a = get_reg(r1);
    set_reg(target, a - imm_v);
}

void CPU::multi(uint8_t target, uint8_t r1, uint32_t imm_v) {
    const uint32_t a = get_reg(r1);
    set_reg(target, a * imm_v);
}

void CPU::_andi(uint8_t target, uint8_t r1, uint32_t imm_v) {
    const uint32_t a = get_reg(r1);
    set_reg(target, a & imm_v);
}

void CPU::_ori(uint8_t target, uint8_t r1, uint32_t imm_v) {
    const uint32_t a = get_reg(r1);
    set_reg(target, a | imm_v);
}

void CPU::_xori(uint8_t target, uint8_t r1, uint32_t imm_v) {
    const uint32_t a = get_reg(r1);
    set_reg(target, a ^ imm_v);
}

void CPU::compi(uint8_t r1, uint32_t imm_v) {
    const uint32_t r1_val = get_reg(r1);

    if (r1_val == imm_v) set_flag(FlagStates::Equal);
    else if (r1_val > imm_v) set_flag(FlagStates::Superior);
    else set_flag(FlagStates::Inferior);
}

void CPU::shli(uint8_t target, uint8_t r1, uint32_t imm_v) {
    const uint32_t a = get_reg(r1);
    set_reg(target, a << imm_v);
}

void CPU::shri(uint8_t target, uint8_t r1, uint32_t imm_v) {
    const uint32_t a = get_reg(r1);
    set_reg(target, a >> imm_v);
}

void CPU::setpix(uint8_t rX, uint8_t rY, uint8_t rColor) {
    //println("const T &value");
    display->setPixel(get_reg(rX), get_reg(rY), get_reg(rColor));
}

void CPU::push() {
    std::array<uint32_t, 15> state;

    std::copy(
        regs + 1,
        regs + 16,
        state.begin()
    );

    data_stack.push_back(state);
}

void CPU::pop() {
    DEBUG_CHECK(data_stack.empty(), "Can't pop an empty data stack");

    auto& state = data_stack.back();

    std::copy(
        state.begin(),
        state.end(),
        regs + 1
    );

    data_stack.pop_back();
}


// [opcode] [arg 1] [arg 2] [arg 3] [imediate]
void CPU::exec(const uint8_t instr[8]) {

    const uint8_t opcode = instr[0];
    const uint8_t arg1 = instr[1];
    const uint8_t arg2 = instr[2];
    const uint8_t arg3 = instr[3];
    const uint8_t arg4 = instr[4]; // only used for 4 arguments instructiions like div

    const uint8_t *imm = instr + 4;

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
        case OpCode::Memgw:
            memgw(arg1, arg2);
            break;
        case OpCode::Memsw:
            memsw(arg1, arg2);
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
        case OpCode::JeqReg:
            JeqReg(arg1);
            break;
        case OpCode::JneqReg:
            JneqReg(arg1);
            break;
        case OpCode::JsupReg:
            JsupReg(arg1);
            break;
        case OpCode::JinfReg:
            JinfReg(arg1);
            break;
        case OpCode::Copy:
            copy(arg1, arg2);
            break;
        case OpCode::Call:
            call(imm_value);
            break;
        case OpCode::CallReg:
            call_reg(arg1);
            break;
        case OpCode::Ret:
            ret();
            break;
        case OpCode::Addi:
            addi(arg1, arg2, imm_value);
            break;
        case OpCode::Subi:
            subi(arg1, arg2, imm_value);
            break;
        case OpCode::Muli:
            multi(arg1, arg2, imm_value);
            break;
        case OpCode::Andi:
            _andi(arg1, arg2, imm_value);
            break;
        case OpCode::Ori:
            _ori(arg1, arg2, imm_value);
            break;
        case OpCode::Xori:
            _xori(arg1, arg2, imm_value);
            break;
        case OpCode::Compi:
            compi(arg1, imm_value);
            break;
        case OpCode::Shli:
            addi(arg1, arg2, imm_value);
            break;
        case OpCode::Shri:
            addi(arg1, arg2, imm_value);
            break;
        case OpCode::Setpix:
            setpix(arg1, arg2, arg3);
            break;
        case OpCode::Push:
            push();
            break;
        case OpCode::Pop:
            pop();
            break;
        case OpCode::Halt:
            running = false;
            break;
        default:
            break;
    }
    return;
}

void CPU::run(uint32_t start_addr) {
    running = true;
    set_pc(start_addr);

    while (running && get_pc() < mem->get_size()) {
        const uint8_t* code = mem->get_slice8(get_pc());
        exec(code);

        regs[Register::PC] += 8;
    }
}


CPU::CPU(Memory *mem, Display *dp): mem(mem), display(dp) {
    regs[Register::FLAGS] = FlagStates::None;
}
