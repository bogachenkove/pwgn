#include "parser.hpp"
#include "metadata.hpp"
#include <charconv>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace pwd {
    PasswordGenerationConfig CommandLineConfigParser::parseArguments(
        int argumentCount, char* argumentValues[]) {

        PasswordGenerationConfig generationConfig;

        for (int commandLineArgumentIndex = 1; commandLineArgumentIndex < argumentCount; ++commandLineArgumentIndex) {
            std::string_view currentArgument = argumentValues[commandLineArgumentIndex];

            if (currentArgument == "-h" || currentArgument == "--help") {
                std::cout << UsageInformation;
                std::exit(EXIT_SUCCESS);
            }
            if (currentArgument == "--no-upper") {
                generationConfig.includeUpperCase = false;
            }
            else if (currentArgument == "--no-lower") {
                generationConfig.includeLowerCase = false;
            }
            else if (currentArgument == "--no-digits") {
                generationConfig.includeNumeric = false;
            }
            else if (currentArgument == "--no-special") {
                generationConfig.includeSpecial = false;
            }
            else if (currentArgument == "-l" || currentArgument == "--length") {
                if (++commandLineArgumentIndex >= argumentCount) {
                    throw std::invalid_argument("Missing value for --length");
                }
                const char* valueStart = argumentValues[commandLineArgumentIndex];
                const char* valueEnd = valueStart + std::strlen(valueStart);
                size_t parsedLength = 0;
                auto parsingResult = std::from_chars(valueStart, valueEnd, parsedLength);

                if (parsingResult.ec != std::errc()) {
                    throw std::invalid_argument("Invalid number for --length");
                }
                if (parsedLength < MinimumPasswordLength || parsedLength > MaximumPasswordLength) {
                    throw std::out_of_range("Password length must be between 10 and 1000000");
                }
                generationConfig.explicitPasswordLength = parsedLength;
            }
            else if (currentArgument == "-q" || currentArgument == "--quantity") {
                if (++commandLineArgumentIndex >= argumentCount) {
                    throw std::invalid_argument("Missing value for --quantity");
                }
                const char* valueStart = argumentValues[commandLineArgumentIndex];
                const char* valueEnd = valueStart + std::strlen(valueStart);
                auto parsingResult = std::from_chars(valueStart, valueEnd, generationConfig.passwordQuantity);

                if (parsingResult.ec != std::errc() || generationConfig.passwordQuantity == 0) {
                    throw std::invalid_argument("Invalid or zero quantity for --quantity");
                }
            }
            else if (currentArgument == "-o" || currentArgument == "--output") {
                if (++commandLineArgumentIndex >= argumentCount) {
                    throw std::invalid_argument("Missing value for --output");
                }
                generationConfig.outputFilePath = argumentValues[commandLineArgumentIndex];
            }
            else {
                throw std::invalid_argument(std::string("Unknown option: ") + std::string(currentArgument));
            }
        }

        if (!generationConfig.includeUpperCase && !generationConfig.includeLowerCase &&
            !generationConfig.includeNumeric && !generationConfig.includeSpecial) {
            throw std::invalid_argument("At least one character set must be enabled");
        }

        return generationConfig;
    }
} // namespace pwd