#pragma once
#include "constant.hpp"
#include <string>
#include <optional>

namespace pwd {
    struct PasswordGenerationConfig {
        std::optional<size_t> explicitPasswordLength;
        size_t passwordQuantity = DefaultPasswordQuantity;
        std::optional<std::string> outputFilePath;
        bool includeUpperCase = true;
        bool includeLowerCase = true;
        bool includeNumeric = true;
        bool includeSpecial = true;
    };

    class CommandLineConfigParser {
    public:
        [[nodiscard]] static PasswordGenerationConfig parseArguments(int argumentCount, char* argumentValues[]);
    };
} // namespace pwd