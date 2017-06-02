/* File: str.h */
/* Creation Date: 2017-01-27 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Helpers for working with strings */

#ifndef STR_H
#define STR_H

#include "types.h"

/********/
/* Main */
/********/

/**
   $ Description:
   #   This function generates an
   #   random string
**/
char* randstring(const uint64 len);

/**
   $ Description:
   #   This function reverses the
   #   string in-place
**/
NoRet str_reverse(char* s);

/**
   $ Description:
   #   This function concatenates
   #   the two strings together
**/
char* str_concat(const char* s1, const char* s2);

/**
   $ Description:
   #   This function "multiplies"
   #   string by the given number n
   #   ("multiply" means concatenate n times with itself)
   @str - string to operate on
   @len - length of a given string
   @n - number of times to concatenate
**/
char* str_mul(const char* str, const uint64 len, uint64 n);

/**
   $ Description:
   #   This function concatenates all the items in the
   #   given array of strings together
**/
char* str_arr_concat(const char** arr, uint64 size);

/* NOTE: This two functions do not handle */
/*       invalid number literals */
int64 str2int(const char* str);

double str2double(const char* str);

/**
   $ Description:
   #   This function compares
   #   two strings together
   #   (it is needed for some DS interfaces)
**/
int str_cmp_f(const void* str1, const void* str2);

#endif /* STR_H */
