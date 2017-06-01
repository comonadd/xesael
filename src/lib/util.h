/* File: util.h */
/* Creation date: 10.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */

#ifndef UTIL_H
#define UTIL_H

#include "types.h"

#define ARR_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

char* get_line_from_text(
    const char* text,
    const uint64 ln);

NoRet itoa(
    int val,
    char s[32]);

bool ch_isspace(const char ch);

bool ch_isnewline(const char ch);

#if TEST
NoRet test_util(void);
#endif /* TEST */
#endif /* UTIL_H */
