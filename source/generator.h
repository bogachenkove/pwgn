#pragma once
#include "parser.h"
#include <stddef.h>

char** generatePasswords(const PasswordGenerationConfig* config, size_t* outPasswordCount);
void freePasswords(char** passwords, size_t passwordCount);