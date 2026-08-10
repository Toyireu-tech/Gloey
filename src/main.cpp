#include "cpu.hpp"
#include "display.hpp"
#include "memory.hpp"
#include "utils.hpp"
#include <cstdint>
#include <fstream>
#include <ios>
#include <iostream>
#include <chrono>
#include <stdexcept>
#include <thread>
#include <zlib.h>

int main(int argc, const char** argv) {
    if (argc < 2) throw std::runtime_error("Invalid command");
    Memory mem(1024 * 1024);
    Display screen(800, 600);
    CPU cpu(&mem, &screen);

    // std::ifstream input_file(argv[1], std::ios::binary | std::ios::ate);
    // if (!input_file.is_open()) throw std::runtime_error("Couldnt open file ");

    // std::streamsize size = input_file.tellg();
    // input_file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer;
    //input_file.read(buffer.data(), size);
    decompressFromGz(argv[1], buffer);

    int p = 0;
    for (auto i : buffer) {
        //std::cout << std::to_string(i);
        mem.set(p, i);
        p++;
    }
    
    std::thread cpuThread([&] () -> void {
        auto start = std::chrono::high_resolution_clock::now();
        cpu.run();
        auto end = std::chrono::high_resolution_clock::now();

        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << "Exécution: " << duration.count() << " ns\n";
        std::cout << "\n" << cpu.get_reg(0);
    }); 

    // double fps = 0.0;
    // auto last_frame_time = std::chrono::high_resolution_clock::now();
    while (screen.update()) {
        // auto now = std::chrono::high_resolution_clock::now();
        // double delta = std::chrono::duration<double>(now - last_frame_time).count();
        // last_frame_time = now;

        // fps = 1.0 / delta;
        // std::cout << fps << std::endl;
    }
    cpu.running = false;
    cpuThread.join();    
    
    return 0;
}
