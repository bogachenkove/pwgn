/**
 * @file generator.hpp
 * @brief Declaration of cryptographic password generator class.
 *
 * Defines CryptographicPasswordGenerator which uses libsodium's RNG to
 * produce secure passwords based on user configuration. The class is
 * non-copyable and manages libsodium initialization.
 */

#pragma once
#include "parser.hpp"
#include <string>
#include <vector>

namespace pwd {

    /**
     * @brief Cryptographic password generator using libsodium.
     *
     * Initializes libsodium in the constructor. Provides a method to generate
     * one or more passwords according to PasswordGenerationConfig settings.
     * Character sets are combined and deduplicated; password length can be
     * fixed or random within a default range.
     *
     * Copy operations are deleted because the class holds no resources that
     * require deep copying, and libsodium context is global.
     */
    class CryptographicPasswordGenerator {
    public:
        /**
         * @brief Constructor – initialize libsodium.
         *
         * Calls sodium_init(). Throws std::runtime_error if initialization fails.
         */
        CryptographicPasswordGenerator();

        /// Default destructor – no special cleanup needed.
        ~CryptographicPasswordGenerator() = default;

        // Prevent copying (no meaningful copy semantics).
        CryptographicPasswordGenerator(const CryptographicPasswordGenerator&) = delete;
        CryptographicPasswordGenerator& operator=(const CryptographicPasswordGenerator&) = delete;

        /**
         * @brief Generate passwords according to configuration.
         *
         * Builds the character pool once, then generates the requested number
         * of passwords. Each password uses either the explicit length from the
         * config or a random length within [RandomDefaultMinLength, RandomDefaultMaxLength].
         *
         * @param generationConfig Configuration specifying character sets, quantity, optional fixed length.
         * @return std::vector<std::string> Vector of generated passwords.
         */
        [[nodiscard]] std::vector<std::string> generatePasswords(const PasswordGenerationConfig& generationConfig);

    private:
        /**
         * @brief Internal structure holding combined character pool.
         *
         * Contains the deduplicated string of all enabled characters and its length.
         */
        struct CharacterPoolData {
            std::string combinedCharacters;   ///< Concatenated unique characters.
            size_t uniqueCharacterCount;      ///< Length of combinedCharacters.
        };

        /**
         * @brief Build the character pool based on configuration.
         *
         * Concatenates enabled character sets, sorts, removes duplicates.
         *
         * @param generationConfig Configuration specifying which character sets to include.
         * @return CharacterPoolData Combined pool and its unique count.
         */
        [[nodiscard]] CharacterPoolData buildCharacterPool(const PasswordGenerationConfig& generationConfig) const;

        /**
         * @brief Generate a single password of given length from a character pool.
         *
         * Uses randombytes_uniform() to pick characters uniformly.
         *
         * @param passwordLength Desired length of the password.
         * @param characterPool String of allowed characters.
         * @return std::string Randomly generated password.
         * @throws std::runtime_error If characterPool is empty.
         */
        [[nodiscard]] std::string generateSinglePassword(size_t passwordLength, std::string_view characterPool) const;
    };

} // namespace pwd