#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    size_t explicitPasswordLength;   // 0 = not set
    bool hasExplicitLength;
    size_t passwordQuantity;
    char* outputFilePath;            // NULL = stdout
    bool includeUppercase;
    bool includeLowercase;
    bool includeNumeric;
    bool includeSpecial;
} PasswordGenerationConfig;

void configInit(PasswordGenerationConfig* config);
int parseCommandLineArguments(int argumentCount, char* argumentValues[], PasswordGenerationConfig* outConfig);