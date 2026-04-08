/**
 * @file parser.cpp
 * @brief Implementation of command-line argument parsing for password generation.
 *
 * Parses command-line options such as character set toggles, password length,
 * quantity, output file, and help. Validates inputs and constructs a
 * PasswordGenerationConfig object. Throws exceptions with descriptive messages
 * on invalid input.
 */

#include "parser.hpp"
#include "metadata.hpp"
#include <charconv>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace pwd {

	/**
	 * @brief Parse command-line arguments and build a configuration.
	 *
	 * Iterates over argv[1..argc-1], interpreting flags and their optional
	 * values. On --help, prints usage and exits successfully. On errors,
	 * throws std::invalid_argument or std::out_of_range.
	 *
	 * @param argumentCount Number of command-line arguments (argc).
	 * @param argumentValues Array of argument strings (argv).
	 * @return PasswordGenerationConfig Populated configuration.
	 * @throws std::invalid_argument If an unknown option, missing value,
	 *         invalid numeric format, or no character set is enabled.
	 * @throws std::out_of_range If password length is outside allowed range.
	 */
	PasswordGenerationConfig CommandLineConfigParser::parseArguments(
		int argumentCount, char* argumentValues[]) {

		PasswordGenerationConfig generationConfig;  // Defaults to all character sets enabled.

		// Skip argv[0] (program name), start from index 1.
		for (int commandLineArgumentIndex = 1; commandLineArgumentIndex < argumentCount; ++commandLineArgumentIndex) {
			std::string_view currentArgument = argumentValues[commandLineArgumentIndex];

			// Help option: display usage and exit gracefully.
			if (currentArgument == "-h" || currentArgument == "--help") {
				std::cout << UsageInformation;
				std::exit(EXIT_SUCCESS);
			}

			// Toggle character set inclusion (all are true by default).
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

			// Password length: expects a following argument.
			else if (currentArgument == "-l" || currentArgument == "--length") {
				// Ensure the next argument exists.
				if (++commandLineArgumentIndex >= argumentCount) {
					throw std::invalid_argument("Missing value for --length");
				}
				const char* valueStart = argumentValues[commandLineArgumentIndex];
				const char* valueEnd = valueStart + std::strlen(valueStart);
				size_t parsedLength = 0;
				// Convert from characters to size_t, without locale sensitivity.
				auto parsingResult = std::from_chars(valueStart, valueEnd, parsedLength);

				if (parsingResult.ec != std::errc()) {
					throw std::invalid_argument("Invalid number for --length");
				}
				// Validate against predefined limits.
				if (parsedLength < MinimumPasswordLength || parsedLength > MaximumPasswordLength) {
					throw std::out_of_range("Password length must be between 10 and 1000000");
				}
				generationConfig.explicitPasswordLength = parsedLength;
			}

			// Password quantity: expects a positive integer.
			else if (currentArgument == "-q" || currentArgument == "--quantity") {
				if (++commandLineArgumentIndex >= argumentCount) {
					throw std::invalid_argument("Missing value for --quantity");
				}
				const char* valueStart = argumentValues[commandLineArgumentIndex];
				const char* valueEnd = valueStart + std::strlen(valueStart);
				// Parse directly into the config field.
				auto parsingResult = std::from_chars(valueStart, valueEnd, generationConfig.passwordQuantity);

				if (parsingResult.ec != std::errc() || generationConfig.passwordQuantity == 0) {
					throw std::invalid_argument("Invalid or zero quantity for --quantity");
				}
			}

			// Output file path: redirects generated passwords to a file.
			else if (currentArgument == "-o" || currentArgument == "--output") {
				if (++commandLineArgumentIndex >= argumentCount) {
					throw std::invalid_argument("Missing value for --output");
				}
				generationConfig.outputFilePath = argumentValues[commandLineArgumentIndex];
			}

			// Any other argument is unknown.
			else {
				throw std::invalid_argument(std::string("Unknown option: ") + std::string(currentArgument));
			}
		}

		// After processing all arguments, ensure at least one character set is enabled.
		if (!generationConfig.includeUpperCase && !generationConfig.includeLowerCase &&
			!generationConfig.includeNumeric && !generationConfig.includeSpecial) {
			throw std::invalid_argument("At least one character set must be enabled");
		}

		return generationConfig;
	}

} // namespace pwd