/* File: str.c */
/* Creation Date: 2017-01-27 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Helpers for working with strings */

#include "str.h"

#include <math.h>

#include "macros.h"
#include "xmalloc.h"

/********/
/* Main */
/********/

char* randstring(const uint64 len)
{
    static char* charset = {
	"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!"
    };
    char* res = NULL;

    if (len) {
	res = xmalloc((len + 1) * sizeof(*res));
	if (res) {
	    for (uint64 n = 0; n < len; ++n) {
		int key = rand() % (int)(sizeof(charset) - 1);
		res[n] = charset[key];
	    }
	    res[len] = STR_END_CH;
	}
    }

    return res;
}

NoRet str_reverse(char* s)
{
    int i, j;

    for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
	char c = s[i];
	s[i] = s[j];
	s[j] = c;
    }
}

char* str_concat(
    const char* s1,
    const char* s2)
{
    char* new_s;
    uint64 s1_len = strlen(s1);
    uint64 s2_len = strlen(s2);
    uint64 new_s_len = s1_len + s2_len;
    new_s = xmalloc((new_s_len + 1) * sizeof(*new_s));

    for (uint64 i = 0; i < s1_len; ++i)
	new_s[i] = s1[i];

    for (uint64 i = s1_len; i < new_s_len; ++i)
	new_s[i] = s2[i - s1_len];
    new_s[new_s_len] = STR_END_CH;

    return new_s;
}

char* str_mul(
    const char* str,
    const uint64 len,
    uint64 n)
{
    char* res = xmalloc((n * len * sizeof(*res)) + 1);
    uint64 res_len = 0;
    while (n > 0) {
	/* Append the given string to the result */
	memcpy(&res[res_len], str, len);
	res_len += len;
	--n;
    }
    res[res_len] = STR_END_CH;
    return res;
}

char* str_arr_concat(
    const char** arr,
    uint64 size)
{
    char* res = NULL;
    uint64 res_len = 0;

    while (size > 0) {
	uint64 item_len = strlen(arr[size - 1]);
	res = xrealloc(res, (item_len + res_len) * sizeof(char));
	memcpy(&res[res_len], arr[size - 1], item_len);
	res_len += item_len;
	--size;
    }

    return res;
}

int64 str2int(const char* str)
{
    uint64 len = strlen(str);
    int64 val = 0;
    bool neg = false;

    if (*str == MINUS_CH) {
	neg = true;
	++str;
	--len;
    }

    while (len > 0) {
	val *= 10;
	val += ASCII_DIGIT2NORMAL_DIGIT(*str);
	++str;
	--len;
    }

    return neg ? -val : val;
}

double str2double(const char* str)
{
    uint64 len = strlen(str);
    double val = 0;
    bool neg = false;

    if (*str == MINUS_CH) {
	neg = true;
	++str;
	--len;
    }

    /* Retreiving all the digits before dot */
    while (*str != DOT_CH) {
	val *= 10;
	val += ASCII_DIGIT2NORMAL_DIGIT(*str);
	++str;
	--len;
    }

    /* Skipping the dot */
    ++str;
    --len;

    /* Retreiving all the digits after dot */
    double pos_after_dot = 1;
    while (len > 0) {
	val += ASCII_DIGIT2NORMAL_DIGIT(*str) / pow(10, pos_after_dot);
	++pos_after_dot;
	++str;
	--len;
    }

    return neg ? -val : val;
}

int str_cmp_f(
    const void* str1,
    const void* str2)
{
    return strcmp(str1, str2);
}
