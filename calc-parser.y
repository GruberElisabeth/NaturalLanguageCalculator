%{
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "table.h"
#include "methods.h"

double factorial(double val);

void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

int yylex(void);
struct Node* table;



%}

%union {
        double value;
        char op;
        char* lexeme;
        bool condition;
        Quantity quantity;
       }

%token <value> UNIT TEEN TEN HUNDRED THOUSAND MILLION BILLION INTEGER DOUBLE NUM
%token <op> POINT EXP FACT NEG PLUS MINUS TIMES DIVIDE LPAREN RPAREN
%token RATIO
%token IF
%token WHILE
%token ELSE
%token <lexeme> ID BOOLEAN RELATION BOOL_ID
%token INT
%token DOU
%token BOOL
%token ASSIGN
%token <quantity> MEASURE

%type <value> expr
%type <value> statements
%type <quantity> quantity parenquantity
%type <value> number
%type <value> line
%type <value> fraction
%type <value> integer
%type <value> group
%type <value> hundrets
%type <condition> boolexpr
%type <lexeme> assignment
%type <lexeme> insertation

%right '='
%left PLUS MINUS
%left TIMES DIVIDE
%nonassoc RELATION
%right NEG
%right EXP
%nonassoc MEASURE 
%nonassoc FACT
%nonassoc LOWEST
%start line 

%%
line  
      : assignment '\n'  { printList(table);}
      | line assignment '\n' {printList(table);}
      | insertation '\n' { printList(table); }
      | line insertation '\n' {printList(table);}
      | statements '\n' {$$ = $1; printf("Result: %s\n", number_to_word_double($1));}
      | line statements '\n' {$$ = $2; printf("Result: %s\n", number_to_word_double($2));}
      | expr '\n'      {$$ = $1; printf("Result: %s\n", number_to_word_double($1));}
      | line expr '\n'  {$$ = $2; printf("Result: %s\n", number_to_word_double($2)); }
      | quantity '\n'      { print_quantity($1); $$ = $1.value; }
      | line quantity '\n' { print_quantity($2); $$ = $2.value; }
      | boolexpr '\n' {
            if ($1)
                printf("Result: true\n");
            else
                printf("Result: false\n");
        }
      | line boolexpr '\n' {
            if ($2)
                printf("Result: true\n");
            else
                printf("Result: false\n");
        }
      ;

number  : integer                       {$$ = $1;}
        | integer POINT fraction        {$$ = $1 + $3;}
        ;

integer : group                         {$$ = $1;}
        | group hundrets                {$$ = $1 * $2;}
        | group hundrets integer        {$$ = $1 * $2 + $3;}
        ;

group   : UNIT                          {$$ = $1;}
        | TEEN                          {$$ = $1;}
        | TEN                           {$$ = $1;}
        | TEN UNIT                      {$$ = $1 + $2;}
        | UNIT HUNDRED                  {$$ = $1 * $2;}
        | UNIT HUNDRED group            {$$ = $1 * $2 + $3;}
        ;

hundrets
    : THOUSAND                   {$$ = 1000;}
    | MILLION                    {$$ = 1000000;}
    | BILLION                    {$$ = 1000000000;}
    ;

fraction    : UNIT                      {$$ = $1 * 0.1;}
            | UNIT UNIT                 {$$ = $1 * 0.1 + $2 * 0.01;}
            | UNIT UNIT UNIT            {$$ = $1 * 0.1 + $2 * 0.01 + $3 * 0.001;}
            ;


expr  : number              {$$ = $1;}
      | expr PLUS expr      {$$ = $1 + $3;}
      | expr MINUS expr     {$$ = $1 - $3;}
      | expr TIMES expr     {$$ = $1 * $3;}
      | expr DIVIDE expr    {
                                if($3 == 0) {
                                        fprintf(stderr, "Error: Division by zero\n");
                                        exit(1);
                                }
                                $$ = $1 / $3;
                            }
      | expr FACT           {$$ = factorial($1);}
      | NEG expr                        {$$ = - $2;}
      | MINUS expr %prec NEG            {$$ = - $2;}
      | expr EXP expr %prec EXP         {$$ = pow($1, $3);}
      | LPAREN expr RPAREN              {$$ = $2;}
      | ID                              {$$ = getDouble(table, $1);}
      ;

quantity: number MEASURE                { $2.value = $1; $$ = $2; }
        | quantity PLUS quantity        { $$ = calc_quantity($1, $3, '+'); }
        | quantity MINUS quantity       { $$ = calc_quantity($1, $3, '-'); }
        | quantity TIMES quantity       { $$ = calc_quantity($1, $3, '*');}
        | quantity DIVIDE quantity {
                                        if ($3.value == 0) {
                                            fprintf(stderr, "Error: Division by zero\n");
                                            exit(1);
                                        }
                                        $$ = calc_quantity($1, $3, '/');
                                    }
        | quantity EXP expr %prec EXP   {$1.dimension = $3; $$ = $1;}
        | expr TIMES quantity           {$3.value = ($1 * $3.value); $$ = $3; }
        | expr DIVIDE quantity {
                                    if ($3.value == 0) {
                                        fprintf(stderr, "Error: Division by zero\n");
                                        exit(1);
                                    }
                                    $3.dimension = - ($3.dimension);
                                    $3.value = ($1 / $3.value);
                                    $$ = $3;
                                }
        | quantity TIMES expr           {$1.value = ($3 * $1.value); $$ = $1; }
        | quantity DIVIDE expr {
                                    if ($3 == 0) {
                                        fprintf(stderr, "Error: Division by zero\n");
                                        exit(1);
                                    }
                                    $1.value = ($1.value / $3);
                                    $$ = $1;
                                }
        | expr FACT MEASURE             {$3.value = factorial($1); $$ = $3;}
        | NEG expr MEASURE              {$3.value = - $2; $$ = $3;}
        | MINUS expr MEASURE %prec NEG  {$3.value = - $2; $$ = $3;}
        | LPAREN parenquantity RPAREN        {$$ = $2;}
        | parenquantity EXP expr %prec EXP{ int i;
                                            Quantity result = $1;
                                            for(i = $3; i > 1; i--){
                                                result = calc_quantity(result, $1, '*');
                                            }
                                            $$ = result;}
        ;

parenquantity : LPAREN quantity RPAREN {$$ = $2;}
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

statements  : WHILE LPAREN boolexpr RPAREN expr %prec LOWEST    {$$ = $5;}      
            | IF LPAREN boolexpr RPAREN expr ELSE expr %prec LOWEST   {$$ = $3?$5:$7;}
            ;

boolexpr    : BOOLEAN       {$$ = $1;}
            | BOOL_ID       {$$ = getBoolean(table, $1);}
            | expr RELATION expr {
                if (strcmp($2, "<") == 0) $$ = $1 < $3;
                else if (strcmp($2, "<=") == 0) $$ = $1 <= $3;
                else if (strcmp($2, ">") == 0) $$ = $1 > $3;
                else if (strcmp($2, ">=") == 0) $$ = $1 >= $3;
                else if (strcmp($2, "=") == 0) $$ = $1 == $3;
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
    printf("Enter expressions (Ctrl+C to exit):\n");
    while(1){
        yyparse();
    }
    printf("\n");
    return 0;
}


double factorial(double val) {

    if (val != floor(val)) {
        yyerror("Factorial only applies to integers");
        exit(1);
    }

    if (val < 0) {
        yyerror("Factorial is not defined for negative numbers");
        exit(1);
    }

    double result = 1.0;
    if (val == 0) return 1.0;
    for (int i = val; i > 1; i--) {
        result *= i;
    }
    return result;
}
