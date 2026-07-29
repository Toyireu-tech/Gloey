#include "cpu.hpp"
#include "memory.hpp"
#include "opcode.hpp"
#include <cstdint>
#include <iostream>

int main() {
    // test
    Memory mem(1024);
    CPU cpu(&mem);

    uint8_t instr_test[8] = {OpCode::Add, 0, 0, 1};
    cpu.load(0, 30);
    cpu.load(1, 12);
    
    cpu.exec(instr_test);
    std::cout << cpu.get_reg(0);
    return 0;
}
