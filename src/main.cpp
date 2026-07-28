#include "cpu.hpp"
#include "memory.hpp"
#include <iostream>

int main() {
    // test
    Memory mem(1024);
    CPU cpu(&mem);
    cpu.load(0, 30);
    cpu.load(1, 12);
    cpu.add(0, 0, 1);
    std::cout << cpu.get_reg(0);
    return 0;
}