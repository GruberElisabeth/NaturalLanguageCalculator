%{
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "table.h"
int factorial(int n);

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
%token <op> POINT EXP FACT NEG PLUS MINUS TIMES DIVIDE LPAREN RPAREN RELATION
%token IF
%token WHILE
%token ELSE
%token <lexeme> ID

%type <value> expr
%type <value> line
%type <value> boolexpr
%type <value> assignment
%type <value> insertation

%right '='
%left PLUS MINUS
%left TIMES DIVIDE
%right NEG
%right EXP
%nonassoc FACT
%nonassoc LOWEST
%start line 

%%
line  : expr '\n'      {$$ = $1; printf("Result: %f\n", $$); exit(0);}
      | assignment '\n'
      | insertation '\n'
      ;


expr  : NUM                 {$$ = $1;}
      | UNIT                {$$ = $1;}
      | TEEN                {$$ = $1;}
      | TEN                 {$$ = $1;}
      | expr PLUS expr       { $$ = $1 + $3; }
      | expr MINUS expr      { $$ = $1 - $3; }
      | expr TIMES expr      { $$ = $1 * $3; }
      | expr DIVIDE expr     { $$ = $1 / $3; }
      | expr FACT {
            if((int)$1 == $1 && $1 >= 0){
                $$ = factorial($1);
            }else{
                yyerror("factorial can just be made for positive Integer");
            }
            }
      | MINUS expr %prec NEG  {$$ = - $2;}
      | expr EXP expr %prec EXP {$$ = pow($1, $3);}
      | LPAREN expr RPAREN {$$ = $2;}
      | WHILE LPAREN boolexpr RPAREN expr %prec LOWEST {$$ = $5;}
      | IF LPAREN boolexpr RPAREN expr ELSE expr %prec LOWEST {$$ = $3?$5:$7;}
      ;

assignment  : ID '=' expr {}
            ;

insertation : INTEGER ID '=' expr {}
            | DOUBLE ID '=' expr {}
            | BOOLEAN ID '=' expr {}
            ;

boolexpr    : BOOLEAN {$$ = $1;}
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
