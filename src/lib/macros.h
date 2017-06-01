/* File: macros.h */
/* Creation Date: 2017-01-03*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The macros */

#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <string.h>
#include <assert.h>

/***********************/
/* Character constants */
/***********************/

#define SPACE_CH ' '
#define TAB_CH '\t'
#define CR_CH '\n'
#define LF_CH '\r'
#define SEMI_CH ';'
#define COLON_CH ':'
#define COMMA_CH ','
#define LBRACKET_CH '['
#define RBRACKET_CH ']'
#define LPAREN_CH '('
#define RPAREN_CH ')'
#define LBRACE_CH '{'
#define RBRACE_CH '}'
#define PLUS_CH '+'
#define MINUS_CH '-'
#define STAR_CH '*'
#define SLASH_CH '/'
#define MODULUS_CH '%'
#define LT_CH '<'
#define GT_CH '>'
#define AMP_CH '&'
#define EXCLAM_CH '!'
#define PIPE_CH '|'
#define TILDA_CH '~'
#define DOLLAR_CH '$'
#define HASH_CH '#'
#define CARET_CH '^'
#define EQ_CH '='
#define AT_CH '@'
#define STR_END_CH '\0'
#define QUOTE_CH '"'
#define DOT_CH '.'
#define BACKSLASH_CH '\\'
#define EOF_CH '\377'
#define UNDERSCORE_CH '_'

/*******************************/
/* Terminal coloring constants */
/*******************************/

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define KRESET "\x1B[0m"

/*************/
/* Constants */
/*************/

#define PTR_SIZE sizeof(void*)

/**********/
/* Macros */
/**********/

#define PASS()
#define CAST(val, type) ((type)(val))
#define UNUSED(a) (void)a
#define ASCII_DIGIT2NORMAL_DIGIT(ASCII_DIGIT_CH) (ASCII_DIGIT_CH - '0')
#define NORMAL_DIGIT2ASCII_DIGIT(NORMAL_DIGIT) (NORMAL_DIGIT + '0')
#define STREQ(s1, s2) (strcmp(s1, s2) == 0)
#define UNLIKELY() do {							\
        printf("[%s:%d] Shouldn't be here\n", __FILE__, __LINE__);	\
        assert(0);							\
    } while (0)
#define IMPOSSIBLE() UNLIKELY()

#define BYTES(n) (8 * (n))
#define KILOBYTES(n) (1024 * BYTES((n)))
#define MEGABYTES(n) (1024 * KILOBYTES((n)))
#define GIGABYTES(n) (1024 * MEGABYTES((n)))

#if DEBUG
#  define DASSERT(___cond, ___msg) do {		\
        if (!(___cond)) {				\
            printf(					\
                "[%s:%d] Assertion failed (%s): %s\n",	\
                __FILE__, __LINE__, #___cond, ___msg);	\
            exit(1);					\
        }						\
    } while (0);
#  define DASSERT_IF(___cond, ___cond_if, ___msg) do {		\
        if ((___cond_if) && !(___cond)) {				\
            printf(							\
                "[%s:%d] Assertion failed ((%s) && (%s)): %s\n",	\
                __FILE__, __LINE__, #___cond_if, #___cond, __msg);		\
            exit(1);							\
        }								\
    } while (0)
#  define PRINT_DEBUG_MSG(...) do {	\
        printf(__VA_ARGS__);		\
    } while (0)
#else
#  define DASSERT(cond, msg)
#  define DASSERT_IF(cond, cond_if, msg)
#  define PRINT_DEBUG_MSG(___frmt, ...)
#endif /* DEBUG */

#if DEBUG && DEBUG_VERBOSE
#  define PRINT_VDEBUG_MSG(...) PRINT_DEBUG_MSG(__VA_ARGS__)
#else
#  define PRINT_VDEBUG_MSG(___frmt, ...)
#endif /* DEBUG && DEBUG_VERBOSE */

#endif /* MACROS_H */
