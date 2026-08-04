#include "assembler.hpp"
#include <iostream>

int main(int argc, const char **argv) {
    if (argc < 3) {
        std::cerr << "Invalid command";
        return 1;
    }
    Assembler::compile_file(argv[1], argv[2]);
    return 0;
}