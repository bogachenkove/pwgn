#include "platform.hpp"
#include "parser.hpp"
#include "generator.hpp"
#include "metadata.hpp"
#include <fstream>
#include <iostream>
#include <memory>

int main(int argumentCount, char* argumentValues[]) {
    try {
        pwd::PasswordGenerationConfig generationConfig =
            pwd::CommandLineConfigParser::parseArguments(argumentCount, argumentValues);

        pwd::CryptographicPasswordGenerator passwordGenerator;
        std::vector<std::string> generatedPasswords = passwordGenerator.generatePasswords(generationConfig);

        std::unique_ptr<std::ostream> ownedOutputStream;
        std::ofstream fileOutputHandle;
        std::ostream* activeOutputStream = &std::cout;

        if (generationConfig.outputFilePath.has_value()) {
            fileOutputHandle.open(generationConfig.outputFilePath.value(), std::ios::out | std::ios::trunc);
            if (!fileOutputHandle.is_open()) {
                std::cerr << "Error: Cannot open output file: " << generationConfig.outputFilePath.value() << "\n";
                return EXIT_FAILURE;
            }
            ownedOutputStream = std::make_unique<std::ofstream>(std::move(fileOutputHandle));
            activeOutputStream = ownedOutputStream.get();
        }

        const bool shouldEnumeratePasswords = generationConfig.passwordQuantity > 1;
        size_t passwordOutputIndex = 1;

        for (const auto& currentPassword : generatedPasswords) {
            if (shouldEnumeratePasswords) {
                *activeOutputStream << passwordOutputIndex << ". ";
            }
            *activeOutputStream << currentPassword << '\n';
            ++passwordOutputIndex;
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception& exceptionObject) {
        std::cerr << "Fatal Error: " << exceptionObject.what() << "\n";
        return EXIT_FAILURE;
    }
    catch (...) {
        std::cerr << "Fatal Error: An unknown exception occurred.\n";
        return EXIT_FAILURE;
    }
}