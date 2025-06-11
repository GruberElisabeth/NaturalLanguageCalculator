/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "calc-parser.y"

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
    int dimension;
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

const char* number_to_word(int num) {
    static const char* units[] = {
        "zero", "one", "two", "three", "four", "five",
        "six", "seven", "eight", "nine", "ten", "eleven",
        "twelve", "thirteen", "fourteen", "fifteen",
        "sixteen", "seventeen", "eighteen", "nineteen"
    };
    
    static const char* ten[] = {
        "", "", "twenty", "thirty", "forty", "fifty",
        "sixty", "seventy", "eighty", "ninety"
    };
    
    static char buf[32];

    if (num < 0 || num >= 100) {
        return "number out of range";
    } else if (num < 20) {
        return units[num];
    } else if (num % 10 == 0) {
        return ten[num / 10];
    } else {
        snprintf(buf, sizeof(buf), "%s-%s", ten[num / 10], units[num % 10]);
        return buf;
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

    if (integer_part >= 0 && integer_part <= 100) {
        strcat(result, number_to_word(integer_part));
    } else {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", integer_part);
        strcat(result, buf);
    }
    
    if (fractional_part > 0) {
        strcat(result, " point");

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
        printf(" %s", quantity.unit);

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



#line 314 "calc-parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "calc-parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_UNIT = 3,                       /* UNIT  */
  YYSYMBOL_TEEN = 4,                       /* TEEN  */
  YYSYMBOL_TEN = 5,                        /* TEN  */
  YYSYMBOL_HUNDRED = 6,                    /* HUNDRED  */
  YYSYMBOL_INTEGER = 7,                    /* INTEGER  */
  YYSYMBOL_DOUBLE = 8,                     /* DOUBLE  */
  YYSYMBOL_NUM = 9,                        /* NUM  */
  YYSYMBOL_POINT = 10,                     /* POINT  */
  YYSYMBOL_EXP = 11,                       /* EXP  */
  YYSYMBOL_FACT = 12,                      /* FACT  */
  YYSYMBOL_NEG = 13,                       /* NEG  */
  YYSYMBOL_PLUS = 14,                      /* PLUS  */
  YYSYMBOL_MINUS = 15,                     /* MINUS  */
  YYSYMBOL_TIMES = 16,                     /* TIMES  */
  YYSYMBOL_DIVIDE = 17,                    /* DIVIDE  */
  YYSYMBOL_LPAREN = 18,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 19,                    /* RPAREN  */
  YYSYMBOL_RATIO = 20,                     /* RATIO  */
  YYSYMBOL_IF = 21,                        /* IF  */
  YYSYMBOL_WHILE = 22,                     /* WHILE  */
  YYSYMBOL_ELSE = 23,                      /* ELSE  */
  YYSYMBOL_ID = 24,                        /* ID  */
  YYSYMBOL_BOOLEAN = 25,                   /* BOOLEAN  */
  YYSYMBOL_RELATION = 26,                  /* RELATION  */
  YYSYMBOL_BOOL_ID = 27,                   /* BOOL_ID  */
  YYSYMBOL_INT = 28,                       /* INT  */
  YYSYMBOL_DOU = 29,                       /* DOU  */
  YYSYMBOL_BOOL = 30,                      /* BOOL  */
  YYSYMBOL_ASSIGN = 31,                    /* ASSIGN  */
  YYSYMBOL_MEASURE = 32,                   /* MEASURE  */
  YYSYMBOL_33_ = 33,                       /* '='  */
  YYSYMBOL_LOWEST = 34,                    /* LOWEST  */
  YYSYMBOL_35_n_ = 35,                     /* '\n'  */
  YYSYMBOL_YYACCEPT = 36,                  /* $accept  */
  YYSYMBOL_line = 37,                      /* line  */
  YYSYMBOL_number = 38,                    /* number  */
  YYSYMBOL_integer = 39,                   /* integer  */
  YYSYMBOL_fraction = 40,                  /* fraction  */
  YYSYMBOL_expr = 41,                      /* expr  */
  YYSYMBOL_quantity = 42,                  /* quantity  */
  YYSYMBOL_assignment = 43,                /* assignment  */
  YYSYMBOL_insertation = 44,               /* insertation  */
  YYSYMBOL_boolexpr = 45                   /* boolexpr  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  39
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   253

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  36
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  10
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  124

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   288


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    33,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    34
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   291,   291,   292,   293,   294,   295,   296,   297,   298,
     299,   306,   314,   315,   318,   319,   320,   321,   322,   323,
     326,   327,   328,   331,   332,   333,   334,   335,   336,   337,
     338,   339,   340,   341,   342,   343,   346,   347,   348,   349,
     350,   351,   352,   353,   354,   355,   356,   357,   358,   362,
     367,   374,   375,   378,   382,   386,   392,   393,   394,   397,
     398,   399
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "UNIT", "TEEN", "TEN",
  "HUNDRED", "INTEGER", "DOUBLE", "NUM", "POINT", "EXP", "FACT", "NEG",
  "PLUS", "MINUS", "TIMES", "DIVIDE", "LPAREN", "RPAREN", "RATIO", "IF",
  "WHILE", "ELSE", "ID", "BOOLEAN", "RELATION", "BOOL_ID", "INT", "DOU",
  "BOOL", "ASSIGN", "MEASURE", "'='", "LOWEST", "'\\n'", "$accept", "line",
  "number", "integer", "fraction", "expr", "quantity", "assignment",
  "insertation", "boolexpr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-15)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     127,     1,   -15,    15,   187,   187,   187,     6,    18,     8,
     -15,    10,    22,    30,    40,    99,    38,    77,    -3,   150,
      36,    53,    59,    57,   -15,   187,   187,   -15,   -15,    54,
      61,   122,   155,   155,   155,   155,    65,    66,    67,   -15,
       5,   172,    70,    71,    72,   -15,   105,   187,    81,   187,
     187,   209,   209,   187,   -15,   187,   209,   209,   209,   209,
     -15,   -15,   -15,   -15,   -15,    -9,    -9,   -15,   187,   187,
     -15,   -15,   -15,   -15,   223,    90,    96,   223,   -15,   223,
     -15,   155,   155,   155,   -15,   -15,   -15,   -15,   -15,   113,
     -15,    -9,   -15,    63,    63,    52,   107,    52,   107,   182,
      -9,   204,    84,    84,    52,   107,    52,   107,    -9,    -9,
     187,   187,   223,   -15,   223,   -15,   223,   -15,   116,   230,
     -15,   -15,   187,   -15
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    14,    15,    16,     0,     0,     0,     0,     0,    33,
      59,    60,     0,     0,     0,     0,    23,    12,     0,     0,
       0,     0,     0,    18,    17,     0,     0,    33,    23,    29,
      30,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,     0,     0,     0,    36,     0,     0,    28,     0,
       0,     0,     0,     0,     6,     0,     0,     0,     0,     0,
       8,     2,     4,    10,    19,    29,    30,    28,     0,     0,
      47,    48,    32,    60,     0,     0,     0,    49,    51,    52,
      50,     0,     0,     0,     7,     9,     3,     5,    11,    20,
      13,    31,    46,    24,    25,    26,    42,    27,    43,    61,
      41,     0,    37,    38,    44,    39,    45,    40,    26,    27,
       0,     0,    53,    56,    54,    57,    58,    55,    21,     0,
      34,    22,     0,    35
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -15,   -15,    33,   102,   -15,     0,   -14,   120,   128,    -5
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    15,    28,    17,    90,    74,    19,    20,    21,    22
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      18,    41,    47,    67,    29,    30,    31,    23,    47,    48,
      44,    49,    50,    51,    52,    40,    47,    48,    24,    49,
      50,    51,    52,    53,    32,    65,    66,    75,    76,    78,
      80,    53,    54,    16,    77,    79,    33,    96,    98,    34,
      84,    35,   102,   103,   105,   107,    36,    91,    16,    93,
      94,    95,    97,    99,    37,   100,   101,   101,   104,   106,
       1,     2,     3,    47,    48,    47,    67,    38,   108,   109,
      45,    61,    47,    67,    47,    67,   113,   115,   117,    68,
      69,   112,   114,   116,    16,    16,    70,    46,    62,    16,
      16,    16,    16,    71,    63,    55,    81,    82,    83,    39,
      58,    59,     1,     2,     3,    86,    87,    88,    89,   110,
     119,   120,     4,    92,     5,   111,   118,     6,    55,   121,
       7,     8,   123,     9,    10,    64,    11,    12,    13,    14,
       1,     2,     3,    47,    67,    42,    49,    50,    68,    69,
       4,    72,     5,    43,     0,     6,     0,     0,     7,     8,
       0,     9,    10,     0,    11,    12,    13,    14,     1,     2,
       3,    55,     0,     0,    56,    57,    58,    59,    25,     0,
      26,     0,     0,     6,     0,     0,     7,     8,     0,    27,
      10,     0,    73,    55,     0,    60,    56,    57,    58,    59,
       1,     2,     3,    47,    67,     0,    49,    50,    68,    69,
      25,     0,    26,     0,     0,     6,     0,    85,     7,     8,
       0,    27,     1,     2,     3,    47,    48,     0,    49,    50,
      51,    52,     4,     0,     5,     0,     0,     6,     0,     0,
       7,     8,     0,    27,    47,    67,     0,    49,    50,    68,
      69,    47,    67,     0,    49,    50,    68,    69,     0,    53,
       0,     0,     0,   122
};

static const yytype_int8 yycheck[] =
{
       0,    15,    11,    12,     4,     5,     6,     6,    11,    12,
      15,    14,    15,    16,    17,    15,    11,    12,     3,    14,
      15,    16,    17,    26,    18,    25,    26,    32,    33,    34,
      35,    26,    35,     0,    34,    35,    18,    51,    52,    31,
      35,    31,    56,    57,    58,    59,    24,    47,    15,    49,
      50,    51,    52,    53,    24,    55,    56,    57,    58,    59,
       3,     4,     5,    11,    12,    11,    12,    27,    68,    69,
      32,    35,    11,    12,    11,    12,    81,    82,    83,    16,
      17,    81,    82,    83,    51,    52,    32,    10,    35,    56,
      57,    58,    59,    32,    35,    11,    31,    31,    31,     0,
      16,    17,     3,     4,     5,    35,    35,    35,     3,    19,
     110,   111,    13,    32,    15,    19,     3,    18,    11,     3,
      21,    22,   122,    24,    25,    23,    27,    28,    29,    30,
       3,     4,     5,    11,    12,    15,    14,    15,    16,    17,
      13,    19,    15,    15,    -1,    18,    -1,    -1,    21,    22,
      -1,    24,    25,    -1,    27,    28,    29,    30,     3,     4,
       5,    11,    -1,    -1,    14,    15,    16,    17,    13,    -1,
      15,    -1,    -1,    18,    -1,    -1,    21,    22,    -1,    24,
      25,    -1,    27,    11,    -1,    35,    14,    15,    16,    17,
       3,     4,     5,    11,    12,    -1,    14,    15,    16,    17,
      13,    -1,    15,    -1,    -1,    18,    -1,    35,    21,    22,
      -1,    24,     3,     4,     5,    11,    12,    -1,    14,    15,
      16,    17,    13,    -1,    15,    -1,    -1,    18,    -1,    -1,
      21,    22,    -1,    24,    11,    12,    -1,    14,    15,    16,
      17,    11,    12,    -1,    14,    15,    16,    17,    -1,    26,
      -1,    -1,    -1,    23
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,    13,    15,    18,    21,    22,    24,
      25,    27,    28,    29,    30,    37,    38,    39,    41,    42,
      43,    44,    45,     6,     3,    13,    15,    24,    38,    41,
      41,    41,    18,    18,    31,    31,    24,    24,    27,     0,
      41,    42,    43,    44,    45,    32,    10,    11,    12,    14,
      15,    16,    17,    26,    35,    11,    14,    15,    16,    17,
      35,    35,    35,    35,    39,    41,    41,    12,    16,    17,
      32,    32,    19,    27,    41,    45,    45,    41,    45,    41,
      45,    31,    31,    31,    35,    35,    35,    35,    35,     3,
      40,    41,    32,    41,    41,    41,    42,    41,    42,    41,
      41,    41,    42,    42,    41,    42,    41,    42,    41,    41,
      19,    19,    41,    45,    41,    45,    41,    45,     3,    41,
      41,     3,    23,    41
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    36,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    38,    38,    39,    39,    39,    39,    39,    39,
      40,    40,    40,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    41,    41,    41,    42,    42,    42,    42,
      42,    42,    42,    42,    42,    42,    42,    42,    42,    43,
      43,    43,    43,    44,    44,    44,    44,    44,    44,    45,
      45,    45
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     3,     2,     3,     2,     3,     2,     3,
       2,     3,     1,     3,     1,     1,     1,     2,     2,     3,
       1,     2,     3,     1,     3,     3,     3,     3,     2,     2,
       2,     3,     3,     1,     5,     7,     2,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     4,     4,     4,     4,     4,     4,     1,
       1,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* line: assignment '\n'  */
#line 291 "calc-parser.y"
                         { printList(table);}
#line 1444 "calc-parser.tab.c"
    break;

  case 3: /* line: line assignment '\n'  */
#line 292 "calc-parser.y"
                             {printList(table);}
#line 1450 "calc-parser.tab.c"
    break;

  case 4: /* line: insertation '\n'  */
#line 293 "calc-parser.y"
                         { printList(table); }
#line 1456 "calc-parser.tab.c"
    break;

  case 5: /* line: line insertation '\n'  */
#line 294 "calc-parser.y"
                              {printList(table);}
#line 1462 "calc-parser.tab.c"
    break;

  case 6: /* line: expr '\n'  */
#line 295 "calc-parser.y"
                       {(yyval.value) = (yyvsp[-1].value); printf("Result: %s\n", number_to_word_double((yyvsp[-1].value)));}
#line 1468 "calc-parser.tab.c"
    break;

  case 7: /* line: line expr '\n'  */
#line 296 "calc-parser.y"
                        {(yyval.value) = (yyvsp[-1].value); printf("Result: %s\n", number_to_word_double((yyvsp[-1].value)));}
#line 1474 "calc-parser.tab.c"
    break;

  case 8: /* line: quantity '\n'  */
#line 297 "calc-parser.y"
                           { print_quantity((yyvsp[-1].quantity)); (yyval.value) = (yyvsp[-1].quantity).value; }
#line 1480 "calc-parser.tab.c"
    break;

  case 9: /* line: line quantity '\n'  */
#line 298 "calc-parser.y"
                           { print_quantity((yyvsp[-1].quantity)); (yyval.value) = (yyvsp[-1].quantity).value; }
#line 1486 "calc-parser.tab.c"
    break;

  case 10: /* line: boolexpr '\n'  */
#line 299 "calc-parser.y"
                      {
            if ((yyvsp[-1].condition))
                printf("Result: true\n");
            else
                printf("Result: false\n");
            exit(0);
        }
#line 1498 "calc-parser.tab.c"
    break;

  case 11: /* line: line boolexpr '\n'  */
#line 306 "calc-parser.y"
                           {
            if ((yyvsp[-2].value))
                printf("Result: true\n");
            else
                printf("Result: false\n");
        }
#line 1509 "calc-parser.tab.c"
    break;

  case 12: /* number: integer  */
#line 314 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[0].value);}
#line 1515 "calc-parser.tab.c"
    break;

  case 13: /* number: integer POINT fraction  */
#line 315 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[-2].value) + (yyvsp[0].value);}
#line 1521 "calc-parser.tab.c"
    break;

  case 14: /* integer: UNIT  */
#line 318 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[0].value);}
#line 1527 "calc-parser.tab.c"
    break;

  case 15: /* integer: TEEN  */
#line 319 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[0].value);}
#line 1533 "calc-parser.tab.c"
    break;

  case 16: /* integer: TEN  */
#line 320 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[0].value);}
#line 1539 "calc-parser.tab.c"
    break;

  case 17: /* integer: TEN UNIT  */
#line 321 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[-1].value) + (yyvsp[0].value);}
#line 1545 "calc-parser.tab.c"
    break;

  case 18: /* integer: UNIT HUNDRED  */
#line 322 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[-1].value) * (yyvsp[0].value);}
#line 1551 "calc-parser.tab.c"
    break;

  case 19: /* integer: UNIT HUNDRED integer  */
#line 323 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[-2].value) * (yyvsp[-1].value) + (yyvsp[0].value);}
#line 1557 "calc-parser.tab.c"
    break;

  case 20: /* fraction: UNIT  */
#line 326 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[0].value) * 0.1;}
#line 1563 "calc-parser.tab.c"
    break;

  case 21: /* fraction: UNIT UNIT  */
#line 327 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[-1].value) * 0.1 + (yyvsp[0].value) * 0.01;}
#line 1569 "calc-parser.tab.c"
    break;

  case 22: /* fraction: UNIT UNIT UNIT  */
#line 328 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[-2].value) * 0.1 + (yyvsp[-1].value) * 0.01 + (yyvsp[0].value) * 0.001;}
#line 1575 "calc-parser.tab.c"
    break;

  case 23: /* expr: number  */
#line 331 "calc-parser.y"
                            {(yyval.value) = (yyvsp[0].value);}
#line 1581 "calc-parser.tab.c"
    break;

  case 24: /* expr: expr PLUS expr  */
#line 332 "calc-parser.y"
                            {(yyval.value) = (yyvsp[-2].value) + (yyvsp[0].value);}
#line 1587 "calc-parser.tab.c"
    break;

  case 25: /* expr: expr MINUS expr  */
#line 333 "calc-parser.y"
                            {(yyval.value) = (yyvsp[-2].value) - (yyvsp[0].value);}
#line 1593 "calc-parser.tab.c"
    break;

  case 26: /* expr: expr TIMES expr  */
#line 334 "calc-parser.y"
                            {(yyval.value) = (yyvsp[-2].value) * (yyvsp[0].value);}
#line 1599 "calc-parser.tab.c"
    break;

  case 27: /* expr: expr DIVIDE expr  */
#line 335 "calc-parser.y"
                            {(yyval.value) = (yyvsp[-2].value) / (yyvsp[0].value);}
#line 1605 "calc-parser.tab.c"
    break;

  case 28: /* expr: expr FACT  */
#line 336 "calc-parser.y"
                  {(yyval.value) = factorial((yyvsp[-1].value));}
#line 1611 "calc-parser.tab.c"
    break;

  case 29: /* expr: NEG expr  */
#line 337 "calc-parser.y"
                                        {(yyval.value) = - (yyvsp[0].value);}
#line 1617 "calc-parser.tab.c"
    break;

  case 30: /* expr: MINUS expr  */
#line 338 "calc-parser.y"
                                        {(yyval.value) = - (yyvsp[0].value);}
#line 1623 "calc-parser.tab.c"
    break;

  case 31: /* expr: expr EXP expr  */
#line 339 "calc-parser.y"
                                        {(yyval.value) = pow((yyvsp[-2].value), (yyvsp[0].value));}
#line 1629 "calc-parser.tab.c"
    break;

  case 32: /* expr: LPAREN expr RPAREN  */
#line 340 "calc-parser.y"
                                        {(yyval.value) = (yyvsp[-1].value);}
#line 1635 "calc-parser.tab.c"
    break;

  case 33: /* expr: ID  */
#line 341 "calc-parser.y"
                                        {(yyval.value) = getDouble(table, (yyvsp[0].lexeme));}
#line 1641 "calc-parser.tab.c"
    break;

  case 34: /* expr: WHILE LPAREN boolexpr RPAREN expr  */
#line 342 "calc-parser.y"
                                                                {(yyval.value) = (yyvsp[0].value);}
#line 1647 "calc-parser.tab.c"
    break;

  case 35: /* expr: IF LPAREN boolexpr RPAREN expr ELSE expr  */
#line 343 "calc-parser.y"
                                                                {(yyval.value) = (yyvsp[-4].condition)?(yyvsp[-2].value):(yyvsp[0].value);}
#line 1653 "calc-parser.tab.c"
    break;

  case 36: /* quantity: number MEASURE  */
#line 346 "calc-parser.y"
                                        { (yyvsp[0].quantity).value = (yyvsp[-1].value); (yyval.quantity) = (yyvsp[0].quantity); }
#line 1659 "calc-parser.tab.c"
    break;

  case 37: /* quantity: quantity PLUS quantity  */
#line 347 "calc-parser.y"
                                        { (yyval.quantity) = calc_quantity((yyvsp[-2].quantity), (yyvsp[0].quantity), '+'); }
#line 1665 "calc-parser.tab.c"
    break;

  case 38: /* quantity: quantity MINUS quantity  */
#line 348 "calc-parser.y"
                                        { (yyval.quantity) = calc_quantity((yyvsp[-2].quantity), (yyvsp[0].quantity), '-'); }
#line 1671 "calc-parser.tab.c"
    break;

  case 39: /* quantity: quantity TIMES quantity  */
#line 349 "calc-parser.y"
                                        { (yyval.quantity) = calc_quantity((yyvsp[-2].quantity), (yyvsp[0].quantity), '*');}
#line 1677 "calc-parser.tab.c"
    break;

  case 40: /* quantity: quantity DIVIDE quantity  */
#line 350 "calc-parser.y"
                                        { (yyval.quantity) = calc_quantity((yyvsp[-2].quantity), (yyvsp[0].quantity), '/');}
#line 1683 "calc-parser.tab.c"
    break;

  case 41: /* quantity: quantity EXP expr  */
#line 351 "calc-parser.y"
                                        {(yyvsp[-2].quantity).dimension = (yyvsp[0].value); (yyval.quantity) = (yyvsp[-2].quantity);}
#line 1689 "calc-parser.tab.c"
    break;

  case 42: /* quantity: expr TIMES quantity  */
#line 352 "calc-parser.y"
                                        {(yyvsp[0].quantity).value = ((yyvsp[-2].value) * (yyvsp[0].quantity).value); (yyval.quantity) = (yyvsp[0].quantity); }
#line 1695 "calc-parser.tab.c"
    break;

  case 43: /* quantity: expr DIVIDE quantity  */
#line 353 "calc-parser.y"
                                        {(yyvsp[0].quantity).value = ((yyvsp[-2].value) / (yyvsp[0].quantity).value); (yyval.quantity) = (yyvsp[0].quantity); }
#line 1701 "calc-parser.tab.c"
    break;

  case 44: /* quantity: quantity TIMES expr  */
#line 354 "calc-parser.y"
                                        {(yyvsp[-2].quantity).value = ((yyvsp[0].value) * (yyvsp[-2].quantity).value); (yyval.quantity) = (yyvsp[-2].quantity); }
#line 1707 "calc-parser.tab.c"
    break;

  case 45: /* quantity: quantity DIVIDE expr  */
#line 355 "calc-parser.y"
                                        {(yyvsp[-2].quantity).value = ((yyvsp[0].value) / (yyvsp[-2].quantity).value); (yyval.quantity) = (yyvsp[-2].quantity); }
#line 1713 "calc-parser.tab.c"
    break;

  case 46: /* quantity: expr FACT MEASURE  */
#line 356 "calc-parser.y"
                                        {(yyvsp[0].quantity).value = factorial((yyvsp[-2].value)); (yyval.quantity) = (yyvsp[0].quantity);}
#line 1719 "calc-parser.tab.c"
    break;

  case 47: /* quantity: NEG expr MEASURE  */
#line 357 "calc-parser.y"
                                        {(yyvsp[0].quantity).value = - (yyvsp[-1].value); (yyval.quantity) = (yyvsp[0].quantity);}
#line 1725 "calc-parser.tab.c"
    break;

  case 48: /* quantity: MINUS expr MEASURE  */
#line 358 "calc-parser.y"
                                        {(yyvsp[0].quantity).value = - (yyvsp[-1].value); (yyval.quantity) = (yyvsp[0].quantity);}
#line 1731 "calc-parser.tab.c"
    break;

  case 49: /* assignment: ID ASSIGN expr  */
#line 362 "calc-parser.y"
                                {  char buffer[20];
                                    gcvt((yyvsp[0].value), 10, buffer);
                                    table = updateItem(table, (yyvsp[-2].lexeme), buffer);
                                    (yyval.lexeme) = (yyvsp[-2].lexeme);
                                }
#line 1741 "calc-parser.tab.c"
    break;

  case 50: /* assignment: BOOL_ID ASSIGN boolexpr  */
#line 367 "calc-parser.y"
                                      { if((yyvsp[0].condition)){
                                        table = updateItem(table, (yyvsp[-2].lexeme), "true");
                                    }else{
                                        table = updateItem(table, (yyvsp[-2].lexeme), "false");
                                    }
                                    (yyval.lexeme) = (yyvsp[-2].lexeme);
                                }
#line 1753 "calc-parser.tab.c"
    break;

  case 51: /* assignment: ID ASSIGN boolexpr  */
#line 374 "calc-parser.y"
                                 {yyerror("You can't assign a boolean expression to an integer variable");}
#line 1759 "calc-parser.tab.c"
    break;

  case 52: /* assignment: BOOL_ID ASSIGN expr  */
#line 375 "calc-parser.y"
                                  {yyerror("You can't assign a numerical expression to a boolean variable");}
#line 1765 "calc-parser.tab.c"
    break;

  case 53: /* insertation: INT ID ASSIGN expr  */
#line 378 "calc-parser.y"
                                 { char buffer[20];
                                    gcvt((yyvsp[0].value), 10, buffer);
                                    table = addToList(table, (yyvsp[-2].lexeme), "int", buffer);
                                    (yyval.lexeme) = (yyvsp[-2].lexeme);}
#line 1774 "calc-parser.tab.c"
    break;

  case 54: /* insertation: DOU ID ASSIGN expr  */
#line 382 "calc-parser.y"
                                  { char buffer[20];
                                    gcvt((yyvsp[0].value), 10, buffer);
                                    table = addToList(table, (yyvsp[-2].lexeme), "double", buffer);
                                    (yyval.lexeme) = (yyvsp[-2].lexeme);}
#line 1783 "calc-parser.tab.c"
    break;

  case 55: /* insertation: BOOL BOOL_ID ASSIGN boolexpr  */
#line 386 "calc-parser.y"
                                           { if((yyvsp[0].condition)){
                                    table = addToList(table, (yyvsp[-2].lexeme), "bool", "true");
                                    }else{
                                    table = addToList(table, (yyvsp[-2].lexeme), "bool", "false");
                                    }
                                    (yyval.lexeme) = (yyvsp[-2].lexeme);}
#line 1794 "calc-parser.tab.c"
    break;

  case 56: /* insertation: INT ID ASSIGN boolexpr  */
#line 392 "calc-parser.y"
                                     {yyerror("You can't assign a boolean expression to an integer variable");}
#line 1800 "calc-parser.tab.c"
    break;

  case 57: /* insertation: DOU ID ASSIGN boolexpr  */
#line 393 "calc-parser.y"
                                     {yyerror("You can't assign a boolean expression to a double variable");}
#line 1806 "calc-parser.tab.c"
    break;

  case 58: /* insertation: BOOL BOOL_ID ASSIGN expr  */
#line 394 "calc-parser.y"
                                       {yyerror("You can't assign a numerical expression to a boolean variable");}
#line 1812 "calc-parser.tab.c"
    break;

  case 59: /* boolexpr: BOOLEAN  */
#line 397 "calc-parser.y"
                            {(yyval.condition) = (yyvsp[0].lexeme);}
#line 1818 "calc-parser.tab.c"
    break;

  case 60: /* boolexpr: BOOL_ID  */
#line 398 "calc-parser.y"
                            {(yyval.condition) = getBoolean(table, (yyvsp[0].lexeme));}
#line 1824 "calc-parser.tab.c"
    break;

  case 61: /* boolexpr: expr RELATION expr  */
#line 399 "calc-parser.y"
                                 {
                if (strcmp((yyvsp[-1].lexeme), "<") == 0) (yyval.condition) = (yyvsp[-2].value) < (yyvsp[0].value);
                else if (strcmp((yyvsp[-1].lexeme), "<=") == 0) (yyval.condition) = (yyvsp[-2].value) <= (yyvsp[0].value);
                else if (strcmp((yyvsp[-1].lexeme), ">") == 0) (yyval.condition) = (yyvsp[-2].value) > (yyvsp[0].value);
                else if (strcmp((yyvsp[-1].lexeme), ">=") == 0) (yyval.condition) = (yyvsp[-2].value) >= (yyvsp[0].value);
                else if (strcmp((yyvsp[-1].lexeme), "=") == 0) (yyval.condition) = (yyvsp[-2].value) = (yyvsp[0].value);
                else if (strcmp((yyvsp[-1].lexeme), "!=") == 0) (yyval.condition) = (yyvsp[-2].value) != (yyvsp[0].value);
                else {
                    yyerror("Unknown relational operator");
                    (yyval.condition) = 0;
                }
            }
#line 1841 "calc-parser.tab.c"
    break;


#line 1845 "calc-parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 413 "calc-parser.y"


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
