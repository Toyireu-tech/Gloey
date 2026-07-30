#include "cpu.hpp"
#include "memory.hpp"
#include <iostream>
#include "assembler.hpp"
#include <chrono>

using namespace Assembler;

int main() {
    // test
    Memory mem(1024);
    CPU cpu(&mem);

    std::string program = 
        R"(
    load r1 0          ; counter
        load r2 100000000    ; limite
        load r3 1          ; increment

    .label loop
        add r1 r1 r3
        comp r1 r2
        jneq loop
        halt
        )";

    int p = 0;
    for (auto i : assemble(program)) {
       // std::cout << std::to_string(i) << std::endl;
        mem.set(p, i);
        p++;
    }
    
    auto start = std::chrono::high_resolution_clock::now();
    cpu.run();
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Exécution: " << duration.count() << " sec\n";
    std::cout << "\n" << cpu.get_reg(1);
    return 0;
}
