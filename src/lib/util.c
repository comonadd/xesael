/* File: util.c */
/* Creation date: 10.12.2016 */
/* Author: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */

#include "util.h"

#include <ctype.h>
#include <string.h>
#include <math.h>

#include "types.h"
#include "macros.h"
#include "xmalloc.h"
#include "dbl.h"
#include "str.h"

char* get_line_from_text(
    const char* text,
    const uint64 ln)
{
    /* If text is NULL => return NULL */
    /* If requested line num is equals to 0 => return NULL */
    if (!text || ln == 0) return NULL;
    /* current text pos = 0 */
    uint64 curr_text_pos = 0;
    /* current line num = 1 */
    uint64 curr_ln = 1;
    uint64 text_len = strlen(text);

    /* For each character in text: */
    while (curr_text_pos < text_len) {
	/* If current line num is equal to requested: */
	if (curr_ln == ln) {
	    /* size = 0 */
	    uint64 line_size = 0;

	    /* Until we see a newline character at current text position + size: */
	    while (!ch_isnewline(text[curr_text_pos + line_size])) {
		/* Increment size */
		++line_size;
	    }

	    /* Allocate string of that size */
	    char* line = xmalloc((line_size + 1) * sizeof(char));
	    uint64 curr_line_pos = 0;
	    /* Until size is greater than 0: */
	    while (line_size > 0) {
		/* Append character in the text at the current position to string */
		line[curr_line_pos] = text[curr_text_pos];
		/* Increment current text pos */
		++curr_text_pos;
		/* Increment current line pos */
		++curr_line_pos;
		/* Decrement size */
		--line_size;
	    }
	    /* Set last char of string to STR_END_CH */
	    line[curr_line_pos++] = STR_END_CH;
	    /* Return allocated string */
	    return line;
	}
	if (ch_isnewline(text[curr_text_pos])) ++curr_ln;
	++curr_text_pos;
    }

    /* Requested line number is greater than */
    /* the actual count of lines in given piece of text */
    return NULL;
}

NoRet itoa(
    int n,
    char s[32])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
	n = -n;          /* make n positive */

    i = 0;

    do {       /* generate digits in reverse order */
	s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
	s[i++] = '-';
    s[i] = STR_END_CH;
    str_reverse(s);
}

bool ch_isspace(const char ch)
{
    return
	(ch == SPACE_CH) ||
	(ch == '\t') ||
	(ch == '\v') ||
	(ch == '\f');
}

bool ch_isnewline(const char ch)
{
    return (ch == CR_CH) || (ch == LF_CH);
}

#if TEST
#include <assert.h>

static NoRet test_str_reverse(void)
{
    char str[] = "something";
    str_reverse(str);
    assert(STREQ(str, "gnihtemos"));
}

static NoRet test_str_concat(void)
{
    char* s1 = "some";
    char* s2 = "thing";
    char* res = str_concat(s1, s2);
    assert(STREQ(res, "something"));
    xfree(res);
}

static NoRet test_str2int(void)
{
    assert(str2int("2") == 2);
    assert(str2int("512") == 512);
    assert(str2int("-512") == -512);
}

static NoRet test_str2double(void)
{
    assert(dbl_eq(str2double("2.123123"), 2.123123));
    assert(dbl_eq(str2double(".123123"), 0.123123));
    assert(dbl_eq(str2double("512.123123"), 512.123123));
    assert(dbl_eq(str2double("-512.123123"), -512.123123));
}

static NoRet test_get_line_from_text(void)
{
    char text0[] = "something\nwhatever\n\n\n\n";
    char* res;

    res = get_line_from_text(text0, 1);
    assert(STREQ(res, "something"));
    xfree(res);

    res = get_line_from_text(text0, 2);
    assert(STREQ(res, "whatever"));
    xfree(res);

    assert(!get_line_from_text(text0, 0));
    assert(!get_line_from_text(text0, 123));
}

static NoRet test_itoa(void)
{
    int a = 32;
    char str[32];
    itoa(a, str);
    assert(STREQ(str, "32"));
}

NoRet test_util(void)
{
    test_str_reverse();
    printf("\t[\"test_str_reverse\"]: passed\n");
    test_str2int();
    printf("\t[\"test_str2int\"]: passed\n");
    test_str2double();
    printf("\t[\"test_str2double\"]: passed\n");
    test_get_line_from_text();
    printf("\t[\"test_get_line_from_text\"]: passed\n");
    test_itoa();
    printf("\t[\"test_itoa\"]: passed\n");
}

#endif /* TEST */
