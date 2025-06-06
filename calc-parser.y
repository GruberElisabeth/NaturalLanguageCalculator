%{
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

int yylex(void);
%}


%union {
        double value;
        char op;
       }

%token <value> UNIT TEEN TEN HUNDRED INTEGER DOUBLE NUM
%token <op> POINT EXP FACT NEG OPERATOR PARENTHESIS

%type <value> expr
%type <value> line

%start line

%%
line  : expr '\n'      {$$ = $1; printf("Result: %f\n", $$); exit(0);}
      ;
expr  : NUM {}
      | expr OPERATOR expr  { 
            switch($2) {
                case '+': $$ = $1 + $3; break;
                case '-': $$ = $1 - $3; break;
                case '*': $$ = $1 * $3; break;
                case '/': $$ = $1 / $3; break;
            }
        }
      | expr FACT {}
      | NEG expr  {}
      | expr EXP expr {}
      | PARENTHESIS expr PARENTHESIS {}
      ;

%%

#include "lex.yy.c"
	
int main(void)
{
  return yyparse();}
