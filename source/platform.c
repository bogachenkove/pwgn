#include "platform.h"
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <stdlib.h>
#endif

void initPlatformEnvironment(void) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    setlocale(LC_ALL, "");
}