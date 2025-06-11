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
%token <op> POINT EXP FACT NEG PLUS MINUS TIMES DIVIDE LPAREN RPAREN
%token <measure> SPACE WEIGHT TIME VOLUME
%token RATIO
%token IF
%token WHILE
%token ELSE
%token <lexeme> ID BOOLEAN RELATION BOOL_ID
%token INT
%token DOU
%token BOOL
%token ASSIGN

%type <value> expr
%type <value> number
%type <value> line
%type <value> integer
%type <value> fraction
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
line  
      : assignment '\n'  { printList(table);}
      | line assignment '\n' {printList(table);}
      | insertation '\n' { printList(table); }
      | line insertation '\n' {printList(table);}
      | expr '\n'      {$$ = $1; printf("Result: %f\n", $$); }
      | line expr '\n'  {$$ = $2; printf("Result: %f\n", $$); }
      | boolexpr '\n' {
            if ($1)
                printf("Result: true\n");
            else
                printf("Result: false\n");
            exit(0);
        }
      | line boolexpr '\n' {
            if ($1)
                printf("Result: true\n");
            else
                printf("Result: false\n");
        }
      
      ;
number  : integer                       {$$ = $1;}
        | integer POINT fraction        {$$ = $1 + $3;}
        ;

integer : UNIT                          {$$ = $1;}
        | TEN                           {$$ = $1;}
        | TEN UNIT                      {$$ = $1 + $2;}
        | UNIT HUNDRED                  {$$ = $1 * $2;}
        | UNIT HUNDRED integer          {$$ = $1 * $2 + $3;}
        ;

fraction    : UNIT                      {$$ = $1 * 0.1;}
            | UNIT UNIT                 {$$ = $1 * 0.1 + $2 * 0.01;}
            | UNIT UNIT UNIT            {$$ = $1 * 0.1 + $2 * 0.01 + $3 * 0.001;}
            ;

expr  : number              {$$ = $1;}
      | expr PLUS expr      {$$ = $1 + $3;}
      | expr MINUS expr     {$$ = $1 - $3;}
      | expr TIMES expr     {$$ = $1 * $3;}
      | expr DIVIDE expr    {$$ = $1 / $3;}
      | expr FACT {
            if((int)$1 == $1 && $1 >= 0){
                $$ = factorial($1);
            }else{
                yyerror("factorial can just be made for positive Integer");
                exit(1);
            }
      }
      | MINUS expr %prec NEG            {$$ = - $2;}
      | expr EXP expr %prec EXP         {$$ = pow($1, $3);}
      | LPAREN expr RPAREN              {$$ = $2;}
      | ID                              {$$ = getDouble(table, $1);}
      | WHILE LPAREN boolexpr RPAREN expr %prec LOWEST          {$$ = $5;}
      | IF LPAREN boolexpr RPAREN expr ELSE expr %prec LOWEST   {$$ = $3?$5:$7;}
      ;


assignment  : ID ASSIGN expr    {  char buffer[20];
                                    gcvt($3, 10, buffer);
                                    table = updateItem(table, $1, buffer);
                                    $$ = $1;
                                }
            | BOOL_ID ASSIGN boolexpr { if($3){
                                        table = updateItem(table, $1, "true");
                                    }else{
                                        table = updateItem(table, $1, "false");
                                    }
                                    $$ = $1;
                                }
            | ID ASSIGN boolexpr {yyerror("You can't assign a boolean expression to an integer variable");}
            | BOOL_ID ASSIGN expr {yyerror("You can't assign a numerical expression to a boolean variable");}
            ;

insertation : INT ID ASSIGN expr { char buffer[20];
                                    gcvt($4, 10, buffer);
                                    table = addToList(table, $2, "int", buffer);
                                    $$ = $2;}
            | DOU ID ASSIGN expr  { char buffer[20];
                                    gcvt($4, 10, buffer);
                                    table = addToList(table, $2, "double", buffer);
                                    $$ = $2;}
            | BOOL BOOL_ID ASSIGN boolexpr { if($4){
                                    table = addToList(table, $2, "bool", "true");
                                    }else{
                                    table = addToList(table, $2, "bool", "false");
                                    }
                                    $$ = $2;}
            | INT ID ASSIGN boolexpr {yyerror("You can't assign a boolean expression to an integer variable");}
            | DOU ID ASSIGN boolexpr {yyerror("You can't assign a boolean expression to a double variable");}
            | BOOL BOOL_ID ASSIGN expr {yyerror("You can't assign a numerical expression to a boolean variable");}
            ;

boolexpr    : BOOLEAN       {$$ = $1;}
            | BOOL_ID       {$$ = getBoolean(table, $1);}
            | expr RELATION expr {
                if (strcmp($2, "<") == 0) $$ = $1 < $3;
                else if (strcmp($2, "<=") == 0) $$ = $1 <= $3;
                else if (strcmp($2, ">") == 0) $$ = $1 > $3;
                else if (strcmp($2, ">=") == 0) $$ = $1 >= $3;
                else if (strcmp($2, "=") == 0) $$ = $1 = $3;
                else if (strcmp($2, "!=") == 0) $$ = $1 != $3;
                else {
                    yyerror("Unknown relational operator");
                    $$ = 0;
                }
            }
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
