#pragma once
#include <string_view>

namespace pwd {
    constexpr size_t MIN_PASSWORD_LENGTH = 10;
    constexpr size_t MAX_PASSWORD_LENGTH = 1'000'000;
    constexpr size_t DEFAULT_PASSWORD_LENGTH = 32;
    constexpr size_t DEFAULT_QUANTITY = 1;

    // Используем string_view для нулевых накладных расходов и совместимости
    constexpr std::string_view CHARSET_UPPER = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    constexpr std::string_view CHARSET_LOWER = "abcdefghijklmnopqrstuvwxyz";
    constexpr std::string_view CHARSET_DIGITS = "0123456789";
    constexpr std::string_view CHARSET_SPECIAL = "!@#$%^&*()-_=+[]{}|;:,.<>?/~`";
}