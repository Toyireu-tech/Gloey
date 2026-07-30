#include "assembler.hpp"
#include <cstdint>
#include <string>
#include <sstream>
#include <utils.h>
#include <cstring>

namespace Assembler {
    inline bool is_valid_line(const std::string& line)
    {
        size_t start = 0;
        while (start < line.size() && std::isspace(static_cast<unsigned char>(line[start])))
        {
            start++;
        }
        if (start == line.size())
            return false;

        if (line[start] == ';')
            return false;

        return true;
    }

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

    bool isLabelDef(const std::string& line) {
        if (split(line, ' ')[0] == ".label") return true;
        return false;
    }

    bool isSectionDef(const std::string& line) {
        if (split(line, ' ')[0] == ".section") return true;
        return false;
    }

    std::string parseLabelDef(const std::string& line) {
        return (split(line, ' ')[1]);
        
    }

    std::string parseSectionDef(const std::string& line) {
        return (split(line, ' ')[1]);
    }

    std::vector<uint8_t> assemble(const std::string& src) {
        std::vector<uint8_t> output;
        auto lines = split(src, '\n');

        uint32_t pc = 0;

        // First iteeration, for detecting sections and labels
        for (auto line : lines) {
            trim(line);
            if (!is_valid_line(line)) continue;

            if(isLabelDef(line)) {
                labels.insert({
                    parseLabelDef(line),
                    pc
                });
                continue;
            }
            
            if(isSectionDef(line)) {
                sections.insert({
                    parseSectionDef(line),
                    pc
                });
                continue;
            }
            pc += 8;
        } 

        

        // Second iteeration, for assemble 
        for (auto line : lines) {
            trim(line);
            replace_all(line, labels);
            replace_all(line, sections);
            if (!is_valid_line(line) || isLabelDef(line) || isSectionDef(line)) continue;
            auto tokens = tokenize(line);
            uint8_t assembled_line[8] = {};
            if (tokens.size() <= 0) throw std::runtime_error("Invalid expression: " + line);

            auto it = mnemonics.find(tokens[0]);
            if (it == mnemonics.end())
                throw std::runtime_error("Unknown instruction: " + tokens[0]);
            assembled_line[0] = it->second;

            int opIndex = 1;
            for (size_t i = 1; i < tokens.size() && opIndex < 8; ++i) {
                const std::string& tok = tokens[i];

                if (isRegister(tok)) {
                    assembled_line[opIndex++] = parseRegister(tok);
                } else {
                    uint32_t imm = static_cast<uint32_t>(std::stoul(tok, nullptr, 0)); 
                    std::memcpy(&assembled_line[4], &imm, 4);
                    opIndex += 4;
                }
            }
            output.insert(output.end(), assembled_line, assembled_line + 8);
        }  

        return output;
    }
}