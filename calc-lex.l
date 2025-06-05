%option noyywrap
%{
#include <stdlib.h>
/*#include (add yacc here)*/
%}

UNIT            zero|one|two|three|four|five|six|seven|eight|nine|first|second|third|fifth|eighth|ninth
TEEN            ten|eleven|twelve|thirteen|fourteen|fifteen|sixteen|seventeen|eighteen|nineteen|twelfth
TEN             twenty|thirty|forty|fifty|sixty|seventy|eighty|ninety|twentieth|thirtieth|fortieth|fiftieth|sixtieth|seventieth|eightieth|ninetieth
HUNDRED         hundred|thousand|million|billion
POINT           point
EXP             (to the power|power|to the)
FACT            factorial
INTEGER         ({UNIT}|{TEEN}|{TEN}|({TEN}{UNIT})|({UNIT}{HUNDRED})+({UNIT}|{TEEN}|{TEN}|({TEN}{UNIT}))?)
DOUBLE          {INTEGER}{POINT}{UNIT}*
EXPONENTIAL     {INTEGER}{EXP}{INTEGER}
FACTORIAL       {INTEGER}{FACT}

%%

[ \t\n]                 {/*skip blanks*/}
"and"|"by"|"of"|"th"    {/*ignore*/}

"zero"              {yylval = 0; return UNIT;}
"one"|"first"       {yylval = 1; return UNIT;}
"two"|"second"      {yylval = 2; return UNIT;}
"three"|"third"     {yylval = 3; return UNIT;}
"four"              {yylval = 4; return UNIT;}
"five"|"fifth"      {yylval = 5; return UNIT;}
"six"               {yylval = 6; return UNIT;}
"seven"             {yylval = 7; return UNIT;}
"eight"|"eighth"    {yylval = 8; return UNIT;}
"nine"|"ninth"      {yylval = 9; return UNIT;}

"ten"               {yylval = 10; return TEEN;}
"eleven"            {yylval = 11; return TEEN;}
"twelve"|"twelfth"  {yylval = 12; return TEEN;}
"thirteen"          {yylval = 13; return TEEN;}
"fourteen"          {yylval = 14; return TEEN;}
"fifteen"           {yylval = 15; return TEEN;}
"sixteen"           {yylval = 16; return TEEN;}
"seventeen"         {yylval = 17; return TEEN;}
"eighteen"          {yylval = 18; return TEEN;}
"nineteen"          {yylval = 19; return TEEN;}

"twenty"|"twentieth"    {yylval = 20; return TEN;}
"thirty"|"thirtieth"    {yylval = 30; return TEN;}
"forty"|"fortieth"      {yylval = 40; return TEN;}
"fifty"|"fiftieth"      {yylval = 50; return TEN;}
"sixty"|"sixtieth"      {yylval = 60; return TEN;}
"seventy"|"seventieth"  {yylval = 70; return TEN;}
"eighty"|"eightieth"    {yylval = 80; return TEN;}
"ninety"|"ninetieth"    {yylval = 90; return TEN;}

"hundred"       {yylval = 100; return HUNDRED;}
"thousand"      {yylval = 1000; return HUNDRED;}
"million"       {yylval = 1000000; return HUNDRED;}
"billion"       {yylval = 1000000000; return HUNDRED;}

"sum"|"plus"|"add"                  {yylval = '+'; return OPERATOR;}
"difference"|"minus"|"subtract"     {yylval = '-'; return OPERATOR;}
"multiplication"|"times"|"multiply" {yylval = '*'; return OPERATOR;}
"division"|"divide"                 {yylval = '/'; return OPERATOR;}

"open"[ ]*"parenthesis"             {yylval = '('; return PARENTHESIS;}
"close"[ ]*"parenthesis"            {yylval = ')'; return PARENTHESIS;}

"point"                             {return POINT;}

"to the"|"power"|"to the power"     {return EXP;}

"factorial"                         {return FACT;}
%%