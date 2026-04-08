#pragma once
#include <string>
#include <optional>
#include "constant.hpp"

namespace pwd {
    struct PasswordConfig {
        size_t length = DEFAULT_PASSWORD_LENGTH;
        size_t quantity = DEFAULT_QUANTITY;
        std::optional<std::string> output_file;
        bool show_entropy = false;
        bool use_upper = true;
        bool use_lower = true;
        bool use_digits = true;
        bool use_special = true;
    };

    class ConfigParser {
    public:
        static PasswordConfig parse(int argc, char* argv[]);
    };
}