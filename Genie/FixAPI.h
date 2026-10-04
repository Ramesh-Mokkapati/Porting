
#ifndef __FIXAPI_H__
#define __FIXAPI_H__
#pragma once

#include <stdio.h>
#include <string.h>
#include <search.h>
#include <ctype.h>

extern void initApiTables( void );
extern int isSuspectExpliciteAPI( const char * const szApiToCheck );
extern const char * const fixBadApi( const char * const szApiToCheck, int *toCheck );

#endif // __FIXAPI_H__
