#ifndef REGKEY_H
#define REGKEY_H

#include <windows.h>
#include <qdebug.h>

#define MAX_VALUE_NAME 256
#define MAX_PATH 256

VOID QueryKey(HKEY hKey);

#endif // REGKEY_H
