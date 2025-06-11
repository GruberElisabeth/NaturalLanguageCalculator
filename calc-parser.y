%{
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include "table.h"

double factorial(double val);

void yyerror(const char *s)
{
    fprintf(stderr, "%s\n", s);
    exit(1);
}

int yylex(void);
struct Node* table;

typedef struct {
    double value;
    char* unit;
    char* type;
    double dimension;
} Quantity;

typedef struct {
    const char* name;
    const char* type;
    double to_base_factor;
} UnitConversion;

const UnitConversion conversion_table[] = {
    // Mass (base: gram)
    {"gram",        "mass",   1.0},
    {"kilogram",    "mass",   1000.0},
    {"milligram",   "mass",   0.001},
    {"pound",       "mass",   453.592},
    {"ton",         "mass",   907185.0},

    // Length (base: meter)
    {"meter",       "length", 1.0},
    {"kilometer",   "length", 1000.0},
    {"centimeter",  "length", 0.01},
    {"millimeter",  "length", 0.001},
    {"mile",        "length", 1609.34},
    {"foot",        "length", 0.3048},
    {"feet",        "length", 0.3048},
    {"inch",        "length", 0.0254},

    // Time (base: second)
    {"second",      "time",   1.0},
    {"millisecond", "time",   0.001},
    {"minute",      "time",   60.0},
    {"hour",        "time",   3600.0},
    {"day",         "time",   86400.0},

    // Volume (base: liter)
    {"liter",       "volume", 1.0},
    {"milliliter",  "volume", 0.001},

    // End of table marker
    {NULL, NULL, 0.0}
};

const UnitConversion* find_unit_info(const char* unit_name) {
    char temp_unit[64];
    strncpy(temp_unit, unit_name, sizeof(temp_unit) - 1);
    temp_unit[sizeof(temp_unit) - 1] = '\0';
    int len = strlen(temp_unit);

    for (int i = 0; conversion_table[i].name != NULL; i++) {
        if (strcmp(conversion_table[i].name, temp_unit) == 0) {
            return &conversion_table[i];
        }
    }
    return NULL;
}

Quantity convert_quantity(Quantity from_q, const char* to_unit_name) {
    const UnitConversion* from_info = find_unit_info(from_q.unit);
    const UnitConversion* to_info = find_unit_info(to_unit_name);
    
    if (strcmp(from_info->type, to_info->type) != 0) {
        fprintf(stderr, "Error: Cannot convert between incompatible types %s and %s.\n",
                from_info->type, to_info->type);
        exit(1);
    }

    double value_in_base_unit = from_q.value * from_info->to_base_factor;
    double final_value = value_in_base_unit / to_info->to_base_factor;

    Quantity result;
    result.value = final_value;
    result.dimension = from_q.dimension;

    result.unit = strdup(to_unit_name);
    result.type = strdup(from_q.type);

    return result;
}


#define YYDEBUG 1  // Enable debug mode

/*methods to convert result back into natural language*/
const char* units[] = {
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

const char* teens[] = {
    "ten", "eleven", "twelve", "thirteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};
    
const char* tens[] = {
    "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
};

const char* number_to_word(int digit) {
    return units[digit];
}
    
void number_to_words_int(int num, char* buffer){
    if (num == 0) {
        strcat(buffer, "zero");
        return;
    }
if (num >= 1000000000) {
        number_to_words_int(num / 1000000000, buffer);
        strcat(buffer, " billion ");
        num %= 1000000000;
    }

    if (num >= 1000000) {
        number_to_words_int(num / 1000000, buffer);
        strcat(buffer, " million ");
        num %= 1000000;
    }

    if (num >= 1000) {
        number_to_words_int(num / 1000, buffer);
        strcat(buffer, " thousand ");
        num %= 1000;
    }

    if (num >= 100) {
        strcat(buffer, units[num / 100]);
        strcat(buffer, " hundred ");
        num %= 100;
    }

    if (num >= 20) {
        strcat(buffer, tens[num / 10]);
        strcat(buffer, " ");
        num %= 10;
    } else if (num >= 10) {
        strcat(buffer, teens[num - 10]);
        strcat(buffer, " ");
        return;
    }

    if (num > 0) {
        strcat(buffer, units[num]);
        strcat(buffer, " ");
    }
}

char* number_to_word_double(double num) {
    static char result[256];
    result[0] = '\0';

    if (num < 0) {
        strcat(result, "negative ");
        num = -num;
    }

    int integer_part = (int)num;
    double fractional_part = num - integer_part;

    number_to_words_int(integer_part, result);
    
    if (fractional_part > 0) {
        strcat(result, "point");

        fractional_part = fractional_part * 1000;
        int frac_int = (int)(fractional_part + 0.5);

        char frac_str[20];
        snprintf(frac_str, sizeof(frac_str), "%03d", frac_int);

        int len = strlen(frac_str);
        while (len > 0 && frac_str[len-1] == '0') {
            frac_str[len-1] = '\0';
            len--;
        }

        for (int i = 0; i < len; i++) {
            strcat(result, " ");
            strcat(result, number_to_word(frac_str[i] - '0'));
        }
    }
    return result;
}

void print_quantity(Quantity quantity) {
    if (quantity.unit == NULL) {
        printf("(null quantity)\n");
        return;
    }

    printf("Result: %s", number_to_word_double(quantity.value));

    if (quantity.dimension != 0) {
        printf("%s", quantity.unit);

        if (quantity.value != 1 && quantity.value != -1) {
            if (strcmp(quantity.unit, "inch") == 0) {
                printf("es");
            } else if (strcmp(quantity.unit, "foot") == 0) {
                printf(" feet");
            } else {
                printf("s");
            }
        }
        if (quantity.dimension != 1) {
            printf(" to the power of %s", number_to_word_double(quantity.dimension));
        }
    }

    printf("\n");
}

Quantity calc_quantity(Quantity q1, Quantity q2, char op_char) {
    if (strcmp(q1.type, q2.type) != 0) {
        yyerror("Cannot perform operation on quantities of different types");
        exit(1);
    }
    if (q1.dimension != q2.dimension && (op_char == '+' || op_char == '-')) {
        yyerror("Cannot perform operation on quantities of different dimensions");
        exit(1);
    }

    Quantity converted = convert_quantity(q2, q1.unit);
    
    double new_value;
    int new_dimension;
    switch (op_char) {
        case '+': new_value = q1.value + converted.value; new_dimension = q1.dimension; break;
        case '-': new_value = q1.value - converted.value; new_dimension = q1.dimension; break;
        case '*': new_value = q1.value * converted.value; new_dimension = q1.dimension + q2.dimension; break;
        case '/': new_value = q1.value / converted.value; new_dimension = q1.dimension - q2.dimension; break;
        default: yyerror("Internal error: Unknown operator"); exit(1);
    }

    Quantity result;
    result.value = new_value;
    result.dimension = new_dimension;

    result.unit = q1.unit;
    result.type = q1.type;

    free(q2.unit);
    free(q2.type);
    free(converted.unit);
    free(converted.type);

    return result;
}


%}

%debug

%union {
        double value;
        char op;
        char* lexeme;
        bool condition;
        Quantity quantity;
       }

%token <value> UNIT TEEN TEN HUNDRED INTEGER DOUBLE NUM
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
%type <quantity> quantity
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
      | expr '\n'      {$$ = $1; printf("Result: %s\n", number_to_word_double($1));}
      | line expr '\n'  {$$ = $2; printf("Result: %s\n", number_to_word_double($2));}
      | quantity '\n'      { print_quantity($1); $$ = $1.value; }
      | line quantity '\n' { print_quantity($2); $$ = $2.value; }
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
        | TEEN                          {$$ = $1;}
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
      | expr FACT {$$ = factorial($1);}
      | NEG expr                        {$$ = - $2;}
      | MINUS expr %prec NEG            {$$ = - $2;}
      | expr EXP expr %prec EXP         {$$ = pow($1, $3);}
      | LPAREN expr RPAREN              {$$ = $2;}
      | ID                              {$$ = getDouble(table, $1);}
      | WHILE LPAREN boolexpr RPAREN expr %prec LOWEST          {$$ = $5;}
      | IF LPAREN boolexpr RPAREN expr ELSE expr %prec LOWEST   {$$ = $3?$5:$7;}
      ;

quantity: number MEASURE                { $2.value = $1; $$ = $2; }
        | quantity PLUS quantity        { $$ = calc_quantity($1, $3, '+'); }
        | quantity MINUS quantity       { $$ = calc_quantity($1, $3, '-'); }
        | quantity TIMES quantity       { $$ = calc_quantity($1, $3, '*');}
        | quantity DIVIDE quantity      { $$ = calc_quantity($1, $3, '/');}
        | quantity EXP expr %prec EXP   {$1.dimension = $3; $$ = $1;}
        | expr TIMES quantity           {$3.value = ($1 * $3.value); $$ = $3; }
        | expr DIVIDE quantity          {$3.value = ($1 / $3.value); $$ = $3; }
        | quantity TIMES expr           {$1.value = ($3 * $1.value); $$ = $1; }
        | quantity DIVIDE expr          {$1.value = ($3 / $1.value); $$ = $1; }
        | expr FACT MEASURE             {$3.value = factorial($1); $$ = $3;}
        | NEG expr MEASURE              {$3.value = - $2; $$ = $3;}
        | MINUS expr MEASURE %prec NEG  {$3.value = - $2; $$ = $3;}
        | LPAREN quantity RPAREN        {$$ = $2;}
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
    printf("Enter expressions (Ctrl+C to exit):\n");
    while(1){
        yyparse();
    }
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
