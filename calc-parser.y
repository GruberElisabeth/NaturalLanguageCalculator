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
struct Node* table;

#define YYDEBUG 1  // Enable debug mode

%}

%debug


%union {
        double value;
        char op;
        char* lexeme;
        bool condition;
        char* measure;
       }


%token <value> UNIT TEEN TEN HUNDRED INTEGER DOUBLE NUM
%token <op> POINT EXP FACT NEG PLUS MINUS TIMES DIVIDE LPAREN RPAREN RELATION
%token <measure> SPACE WEIGHT TIME VOLUME
%token RATIO
%token IF
%token WHILE
%token ELSE
%token <lexeme> ID
%token <lexeme> BOOL
%token INT
%token DOU
%token BOOLEAN


%type <value> expr
%type <value> line
%type <condition> boolexpr
%type <lexeme> assignment
%type <lexeme> insertation


%right '='
%left PLUS MINUS
%left TIMES DIVIDE
%right NEG
%right EXP
%nonassoc FACT
%nonassoc LOWEST
%start line 

%%
line  : assignment '\n'  { printList(table); printf("Assignment complete\n"); }
      | line assignment '\n' {printf("complete 2");}
      | insertation '\n' { printList(table); printf("Insertation complete\n"); }
      | line insertation '\n' {printf("complete 2");}
      | expr '\n'      {$$ = $1; printf("Result: %f\n", $$); }
      | line expr '\n'  {$$ = $2; printf("Result: %f\n", $$); }
      
      ;


expr  : NUM                 {$$ = $1;}
      | UNIT                {$$ = $1;}
      | TEEN                {$$ = $1;}
      | TEN                 {$$ = $1;}
      | expr PLUS expr      { $$ = $1 + $3;}
      | expr MINUS expr      { $$ = $1 - $3; }
      | expr TIMES expr      { $$ = $1 * $3; }
      | expr DIVIDE expr     { $$ = $1 / $3; }
      | expr FACT {
            if((int)$1 == $1 && $1 >= 0){
                $$ = factorial($1);
            }else{
                yyerror("factorial can just be made for positive Integer");
                exit(1);
            }
            }
      | MINUS expr %prec NEG  {$$ = - $2;}
      | expr EXP expr %prec EXP {$$ = pow($1, $3);}
      | LPAREN expr RPAREN {$$ = $2;}
      | ID  {$$ = getDouble(table, $1);}
      | WHILE LPAREN boolexpr RPAREN expr %prec LOWEST {$$ = $5;}
      | IF LPAREN boolexpr RPAREN expr ELSE expr %prec LOWEST {$$ = $3?$5:$7;}
      ;

assignment  : ID RELATION expr    {  char buffer[20];
                                    gcvt($3, 10, buffer);
                                    table = updateItem(table, $1, buffer);
                                    $$ = $1;
                                }
            | ID RELATION boolexpr { if($3){
                                        table = updateItem(table, $1, "true");
                                    }else{
                                        table = updateItem(table, $1, "false");
                                    }
                                    $$ = $1;
                                }
            ;


insertation : INT ID                { 
                                    table = addToList(table, $2, "int", "0");
                                    printf("Variable %s\n", $2);
                                    $$ = $2;}
            | DOU ID                { table = addToList(table, $2, "double","0.0");
                                    printf("Variable %s\n", $2);
                                    $$ = $2;}
            | BOOLEAN ID            { table = addToList(table, $2, "bool", "true");
                                    printf("Variable %s\n", $2);
                                    $$ = $2;}



boolexpr    : BOOL          {$$ = $1;}
            | ID         { $$ = getBoolean(table, $1);}
            ;



%%

#include "lex.yy.c"

	
int main(void)
{
    table = NULL;
    printf("Enter expressions (Ctrl+D to exit):\n");
    while(1){
        yyparse();
    }
    return 0;
}


int factorial(int n){
    if(n == 1){
        return 1;
    }

    return n * factorial(n - 1);
}
