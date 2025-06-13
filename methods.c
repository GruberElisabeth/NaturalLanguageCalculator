#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "methods.h"

// Contains all necessary information to convert units of measurement

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

// gets the information of the paramater unit and returns the UnitConversion object from the conversion table
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
// converts the unit of a given quantitiy into another given unit of measurement of the same type
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

/*methods to convert result back into natural language*/
const char* units[] = {
    "zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"
};

const char* teens[] = {
    "ten", "eleven", "twelve", "thirteen", "fourteen", "fifteen", "sixteen", "seventeen", "eighteen", "nineteen"
};
    
const char* tens[] = {
    "", "", "twenty", "thirty", "forty", "fifty", "sixty", "seventy", "eighty", "ninety"
};

const char* number_to_word(int digit) {
    return units[digit];
}
    
void number_to_words_int(long long num, char* buffer) {
    if (num == 0) {
        strcat(buffer, "zero ");
        return;
    }

    if (num >= 1000000000) {
        number_to_words_int(num / 1000000000, buffer);
        strcat(buffer, "billion ");
        num %= 1000000000;
    }

    if (num >= 1000000) {
        number_to_words_int(num / 1000000, buffer);
        strcat(buffer, "million ");
        num %= 1000000;
    }

    if (num >= 1000) {
        number_to_words_int(num / 1000, buffer);
        strcat(buffer, "thousand ");
        num %= 1000;
    }

    if (num >= 100) {
        number_to_words_int(num / 100, buffer);
        strcat(buffer, "hundred ");
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

/*method to convert fractional back into natural language*/
char* number_to_word_double(double num) {
    static char result[1024];
    result[0] = '\0';

    if (num < 0) {
        strcat(result, "negative ");
        num = -num;
    }

    long long integer_part = (long long)num;
    double fractional_part = num - integer_part;

    number_to_words_int(integer_part, result);
    
    if (fractional_part > 0) {
        strcat(result, "point ");

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

// Special rules to print quantities
void print_quantity(Quantity quantity) {
    if (quantity.unit == NULL) {
        printf("(null quantity)\n");
        return;
    }

    printf("Result: %s ", number_to_word_double(quantity.value));

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

// COntains the logic of all four basic operations for quantities: subtractio, addition, multiplication, division
Quantity calc_quantity(Quantity q1, Quantity q2, char op_char) {
    if (strcmp(q1.type, q2.type) != 0) {
        fprintf(stderr, "Cannot perform operation on quantities of different types");
        exit(1);
    }
    if (q1.dimension != q2.dimension && (op_char == '+' || op_char == '-')) {
        fprintf(stderr, "Cannot perform operation on quantities of different dimensions");
        exit(1);
    }

    Quantity converted = convert_quantity(q2, q1.unit);
    
    double new_value;
    int new_dimension;
    switch (op_char) {
        case '+': new_value = q1.value + converted.value; new_dimension = q1.dimension; break;
        case '-': new_value = q1.value - converted.value; new_dimension = q1.dimension; break;
        case '*': new_value = q1.value * converted.value; new_dimension = q1.dimension + q2.dimension; break;
        case '/':  if (converted.value == 0) {
                        fprintf(stderr, "Error: Division by zero\n");
                        exit(1);
                    }
                    new_value = q1.value / converted.value; 
                    new_dimension = q1.dimension - q2.dimension; 
                    break;
        default: fprintf(stderr, "Internal error: Unknown operator"); exit(1);
    }

    Quantity result;
    result.value = new_value;
    result.dimension = new_dimension;

    result.unit = q1.unit;
    result.type = q1.type;

    free(converted.unit);
    free(converted.type);

    return result;
}