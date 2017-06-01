/* File: compiler.c */
/* Creation Date: 2017-02-01 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The compiler */

#include "core/compiler/compiler.h"

#include <stdio.h>
#include <stdarg.h>

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"

#include "err.h"

/********************/
/* Global variables */
/********************/

struct Compiler compiler;

/***********/
/* Methods */
/***********/

NoRet compiler_prepare(char const* src_name)
{
    compiler.src_name = src_name;
}

NoRet compiler_err_set(
    const LineNumber ln,
    char const* msg)
{
    err_set_custom(
        "[!] Compiler Error [%s, %lu]: %s\n",
        compiler.src_name, ln, msg);
}

NoRet compiler_err_setf(
    const LineNumber ln,
    char const* frmt, ...)
{
    /* Initialize the arguments */
    va_list vargs;
    va_start(vargs, frmt);

    /* Format the message */
    char* msg;
    vasprintf(&msg, frmt, vargs);

    /* Set the error */
    compiler_err_set(ln, msg);

    /* Free all the stuff */
    va_end(vargs);
    xfree(msg);
}
