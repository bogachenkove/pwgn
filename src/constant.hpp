/**
 * @file constant.hpp
 * @brief Defines constants used throughout the password generation library.
 *
 * Contains limits for password lengths, default random range, default quantity,
 * and character sets for upper case, lower case, digits, and special symbols.
 */

#pragma once
#include <string_view>

namespace pwd {

	/**
	 * @brief Minimum allowed password length when explicitly specified.
	 *
	 * Ensures passwords have at least this many characters for minimal security.
	 */
	constexpr size_t MinimumPasswordLength = 10;

	/**
	 * @brief Maximum allowed password length when explicitly specified.
	 *
	 * Prevents excessive memory allocation; 1,000,000 characters is a generous upper bound.
	 */
	constexpr size_t MaximumPasswordLength = 1'000'000;

	/**
	 * @brief Minimum password length for random-length generation.
	 *
	 * When generating passwords with random length, the length will be at least this value.
	 */
	constexpr size_t RandomDefaultMinLength = 10;

	/**
	 * @brief Maximum password length for random-length generation.
	 *
	 * When generating passwords with random length, the length will be at most this value.
	 */
	constexpr size_t RandomDefaultMaxLength = 100;

	/**
	 * @brief Default number of passwords to generate if --quantity is not specified.
	 */
	constexpr size_t DefaultPasswordQuantity = 1;

	/**
	 * @brief Uppercase English alphabet: A-Z.
	 */
	constexpr std::string_view UpperCaseCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	/**
	 * @brief Lowercase English alphabet: a-z.
	 */
	constexpr std::string_view LowerCaseCharacters = "abcdefghijklmnopqrstuvwxyz";

	/**
	 * @brief Numeric digits: 0-9.
	 */
	constexpr std::string_view NumericCharacters = "0123456789";

	/**
	 * @brief Common special symbols for password strength.
	 *
	 * Includes punctuation and symbols typically allowed in passwords.
	 */
	constexpr std::string_view SpecialCharacters = "!@#$%^&*()-_=+[]{}|;:,.<>?/~`";

} // namespace pwd