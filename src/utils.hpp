#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

inline uint32_t little_endian_to_u32(const uint8_t bytes[4])
{
    return  (uint32_t(bytes[0])      ) |
            (uint32_t(bytes[1]) <<  8) |
            (uint32_t(bytes[2]) << 16) |
            (uint32_t(bytes[3]) << 24);
}

inline void u32_to_little_endian(uint32_t value, uint8_t bytes[4])
{
    bytes[0] = static_cast<uint8_t>(value);
    bytes[1] = static_cast<uint8_t>(value >> 8);
    bytes[2] = static_cast<uint8_t>(value >> 16);
    bytes[3] = static_cast<uint8_t>(value >> 24);
}

template<typename T>
inline void println(const T& value)
{
    std::cout << value << '\n';
}

inline std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> result;

    size_t start = 0;

    for (size_t i = 0; i <= str.size(); i++)
    {
        if (i == str.size() || str[i] == delimiter)
        {
            result.emplace_back(str.substr(start, i - start));
            start = i + 1;
        }
    }

    return result;
}

inline void replace_all(std::string& str,
                 const std::unordered_map<std::string, uint32_t>& map)
{
    for (const auto& [key, value] : map)
    {
        const std::string replacement = std::to_string(value);

        size_t pos = 0;

        while ((pos = str.find(key, pos)) != std::string::npos)
        {
            str.replace(pos, key.length(), replacement);
            pos += replacement.length();
        }
    }
}

inline void replace_allw(std::string& str,
                          const std::unordered_map<std::string, uint32_t>& map)
{
    auto words = split(str, ' ');

    for (auto& w : words) {
        auto it = map.find(w);        // égalité stricte, pas de substring
        if (it != map.end()) {
            w = std::to_string(it->second);
        }
    }

    // reconstruire la ligne à partir des mots modifiés
    std::string rebuilt;
    for (size_t i = 0; i < words.size(); ++i) {
        if (i > 0) rebuilt += ' ';
        rebuilt += words[i];
    }
    str = rebuilt;
}

inline void replace_all(std::string& str,
                 const std::unordered_map<std::string, std::string>& map)
{
    for (const auto& [key, value] : map)
    {
        const std::string replacement = value;

        size_t pos = 0;

        while ((pos = str.find(key, pos)) != std::string::npos)
        {
            str.replace(pos, key.length(), replacement);
            pos += replacement.length();
        }
    }
} 

inline void replace_allw(std::string& str,
                          const std::unordered_map<std::string, std::string>& map)
{
    auto words = split(str, ' ');

    for (auto& w : words) {
        auto it = map.find(w);        // égalité stricte, pas de substring
        if (it != map.end()) {
            w = (it->second);
        }
    }

    // reconstruire la ligne à partir des mots modifiés
    std::string rebuilt;
    for (size_t i = 0; i < words.size(); ++i) {
        if (i > 0) rebuilt += ' ';
        rebuilt += words[i];
    }
    str = rebuilt;
}

inline void print_map(const std::unordered_map<std::string, uint32_t>& map)
{
    std::cout << '{';
    for (const auto& [key, value] : map)
    {
        std::cout << key << " : "
                  << static_cast<int>(value)
                  << '\n';
    }
    std::cout << "}\n";
}

inline void print_map(const std::unordered_map<std::string, std::string>& map)
{
    std::cout << '{';
    for (const auto& [key, value] : map)
    {
        std::cout << key << " : "
                  << value
                  << '\n';
    }
    std::cout << "}\n";
}

inline std::string join(const std::vector<std::string>& words, char sep) {
    std::string result;
    for (size_t i = 0; i < words.size(); ++i) {
        if (i > 0) result += sep;
        result += words[i];
    }
    return result;
}

inline void trim(std::string& str)
{
    size_t start = 0;

    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
        ++start;

    size_t end = str.size();

    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
        --end;

    str = str.substr(start, end - start);
}

inline uint32_t eval(const std::string& expr) {
    uint32_t result = 0;
    uint32_t term = 0;
    uint32_t number = 0;
    char op = '+';

    for (size_t i = 0; i <= expr.size(); ++i) {
        char c = i < expr.size() ? expr[i] : '+';

        if (std::isdigit(c)) {
            number = number * 10 + (c - '0');
            continue;
        }

        if (c == ' ')
            continue;

        switch (op) {
            case '+':
                result += term;
                term = number;
                break;

            case '-':
                result += term;
                term = -number;
                break;

            case '*':
                term *= number;
                break;
        }

        number = 0;
        op = c;
    }

    return result + term;
}

inline std::string eval_str(const std::string& expr) {
    return std::to_string(eval(expr));
}