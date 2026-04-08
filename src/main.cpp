/**
 * @file main.cpp
 * @brief Entry point for the password generation CLI tool.
 *
 * Parses command-line arguments, generates passwords according to the
 * specified configuration, and writes the results to standard output or
 * a file. Handles file output redirection, password enumeration when
 * multiple passwords are requested, and graceful error handling.
 */

#include "platform.hpp"
#include "parser.hpp"
#include "generator.hpp"
#include "metadata.hpp"
#include <fstream>
#include <iostream>
#include <memory>

 /**
  * @brief Program entry point.
  *
  * Validates command-line arguments, constructs a password generation
  * configuration, invokes the password generator, and outputs the resulting
  * passwords. Supports optional file output and automatic line numbering
  * when generating more than one password.
  *
  * @param argumentCount Number of command-line arguments (argc).
  * @param argumentValues Array of argument strings (argv).
  * @return int EXIT_SUCCESS on success, EXIT_FAILURE on error.
  */
int main(int argumentCount, char* argumentValues[]) {
	try {
		// Step 1: Parse command-line arguments into a configuration object.
		pwd::PasswordGenerationConfig generationConfig =
			pwd::CommandLineConfigParser::parseArguments(argumentCount, argumentValues);

		// Step 2: Generate the requested passwords using the cryptographic generator.
		pwd::CryptographicPasswordGenerator passwordGenerator;
		std::vector<std::string> generatedPasswords = passwordGenerator.generatePasswords(generationConfig);

		// Step 3: Prepare output stream – either std::cout or a file.
		std::unique_ptr<std::ostream> ownedOutputStream;   // Holds file stream if used.
		std::ofstream fileOutputHandle;                    // Raw file handle.
		std::ostream* activeOutputStream = &std::cout;     // Default to console.

		// If an output file path was provided, open it and switch the active stream.
		if (generationConfig.outputFilePath.has_value()) {
			fileOutputHandle.open(generationConfig.outputFilePath.value(), std::ios::out | std::ios::trunc);
			if (!fileOutputHandle.is_open()) {
				std::cerr << "Error: Cannot open output file: " << generationConfig.outputFilePath.value() << "\n";
				return EXIT_FAILURE;
			}
			// Transfer ownership of the file stream to a unique_ptr for automatic cleanup.
			ownedOutputStream = std::make_unique<std::ofstream>(std::move(fileOutputHandle));
			activeOutputStream = ownedOutputStream.get();
		}

		// Step 4: Output each password, optionally prefixed with a sequential number.
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
	// Catch known standard exceptions and print the error message.
	catch (const std::exception& exceptionObject) {
		std::cerr << "Fatal Error: " << exceptionObject.what() << "\n";
		return EXIT_FAILURE;
	}
	// Catch any non-standard exception (e.g., thrown integer or custom type).
	catch (...) {
		std::cerr << "Fatal Error: An unknown exception occurred.\n";
		return EXIT_FAILURE;
	}
}