#include "generator.hpp"
#include <sodium.h>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace pwd {
    CryptographicPasswordGenerator::CryptographicPasswordGenerator() {
        if (sodium_init() < 0) {
            throw std::runtime_error("Failed to initialize libsodium cryptographic library");
        }
    }

    CryptographicPasswordGenerator::CharacterPoolData CryptographicPasswordGenerator::buildCharacterPool(
        const PasswordGenerationConfig& generationConfig) const {

        CharacterPoolData characterPoolData;
        characterPoolData.combinedCharacters.reserve(128);

        if (generationConfig.includeUpperCase) {
            characterPoolData.combinedCharacters.append(UpperCaseCharacters.data(), UpperCaseCharacters.length());
        }
        if (generationConfig.includeLowerCase) {
            characterPoolData.combinedCharacters.append(LowerCaseCharacters.data(), LowerCaseCharacters.length());
        }
        if (generationConfig.includeNumeric) {
            characterPoolData.combinedCharacters.append(NumericCharacters.data(), NumericCharacters.length());
        }
        if (generationConfig.includeSpecial) {
            characterPoolData.combinedCharacters.append(SpecialCharacters.data(), SpecialCharacters.length());
        }

        std::sort(characterPoolData.combinedCharacters.begin(), characterPoolData.combinedCharacters.end());
        auto uniqueEndIterator = std::unique(characterPoolData.combinedCharacters.begin(), characterPoolData.combinedCharacters.end());
        characterPoolData.combinedCharacters.erase(uniqueEndIterator, characterPoolData.combinedCharacters.end());
        characterPoolData.uniqueCharacterCount = characterPoolData.combinedCharacters.length();

        return characterPoolData;
    }

    std::string CryptographicPasswordGenerator::generateSinglePassword(
        size_t passwordLength, std::string_view characterPool) const {

        if (characterPool.empty()) {
            throw std::runtime_error("Character pool is empty. Cannot generate password.");
        }

        std::string generatedPassword;
        generatedPassword.reserve(passwordLength);
        const uint32_t characterPoolSize = static_cast<uint32_t>(characterPool.length());

        for (size_t characterSelectionIndex = 0; characterSelectionIndex < passwordLength; ++characterSelectionIndex) {
            uint32_t randomCharacterIndex = randombytes_uniform(characterPoolSize);
            generatedPassword += characterPool[randomCharacterIndex];
        }
        return generatedPassword;
    }

    std::vector<std::string> CryptographicPasswordGenerator::generatePasswords(
        const PasswordGenerationConfig& generationConfig) {

        CharacterPoolData characterPoolData = buildCharacterPool(generationConfig);
        std::vector<std::string> generatedPasswords;
        generatedPasswords.reserve(generationConfig.passwordQuantity);

        for (size_t passwordGenerationIndex = 0; passwordGenerationIndex < generationConfig.passwordQuantity; ++passwordGenerationIndex) {
            size_t currentPasswordLength;
            if (generationConfig.explicitPasswordLength.has_value()) {
                currentPasswordLength = generationConfig.explicitPasswordLength.value();
            }
            else {
                const size_t lengthRange = RandomDefaultMaxLength - RandomDefaultMinLength + 1;
                currentPasswordLength = RandomDefaultMinLength +
                    randombytes_uniform(static_cast<uint32_t>(lengthRange));
            }

            generatedPasswords.emplace_back(
                generateSinglePassword(currentPasswordLength, characterPoolData.combinedCharacters));
        }

        return generatedPasswords;
    }
} // namespace pwd