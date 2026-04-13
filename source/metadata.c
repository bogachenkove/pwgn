#include "metadata.h"

const char* const USAGE_INFORMATION =
"Options:\n"
"  -l, --length <N>       Password length [10-1000000]. If omitted, length is randomized per password [10-100]\n"
"  -q, --quantity <N>     Number of passwords to generate. Default: 1\n"
"  -o, --output <FILE>    Output file path. Default: stdout\n"
"  --no-upper             Exclude uppercase letters\n"
"  --no-lower             Exclude lowercase letters\n"
"  --no-digits            Exclude digits\n"
"  --no-special           Exclude special characters\n"
"  -h, --help             Show this help message\n";