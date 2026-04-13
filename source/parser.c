#include "parser.h"
#include "constant.h"
#include "metadata.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

static char* copyString(const char* source) {
    if (!source) return NULL;
    size_t length = strlen(source);
    char* destination = malloc(length + 1);
    if (destination) {
        strcpy(destination, source);
    }
    return destination;
}

void configInit(PasswordGenerationConfig* config) {
    config->explicitPasswordLength = 0;
    config->hasExplicitLength = false;
    config->passwordQuantity = DEFAULT_PASSWORD_QUANTITY;
    config->outputFilePath = NULL;
    config->includeUppercase = true;
    config->includeLowercase = true;
    config->includeNumeric = true;
    config->includeSpecial = true;
}

static void printHelpAndExit(void) {
    printf("%s", USAGE_INFORMATION);
    exit(EXIT_SUCCESS);
}

int parseCommandLineArguments(int argumentCount, char* argumentValues[], PasswordGenerationConfig* outConfig) {
    for (int currentIndex = 1; currentIndex < argumentCount; ++currentIndex) {
        char* currentArgument = argumentValues[currentIndex];

        if (strcmp(currentArgument, "-h") == 0 || strcmp(currentArgument, "--help") == 0) {
            printHelpAndExit();
        }
        else if (strcmp(currentArgument, "--no-upper") == 0) {
            outConfig->includeUppercase = false;
        }
        else if (strcmp(currentArgument, "--no-lower") == 0) {
            outConfig->includeLowercase = false;
        }
        else if (strcmp(currentArgument, "--no-digits") == 0) {
            outConfig->includeNumeric = false;
        }
        else if (strcmp(currentArgument, "--no-special") == 0) {
            outConfig->includeSpecial = false;
        }
        else if (strcmp(currentArgument, "-l") == 0 || strcmp(currentArgument, "--length") == 0) {
            if (++currentIndex >= argumentCount) return -1;
            char* endPointer;
            unsigned long parsedValue = strtoul(argumentValues[currentIndex], &endPointer, 10);
            if (*endPointer != '\0' || parsedValue < MIN_PASSWORD_LENGTH || parsedValue > MAX_PASSWORD_LENGTH)
                return -2;
            outConfig->explicitPasswordLength = (size_t)parsedValue;
            outConfig->hasExplicitLength = true;
        }
        else if (strcmp(currentArgument, "-q") == 0 || strcmp(currentArgument, "--quantity") == 0) {
            if (++currentIndex >= argumentCount) return -3;
            char* endPointer;
            unsigned long parsedValue = strtoul(argumentValues[currentIndex], &endPointer, 10);
            if (*endPointer != '\0' || parsedValue == 0) return -4;
            outConfig->passwordQuantity = (size_t)parsedValue;
        }
        else if (strcmp(currentArgument, "-o") == 0 || strcmp(currentArgument, "--output") == 0) {
            if (++currentIndex >= argumentCount) return -5;
            if (outConfig->outputFilePath) {
                free(outConfig->outputFilePath);
                outConfig->outputFilePath = NULL;
            }
            outConfig->outputFilePath = copyString(argumentValues[currentIndex]);
            if (!outConfig->outputFilePath) return -6;
        }
        else {
            return -7;
        }
    }

    if (!outConfig->includeUppercase && !outConfig->includeLowercase &&
        !outConfig->includeNumeric && !outConfig->includeSpecial)
        return -8;

    return 0;
}