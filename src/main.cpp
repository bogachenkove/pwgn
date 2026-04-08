#include "config_parser.hpp"
#include "password_generator.hpp"
#include <iostream>
#include <fstream>
#include <memory>
#include <iomanip>     // std::setprecision, std::fixed
#include "constant.hpp"

int main(int argc, char* argv[]) {
    try {
        // 1. Парсинг и валидация
        pwd::PasswordConfig config = pwd::ConfigParser::parse(argc, argv);

        // 2. Инициализация генератора (RAII)
        pwd::PasswordGenerator generator;
        std::vector<std::string> passwords = generator.generate(config);

        // 3. Настройка потока вывода
        std::unique_ptr<std::ostream> out_stream;
        std::ofstream file_stream;
        std::ostream* output = &std::cout;

        if (config.output_file.has_value()) {
            file_stream.open(config.output_file.value(), std::ios::out | std::ios::trunc);
            if (!file_stream.is_open()) {
                std::cerr << "Error: Cannot open output file: " << config.output_file.value() << "\n";
                return EXIT_FAILURE;
            }
            out_stream = std::make_unique<std::ofstream>(std::move(file_stream));
            output = out_stream.get();
        }

        // 4. Расчёт размера алфавита для энтропии
        size_t charset_size = 0;
        if (config.use_upper)   charset_size += pwd::CHARSET_UPPER.length();
        if (config.use_lower)   charset_size += pwd::CHARSET_LOWER.length();
        if (config.use_digits)  charset_size += pwd::CHARSET_DIGITS.length();
        if (config.use_special) charset_size += pwd::CHARSET_SPECIAL.length();

        // 5. Вывод
        for (const auto& pwd_str : passwords) {
            *output << pwd_str;
            if (config.show_entropy) {
                double entropy = generator.calculate_entropy(config.length, charset_size);
                *output << " (Entropy: " << std::fixed << std::setprecision(2) << entropy << " bits)";
            }
            *output << "\n";
        }

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e) {
        std::cerr << "Fatal Error: " << e.what() << "\n";
        return EXIT_FAILURE;
    }
}