#include "generator.h"
#include "constant.h"
#include <sodium.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char* characters;
    size_t characterCount;
} CharacterPool;

static int compareCharacters(const void* first, const void* second) {
    return *(const char*)first - *(const char*)second;
}

static CharacterPool buildCharacterPool(const PasswordGenerationConfig* config) {
    CharacterPool pool = { NULL, 0 };

    size_t totalCapacity = 0;
    if (config->includeUppercase) totalCapacity += strlen(UPPER_CASE_CHARS);
    if (config->includeLowercase) totalCapacity += strlen(LOWER_CASE_CHARS);
    if (config->includeNumeric)   totalCapacity += strlen(NUMERIC_CHARS);
    if (config->includeSpecial)   totalCapacity += strlen(SPECIAL_CHARS);
    if (totalCapacity == 0) return pool;

    char* temporaryBuffer = malloc(totalCapacity + 1);
    if (!temporaryBuffer) return pool;
    temporaryBuffer[0] = '\0';

    // Using strcat is safe because we pre-allocated totalCapacity+1 bytes
    if (config->includeUppercase) strcat(temporaryBuffer, UPPER_CASE_CHARS);
    if (config->includeLowercase) strcat(temporaryBuffer, LOWER_CASE_CHARS);
    if (config->includeNumeric)   strcat(temporaryBuffer, NUMERIC_CHARS);
    if (config->includeSpecial)   strcat(temporaryBuffer, SPECIAL_CHARS);

    size_t bufferLength = strlen(temporaryBuffer);
    qsort(temporaryBuffer, bufferLength, 1, compareCharacters);

    char* uniqueBuffer = malloc(bufferLength + 1);
    if (!uniqueBuffer) {
        free(temporaryBuffer);
        return pool;
    }

    char* writePointer = uniqueBuffer;
    for (size_t position = 0; position < bufferLength; ++position) {
        if (position == 0 || temporaryBuffer[position] != temporaryBuffer[position - 1])
            *writePointer++ = temporaryBuffer[position];
    }
    *writePointer = '\0';

    free(temporaryBuffer);
    pool.characters = uniqueBuffer;
    pool.characterCount = (size_t)(writePointer - uniqueBuffer);
    return pool;
}

static char* generateSinglePassword(size_t passwordLength, const char* characterPool, size_t poolSize) {
    if (poolSize == 0) return NULL;
    char* password = malloc(passwordLength + 1);
    if (!password) return NULL;

    for (size_t characterIndex = 0; characterIndex < passwordLength; ++characterIndex) {
        uint32_t randomIndex = randombytes_uniform((uint32_t)poolSize);
        password[characterIndex] = characterPool[randomIndex];
    }
    password[passwordLength] = '\0';
    return password;
}

static void cleanupFailedGeneration(char** passwords, size_t successfullyGeneratedCount) {
    for (size_t index = 0; index < successfullyGeneratedCount; ++index) {
        if (passwords[index]) {
            sodium_memzero(passwords[index], strlen(passwords[index]));
            free(passwords[index]);
        }
    }
    free(passwords);
}

char** generatePasswords(const PasswordGenerationConfig* config, size_t* outPasswordCount) {
    static int sodiumInitialized = 0;
    if (!sodiumInitialized) {
        if (sodium_init() < 0) return NULL;
        sodiumInitialized = 1;
    }

    CharacterPool characterPool = buildCharacterPool(config);
    if (characterPool.characters == NULL) return NULL;

    size_t requiredQuantity = config->passwordQuantity;
    char** generatedPasswords = malloc(sizeof(char*) * requiredQuantity);
    if (!generatedPasswords) {
        free(characterPool.characters);
        return NULL;
    }

    for (size_t passwordIndex = 0; passwordIndex < requiredQuantity; ++passwordIndex) {
        size_t currentLength;
        if (config->hasExplicitLength) {
            currentLength = config->explicitPasswordLength;
        }
        else {
            size_t lengthRange = RANDOM_DEFAULT_MAX_LEN - RANDOM_DEFAULT_MIN_LEN + 1;
            currentLength = RANDOM_DEFAULT_MIN_LEN + randombytes_uniform((uint32_t)lengthRange);
        }

        generatedPasswords[passwordIndex] = generateSinglePassword(currentLength,
            characterPool.characters,
            characterPool.characterCount);
        if (!generatedPasswords[passwordIndex]) {
            cleanupFailedGeneration(generatedPasswords, passwordIndex);
            free(characterPool.characters);
            return NULL;
        }
    }

    free(characterPool.characters);
    *outPasswordCount = requiredQuantity;
    return generatedPasswords;
}

void freePasswords(char** passwords, size_t passwordCount) {
    if (!passwords) return;
    for (size_t index = 0; index < passwordCount; ++index) {
        if (passwords[index]) {
            sodium_memzero(passwords[index], strlen(passwords[index]));
            free(passwords[index]);
        }
    }
    free(passwords);
}