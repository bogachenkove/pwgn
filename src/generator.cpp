/**
 * @file generator.cpp
 * @brief Implementation of cryptographic password generator.
 *
 * Uses libsodium's random number generator to produce secure passwords.
 * Handles character set selection, password length (fixed or random),
 * and batch generation of multiple passwords.
 */

#include "generator.hpp"
#include <sodium.h>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace pwd {

	/**
	 * @brief Constructor – initialize libsodium.
	 *
	 * Calls sodium_init() to ensure the cryptographic random number
	 * generator is ready. Throws if initialization fails.
	 *
	 * @throws std::runtime_error If libsodium initialization fails.
	 */
	CryptographicPasswordGenerator::CryptographicPasswordGenerator() {
		if (sodium_init() < 0) {
			throw std::runtime_error("Failed to initialize libsodium cryptographic library");
		}
	}

	/**
	 * @brief Build the character pool based on configuration flags.
	 *
	 * Concatenates the enabled character sets (upper case, lower case,
	 * digits, special characters), then sorts and removes duplicates.
	 * Returns a structure containing the combined pool and its unique length.
	 *
	 * @param generationConfig Configuration specifying which character sets to include.
	 * @return CharacterPoolData Combined character string and its unique count.
	 */
	CryptographicPasswordGenerator::CharacterPoolData CryptographicPasswordGenerator::buildCharacterPool(
		const PasswordGenerationConfig& generationConfig) const {

		CharacterPoolData characterPoolData;
		characterPoolData.combinedCharacters.reserve(128); // Pre-allocate typical capacity.

		// Append each enabled character set.
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

		// Remove duplicate characters that may appear across sets (e.g., if same char is in multiple).
		std::sort(characterPoolData.combinedCharacters.begin(), characterPoolData.combinedCharacters.end());
		auto uniqueEndIterator = std::unique(characterPoolData.combinedCharacters.begin(), characterPoolData.combinedCharacters.end());
		characterPoolData.combinedCharacters.erase(uniqueEndIterator, characterPoolData.combinedCharacters.end());
		characterPoolData.uniqueCharacterCount = characterPoolData.combinedCharacters.length();

		return characterPoolData;
	}

	/**
	 * @brief Generate a single password of given length from the specified character pool.
	 *
	 * Uses libsodium's randombytes_uniform() to pick characters uniformly.
	 *
	 * @param passwordLength Desired length of the password.
	 * @param characterPool String of allowed characters.
	 * @return std::string Randomly generated password.
	 * @throws std::runtime_error If the character pool is empty.
	 */
	std::string CryptographicPasswordGenerator::generateSinglePassword(
		size_t passwordLength, std::string_view characterPool) const {

		if (characterPool.empty()) {
			throw std::runtime_error("Character pool is empty. Cannot generate password.");
		}

		std::string generatedPassword;
		generatedPassword.reserve(passwordLength);
		const uint32_t characterPoolSize = static_cast<uint32_t>(characterPool.length());

		// Generate each character independently.
		for (size_t characterSelectionIndex = 0; characterSelectionIndex < passwordLength; ++characterSelectionIndex) {
			uint32_t randomCharacterIndex = randombytes_uniform(characterPoolSize);
			generatedPassword += characterPool[randomCharacterIndex];
		}
		return generatedPassword;
	}

	/**
	 * @brief Generate a batch of passwords according to configuration.
	 *
	 * Builds the character pool once, then generates the requested number
	 * of passwords. For each password, if an explicit length is set, that
	 * length is used; otherwise a random length within [RandomDefaultMinLength,
	 * RandomDefaultMaxLength] is chosen.
	 *
	 * @param generationConfig Configuration object (character sets, quantity, optional fixed length).
	 * @return std::vector<std::string> Vector of generated passwords.
	 */
	std::vector<std::string> CryptographicPasswordGenerator::generatePasswords(
		const PasswordGenerationConfig& generationConfig) {

		// Precompute the character pool (shared across all passwords).
		CharacterPoolData characterPoolData = buildCharacterPool(generationConfig);
		std::vector<std::string> generatedPasswords;
		generatedPasswords.reserve(generationConfig.passwordQuantity);

		for (size_t passwordGenerationIndex = 0; passwordGenerationIndex < generationConfig.passwordQuantity; ++passwordGenerationIndex) {
			size_t currentPasswordLength;
			if (generationConfig.explicitPasswordLength.has_value()) {
				// Use fixed length if provided.
				currentPasswordLength = generationConfig.explicitPasswordLength.value();
			}
			else {
				// Random length within the default range.
				const size_t lengthRange = RandomDefaultMaxLength - RandomDefaultMinLength + 1;
				currentPasswordLength = RandomDefaultMinLength +
					randombytes_uniform(static_cast<uint32_t>(lengthRange));
			}

			// Generate and store one password.
			generatedPasswords.emplace_back(
				generateSinglePassword(currentPasswordLength, characterPoolData.combinedCharacters));
		}

		return generatedPasswords;
	}

} // namespace pwd