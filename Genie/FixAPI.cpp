#include "FixAPI.h"

/*extern "C" void initApiTables( void );
extern "C" int isSuspectExpliciteAPI( const char * const szApiToCheck );
extern "C" const char * const fixBadApi( const char * const szApiToCheck, int *toCheck );
*/

/*
This hard-coded tables are really a temporary hack.
It would be nice to load this tables from an external file
(maybe with actions, conditions, warning messages)
But this will do for now.
*/

typedef struct ApiEntry
{
    const char* szToFix;
    const char* szFixed;
} ApiEntry;

/*
Table of stuff to replace, together with the replacement
*/
ApiEntry BadAPIs[] =
{
    // Data Type Mappings
    { "CHAR",            "TCHAR"},
    { "char",            "TCHAR"},
    { "LPSTR",           "LPTSTR"},
    { "LPCSTR",          "LPCTSTR"},

    // Constant and Global Variable Mappings
    { "EOF",             "_TEOF"},
    { "_environ",        "_tenviron"},
    { "_finddata_t",     "_tfinddata_t"},
    { "TRACE0",          "TRACE" },
    { "TRACE2",          "TRACE" },

    // Generic-Text                                        
    { "fgetc",           "_fgettc"},
    { "fgetchar",    "_fgettchar"},
    { "fgets",        "_fgetts"},
    { "fputc",           "_fputtc"},
    { "fputchar",        "_fputtchar"},
    { "fputs",           "_fputts"},
    { "fprintf",         "_ftprintf"},
    { "fscanf",          "_ftscanf"},
    { "getc",            "_gettc"},
    { "getchar",         "_gettchar"},
    { "gets",            "_getts"},
    { "isalnum",         "_istalnum"},
    { "isalpha",         "_istalpha"},
    { "__isascii",       "_istascii"},
    { "iscntrl",         "_istcntrl"},
    { "isdigit",         "_istdigit"},
    { "isgraph",         "_istgraph"},
    { "islower",         "_istlower"},
    { "isprint",         "_istprint"},
    { "ispunct",         "_istpunct"},
    { "isspace",         "_istspace"},
    { "isupper",         "_istupper"},
    { "isxdigit",        "_istxdigit"},
    { "itoa",            "_itot"},
    { "_itoa",           "_itot"},
    { "ltoa",            "_ltot"},
    { "_ltoa",           "_ltot"},
    { "_i64toa",         "_i64tot"},
    { "ultoa",           "_ultot"},
    { "_ultoa",          "_ultot"},
    { "_ui64toa",        "_ui64tot"},
    { "putc",            "_puttc"},
    { "putchar",         "_puttchar"},
    { "puts",            "_putts"},
    { "_snprintf",       "_sntprintf"},
    { "sprintf",         "_stprintf"},
    { "sscanf",          "_stscanf"},
    { "_access",         "_taccess"},
    { "asctime",         "_tasctime"},
    { "strcat",          "_tcscat"},
    { "strchr",          "_tcschr"},
    { "strlen",          "_tcslen"},
    { "strcmp",          "_tcscmp"},
    { "strcoll",         "_tcscoll"},
    { "strcpy",          "_tcscpy"},
    { "strcspn",         "_tcscspn"},
    { "_strdec",         "_tcsdec"},
    { "_strdup",         "_tcsdup"},
    { "strftime",        "_tcsftime"},
    { "_stricmp",        "_tcsicmp"},
    { "_stricoll",       "_tcsicoll"},
    { "_strinc",         "_tcsinc"},
    { "_strlwr",         "_tcslwr"},
    { "_strncnt",        "_tcsnbcnt"},
    { "_strncnt",        "_tcsnccnt"},
    { "strncat",         "_tcsnccat"},
    { "strncmp",         "_tcsnccmp"},
    { "strncpy",         "_tcsnccpy"},
    { "_strnicmp",       "_tcsncicmp"},
    { "_strnset",        "_tcsncset"},
    { "_strnextc",       "_tcsnextc"},
    { "_strnicmp",       "_tcsnicmp"},
    { "_strnicoll",      "_tcsnicoll"},
    { "_strninc",        "_tcsninc"},
    { "_strnset",        "_tcsnset"},
    { "strpbrk",         "_tcspbrk"},
    { "_strspnp",        "_tcsspnp"},
    { "strrchr",         "_tcsrchr"},
    { "_strrev",         "_tcsrev"},
    { "_strset",         "_tcsset"},
    { "strspn",          "_tcsspn"},
    { "strstr",          "_tcsstr"},
    { "strtod",          "_tcstod"},
    { "strtok",          "_tcstok"},
    { "strtol",          "_tcstol"},
    { "strtoul",         "_tcstoul"},
    { "strupr",          "_tcsupr"},
    { "_strupr",         "_tcsupr"},
    { "strxfrm",         "_tcsxfrm"},
    { "ctime",           "_tctime"},
    { "_execl",          "_texecl"},
    { "_execle",         "_texecle"},
    { "_execlp",         "_texeclp"},
    { "_execlpe",        "_texeclpe"},
    { "_execv",          "_texecv"},
    { "_execve",         "_texecve"},
    { "_execvp",         "_texecvp"},
    { "_execvpe",        "_texecvpe"},
    { "_fdopen",         "_tfdopen"},
    { "_findfirst",      "_tfindfirst"},
    { "_findnext",       "_tfindnext"},
    { "fopen",           "_tfopen"},
    { "freopen",         "_tfreopen"},
    { "fdopen",          "_tfdopen"},
    { "_fsopen",         "_tfsopen"},
    { "_fullpath",       "_tfullpath"},
    { "_getcwd",         "_tgetcwd"},
    { "getenv",          "_tgetenv"},
    // "main",            "_tmain"},
    { "_makepath",       "_tmakepath"},
    { "_mkdir",          "_tmkdir"},
    { "_mktemp",         "_tmktemp"},
    { "perror",          "_tperror"},
    { "_open",           "_topen"},
    { "tolower",         "_totlower"},
    { "toupper",         "_totupper"},
    { "_popen",          "_tpopen"},
    { "printf",          "_tprintf"},
    //{ "remove",          "_tremove"},
    { "rename",          "_trename"},
    { "rmdir",           "_trmdir"},
    { "_rmdir",          "_trmdir"},
    { "mkdir",           "_tmkdir"},
    { "_mkdir",          "_tmkdir"},
    { "chdir",           "_tchdir"},
    { "_chdir",          "_tchdir"},
    { "creat",           "_tcreat"},
    { "_creat",          "_tcreat"},
    { "getcwd",          "_tgetcwd"},
    { "_getcwd",         "_tgetcwd"},
    { "chmod",           "_tchmod"},
    { "_chmod",          "_tchmod"},
    { "access",          "_taccess"},
    { "_access",         "_taccess"},
    { "_searchenv",      "_tsearchenv"},
    { "scanf",           "_tscanf"},
    { "setlocale",       "_tsetlocale"},
    { "_sopen",          "_tsopen"},
    { "_spawnl",         "_tspawnl"},
    { "_spawnle",        "_tspawnle"},
    { "_spawnlp",        "_tspawnlp"},
    { "_spawnlpe",       "_tspawnlpe"},
    { "_spawnv",         "_tspawnv"},
    { "_spawnve",        "_tspawnve"},
    { "_spawnvp",        "_tspawnvp"},
    { "_spawnvpe",       "_tspawnvpe"},
    { "_splitpath",      "_tsplitpath"},
    { "_stat",           "_tstat"},
    { "_strdate",        "_tstrdate"},
    { "_strtime",        "_tstrtime"},
    { "system",          "_tsystem"},
    { "_tempnam",        "_ttempnam"},
    { "tmpnam",          "_ttmpnam"},
    { "atoi",            "_ttoi"},
    { "atoi64",          "_ttoi64"},
    { "atol",            "_ttol"},
    { "strtoi64",        "_tcstoi64"},
    { "_strtoi64",       "_tcstoi64"},
    { "_utime",          "_tutime"},
    { "WinMain",         "_tWinMain"},
    { "ungetc",          "_ungettc"},
    { "vfprintf",        "_vftprintf"},
    { "vsnprintf",       "_vsntprintf"},
    { "_vsnprintf",      "_vsntprintf"},
    { "vsprintf",        "_vstprintf"},
    { "vprintf",         "_vtprintf"},
    { "strnicmp",        "_tcsnicmp"},
    { "stricmp",         "_tcsicmp"},
    { "_putenv",         "_tputenv"}
};
#define BADAPI_COUNT	(sizeof(BadAPIs)/sizeof(BadAPIs[0]))

/*
Table of stuff to that should give warnings
*/
char* WarnAPIs[] =
{
    "MultiByteToWideChar",
    "WideCharToMultiByte",
    "GetProcAddress",
    "sizeof"
};
#define WARNAPI_COUNT	(sizeof(WarnAPIs)/sizeof(WarnAPIs[0]))


int compareApiEntry(const void* arg1, const void* arg2)
{
    ApiEntry* ae1 = (ApiEntry*)arg1;
    ApiEntry* ae2 = (ApiEntry*)arg2;
    return strcmp(ae1->szToFix, ae2->szToFix);
}

int compareStrings(const void* arg1, const void* arg2)
{
    return strcmp(*(const char**)arg1, *(const char**)arg2);
}

void initApiTables(void)
{
    qsort(BadAPIs, BADAPI_COUNT, sizeof(BadAPIs[0]), compareApiEntry);
    qsort(WarnAPIs, WARNAPI_COUNT, sizeof(WarnAPIs[0]), compareStrings);
}

/*
Checks if something looks like an explicite wide or ansi call.
Examples: CharNextA or CharNextW
Less reliable than a table, but a does not need updating, like a table

It detects both stand-alone or in string:
   char *pApiName = CharNextA( someString );
and
   char *pApiName = "CharNextA";
(the second one might be used with GetProcAddress)
*/
int isSuspectExpliciteAPI(const char* const szApiToCheck)
{
    int nLen, nLower;
    nLen = nLower = 0;

    while (szApiToCheck[nLen])
    {
        if (isspace(szApiToCheck[nLen]))
        {
            return 0;
        }

        if (islower(szApiToCheck[nLen]))
        {
            nLower++;
        }
        nLen++;
    }

    // If I have found no lowercase, it is probably not an API.
    // This might miss some cases, but otherwise I get warnings
    // about all kind of flags, all uppercase (SW_SHOW, etc.)
    if (!nLower)
    {
        return 0;
    }

    if (nLen && szApiToCheck[nLen - 1] == '"')
    {
        nLen--;
    }

    if (nLen)
    {
        if (szApiToCheck[nLen - 1] == 'W')
        {
            return 1;
        }

        if (szApiToCheck[nLen - 1] == 'A')
        {
            return 1;
        }
    }

    return 0;
}

const char* const fixBadApi(const char* const szApiToCheck, int* toCheck)
{
    ApiEntry aeToFind = { szApiToCheck, NULL };
    ApiEntry* aeResult;

    if (bsearch(&szApiToCheck, WarnAPIs, WARNAPI_COUNT, sizeof(WarnAPIs[0]), compareStrings))
    {
        *toCheck = 1;
    }
    else
    {
        *toCheck = 0;
    }

    if (isSuspectExpliciteAPI(szApiToCheck))
    {
        *toCheck = 1;
    }

    aeResult = (ApiEntry*)bsearch(&aeToFind, BadAPIs, BADAPI_COUNT, sizeof(BadAPIs[0]), compareApiEntry);

    return aeResult ? aeResult->szFixed : szApiToCheck;
}

/*
More stuff to do:
 - load API tables (with messages and actions) from external file
 - improve sizeof handling (_countof ?)
 - nice integration with VS (errors/warnings are already in compatible format)
 - macros (__DATE__,__FILE__,__TIMESTAMP__,__TIME__)
 ? preproc (include,if,else,elsif,endif,error,import)
 ? pragmas (code_seg,data_seg,comment,data_seg,error,message)
 ? incremnt (++), decrement (--)
 ? work on memory-buffers instead of files
 ? might need to bring bison into this, if I want to get smarter
*/
