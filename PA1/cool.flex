 /*
  *  The scanner definition for COOL.
  */
%option noyywrap
 /*
  *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
  *  output, so headers and global definitions are placed here to be visible
  * to the code in the file.  Don't remove anything that was here initially
  */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1024
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST + 1]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */
/* 0 means outside of comments */
int comment_depth = 0;

/* 0 means currently no error for this string */
bool in_str_error = 0;
%}


/*
 * Define names for regular expressions here.
 */
/* Keywords */
DARROW          =>
CLASS           [cC][lL][aA][sS][sS]
ELSE            [eE][lL][sS][eE]
FI              [fF][iI]
IF              [iI][fF]
IN              [iI][nN]
INHERITS        [iI][nN][hH][eE][rR][iI][tT][sS]
ISVOID          [iI][sS][vV][oO][iI][dD]
LET             [lL][eE][tT]
LOOP            [lL][oO][oO][pP]
POOL            [pP][oO][oO][lL]
THEN            [tT][hH][eE][nN]
WHILE           [wW][hH][iI][lL][eE]
CASE            [cC][aA][sS][eE]
ESAC            [eE][sS][aA][cC]
NEW             [nN][eE][wW]
OF              [oO][fF]
NOT             [nN][oO][tT]
TRUE            t[rR][uU][eE]
FALSE           f[aA][lL][sS][eE]

/* Exclusive start states */
%x COMMENT
%x LINE_COMMENT
%x STRING

%%

 /* Single line comments */
"--" { BEGIN(LINE_COMMENT); }
<LINE_COMMENT>[^\n] {}
<LINE_COMMENT>\n {
    ++curr_lineno;
    BEGIN(INITIAL);
}

 /* Nested comments */
"(*" { ++comment_depth; BEGIN(COMMENT); }
<COMMENT>"(*" { ++comment_depth; }       
<COMMENT>[^\n] {}
<COMMENT>\n { ++curr_lineno; }
<COMMENT>"*)" {
    --comment_depth;
    if (comment_depth == 0) {
        BEGIN(INITIAL);
    }
}


 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  */
{DARROW}        { return (DARROW); }
{CLASS}         { return (CLASS); }
{ELSE}          { return (ELSE); }
{FI}            { return (FI); }
{IF}            { return (IF); }
{IN}            { return (IN); }
{INHERITS}      { return (INHERITS); }
{ISVOID}        { return (ISVOID); }
{LET}           { return (LET); }
{LOOP}          { return (LOOP); }
{POOL}          { return (POOL); }
{THEN}          { return (THEN); }
{WHILE}         { return (WHILE); }
{CASE}          { return (CASE); }
{ESAC}          { return (ESAC); }
{NEW}           { return (NEW); }
{OF}            { return (OF); }
{NOT}           { return (NOT); }
{TRUE}          { yylval.boolean = true; return (BOOL_CONST); }
{FALSE}         { yylval.boolean = false; return (BOOL_CONST); }

 /* Integers */
[0-9]+ {
    yylval.symbol = inttable.add_string(yytext);
    return (INT_CONST);
}
 /* Multi char special notations */
"<-"            { return (ASSIGN); }
"<="            { return (LE); }

 /* Single char special notations */
"{"             { return '{'; }
"}"             { return '}'; }
"("             { return '('; }
")"             { return ')'; }
","             { return ','; }
"."             { return '.'; }
";"             { return ';'; }
":"             { return ':'; }
"@"             { return '@'; }
"+"             { return '+'; }
"-"             { return '-'; }
"*"             { return '*'; }
"/"             { return '/'; }
"~"             { return '~'; }
"="             { return '='; }
"<"             { return '<'; }


 /* Type IDs */
[A-Z][a-zA-Z0-9_]* {
    yylval.symbol = idtable.add_string(yytext);
    return (TYPEID);
}

 /* Object IDs */
[a-z][a-zA-Z0-9_]* {
    yylval.symbol = idtable.add_string(yytext);
    return (OBJECTID);
}

 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  *
  */
 /* Start a string */
\" {
    BEGIN(STRING);  // Only case to start a string
    memset(string_buf, 0, sizeof(string_buf));
    string_buf_ptr = &string_buf[0];
    in_str_error = 0;
}

<STRING><<EOF>> {
    BEGIN(INITIAL);  // Case 1 to end a string
    if (!in_str_error) {
        /* No need to update in_str_error */
        yylval.error_msg = strdup("EOF in string constant");
        return (ERROR);
    }
    /* If another error is reported for this string
     * Then no need to do anything else but to end string
     */
}

<STRING>\0 {
    if (!in_str_error) {
        in_str_error = 1;
        yylval.error_msg = strdup("String contains null character.");
        return (ERROR);
    }
}

<STRING>\\b {
    *string_buf_ptr = '\b';
    ++string_buf_ptr;
}

<STRING>\\t {
    *string_buf_ptr = '\t';
    ++string_buf_ptr;
}

<STRING>\\n {
    *string_buf_ptr = '\n';
    ++string_buf_ptr;
}

<STRING>\\f {
    *string_buf_ptr = '\f';
    ++string_buf_ptr;
}

 /* A new line with escape */
<STRING>\\\n {
    ++curr_lineno;
    *string_buf_ptr = '\n';
    ++string_buf_ptr;
}

<STRING>\\[^btnf] {
    if ((string_buf_ptr - &string_buf[0]) >= 1024) {
        /* Only report this error when there is no other errors before */
        if (!in_str_error) {
            in_str_error = 1;
            yylval.error_msg = strdup("String constant too long");
            return (ERROR);
        }
    } else {
        *string_buf_ptr = yytext[1];
        ++string_buf_ptr;
    }
}

 /* A new line without escape */
<STRING>\n {
    ++curr_lineno;
    BEGIN(INITIAL);  // Case 2 to end a string

    if (!in_str_error) {
        in_str_error = 0;
        yylval.error_msg = strdup("Unterminated string constant");
        return (ERROR);
    }
}

<STRING>\" {
    BEGIN(INITIAL);  // Case 3 to end a string
    yylval.symbol = stringtable.add_string(&string_buf[0]);
    string_buf_ptr = &string_buf[0];
    if (in_str_error) {
        in_str_error = 0;
    } else {
        return (STR_CONST);
    }
}

 /* A normal character */
<STRING>. {
    if ((string_buf_ptr - &string_buf[0]) >= 1024) {
        if (!in_str_error) {
            in_str_error = 1;
            yylval.error_msg = strdup("String constant too long");
            return (ERROR);
        }
    } else {
        *string_buf_ptr = yytext[0];
        ++string_buf_ptr;
    }
}

 /* White space */
[ \f\r\t\v] { }
\n { ++curr_lineno; }

%%
