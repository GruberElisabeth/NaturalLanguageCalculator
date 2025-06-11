%option noyywrap
%{
#include <stdlib.h>
#include <stdbool.h>
#include "calc-parser.tab.h"

char* remove_plural(const char* str) {
    if (strcmp(str, "feet") == 0) {
        return strdup("foot");
    }
    if (strcmp(str, "inches") == 0) {
        return strdup("inch");
    }
    size_t len = strlen(str);
    char* copy = strdup(str);
    if (len > 1 && copy[len - 1] == 's') {
        copy[len - 1] = '\0';
    }
    return copy;
}

%}

UNIT            zero|one|two|three|four|five|six|seven|eight|nine|first|second|third|fifth|eighth|ninth
TEEN            ten|eleven|twelve|thirteen|fourteen|fifteen|sixteen|seventeen|eighteen|nineteen|twelfth
TEN             twenty|thirty|forty|fifty|sixty|seventy|eighty|ninety|twentieth|thirtieth|fortieth|fiftieth|sixtieth|seventieth|eightieth|ninetieth
HUNDRED         hundred|thousand|million|billion
POINT           point
EXP             (to the power|power|to the)
FACT            factorial
NEG             negative
INTEGER         ({UNIT}|{TEEN}|{TEN}|({TEN}{UNIT})|({UNIT}{HUNDRED})+({UNIT}|{TEEN}|{TEN}|({TEN}{UNIT}))?)
DOUBLE          {INTEGER}{POINT}{UNIT}+
NUM             ({INTEGER} | {DOUBLE})
EXPONENTIAL     {INTEGER}{EXP}{INTEGER}
FACTORIAL       {INTEGER}{FACT}
NEGATION        {NEG}{INTEGER}
OPERATOR        plus|minus|times|divide by
LPAREN          open[ \t]*parenthesis
RPAREN          close[ \t]*parenthesis
RELATION        greater|smaller|equal|equals

WHILE           while
IF              if  
ELSE            else

WEIGHT          ((kilo|milli)?gram|pound|ton)s?
TIME            (day|hour|minute|second|millisecond)s?
SPACE           ((kilo|centi|milli)?meter|mile)s?|foot|feet|inch(es)?
VOLUME          ((milli)?liter)s?

INT             integer
DOU             double

DIGIT           [0-9]
LETTER          [a-zA-Z]*
BOOL_ID         {LETTER}({LETTER}|{DIGIT})*[B]+
ID              {LETTER}({LETTER}|{DIGIT})*

BOOLEAN         (true|false)


%%

[ \t]                          {/*skip blanks*/}
"and"|"by"|"of"|"than"|"from"|"th"'-'  {/*ignore*/}
\-                 {/*ignore*/}

"zero"              {yylval.value = 0; return UNIT;}
"one"|"first"       {yylval.value = 1; return UNIT;}
"two"               {yylval.value = 2; return UNIT;}
"three"|"third"     {yylval.value = 3; return UNIT;}
"four"              {yylval.value = 4; return UNIT;}
"five"|"fifth"      {yylval.value = 5; return UNIT;}
"six"               {yylval.value = 6; return UNIT;}
"seven"             {yylval.value = 7; return UNIT;}
"eight"|"eighth"    {yylval.value = 8; return UNIT;}
"nine"|"ninth"      {yylval.value = 9; return UNIT;}

"ten"               {yylval.value = 10; return TEEN;}
"eleven"            {yylval.value = 11; return TEEN;}
"twelve"|"twelfth"  {yylval.value = 12; return TEEN;}
"thirteen"          {yylval.value = 13; return TEEN;}
"fourteen"          {yylval.value = 14; return TEEN;}
"fifteen"           {yylval.value = 15; return TEEN;}
"sixteen"           {yylval.value = 16; return TEEN;}
"seventeen"         {yylval.value = 17; return TEEN;}
"eighteen"          {yylval.value = 18; return TEEN;}
"nineteen"          {yylval.value = 19; return TEEN;}

"twenty"|"twentieth"    {yylval.value = 20; return TEN;}
"thirty"|"thirtieth"    {yylval.value = 30; return TEN;}
"forty"|"fortieth"      {yylval.value = 40; return TEN;}
"fifty"|"fiftieth"      {yylval.value = 50; return TEN;}
"sixty"|"sixtieth"      {yylval.value = 60; return TEN;}
"seventy"|"seventieth"  {yylval.value = 70; return TEN;}
"eighty"|"eightieth"    {yylval.value = 80; return TEN;}
"ninety"|"ninetieth"    {yylval.value = 90; return TEN;}

"hundred"       {yylval.value = 100; return HUNDRED;}
"thousand"      {yylval.value = 1000; return HUNDRED;}
"million"       {yylval.value = 1000000; return HUNDRED;}
"billion"       {yylval.value = 1000000000; return HUNDRED;}

"plus"              { yylval.op = '+'; return PLUS; }
"minus"             { yylval.op = '-';  return MINUS; }
"times"             { yylval.op = '*'; return TIMES; }
"divided by"            { yylval.op = '/'; return DIVIDE; }

"open"[ ]*"parenthesis"             {yylval.op = '('; return LPAREN;}
"close"[ ]*"parenthesis"            {yylval.op = ')'; return RPAREN;}

"point"                             {return POINT;}

"to the"|"power"|"to the power"     {return EXP;}

"factorial"                         {return FACT;}

"negative"                          {yylval.op = '-'; return NEG;}

"while"                             {return WHILE;}
"if"                                {return IF;}
"else"                              {return ELSE;}

"true"                              {yylval.value = true; return BOOLEAN;}
"false"                             {yylval.value = false; return BOOLEAN;}

"integer"                           {return INT;}
"double"                            {return DOU;}
"boolean"                           {return BOOL;}

"greater equal"|"greater equals"        {yylval.lexeme = strdup(">="); return RELATION;}
"smaller equal"|"smaller equals"        {yylval.lexeme = strdup("<="); return RELATION;}
"different"|"not equal"|"not equals"    {yylval.lexeme = strdup("!="); return RELATION;}
"greater"                               {yylval.lexeme = strdup(">"); return RELATION;}
"equal"|"equals"                        {yylval.lexeme = strdup("="); return RELATION;}
"smaller"                               {yylval.lexeme = strdup("<"); return RELATION;}

"is"                                 {return ASSIGN;}

{SPACE} {
    yylval.quantity.unit = remove_plural(yytext);
    yylval.quantity.type = strdup("SPACE");
    yylval.quantity.value = 1;
    yylval.quantity.dimension = 1;
    return MEASURE;
}

{WEIGHT} {
    yylval.quantity.unit = remove_plural(yytext);
    yylval.quantity.type = strdup("WEIGHT");
    yylval.quantity.value = 1;
    yylval.quantity.dimension = 1;
    return MEASURE;
}

{TIME} {
    yylval.quantity.unit = remove_plural(yytext);
    yylval.quantity.type = strdup("TIME");
    yylval.quantity.value = 1;
    yylval.quantity.dimension = 1;
    return MEASURE;
}

{VOLUME} {
    yylval.quantity.unit = remove_plural(yytext);
    yylval.quantity.type = strdup("VOLUME");
    yylval.quantity.value = 1;
    yylval.quantity.dimension = 1;
    return MEASURE;
}

{BOOL_ID}                           {yylval.lexeme = strdup(yytext); return BOOL_ID;}
{ID}                                {yylval.lexeme = strdup(yytext); return ID;}

\n      {return '\n';}

%%
