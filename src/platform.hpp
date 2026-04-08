/**
 * @file platform.hpp
 * @brief Platform-specific includes and library linking configuration.
 *
 * Aggregates all standard library includes used across the project
 * and handles conditional inclusion of OS headers.
 * On Windows, links against libsodium.lib; on Unix-like systems,
 * includes unistd.h for POSIX API access.
 */

#pragma once

 // Cryptographic library (randombytes_uniform, etc.)
#include <sodium.h>

// Standard I/O and file handling
#include <iostream>
#include <fstream>

// String and text processing
#include <string>
#include <string_view>
#include <iomanip>
#include <charconv>
#include <cstring>

// Containers and algorithms
#include <vector>
#include <algorithm>
#include <cmath>

// Error handling and memory management
#include <stdexcept>
#include <optional>
#include <memory>

// Platform-specific adaptations
#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "libsodium.lib")
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif