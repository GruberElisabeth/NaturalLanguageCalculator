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
struct node* table;

%}


%union {
        double value;
        char op;
       }

%token <value> UNIT TEEN TEN HUNDRED INTEGER DOUBLE NUM BOOLEAN
%token <op> POINT EXP FACT NEG OPERATOR LPAREN RPAREN
%token IF
%token WHILE
%token ELSE
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
            if((int)$1 == $1 && $1 >= 0){
                $$ = factorial($1);
            }else{
                yyerror("factorial can just be made for positive Integer");
            }
            }
      | NEG expr  {$$ = - $2;}
      | expr EXP expr {$$ = pow($1, $3);}
      | LPAREN expr RPAREN {$$ = $2;}
      | WHILE LPAREN boolexpr RPAREN expr 
      | IF LPAREN boolexpr RPAREN expr ELSE expr {$$ = $3?$5:$7;}
      ;

assignment  : ID '=' expr {}
            ;

insertation : INTEGER ID '=' expr {}
            | DOUBLE ID '=' expr {}
            | BOOLEAN ID '=' expr {}
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
