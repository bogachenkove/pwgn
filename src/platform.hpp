#pragma once


#include <sodium.h>
#include <iostream>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <optional>
#include <memory>
#include <iomanip>
#include <charconv>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "libsodium.lib")
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#endif