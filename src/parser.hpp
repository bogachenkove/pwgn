/**
 * @file parser.hpp
 * @brief Declaration of command-line argument parser for password generation.
 *
 * Defines PasswordGenerationConfig structure to hold user settings and
 * CommandLineConfigParser class that translates argv into a config object.
 */

#pragma once
#include "constant.hpp"
#include <string>
#include <optional>

namespace pwd {

    /**
     * @brief Configuration object for password generation.
     *
     * Holds all user-specified options: character set flags, password length,
     * quantity, and optional output file. Default values are provided for
     * all fields where the user may omit an argument.
     */
    struct PasswordGenerationConfig {
        std::optional<size_t> explicitPasswordLength;   ///< Fixed length (if set); otherwise random length used.
        size_t passwordQuantity = DefaultPasswordQuantity; ///< Number of passwords to generate.
        std::optional<std::string> outputFilePath;      ///< Optional output file (std::cout if not set).
        bool includeUpperCase = true;                   ///< Include A-Z characters.
        bool includeLowerCase = true;                   ///< Include a-z characters.
        bool includeNumeric = true;                     ///< Include 0-9 digits.
        bool includeSpecial = true;                     ///< Include special symbols.
    };

    /**
     * @brief Parser for command-line arguments.
     *
     * Provides a single static method that processes argc/argv, validates
     * inputs, and returns a fully populated PasswordGenerationConfig.
     * Throws descriptive exceptions on invalid input.
     */
    class CommandLineConfigParser {
    public:
        /**
         * @brief Parse command-line arguments into a configuration object.
         *
         * Recognizes options: --help, --no-upper, --no-lower, --no-digits,
         * --no-special, -l/--length, -q/--quantity, -o/--output.
         * On --help, prints usage and exits successfully.
         *
         * @param argumentCount Number of command-line arguments (argc).
         * @param argumentValues Array of argument strings (argv).
         * @return PasswordGenerationConfig Populated configuration.
         * @throws std::invalid_argument On unknown option, missing value,
         *         invalid number, or no character set enabled.
         * @throws std::out_of_range If password length outside allowed range.
         */
        [[nodiscard]] static PasswordGenerationConfig parseArguments(int argumentCount, char* argumentValues[]);
    };

} // namespace pwd