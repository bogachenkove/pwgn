#include "password_generator.hpp"
#include <sodium.h>      // libsodium CSPRNG & Init
#include <cmath>         // std::log2
#include <algorithm>     // std::sort, std::unique
#include <stdexcept>     // std::runtime_error
#include "constant.hpp"

namespace pwd {
    PasswordGenerator::PasswordGenerator() {
        // Инициализация libsodium (потокобезопасна после успешного вызова)
        if (sodium_init() < 0) {
            throw std::runtime_error("Failed to initialize libsodium cryptographic library");
        }
    }

    std::string PasswordGenerator::build_charset(const PasswordConfig& config) const {
        std::string charset;
        charset.reserve(100);
        if (config.use_upper)   charset.append(CHARSET_UPPER.begin(), CHARSET_UPPER.end());
        if (config.use_lower)   charset.append(CHARSET_LOWER.begin(), CHARSET_LOWER.end());
        if (config.use_digits)  charset.append(CHARSET_DIGITS.begin(), CHARSET_DIGITS.end());
        if (config.use_special) charset.append(CHARSET_SPECIAL.begin(), CHARSET_SPECIAL.end());

        // Удаляем дубликаты для точного расчёта энтропии
        std::sort(charset.begin(), charset.end());
        charset.erase(std::unique(charset.begin(), charset.end()), charset.end());
        return charset;
    }

    std::string PasswordGenerator::generate_single(size_t length, const std::string& charset) const {
        if (charset.empty()) throw std::runtime_error("Character pool is empty");

        std::string password;
        password.reserve(length);
        const uint32_t pool_size = static_cast<uint32_t>(charset.size());

        // randombytes_uniform использует отбрасывающую выборку (rejection sampling)
        // для полного устранения modulo bias. Потокобезопасно.
        for (size_t i = 0; i < length; ++i) {
            password += charset[randombytes_uniform(pool_size)];
        }
        return password;
    }

    double PasswordGenerator::calculate_entropy(size_t length, size_t charset_size) const {
        if (charset_size <= 1) return 0.0;
        // Формула Шеннона: H = L * log2(N)
        return static_cast<double>(length) * std::log2(static_cast<double>(charset_size));
    }

    std::vector<std::string> PasswordGenerator::generate(const PasswordConfig& config) {
        std::vector<std::string> passwords;
        passwords.reserve(config.quantity);
        std::string charset = build_charset(config);
        for (size_t i = 0; i < config.quantity; ++i) {
            passwords.emplace_back(generate_single(config.length, charset));
        }
        return passwords;
    }
}