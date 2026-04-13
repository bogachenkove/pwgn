#include "parser.h"
#include "generator.h"
#include "platform.h"
#include "metadata.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(int argumentCount, char* argumentValues[]) {
    initPlatformEnvironment();

    PasswordGenerationConfig generationConfig;
    configInit(&generationConfig);

    int parseResult = parseCommandLineArguments(argumentCount, argumentValues, &generationConfig);
    if (parseResult != 0) {
        fprintf(stderr, "Error parsing arguments (code %d). Use --help for usage.\n", parseResult);
        return EXIT_FAILURE;
    }

    size_t generatedPasswordCount = 0;
    char** passwords = generatePasswords(&generationConfig, &generatedPasswordCount);
    if (!passwords) {
        fprintf(stderr, "Failed to generate passwords.\n");
        if (generationConfig.outputFilePath) free(generationConfig.outputFilePath);
        return EXIT_FAILURE;
    }

    FILE* outputStream = stdout;
    int needToCloseFile = 0;
    if (generationConfig.outputFilePath) {
        outputStream = fopen(generationConfig.outputFilePath, "w");
        if (!outputStream) {
            fprintf(stderr, "Cannot open output file: %s (error %s)\n",
                generationConfig.outputFilePath, strerror(errno));
            freePasswords(passwords, generatedPasswordCount);
            free(generationConfig.outputFilePath);
            return EXIT_FAILURE;
        }
        needToCloseFile = 1;
    }

    int shouldEnumerate = (generatedPasswordCount > 1);
    for (size_t passwordIndex = 0; passwordIndex < generatedPasswordCount; ++passwordIndex) {
        if (shouldEnumerate) {
            fprintf(outputStream, "%zu. ", passwordIndex + 1);
        }
        fprintf(outputStream, "%s", passwords[passwordIndex]);
        if (needToCloseFile) {
            fprintf(outputStream, "\r\n");
        }
        else {
            fprintf(outputStream, "\n");
        }
    }

    if (needToCloseFile) fclose(outputStream);
    if (generationConfig.outputFilePath) free(generationConfig.outputFilePath);
    freePasswords(passwords, generatedPasswordCount);
    return EXIT_SUCCESS;
}