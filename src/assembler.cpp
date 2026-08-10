#include "assembler.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <ios>
#include <span>
#include <stdexcept>
#include <string>
#include <sstream>
#include <unordered_map>
#include <utility>
#include "utils.hpp"
#include <cstring>
#include <fstream>
#include <vector>

using std::ios; 

namespace Assembler {

    

    template<typename T>
    std::string resolve(const std::string& token,
                        const std::vector<std::string>& stack,
                        const std::unordered_map<std::string, T>& table) {
        for (int depth = (int)stack.size(); depth >= 0; --depth) {
            std::string prefix;
            for (int i = 0; i < depth; ++i) {
                if (i > 0) prefix += ".";
                prefix += stack[i];
            }
            std::string key = prefix.empty() ? token : prefix + "." + token;
            auto it = table.find(key);
            if (it != table.end()) {
                if constexpr (std::is_same_v<T, uint32_t>)
                    return std::to_string(it->second);
                else
                    return it->second;
            }
        }
        return token;
    }
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

    std::string current_prefix() {
        std::string prefix;
        for (auto& m : module_stack) {
            if (!prefix.empty()) prefix += ".";
            prefix += m;
        }
        return prefix;
    }

    std::string make_key(const std::string& name) {
        std::string prefix = current_prefix();
        return prefix.empty() ? name : prefix + "." + name;
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

    bool isLabelDef(const std::string& line) {return (split(line, ' ')[0] == ".label"); }
    bool isSectionDef(const std::string& line) {return split(line, ' ')[0] == ".section"; }
    bool isBytesDef(const std::string& line) {return split(line, ' ')[0] == ".bytes"; }
    bool isIncBinDef(const std::string& line) { return split(line, ' ')[0] == ".incbin"; }
    bool isWordDef(const std::string& line) {return split(line, ' ')[0] == ".word"; }
    bool isReserveDef(const std::string& line) {return split(line, ' ')[0] == ".reserve"; }
    bool isStringDef(const std::string& line) {return split(line, ' ')[0] == ".string"; }
    bool isAliasDef(const std::string& line) {return split(line, ' ')[0] == "#alias"; }
    bool isModDef(const std::string& line) { return split(line, ' ')[0] == "#defmod"; }
    bool isEndMod(const std::string& line) { return split(line, ' ')[0] == "#endmod"; }
    bool isInclude(const std::string& line) { return split(line, ' ')[0] == "#include"; }

    std::string parseLabelDef(const std::string& line) {
        return make_key(split(line, ' ')[1]);
        
    }

    std::string parseSectionDef(const std::string& line) {
        return make_key(split(line, ' ')[1]);
    }

    std::pair<std::string, std::string> parseAliasDef(const std::string& line) {
        return {
                (split(line, ' ')[1]),
                split(line, ' ')[2]
            };
    }

    std::vector<std::string> preprocessIncludes(const std::vector<std::string>& input_lines) {
        std::vector<std::string> expanded_lines;

        for (auto line : input_lines) {
            // On vérifie si la ligne contient un #include
            trim(line);
            if (isInclude(line)) {
                auto first = line.find('"');
                auto last  = line.rfind('"');

                if (first == std::string::npos || last == std::string::npos || first == last) {
                    throw std::runtime_error("Invalid syntax : #include \"filename\"");
                }

                // Extraction du nom du fichier
                std::string filename = line.substr(first + 1, last - first - 1);

                // Ouverture du fichier inclus
                std::ifstream file(filename);
                if (!file) {
                    throw std::runtime_error("Could not open included file : " + filename);
                }

                // Lecture des lignes du fichier inclus
                std::vector<std::string> included_file_lines;
                std::string file_line;
                while (std::getline(file, file_line)) {
                    included_file_lines.push_back(file_line);
                }

                // Appel récursif pour gérer les #include imbriqués dans ce fichier
                auto expanded_file = preprocessIncludes(included_file_lines);

                // Inserer toutes les lignes lues à la place de la ligne #include
                expanded_lines.insert(expanded_lines.end(), expanded_file.begin(), expanded_file.end());
            } else {
                // Ligne normale : on la conserve telle quelle
                expanded_lines.push_back(line);
            }
        }

        return expanded_lines;
    }

    std::vector<uint8_t> assemble(std::string& src) {
        std::vector<uint8_t> output;
        auto lines = split(src, '\n');
        lines = preprocessIncludes(lines);

        uint32_t pc = 0;

        // First iteeration, for detecting sections and labels
        for (auto line : lines) {
            trim(line);
            if (!is_valid_line(line)) continue;

            if (isModDef(line)) { module_stack.push_back(split(line, ' ')[1]); continue; }
            if (isEndMod(line)) {
                if (module_stack.empty()) throw std::runtime_error("Cant close a non-existant module");
                module_stack.pop_back();
                continue;
            }

            if (isLabelDef(line))   { labels[parseLabelDef(line)] = pc; continue; }
            if (isSectionDef(line)) { sections[parseSectionDef(line)] = pc; continue; }
            if (isAliasDef(line)) {
                auto p = parseAliasDef(line);
                alias[make_key('$' + p.first)] = p.second;   // operator[], pas insert()
                continue;
            }

            if (isIncBinDef(line)) {
                auto first = line.find('"');
                auto last  = line.rfind('"');

                if (first == std::string::npos || last == std::string::npos || first == last) {
                    throw std::runtime_error("Invalid syntax : incbin \"filename\"");
                }

                std::string filename = line.substr(first + 1, last - first - 1);

                std::ifstream file(filename, std::ios::binary);
                if (!file) {
                    throw std::runtime_error("Could not open file named : " + filename);
                }
                pc += std::filesystem::file_size(filename);

                // Padding
                if (pc % 2 != 0) {
                    pc++;
                }
                continue;
            }
            
            if (isReserveDef(line)) {
                auto size = std::stoul(split(line, ' ')[1], nullptr, 0);
                pc += size;
                continue;
            }


            if (isStringDef(line)) {
                auto first = line.find('"');
                auto last  = line.rfind('"');

                if (first == std::string::npos || last == std::string::npos || first == last) {
                    throw std::runtime_error("Invalid syntax : .string \"str\"");
                }
                std::string str = line.substr(first + 1, last - first - 1);
                std::unordered_map<std::string, std::string> mapp({
                    {"\\n", "\n"}
                });
                replace_all(str, mapp);
                pc += str.size() + 1; 
                continue;
            }

            if (isWordDef(line)) {
                pc += 4;
                continue;
            }

            pc += 8;
        } 
        if (!module_stack.empty()) throw std::runtime_error("Module never closed");    
        

        



        // Second iteeration, for assemble
        size_t lc = 0;
        module_stack.clear();
        pc = 0;

        for (auto& line : lines) {
            trim(line);

            if (isModDef(line)) { module_stack.push_back(split(line, ' ')[1]); continue; }
            if (isEndMod(line)) { module_stack.pop_back(); continue; }

            // remplacer les 3 appels replace_allw par une résolution mot-à-mot, stack-aware
            auto words = split(line, ' ');
            for (auto& w : words) {
                std::string r = resolve(w, module_stack, labels);
                if (r == w) r = resolve(w, module_stack, sections);
                if (r == w) r = resolve(w, module_stack, alias);
                w = r;
            }
            line = join(words, ' ');  

            if (!is_valid_line(line) || isLabelDef(line) || isSectionDef(line) || 
                isAliasDef(line) || isModDef(line) || isEndMod(line))
                continue;

            
            //print_map(alias);

            if (isBytesDef(line)) {
                auto arr = split(line, ' ');
                std::span<std::string> slice = std::span(arr).subspan(1);
                for (auto& i : slice) {
                    output.push_back(static_cast<uint8_t>(std::stoul(i, nullptr, 0)));
                    pc++;
                }
                continue;
            }

                        println(line + " pc: " + std::to_string(pc));

            if (isWordDef(line)) {
                auto arr = split(line, ' ');
                std::span<std::string> slice = std::span(arr).subspan(1);
                std::vector<uint8_t> imm_arr = {0, 0, 0, 0};
                u32_to_little_endian( static_cast<uint32_t>(std::stoul(slice[0], nullptr, 0)), imm_arr.data());
                output.insert(output.end(), imm_arr.begin(), imm_arr.end());
                pc += 4;
                continue;
            }

            if (isIncBinDef(line)) {
                auto first = line.find('"');
                auto last  = line.rfind('"');

                if (first == std::string::npos || last == std::string::npos || first == last) {
                    throw std::runtime_error("Invalid syntax : incbin \"filename\"");
                }

                std::string filename = line.substr(first + 1, last - first - 1);

                std::ifstream file(filename, std::ios::binary);
                if (!file) {
                    throw std::runtime_error("Could not open file named : " + filename);
                }

                char byte;
                while (file.get(byte)) {
                    output.push_back(static_cast<uint8_t>(byte));
                    pc++;
                }

                if (pc % 2 != 0) {
                    output.push_back(0x00); // On insère 1 octet NUL de bourrage dans le binaire
                    pc++;
                }
                continue;
            }



            if (isReserveDef(line)) {
                auto size = std::stoul(split(line, ' ')[1], nullptr, 0);
                for (uint32_t i = 0; i < size; i++) {
                    output.push_back(0);
                }
                pc += size;
                continue;
            }

            

            if (isStringDef(line)) {
                auto first = line.find('"');
                auto last  = line.rfind('"');

                if (first == std::string::npos || last == std::string::npos || first == last) {
                    throw std::runtime_error("Invalid syntax : .string \"str\"");
                }
                

                std::string str = line.substr(first + 1, last - first - 1);
                //println("ff : " + str);
                std::unordered_map<std::string, std::string> mapp({
                    {"\\n", "\n"}
                });
                
                replace_all(str, mapp);
                for (char c : str)
                    output.push_back(static_cast<uint8_t>(c));

                output.push_back(0);
                pc += str.size() + 1;
                continue;
            }




            auto tokens = tokenize(line);
            uint8_t assembled_line[8] = {};
            if (tokens.size() <= 0) throw std::runtime_error("Invalid expression: " + line);

            auto it = mnemonics.find(tokens[0]);
            if (it == mnemonics.end())
                throw std::runtime_error("Unknown instruction at line " + std::to_string(lc) + " : " + tokens[0]);
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
            lc++;
            pc+=8;
        }  

        return output;
    }

    void compile_file(const std::string& input_name, const std::string& output_name) {
        std::string input = "";
        std::vector<uint8_t> output;
        std::ifstream input_file(input_name);
        std::ofstream output_file(output_name, ios::binary | ios::trunc);
        std::string line;

        if (input_file.is_open()) {
            while (std::getline(input_file, line)) {
                input += line + '\n';
            }
        } else {
            throw std::runtime_error("Counld not open file named \"" + input_name + "\"");
        }

        output = assemble(input);
        compressToGz(output, output_name);

        // if (output_file.is_open()) {
        //     output_file.write(reinterpret_cast<const char*>(output.data()), output.size());
        // } else {
        //     throw std::runtime_error("Counld not open file named \"" + output_name + "\"");
        // }

        input_file.close();
        output_file.close(); 
    }
}