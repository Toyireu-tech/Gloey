#include "cpu.hpp"
#include "display.hpp"
#include "memory.hpp"
#include <fstream>
#include <ios>
#include <iostream>
#include <chrono>
#include <stdexcept>
#include <thread>

int main(int argc, const char** argv) {
    if (argc < 2) throw std::runtime_error("Invalid command");
    Memory mem(1024);
    Display screen(800, 600);
    CPU cpu(&mem, &screen);

    std::ifstream input_file(argv[1], std::ios::binary | std::ios::ate);
    if (!input_file.is_open()) throw std::runtime_error("Couldnt open file ");

    std::streamsize size = input_file.tellg();
    input_file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    input_file.read(buffer.data(), size);

    int p = 0;
    for (auto i : buffer) {
        //std::cout << std::to_string(i);
        mem.set(p, i);
        p++;
    }

    
    std::thread cpuThread([&] () {
        auto start = std::chrono::high_resolution_clock::now();
        cpu.run();
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "Exécution: " << duration.count() << " ms\n";
        std::cout << "\n" << cpu.get_reg(0);
    }); 

    while (screen.update()) {}
    cpu.running = false;
    cpuThread.join();    
    
    return 0;
}
