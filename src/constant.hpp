#pragma once
#include <string_view>

namespace pwd {
    constexpr size_t MinimumPasswordLength = 10;
    constexpr size_t MaximumPasswordLength = 1'000'000;

    constexpr size_t RandomDefaultMinLength = 10;
    constexpr size_t RandomDefaultMaxLength = 100;

    constexpr size_t DefaultPasswordQuantity = 1;

    constexpr std::string_view UpperCaseCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    constexpr std::string_view LowerCaseCharacters = "abcdefghijklmnopqrstuvwxyz";
    constexpr std::string_view NumericCharacters = "0123456789";
    constexpr std::string_view SpecialCharacters = "!@#$%^&*()-_=+[]{}|;:,.<>?/~`";
} // namespace pwd