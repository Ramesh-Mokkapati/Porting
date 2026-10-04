

%{
#include <string.h>
#include <io.h>
#include "FixAPI.h"

#ifdef __cplusplus
	#define my_yy_input yyinput
#else
	#define my_yy_input input
#endif

const char *szFileIn = "<stdin>";
const char *szTextGen = "_T(%s)";

%}

ID       [_A-Za-z][_A-Za-z0-9]*
SPACE    [ \t\n\r\v\f]
XNUMBER   0x[0-9A-Fa-f]+
NUMBER   [0-9]+
STRING   "\""(\\.|\\\n|\\\r|[^\\"])*"\""
CHAR     "'"(\\.|\\\n|\\\r|[^\\'])"'"
GTEXT	{STRING}|{CHAR}

%x CMTSTATE

%%

"#"{SPACE}*"include"{SPACE}+"<".*">"	|
"#"{SPACE}*"include"{SPACE}+{STRING}	|
"extern"{SPACE}+"\"C\""		            ECHO;

{GTEXT}	{
	if( isSuspectExpliciteAPI( yytext ) )
		fprintf( stderr, "%s(%d) : warning ToUnicode: check if explicite A/W API call: %s\n", szFileIn, yylineno, yytext );
	fprintf( yyout, szTextGen, yytext );
}

L({GTEXT})                                        |
(_T|TEXT){SPACE}*\({SPACE}*{GTEXT}{SPACE}*\)	  ECHO;

{ID}	{
	int toCheck;
	fprintf( yyout, "%s", fixBadApi( yytext, &toCheck ) );
	if( toCheck )
		fprintf( stderr, "%s(%d) : warning ToUnicode: check API usage : %s\n", szFileIn, yylineno, yytext );
}

{NUMBER} ECHO;
{XNUMBER} ECHO;

{SPACE} ECHO;

"/*" {
	ECHO;
	BEGIN CMTSTATE;
}
<CMTSTATE>.    |
<CMTSTATE>\n   ECHO;

<CMTSTATE>"*/" {
	ECHO;
	BEGIN INITIAL;
}

"//".*"\n"	ECHO; /* line comment */

.	        ECHO;

%%


int yywrap( void ) {
	return 1;
}

int flexmain(char * inputfile, char * outputfile)
{
	szTextGen = "_T(%s)";
	

	initApiTables();

	szFileIn = inputfile;
	yyin = fopen( szFileIn, "r" );

	yyout = fopen( outputfile, "w" );

	yylex();

	fclose( yyout );
	fclose( yyin );

	return 0;
}
