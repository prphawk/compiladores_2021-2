/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

#include<stdio.h>
#include<string.h>
#include "ast.h"
#include "main.h"
int yylex(void);
int yyerror (char const *s); //mudar pra void?
extern int get_line_number (void);
nodo *adiciona_nodo(valorLexico valor_lexico);
nodo *adiciona_nodo_label(char *label);
void adiciona_filho(nodo *pai, nodo *filho);
void imprime_arvore(nodo *nodo, int profundidade);
lseNodo *acha_ultimo_filho(lseNodo *filhos);
void adiciona_irmao(lseNodo *irmao, lseNodo *novo_irmao);
void libera_arvore(void *pai);
void libera_irmaos(void *filhos);
void libera_nodo(nodo *nodo);
void libera_valor_lexico(valorLexico valor_lexico);

#line 90 "parser.tab.c"

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

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 21 "parser.y"

   #include "valor_lexico.h"

#line 137 "parser.tab.c"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_PR_INT = 258,
    TK_PR_FLOAT = 259,
    TK_PR_BOOL = 260,
    TK_PR_CHAR = 261,
    TK_PR_STRING = 262,
    TK_PR_IF = 263,
    TK_PR_THEN = 264,
    TK_PR_ELSE = 265,
    TK_PR_WHILE = 266,
    TK_PR_DO = 267,
    TK_PR_INPUT = 268,
    TK_PR_OUTPUT = 269,
    TK_PR_RETURN = 270,
    TK_PR_CONST = 271,
    TK_PR_STATIC = 272,
    TK_PR_FOREACH = 273,
    TK_PR_FOR = 274,
    TK_PR_SWITCH = 275,
    TK_PR_CASE = 276,
    TK_PR_BREAK = 277,
    TK_PR_CONTINUE = 278,
    TK_PR_CLASS = 279,
    TK_PR_PRIVATE = 280,
    TK_PR_PUBLIC = 281,
    TK_PR_PROTECTED = 282,
    TK_PR_END = 283,
    TK_PR_DEFAULT = 284,
    TK_OC_LE = 285,
    TK_OC_GE = 286,
    TK_OC_EQ = 287,
    TK_OC_NE = 288,
    TK_OC_AND = 289,
    TK_OC_OR = 290,
    TK_OC_SL = 291,
    TK_OC_SR = 292,
    TK_LIT_INT = 293,
    TK_LIT_FLOAT = 294,
    TK_LIT_FALSE = 295,
    TK_LIT_TRUE = 296,
    TK_LIT_CHAR = 297,
    TK_LIT_STRING = 298,
    TK_IDENTIFICADOR = 299,
    TOKEN_ERRO = 300
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 24 "parser.y"

   valorLexico valor_lexico;
   nodo *nodo;

#line 199 "parser.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */



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
typedef yytype_uint8 yy_state_t;

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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


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
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   227

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  46
/* YYNRULES -- Number of rules.  */
#define YYNRULES  122
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  205

#define YYUNDEFTOK  2
#define YYMAXUTOK   300


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    66,     2,    68,     2,    59,    62,     2,
      50,    51,    57,    60,    49,    61,     2,    58,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,    46,
      64,    54,    65,    67,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    47,     2,    48,    56,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    52,    63,    53,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   127,   127,   129,   133,   135,   135,   137,   137,   139,
     139,   141,   141,   143,   149,   150,   152,   152,   154,   154,
     156,   156,   158,   158,   158,   158,   158,   160,   162,   163,
     165,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   181,   182,   183,   184,   187,   188,   195,
     202,   209,   216,   229,   236,   242,   250,   257,   267,   274,
     286,   293,   300,   310,   319,   328,   330,   335,   337,   337,
     339,   340,   341,   342,   343,   344,   347,   348,   348,   348,
     349,   349,   350,   350,   351,   352,   353,   354,   355,   356,
     357,   358,   361,   363,   364,   365,   366,   367,   368,   371,
     372,   380,   381,   387,   388,   394,   395,   401,   402,   408,
     409,   415,   416,   421,   427,   428,   430,   431,   438,   445,
     446,   447,   448
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 1
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_PR_INT", "TK_PR_FLOAT", "TK_PR_BOOL",
  "TK_PR_CHAR", "TK_PR_STRING", "TK_PR_IF", "TK_PR_THEN", "TK_PR_ELSE",
  "TK_PR_WHILE", "TK_PR_DO", "TK_PR_INPUT", "TK_PR_OUTPUT", "TK_PR_RETURN",
  "TK_PR_CONST", "TK_PR_STATIC", "TK_PR_FOREACH", "TK_PR_FOR",
  "TK_PR_SWITCH", "TK_PR_CASE", "TK_PR_BREAK", "TK_PR_CONTINUE",
  "TK_PR_CLASS", "TK_PR_PRIVATE", "TK_PR_PUBLIC", "TK_PR_PROTECTED",
  "TK_PR_END", "TK_PR_DEFAULT", "TK_OC_LE", "TK_OC_GE", "TK_OC_EQ",
  "TK_OC_NE", "TK_OC_AND", "TK_OC_OR", "TK_OC_SL", "TK_OC_SR",
  "TK_LIT_INT", "TK_LIT_FLOAT", "TK_LIT_FALSE", "TK_LIT_TRUE",
  "TK_LIT_CHAR", "TK_LIT_STRING", "TK_IDENTIFICADOR", "TOKEN_ERRO", "';'",
  "'['", "']'", "','", "'('", "')'", "'{'", "'}'", "'='", "':'", "'^'",
  "'*'", "'/'", "'%'", "'+'", "'-'", "'&'", "'|'", "'<'", "'>'", "'!'",
  "'?'", "'#'", "$accept", "programa", "declaracoes", "declaracao",
  "declaracao_variavel_global", "nome_variavel", "lista_nome_variavel",
  "declaracao_funcao", "cabecalho", "parametros", "lista_parametros",
  "parametro", "tipo", "corpo", "lista_comandos", "bloco_comandos",
  "comando_simples", "declaracao_var_local", "lista_nome_variavel_local",
  "comando_atribuicao", "comando_entrada", "comando_saida",
  "comando_shift", "comando_retorno", "comando_condicional",
  "comando_iterativo", "argumento", "argumentos", "lista_argumentos",
  "literal", "operador_binario_prec1", "operador_binario_prec2",
  "operador_binario_prec3", "operador_binario_prec4",
  "operador_binario_prec5", "operador_asterisco", "operador_unario",
  "expressao", "expr_binaria_ou", "expr_binaria_1_ou", "expr_binaria_2_ou",
  "expr_binaria_3_ou", "expr_binaria_4_ou", "expr_unaria_ou",
  "expr_parenteses_ou", "operando", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,    59,    91,    93,    44,
      40,    41,   123,   125,    61,    58,    94,    42,    47,    37,
      43,    45,    38,   124,    60,    62,    33,    63,    35
};
# endif

#define YYPACT_NINF (-103)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     130,  -103,  -103,  -103,  -103,  -103,   189,    20,  -103,   130,
    -103,  -103,     6,    27,    37,  -103,  -103,    23,  -103,  -103,
     -40,    17,    58,    79,    75,    99,   100,   118,   136,    56,
     189,   135,   116,  -103,  -103,  -103,  -103,  -103,  -103,    78,
      56,  -103,  -103,  -103,  -103,  -103,  -103,  -103,   123,   115,
    -103,   125,  -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,
      56,   120,   102,   -17,    36,    83,    51,   134,  -103,  -103,
    -103,   143,   149,   128,  -103,   149,  -103,    56,    56,  -103,
    -103,  -103,  -103,  -103,  -103,  -103,  -103,  -103,    80,   102,
     123,   189,   123,   153,   160,   161,    56,    56,    56,   -16,
      -6,  -103,  -103,    23,  -103,  -103,    56,  -103,  -103,  -103,
    -103,  -103,  -103,  -103,  -103,    56,  -103,  -103,    56,  -103,
    -103,    56,  -103,  -103,  -103,    56,  -103,    56,   152,   189,
     150,  -103,   154,   158,   157,  -103,   155,    -7,    25,    56,
    -103,   123,  -103,    65,   156,  -103,  -103,   -26,   159,  -103,
     162,   102,   102,  -103,   145,   123,  -103,    22,    36,    83,
      51,   134,  -103,  -103,   163,  -103,   149,  -103,  -103,     6,
     193,    11,  -103,    56,    56,    66,    56,  -103,  -103,  -103,
    -103,    56,  -103,  -103,   199,     6,  -103,    34,   -46,   172,
     174,    56,  -103,   -17,     6,  -103,   164,   153,  -103,  -103,
     102,  -103,   165,     6,  -103
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       4,    22,    23,    25,    24,    26,     0,     0,     2,     4,
       5,     6,     0,     0,     0,     1,     3,    29,    13,    27,
       9,    11,     0,     9,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    37,    38,   122,   121,   120,   119,   116,
       0,    92,    94,    93,    96,    95,    97,    98,     0,     0,
      42,     0,    31,    32,    33,    34,    35,    36,    39,    40,
       0,     0,    41,    99,   101,   103,   105,   107,   109,   111,
     114,     0,    17,     0,     8,    17,     7,     0,     0,    53,
      75,    74,    73,    72,    70,    71,    54,    55,   116,    60,
       0,     0,     0,     0,     0,     0,     0,    69,     0,     0,
      47,    46,    30,    29,   113,   112,     0,    86,    88,    87,
      89,    91,    90,    84,    85,     0,    82,    83,     0,    80,
      81,     0,    77,    78,    79,     0,    76,     0,     0,     0,
       0,    16,    18,     0,     9,    12,     0,     0,     0,     0,
      44,     0,    45,     0,     0,    56,    58,     0,    67,    68,
       0,    65,    51,   115,     0,     0,    28,     0,   102,   104,
     106,   108,   110,    10,     0,    15,     0,    20,    14,     0,
       0,     0,    43,     0,     0,   117,     0,   118,    48,    49,
      50,     0,    21,    19,    61,     0,   117,     0,     0,     0,
       0,     0,    66,   100,     0,    64,     0,     0,    57,    59,
      52,    62,     0,     0,    63
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -103,  -103,   205,  -103,  -103,  -103,    -8,  -103,  -103,   140,
      53,  -103,     2,  -103,   114,   -12,  -103,  -103,   -67,   -90,
    -103,  -103,  -103,  -103,  -103,  -103,  -103,    44,  -103,    67,
    -103,  -103,  -103,  -103,  -103,  -103,  -103,   -28,  -102,   107,
     105,   103,   101,   -55,   166,  -103
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,     9,    10,    21,    22,    11,    12,   130,
     131,   132,   133,    18,    49,    50,    51,    52,   101,    53,
      54,    55,    56,    57,    58,    59,   148,   149,   150,    87,
     127,   125,   121,   118,   115,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      19,    89,    13,   144,   157,   104,    24,    71,    14,   197,
      72,    13,    99,   107,   108,   109,   110,   111,   112,    48,
      15,   106,   175,   140,   154,   142,     1,     2,     3,     4,
       5,    25,    90,    92,    26,   153,    27,    28,    29,    30,
      31,   106,    32,   155,   169,    33,    34,   113,   114,   137,
     138,   106,   107,   108,   109,   110,   111,   112,    17,   186,
     106,    35,    36,    37,    38,   135,    73,    39,   147,   151,
     152,    20,   162,    40,   172,    17,   170,   181,   106,   193,
      41,    23,   196,    42,    43,    44,   113,   114,   180,    45,
      46,    47,   106,   141,    35,    36,    37,    38,   116,   117,
      88,   106,   189,   190,    74,    48,    40,   202,   122,   123,
     124,   171,   173,    41,    94,    95,    42,    43,    44,    98,
     191,    76,    45,    46,    47,    96,    71,   139,    97,    75,
      97,   164,    98,     1,     2,     3,     4,     5,     1,     2,
       3,     4,     5,   119,   120,   187,   188,     6,   151,    77,
      78,    91,     1,     2,     3,     4,     5,   184,    35,    36,
      37,    38,    79,   200,    88,   129,    93,   100,   102,   106,
      40,   103,   134,   195,    80,    81,    82,    83,    84,    85,
      86,   128,   201,    80,    81,    82,    83,    84,    85,   178,
     126,   204,     1,     2,     3,     4,     5,   143,   145,   146,
     163,   165,   167,   166,    71,   185,   168,   182,   176,   194,
     198,   174,   199,   177,    16,   136,   203,   156,   191,   183,
     192,   179,   158,   159,   160,     0,   161,   105
};

static const yytype_int16 yycheck[] =
{
      12,    29,     0,    93,   106,    60,    14,    47,     6,    55,
      50,     9,    40,    30,    31,    32,    33,    34,    35,    17,
       0,    67,    48,    90,    30,    92,     3,     4,     5,     6,
       7,     8,    30,    31,    11,    51,    13,    14,    15,    16,
      17,    67,    19,    49,    51,    22,    23,    64,    65,    77,
      78,    67,    30,    31,    32,    33,    34,    35,    52,    48,
      67,    38,    39,    40,    41,    73,    49,    44,    96,    97,
      98,    44,   127,    50,   141,    52,    51,    55,    67,   181,
      57,    44,    48,    60,    61,    62,    64,    65,   155,    66,
      67,    68,    67,    91,    38,    39,    40,    41,    62,    63,
      44,    67,    36,    37,    46,   103,    50,   197,    57,    58,
      59,   139,    47,    57,    36,    37,    60,    61,    62,    54,
      54,    46,    66,    67,    68,    47,    47,    47,    50,    50,
      50,   129,    54,     3,     4,     5,     6,     7,     3,     4,
       5,     6,     7,    60,    61,   173,   174,    17,   176,    50,
      50,    16,     3,     4,     5,     6,     7,   169,    38,    39,
      40,    41,    44,   191,    44,    16,    50,    44,    53,    67,
      50,    46,    44,   185,    38,    39,    40,    41,    42,    43,
      44,    38,   194,    38,    39,    40,    41,    42,    43,    44,
      56,   203,     3,     4,     5,     6,     7,    44,    38,    38,
      48,    51,    44,    49,    47,    12,    51,    44,    49,    10,
      38,    55,    38,    51,     9,    75,    51,   103,    54,   166,
     176,   154,   115,   118,   121,    -1,   125,    61
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,     4,     5,     6,     7,    17,    70,    71,    72,
      73,    76,    77,    81,    81,     0,    71,    52,    82,    84,
      44,    74,    75,    44,    75,     8,    11,    13,    14,    15,
      16,    17,    19,    22,    23,    38,    39,    40,    41,    44,
      50,    57,    60,    61,    62,    66,    67,    68,    81,    83,
      84,    85,    86,    88,    89,    90,    91,    92,    93,    94,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    47,    50,    49,    46,    50,    46,    50,    50,    44,
      38,    39,    40,    41,    42,    43,    44,    98,    44,   106,
      81,    16,    81,    50,    36,    37,    47,    50,    54,   106,
      44,    87,    53,    46,   112,   113,    67,    30,    31,    32,
      33,    34,    35,    64,    65,   103,    62,    63,   102,    60,
      61,   101,    57,    58,    59,   100,    56,    99,    38,    16,
      78,    79,    80,    81,    44,    75,    78,   106,   106,    47,
      87,    81,    87,    44,    88,    38,    38,   106,    95,    96,
      97,   106,   106,    51,    30,    49,    83,   107,   108,   109,
     110,   111,   112,    48,    81,    51,    49,    44,    51,    51,
      51,   106,    87,    47,    55,    48,    49,    51,    44,    98,
      87,    55,    44,    79,    84,    12,    48,   106,   106,    36,
      37,    54,    96,   107,    10,    84,    48,    55,    38,    38,
     106,    84,    88,    51,    84
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    69,    70,    71,    71,    72,    72,    73,    73,    74,
      74,    75,    75,    76,    77,    77,    78,    78,    79,    79,
      80,    80,    81,    81,    81,    81,    81,    82,    83,    83,
      84,    85,    85,    85,    85,    85,    85,    85,    85,    85,
      85,    85,    85,    86,    86,    86,    86,    87,    87,    87,
      87,    88,    88,    89,    90,    90,    91,    91,    91,    91,
      92,    93,    93,    94,    94,    95,    96,    96,    97,    97,
      98,    98,    98,    98,    98,    98,    99,   100,   100,   100,
     101,   101,   102,   102,   103,   103,   103,   103,   103,   103,
     103,   103,   104,   105,   105,   105,   105,   105,   105,   106,
     106,   107,   107,   108,   108,   109,   109,   110,   110,   111,
     111,   112,   112,   112,   113,   113,   114,   114,   114,   114,
     114,   114,   114
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     1,     4,     3,     1,
       4,     1,     3,     2,     6,     5,     1,     0,     1,     3,
       2,     3,     1,     1,     1,     1,     1,     1,     3,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     4,     3,     3,     2,     1,     3,     3,
       3,     3,     6,     2,     2,     2,     3,     6,     3,     6,
       2,     5,     7,     9,     6,     1,     3,     1,     1,     0,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     2,     2,     1,     3,     1,     4,     4,     1,
       1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
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
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
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
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
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
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
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

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
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

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
  case 2:
#line 127 "parser.y"
                      { (yyval.nodo) = (yyvsp[0].nodo); arvore = (yyval.nodo); imprime_arvore((yyvsp[0].nodo), 0); }
#line 1544 "parser.tab.c"
    break;

  case 3:
#line 130 "parser.y"
            {
                if((yyvsp[0].nodo)!=NULL) adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                (yyval.nodo) = (yyvsp[-1].nodo);
            }
#line 1553 "parser.tab.c"
    break;

  case 4:
#line 133 "parser.y"
                { (yyval.nodo) = NULL; }
#line 1559 "parser.tab.c"
    break;

  case 6:
#line 135 "parser.y"
                                                           { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1565 "parser.tab.c"
    break;

  case 13:
#line 144 "parser.y"
                {
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                    (yyval.nodo) = (yyvsp[-1].nodo);
                }
#line 1574 "parser.tab.c"
    break;

  case 14:
#line 149 "parser.y"
                                                                 { (yyval.nodo) = adiciona_nodo((yyvsp[-3].valor_lexico)); }
#line 1580 "parser.tab.c"
    break;

  case 15:
#line 150 "parser.y"
                                                       { (yyval.nodo) = adiciona_nodo((yyvsp[-3].valor_lexico)); }
#line 1586 "parser.tab.c"
    break;

  case 27:
#line 160 "parser.y"
                      { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1592 "parser.tab.c"
    break;

  case 28:
#line 162 "parser.y"
                                                   { if((yyvsp[0].nodo)!=NULL) adiciona_filho((yyvsp[-2].nodo), (yyvsp[0].nodo)); (yyval.nodo) = (yyvsp[-2].nodo); }
#line 1598 "parser.tab.c"
    break;

  case 29:
#line 163 "parser.y"
                  { (yyval.nodo) = NULL; }
#line 1604 "parser.tab.c"
    break;

  case 30:
#line 165 "parser.y"
                                       { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 1610 "parser.tab.c"
    break;

  case 31:
#line 167 "parser.y"
                                      { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1616 "parser.tab.c"
    break;

  case 32:
#line 168 "parser.y"
                                    { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1622 "parser.tab.c"
    break;

  case 33:
#line 169 "parser.y"
                                 { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1628 "parser.tab.c"
    break;

  case 34:
#line 170 "parser.y"
                               { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1634 "parser.tab.c"
    break;

  case 35:
#line 171 "parser.y"
                               { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1640 "parser.tab.c"
    break;

  case 36:
#line 172 "parser.y"
                                 { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1646 "parser.tab.c"
    break;

  case 37:
#line 173 "parser.y"
                             { (yyval.nodo) = adiciona_nodo_label("break"); }
#line 1652 "parser.tab.c"
    break;

  case 38:
#line 174 "parser.y"
                                { (yyval.nodo) = adiciona_nodo_label("continue"); }
#line 1658 "parser.tab.c"
    break;

  case 39:
#line 175 "parser.y"
                                     { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1664 "parser.tab.c"
    break;

  case 40:
#line 176 "parser.y"
                                   { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1670 "parser.tab.c"
    break;

  case 41:
#line 177 "parser.y"
                           { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1676 "parser.tab.c"
    break;

  case 42:
#line 178 "parser.y"
                                { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1682 "parser.tab.c"
    break;

  case 43:
#line 181 "parser.y"
                                                                              { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1688 "parser.tab.c"
    break;

  case 44:
#line 182 "parser.y"
                                                                  { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1694 "parser.tab.c"
    break;

  case 45:
#line 183 "parser.y"
                                                                   { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1700 "parser.tab.c"
    break;

  case 46:
#line 184 "parser.y"
                                                      { (yyval.nodo) = (yyvsp[0].nodo);}
#line 1706 "parser.tab.c"
    break;

  case 47:
#line 187 "parser.y"
                                            { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico));}
#line 1712 "parser.tab.c"
    break;

  case 48:
#line 189 "parser.y"
                            { 
                                nodo *novo_nodo = adiciona_nodo((yyvsp[-1].valor_lexico));
                                adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[-2].valor_lexico)));
                                adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[0].valor_lexico)));
                                (yyval.nodo) = novo_nodo;
                            }
#line 1723 "parser.tab.c"
    break;

  case 49:
#line 196 "parser.y"
                           { 
                                nodo *novo_nodo = adiciona_nodo((yyvsp[-1].valor_lexico));
                                adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[-2].valor_lexico)));
                                adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                                (yyval.nodo) = novo_nodo;
                            }
#line 1734 "parser.tab.c"
    break;

  case 50:
#line 203 "parser.y"
                           { 
                                adiciona_filho((yyval.nodo), adiciona_nodo((yyvsp[-2].valor_lexico)));
                                adiciona_filho((yyval.nodo), (yyvsp[0].nodo));
                            }
#line 1743 "parser.tab.c"
    break;

  case 51:
#line 210 "parser.y"
                    {
                        nodo *novo_nodo = adiciona_nodo_label("=");
                        adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[-2].valor_lexico)));
                        adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                        (yyval.nodo) = novo_nodo;
                    }
#line 1754 "parser.tab.c"
    break;

  case 52:
#line 217 "parser.y"
                    {
                        nodo *nodo_vetor = adiciona_nodo_label("[]");
                        adiciona_filho(nodo_vetor, adiciona_nodo((yyvsp[-5].valor_lexico)));
                        adiciona_filho(nodo_vetor, (yyvsp[-3].nodo));

                        nodo *novo_nodo = adiciona_nodo_label("=");
                        adiciona_filho(novo_nodo, nodo_vetor);
                        adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                        (yyval.nodo) = novo_nodo;
                    }
#line 1769 "parser.tab.c"
    break;

  case 53:
#line 230 "parser.y"
                {
                    nodo *novo_nodo = adiciona_nodo_label("input");
                    adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[0].valor_lexico)));
                    (yyval.nodo) = novo_nodo;
                }
#line 1779 "parser.tab.c"
    break;

  case 54:
#line 237 "parser.y"
                {
                    nodo *novo_nodo = adiciona_nodo_label("output");
                    adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[0].valor_lexico)));
                    (yyval.nodo) = novo_nodo;
                }
#line 1789 "parser.tab.c"
    break;

  case 55:
#line 243 "parser.y"
               {
                    nodo *novo_nodo = adiciona_nodo_label("output");
                    adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                    (yyval.nodo) = novo_nodo;
                }
#line 1799 "parser.tab.c"
    break;

  case 56:
#line 251 "parser.y"
                {
                    nodo *novo_nodo = adiciona_nodo((yyvsp[-1].valor_lexico));
                    adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[-2].valor_lexico)));
                    adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[0].valor_lexico)));
                    (yyval.nodo) = novo_nodo;
                }
#line 1810 "parser.tab.c"
    break;

  case 57:
#line 258 "parser.y"
               {
                    nodo *nodo_vetor = adiciona_nodo_label("[]");
                    adiciona_filho(nodo_vetor, adiciona_nodo((yyvsp[-5].valor_lexico)));
                    adiciona_filho(nodo_vetor, (yyvsp[-3].nodo));
                    nodo *novo_nodo = adiciona_nodo((yyvsp[-1].valor_lexico));
                    adiciona_filho(novo_nodo, nodo_vetor);
                    adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[0].valor_lexico)));
                    (yyval.nodo) = novo_nodo;
                }
#line 1824 "parser.tab.c"
    break;

  case 58:
#line 268 "parser.y"
               {
                    nodo *novo_nodo = adiciona_nodo((yyvsp[-1].valor_lexico));
                    adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[-2].valor_lexico)));
                    adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[0].valor_lexico)));
                    (yyval.nodo) = novo_nodo;
                }
#line 1835 "parser.tab.c"
    break;

  case 59:
#line 275 "parser.y"
               {
                    nodo *nodo_vetor = adiciona_nodo_label("[]");
                    adiciona_filho(nodo_vetor, adiciona_nodo((yyvsp[-5].valor_lexico)));
                    adiciona_filho(nodo_vetor, (yyvsp[-3].nodo));
                    nodo *novo_nodo = adiciona_nodo((yyvsp[-1].valor_lexico));
                    adiciona_filho(novo_nodo, nodo_vetor);
                    adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[0].valor_lexico)));
                    (yyval.nodo) = novo_nodo;
                }
#line 1849 "parser.tab.c"
    break;

  case 60:
#line 287 "parser.y"
                {
                    nodo *novo_nodo = adiciona_nodo_label("return");
                    adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                    (yyval.nodo) = novo_nodo;
                }
#line 1859 "parser.tab.c"
    break;

  case 61:
#line 294 "parser.y"
                        {
                            nodo *novo_nodo = adiciona_nodo_label("if");
                            adiciona_filho(novo_nodo, (yyvsp[-2].nodo));
                            adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                            (yyval.nodo) = novo_nodo;
                        }
#line 1870 "parser.tab.c"
    break;

  case 62:
#line 301 "parser.y"
                        {
                            nodo *novo_nodo = adiciona_nodo_label("if");
                            adiciona_filho(novo_nodo, (yyvsp[-4].nodo));
                            adiciona_filho(novo_nodo, (yyvsp[-2].nodo));
                            adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                            (yyval.nodo) = novo_nodo;
                        }
#line 1882 "parser.tab.c"
    break;

  case 63:
#line 311 "parser.y"
                    {
                        nodo *novo_nodo = adiciona_nodo_label("for");
                        adiciona_filho(novo_nodo, (yyvsp[-6].nodo));
                        adiciona_filho(novo_nodo, (yyvsp[-4].nodo));
                        adiciona_filho(novo_nodo, (yyvsp[-2].nodo));
                        adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                        (yyval.nodo) = novo_nodo;
                    }
#line 1895 "parser.tab.c"
    break;

  case 64:
#line 320 "parser.y"
                  {
                      nodo *novo_nodo = adiciona_nodo_label("while");
                      adiciona_filho(novo_nodo, (yyvsp[-3].nodo));
                      adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                      (yyval.nodo) = novo_nodo;
                  }
#line 1906 "parser.tab.c"
    break;

  case 65:
#line 328 "parser.y"
                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1912 "parser.tab.c"
    break;

  case 66:
#line 331 "parser.y"
            {;
                adiciona_filho((yyvsp[-2].nodo), (yyvsp[0].nodo));
                (yyval.nodo) = (yyvsp[-2].nodo);
            }
#line 1921 "parser.tab.c"
    break;

  case 67:
#line 335 "parser.y"
                        { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1927 "parser.tab.c"
    break;

  case 68:
#line 337 "parser.y"
                             { (yyval.nodo) = (yyvsp[0].nodo); }
#line 1933 "parser.tab.c"
    break;

  case 69:
#line 337 "parser.y"
                                           { (yyval.nodo) = NULL; }
#line 1939 "parser.tab.c"
    break;

  case 70:
#line 339 "parser.y"
                     { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico));}
#line 1945 "parser.tab.c"
    break;

  case 71:
#line 340 "parser.y"
                         { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico));}
#line 1951 "parser.tab.c"
    break;

  case 72:
#line 341 "parser.y"
                       { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico));}
#line 1957 "parser.tab.c"
    break;

  case 73:
#line 342 "parser.y"
                        { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico));}
#line 1963 "parser.tab.c"
    break;

  case 74:
#line 343 "parser.y"
                        { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico));}
#line 1969 "parser.tab.c"
    break;

  case 75:
#line 344 "parser.y"
                      { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico));}
#line 1975 "parser.tab.c"
    break;

  case 76:
#line 347 "parser.y"
                            { (yyval.nodo) = adiciona_nodo_label("^"); }
#line 1981 "parser.tab.c"
    break;

  case 77:
#line 348 "parser.y"
                            { (yyval.nodo) = adiciona_nodo_label("*"); }
#line 1987 "parser.tab.c"
    break;

  case 78:
#line 348 "parser.y"
                                                                     { (yyval.nodo) = adiciona_nodo_label("/"); }
#line 1993 "parser.tab.c"
    break;

  case 79:
#line 348 "parser.y"
                                                                                                              { (yyval.nodo) = adiciona_nodo_label("%"); }
#line 1999 "parser.tab.c"
    break;

  case 80:
#line 349 "parser.y"
                            { (yyval.nodo) = adiciona_nodo_label("+"); }
#line 2005 "parser.tab.c"
    break;

  case 81:
#line 349 "parser.y"
                                                                     { (yyval.nodo) = adiciona_nodo_label("-"); }
#line 2011 "parser.tab.c"
    break;

  case 82:
#line 350 "parser.y"
                            { (yyval.nodo) = adiciona_nodo_label("&"); }
#line 2017 "parser.tab.c"
    break;

  case 83:
#line 350 "parser.y"
                                                                     { (yyval.nodo) = adiciona_nodo_label("|"); }
#line 2023 "parser.tab.c"
    break;

  case 84:
#line 351 "parser.y"
                            { (yyval.nodo) = adiciona_nodo_label("<"); }
#line 2029 "parser.tab.c"
    break;

  case 85:
#line 352 "parser.y"
                          { (yyval.nodo) = adiciona_nodo_label(">"); }
#line 2035 "parser.tab.c"
    break;

  case 86:
#line 353 "parser.y"
                               { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2041 "parser.tab.c"
    break;

  case 87:
#line 354 "parser.y"
                               { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2047 "parser.tab.c"
    break;

  case 88:
#line 355 "parser.y"
                               { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2053 "parser.tab.c"
    break;

  case 89:
#line 356 "parser.y"
                               { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2059 "parser.tab.c"
    break;

  case 90:
#line 357 "parser.y"
                               { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2065 "parser.tab.c"
    break;

  case 91:
#line 358 "parser.y"
                                { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2071 "parser.tab.c"
    break;

  case 92:
#line 361 "parser.y"
                        { (yyval.nodo) = adiciona_nodo_label("*"); }
#line 2077 "parser.tab.c"
    break;

  case 93:
#line 363 "parser.y"
                     { (yyval.nodo) = adiciona_nodo_label("-"); }
#line 2083 "parser.tab.c"
    break;

  case 94:
#line 364 "parser.y"
                     { (yyval.nodo) = adiciona_nodo_label("+"); }
#line 2089 "parser.tab.c"
    break;

  case 95:
#line 365 "parser.y"
                     { (yyval.nodo) = adiciona_nodo_label("!"); }
#line 2095 "parser.tab.c"
    break;

  case 96:
#line 366 "parser.y"
                     { (yyval.nodo) = adiciona_nodo_label("&"); }
#line 2101 "parser.tab.c"
    break;

  case 97:
#line 367 "parser.y"
                     { (yyval.nodo) = adiciona_nodo_label("?"); }
#line 2107 "parser.tab.c"
    break;

  case 98:
#line 368 "parser.y"
                     { (yyval.nodo) = adiciona_nodo_label("#"); }
#line 2113 "parser.tab.c"
    break;

  case 99:
#line 371 "parser.y"
                           { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2119 "parser.tab.c"
    break;

  case 100:
#line 373 "parser.y"
            {
                nodo *novo_nodo = adiciona_nodo_label("?:");
                adiciona_filho(novo_nodo, (yyvsp[-4].nodo));
                adiciona_filho(novo_nodo, (yyvsp[-2].nodo));
                adiciona_filho(novo_nodo, (yyvsp[0].nodo));
                (yyval.nodo) = novo_nodo;
            }
#line 2131 "parser.tab.c"
    break;

  case 101:
#line 380 "parser.y"
                                   { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2137 "parser.tab.c"
    break;

  case 102:
#line 382 "parser.y"
                {
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[-2].nodo));
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                    (yyval.nodo) = (yyvsp[-1].nodo);
                }
#line 2147 "parser.tab.c"
    break;

  case 103:
#line 387 "parser.y"
                                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2153 "parser.tab.c"
    break;

  case 104:
#line 389 "parser.y"
                {
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[-2].nodo));
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                    (yyval.nodo) = (yyvsp[-1].nodo);
                }
#line 2163 "parser.tab.c"
    break;

  case 105:
#line 394 "parser.y"
                                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2169 "parser.tab.c"
    break;

  case 106:
#line 396 "parser.y"
                {
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[-2].nodo));
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                    (yyval.nodo) = (yyvsp[-1].nodo);
                }
#line 2179 "parser.tab.c"
    break;

  case 107:
#line 401 "parser.y"
                                     { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2185 "parser.tab.c"
    break;

  case 108:
#line 403 "parser.y"
                {
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[-2].nodo));
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                    (yyval.nodo) = (yyvsp[-1].nodo);
                }
#line 2195 "parser.tab.c"
    break;

  case 109:
#line 408 "parser.y"
                                  { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2201 "parser.tab.c"
    break;

  case 110:
#line 410 "parser.y"
                {
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[-2].nodo));
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                    (yyval.nodo) = (yyvsp[-1].nodo);
                }
#line 2211 "parser.tab.c"
    break;

  case 111:
#line 415 "parser.y"
                                   { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2217 "parser.tab.c"
    break;

  case 112:
#line 417 "parser.y"
                {
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                    (yyval.nodo) = (yyvsp[-1].nodo);
                }
#line 2226 "parser.tab.c"
    break;

  case 113:
#line 422 "parser.y"
                {
                    adiciona_filho((yyvsp[-1].nodo), (yyvsp[0].nodo));
                    (yyval.nodo) = (yyvsp[-1].nodo);
                }
#line 2235 "parser.tab.c"
    break;

  case 114:
#line 427 "parser.y"
                             { (yyval.nodo) = (yyvsp[0].nodo); }
#line 2241 "parser.tab.c"
    break;

  case 115:
#line 428 "parser.y"
                                        { (yyval.nodo) = (yyvsp[-1].nodo); }
#line 2247 "parser.tab.c"
    break;

  case 116:
#line 430 "parser.y"
                           { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2253 "parser.tab.c"
    break;

  case 117:
#line 432 "parser.y"
        { 
            nodo *novo_nodo = adiciona_nodo_label("[]");
            adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[-3].valor_lexico)));
            adiciona_filho(novo_nodo, (yyvsp[-1].nodo));
            (yyval.nodo) = novo_nodo;
        }
#line 2264 "parser.tab.c"
    break;

  case 118:
#line 439 "parser.y"
        { 
            nodo *novo_nodo = adiciona_nodo_label("call");
            adiciona_filho(novo_nodo, adiciona_nodo((yyvsp[-3].valor_lexico)));
            adiciona_filho(novo_nodo, (yyvsp[-1].nodo));
            (yyval.nodo) = novo_nodo;
        }
#line 2275 "parser.tab.c"
    break;

  case 119:
#line 445 "parser.y"
                       { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2281 "parser.tab.c"
    break;

  case 120:
#line 446 "parser.y"
                        { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2287 "parser.tab.c"
    break;

  case 121:
#line 447 "parser.y"
                        { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2293 "parser.tab.c"
    break;

  case 122:
#line 448 "parser.y"
                      { (yyval.nodo) = adiciona_nodo((yyvsp[0].valor_lexico)); }
#line 2299 "parser.tab.c"
    break;


#line 2303 "parser.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

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
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
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
                  yystos[+*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 450 "parser.y"

int yyerror (char const *s) {
   printf("line %d: %s\n", get_line_number(), s);
   return 1;
}

nodo *adiciona_nodo(valorLexico valor_lexico)
{
    nodo *nodo;

    nodo = malloc(sizeof(nodo)*8);
    nodo->valor_lexico = valor_lexico;
    nodo->filhos = NULL;

    return nodo;
}

nodo *adiciona_nodo_label(char *label)
{
    valorLexico valor_lexico;
    valor_lexico.linha = -1;
    valor_lexico.tipo = OUTRO;
    valor_lexico.tipo_literal = NAO_LITERAL;
    valor_lexico.label = strdup(label);

    nodo *nodo;

    nodo = malloc(sizeof(nodo)*8);
    nodo->valor_lexico = valor_lexico;
    nodo->filhos = NULL;

    return nodo;
}

void adiciona_filho(nodo *pai, nodo *filho) 
{
   if(pai!= NULL && filho!=NULL)
   {
       lseNodo *lse_nodo;
       lse_nodo = malloc(sizeof(lse_nodo)*8);
       lse_nodo->nodo = filho;
       lse_nodo->proximo = NULL;
       lse_nodo->nodo->pai = pai;
       if(pai->filhos == NULL)
       {
           pai->filhos = lse_nodo;
       }
       else
       {
           adiciona_irmao(acha_ultimo_filho(pai->filhos), lse_nodo);
       }
   }
   return;
   
}

//TODO função de alterar nodo
//TODO função de remover nodo
void imprime_arvore(nodo *nodo, int profundidade)
{
    int i = 0;

    if (nodo == NULL)
        return;
    
    for(i = 0; i<profundidade-1; i++) 
    {
        printf("    ");
    }

    if (profundidade == 0)
        printf(nodo->valor_lexico.label);
    else 
    {
        printf("+---");
        printf(nodo->valor_lexico.label);
    }
    printf("\n");

    lseNodo *nodo_f;
    nodo_f = nodo->filhos;
    while(nodo_f!=NULL)
    {
            imprime_arvore(nodo_f->nodo, profundidade+1);
            nodo_f = nodo_f->proximo;
    }
    
    return;
}

lseNodo *acha_ultimo_filho(lseNodo *filhos)
{
    lseNodo *aux_nodo = filhos;
    while(aux_nodo->proximo!=NULL)
    {
        aux_nodo = aux_nodo->proximo;
    }
    return aux_nodo;
}

void adiciona_irmao(lseNodo *irmao, lseNodo *novo_irmao)
{
    irmao->proximo = novo_irmao;
    novo_irmao->proximo = NULL;
    return;
}

void libera(void *pai)
{
    if(pai == NULL) return;

    nodo *pai_arvore = (nodo*)pai;

    lseNodo *filhos = pai_arvore->filhos;

    libera_nodo(pai_arvore);
    libera_irmaos(filhos);
    return;
}

void libera_irmaos(void *filhos)
{
    if(filhos == NULL) return;

    lseNodo *irmaos = (lseNodo*)filhos; 

    nodo *nodo_irmao = irmaos->nodo;
    lseNodo *proximo = irmaos->proximo;
    
    free(irmaos);

    libera(nodo_irmao);
    libera_irmaos(proximo);
    return;
}

void libera_nodo(nodo *nodo)
{
    valorLexico valor_lexico = nodo->valor_lexico;
    free(nodo);
    libera_valor_lexico(valor_lexico);
    return;
}

void libera_valor_lexico(valorLexico valor_lexico)
{
    if(valor_lexico.tipo_literal == STRING)
        free(valor_lexico.valor.valor_string);
    if(valor_lexico.label != NULL)
        free(valor_lexico.label);
    return;
}
