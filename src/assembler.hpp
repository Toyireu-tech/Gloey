#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <unordered_map>

#include "opcode.hpp"

namespace Assembler {

    static std::unordered_map<std::string, uint32_t> labels;
    static std::unordered_map<std::string, uint32_t> sections;

    static const std::unordered_map<std::string, uint8_t> mnemonics = {
        {"nop",      OpCode::Nop},

        {"add",      OpCode::Add},
        {"sub",      OpCode::Sub},
        {"mul",      OpCode::Mul},
        {"div",      OpCode::Div},

        {"and",      OpCode::And},
        {"or",       OpCode::Or},
        {"xor",      OpCode::Xor},
        {"not",      OpCode::Not},

        {"shl",      OpCode::Shl},
        {"shr",      OpCode::Shr},

        {"load",     OpCode::Load},

        {"memg",     OpCode::Memg},
        {"mems",     OpCode::Mems},
        {"memgw",    OpCode::Memgw},
        {"memsw",    OpCode::Memsw},

        {"comp",     OpCode::Comp},

        {"jump",     OpCode::Jump},
        {"jeq",      OpCode::Jeq},
        {"jneq",     OpCode::Jneq},
        {"jsup",     OpCode::Jsup},
        {"jinf",     OpCode::Jinf},

        {"jreg",     OpCode::Jreg},
        {"jeqreg",   OpCode::JeqReg},
        {"jneqreg",  OpCode::JneqReg},
        {"jsupreg",  OpCode::JsupReg},
        {"jinfreg",  OpCode::JinfReg},

        {"copy",     OpCode::Copy},

        {"call",     OpCode::Call},
        {"callreg",  OpCode::CallReg},
        {"ret",      OpCode::Ret},

        {"halt",     OpCode::Halt},
    };

    bool is_valid_line(const std::string& line);
    std::vector<std::string> tokenize(const std::string& line);
    bool isRegister(const std::string& tok);
    uint8_t parseRegister(const std::string& tok);
    bool isLabelDef(const std::string& line);
    bool isSectionDef(const std::string& line);
    std::string parseLabelDef(const std::string& line);
    std::string parseSectionDef(const std::string& line);
    std::vector<uint8_t> assemble(const std::string& src);
}
