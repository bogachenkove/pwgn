#pragma once
#include "config_parser.hpp"
#include <vector>
#include <string>

namespace pwd {
    class PasswordGenerator {
    public:
        PasswordGenerator();
        ~PasswordGenerator() = default;
        PasswordGenerator(const PasswordGenerator&) = delete;
        PasswordGenerator& operator=(const PasswordGenerator&) = delete;

        std::vector<std::string> generate(const PasswordConfig& config);
        double calculate_entropy(size_t length, size_t charset_size) const;

    private:
        std::string build_charset(const PasswordConfig& config) const;
        std::string generate_single(size_t length, const std::string& charset) const;
    };
}