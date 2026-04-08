#pragma once
#include "parser.hpp"
#include <string>
#include <vector>

namespace pwd {
    class CryptographicPasswordGenerator {
    public:
        CryptographicPasswordGenerator();
        ~CryptographicPasswordGenerator() = default;
        CryptographicPasswordGenerator(const CryptographicPasswordGenerator&) = delete;
        CryptographicPasswordGenerator& operator=(const CryptographicPasswordGenerator&) = delete;

        [[nodiscard]] std::vector<std::string> generatePasswords(const PasswordGenerationConfig& generationConfig);

    private:
        struct CharacterPoolData {
            std::string combinedCharacters;
            size_t uniqueCharacterCount;
        };

        [[nodiscard]] CharacterPoolData buildCharacterPool(const PasswordGenerationConfig& generationConfig) const;
        [[nodiscard]] std::string generateSinglePassword(size_t passwordLength, std::string_view characterPool) const;
    };
} // namespace pwd