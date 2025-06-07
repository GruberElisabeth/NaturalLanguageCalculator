%{
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "table.h"

void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

int yylex(void);
struct symtab* table;

%}


%union {
        double value;
        char op;
       }

%token <value> UNIT TEEN TEN HUNDRED INTEGER DOUBLE NUM
%token <op> POINT EXP FACT NEG OPERATOR PARENTHESIS
%token IF
%token WHILE
%token DO
%token <lexeme> ID

%type <value> expr
%type <value> line
%type <value> boolexpr
%type <value> assignment
%type <value> insertation


//hier müssen die precedence fälle noch definiert werden

%start line

%%
line  : expr '\n'      {$$ = $1; printf("Result: %f\n", $$); exit(0);}
      ;


expr  : NUM {$$ = $1;}
      | expr OPERATOR expr  { 
            switch($2) {
                case '+': $$ = $1 + $3; break;
                case '-': $$ = $1 - $3; break;
                case '*': $$ = $1 * $3; break;
                case '/': $$ = $1 / $3; break;
            }
        }
      | expr FACT {
            if(typeof($1) == INTEGER){
                $$ = factorial($1);
            }else{
                yyerror("factorial can just be made for positive Integer");
            }
            }
      | NEG expr  {$$ = - $2;}
      | expr EXP expr {$$ = pow($1, $3);}
      | PARENTHESIS expr PARENTHESIS {
            if(($1 == LPAREN) && ($3 == RPAREN)){
                $$ = $2;
            }else{
                yyerror("parenthesis don't match expectations");
            }
            
            }
      | WHILE '(' boolexpr ')' expr {}
      | IF '(' boolexpr ')' expr {}
      ;

assignment  : ID '=' expr {}
            ;

insertation : INTEGER ID '=' expr {}
            | DOUBLE ID '=' {}
            ;

boolexpr    : 
            ;


%%

#include "lex.yy.c"
	
int main(void)
{
  return yyparse();}



int factorial(int n){
    if(n == 1){
        return 1;
    }

    return n * factorial(n - 1);
}
