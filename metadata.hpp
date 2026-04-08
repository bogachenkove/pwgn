#pragma once
#include "constant.hpp"

namespace pwd {
    constexpr const char* PROGRAM_NAME = "pwgn";
    constexpr const char* VERSION = "0.0.0.1";

    // ������� raw-string ��� ��������� inline-���������� � ������������ � ����������
    constexpr const char* USAGE_STR =
        "Options:\n"
        "  -l, --length <N>       Password length [10-1000000]. Default: 32\n"
        "  -q, --quantity <N>     Number of passwords to generate. Default: 1\n"
        "  -o, --output <FILE>    Output file path. Default: stdout\n"
        "  -e, --entropy          Display entropy estimation per password\n"
        "  --no-upper             Exclude uppercase letters\n"
        "  --no-lower             Exclude lowercase letters\n"
        "  --no-digits            Exclude digits\n"
        "  --no-special           Exclude special characters\n"
        "  -h, --help             Show this help message\n";
}