#pragma once

// Cross-platform standard includes & OS-specific adjustments
#include <sodium.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <optional>
#include <memory>
#include <iomanip>
#include <sstream>
#include <cstring>
#include <charconv>

#ifdef _WIN32
    #include <Windows.h>
    #pragma comment(lib, "libsodium.lib")
#elif defined(__unix__) || defined(__APPLE__)
    #include <unistd.h>
    // POSIX systems link libsodium via -lsodium
#endif

// Macro for modern attribute support
#ifndef [[nodiscard]]
    #define [[nodiscard]]
#endif