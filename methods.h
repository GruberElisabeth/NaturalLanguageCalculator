#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

const UnitConversion* find_unit_info(const char* unit_name);

Quantity convert_quantity(Quantity from_q, const char* to_unit_name);

const char* number_to_word(int digit);

void number_to_words_int(int num, char* buffer);

char* number_to_word_double(double num);

void print_quantity(Quantity quantity);

Quantity calc_quantity(Quantity q1, Quantity q2, char op_char);

