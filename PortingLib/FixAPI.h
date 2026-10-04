#pragma once

#include <stdio.h>
#include <string.h>
#include <search.h>
#include <ctype.h>

extern void initApiTables(void);
extern int isSuspectExpliciteAPI(const char* const szApiToCheck);
extern const char* const fixBadApi(const char* const szApiToCheck, int* toCheck);
extern const char* const fixBadApiForMode(const char* const szApiToCheck, int* toCheck, int mode);

enum ApiTransformMode
{
    ApiTransformUnicode = 0,
    ApiTransformX64 = 1
};
