#include "cpu.hpp"
#include "memory.hpp"
#include "opcode.hpp"
#include <cstdint>
#include <iostream>

#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <cstdint>
#include <cstring>
#include <stdexcept>


static const std::unordered_map<std::string, uint8_t> mnemonics = {
    {"NOP",  OpCode::Nop},
    {"ADD",  OpCode::Add},
    {"SUB",  OpCode::Sub},
    {"MUL",  OpCode::Mul},
    {"DIV",  OpCode::Div},
    {"AND",  OpCode::And},
    {"OR",   OpCode::Or},
    {"XOR",  OpCode::Xor},
    {"NOT",  OpCode::Not},
    {"SHL",  OpCode::Shl},
    {"SHR",  OpCode::Shr},
    {"LOAD", OpCode::Load},
    {"MEMG", OpCode::Memg},
    {"MEMS", OpCode::Mems},
    {"COMP", OpCode::Comp},
    {"JUMP", OpCode::Jump},
    {"JEQ",  OpCode::Jeq},
    {"JNEQ", OpCode::Jneq},
    {"JSUP", OpCode::Jsup},
    {"JINF", OpCode::Jinf},
    {"JREG", OpCode::Jreg},
    {"HALT", OpCode::Halt},
};

std::vector<std::string> tokenize(const std::string& line) {
    std::string cleaned = line.substr(0, line.find(';'));
    std::vector<std::string> tokens;
    std::istringstream iss(cleaned);
    std::string tok;
    while (iss >> tok) tokens.push_back(tok);
    return tokens;
}

bool isRegister(const std::string& tok) {
    return (tok.size() >= 2 && (tok[0] == 'R' || tok[0] == 'r') && isdigit(tok[1]));
}

uint8_t parseRegister(const std::string& tok) {
    return static_cast<uint8_t>(std::stoi(tok.substr(1)));
}

std::vector<uint8_t> assemble(const std::vector<std::string>& lines) {
    std::vector<uint8_t> output;

    for (const auto& line : lines) {
        auto tokens = tokenize(line);
        if (tokens.empty()) continue; 

        std::string mnemonic = tokens[0];
        auto it = mnemonics.find(mnemonic);
        if (it == mnemonics.end()) {
            throw std::runtime_error("Instruction inconnue: " + mnemonic);
        }

        uint8_t instr[8] = {0};
        instr[0] = it->second;

        int opIndex = 1;
        for (size_t i = 1; i < tokens.size() && opIndex < 8; ++i) {
            const std::string& tok = tokens[i];

            if (isRegister(tok)) {
                instr[opIndex++] = parseRegister(tok);
            } else {
                
                uint32_t imm = static_cast<uint32_t>(std::stoul(tok, nullptr, 0)); 
                std::memcpy(&instr[opIndex], &imm, 4);
                opIndex += 4;
            }
        }

        output.insert(output.end(), instr, instr + 8);
    }

    return output;
}

int main() {
    // test
    Memory mem(1024);
    CPU cpu(&mem);

    std::vector<std::string> program = {
        "ADD R0, R1, R2",
        ";JUMP 0x00",
        "HALT"
    };

    int p = 0;
    for (auto i : assemble(program)) {
        //std::cout << std::to_string(i) << std::endl;
        mem.set(p, i);
        p++;
    }
    

    //uint8_t halt[8] = {OpCode::Halt};
    //uint8_t instr_test[8] = {OpCode::Add, 0, 0, 1};
    //cpu.load(0, 30);
    //cpu.load(1, 12);
    
    //mem.add_instr(0, OpCode::Mul, 0,0, 1);
    //mem.add_instr(8, OpCode::Halt);
    cpu.run();
    std::cout << cpu.get_reg(0);
    return 0;
}
