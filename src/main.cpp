#include "cpu.hpp"
#include "memory.hpp"
#include <iostream>
#include "assembler.hpp"
#include <chrono>

using namespace Assembler;


uint64_t fib(uint64_t n) {
    if (n < 2)
        return n;

    return fib(n - 1) + fib(n - 2);
}

int main() {
    // test
    Memory mem(1024);
    CPU cpu(&mem);

    std::string program = 
        R"(

jump _start

; r6 a, r7 b, r8 tmp, r1 lim, r10 incrr base, r11 a+b
.section fib
    load r6 0
    load r7 1

    .label fib_loop

        add r11 r6 r7
        copy r6 r7
        copy r7 r11

        addi r10 r10 1
        comp r10 r1
        jneq fib_loop
    copy r0 r6
    ret

.label _start
    load r1 1
    call fib
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
    std::cout << "Exécution: " << duration.count() << " ms\n";
    std::cout << "\n" << cpu.get_reg(0);
    return 0;
}
