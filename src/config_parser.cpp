#include "config_parser.hpp"
#include "metadata.hpp"
#include <iostream>
#include <cstdlib>
#include <charconv>
#include <cstring>

namespace pwd {
    PasswordConfig ConfigParser::parse(int argc, char* argv[]) {
        PasswordConfig config;

        for (int i = 1; i < argc; ++i) {
            std::string arg = argv[i];
            if (arg == "-h" || arg == "--help") {
                std::cout << USAGE_STR;
                std::exit(EXIT_SUCCESS);
            }
            if (arg == "-e" || arg == "--entropy") { config.show_entropy = true; }
            else if (arg == "--no-upper") { config.use_upper = false; }
            else if (arg == "--no-lower") { config.use_lower = false; }
            else if (arg == "--no-digits") { config.use_digits = false; }
            else if (arg == "--no-special") { config.use_special = false; }
            else if (arg == "-l" || arg == "--length") {
                if (++i >= argc) { std::cerr << "Error: Missing value for --length\n"; std::exit(EXIT_FAILURE); }
                auto res = std::from_chars(argv[i], argv[i] + std::strlen(argv[i]), config.length);
                if (res.ec != std::errc()) { std::cerr << "Error: Invalid number for --length\n"; std::exit(EXIT_FAILURE); }
                if (config.length < MIN_PASSWORD_LENGTH || config.length > MAX_PASSWORD_LENGTH) {
                    std::cerr << "Error: Length must be between " << MIN_PASSWORD_LENGTH << " and " << MAX_PASSWORD_LENGTH << "\n";
                    std::exit(EXIT_FAILURE);
                }
            }
            else if (arg == "-q" || arg == "--quantity") {
                if (++i >= argc) { std::cerr << "Error: Missing value for --quantity\n"; std::exit(EXIT_FAILURE); }
                auto res = std::from_chars(argv[i], argv[i] + std::strlen(argv[i]), config.quantity);
                if (res.ec != std::errc() || config.quantity == 0) { std::cerr << "Error: Invalid quantity\n"; std::exit(EXIT_FAILURE); }
            }
            else if (arg == "-o" || arg == "--output") {
                if (++i >= argc) { std::cerr << "Error: Missing value for --output\n"; std::exit(EXIT_FAILURE); }
                config.output_file = argv[i];
            }
            else {
                std::cerr << "Error: Unknown option: " << arg << "\n";
                std::exit(EXIT_FAILURE);
            }
        }

        if (!config.use_upper && !config.use_lower && !config.use_digits && !config.use_special) {
            std::cerr << "Error: At least one character set must be enabled\n";
            std::exit(EXIT_FAILURE);
        }
        return config;
    }
}