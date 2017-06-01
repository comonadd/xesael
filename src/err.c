/* File: err.c */
/* Creation Date: 2017-01-14 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Error reporting system */

#include "err.h"

#include <stdarg.h>
#include <stdio.h>
#include <errno.h>

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"
#include "lib/util.h"
#include "lib/str.h"

#include "log.h"

/*********************/
/* Private constants */
/*********************/

static char const* E_PREFIXES[E_TYPES_COUNT] = {
    "[!] Invalid Error: ",
    "[!] Argument Error: ",
    "[!] System Error: ",
    "[!] Internal Error: ",
    "[!] Compile Error: ",
    "[!] Execution Error: "
};

/*********************/
/* Private variables */
/*********************/

static enum ErrorType __err_type;
static char* __err_msg = NULL;

/**********************/
/* Private prototypes */
/**********************/

static NoRet __err_set_type(const enum ErrorType type);
static NoRet __err_set_msg(char* msg);

/***********/
/* Helpers */
/***********/

static NoRet __err_set_type(const enum ErrorType type)
{
    __err_type = type;
}

static NoRet __err_set_msg(char* msg)
{
    __err_msg = msg;
}

/**************/
/* Main stuff */
/**************/

NoRet err_set(
    const enum ErrorType type,
    char const* msg)
{
    /* Set the type of a error */
    __err_set_type(type);

    /* Set the error message */
    __err_msg = str_concat(E_PREFIXES[type], msg);
}

bool err_setf(
    const enum ErrorType type,
    const char* frmt,
    ...)
{
    /* Set the type of a error */
    __err_set_type(type);

    va_list args;
    va_start(args, frmt);

    char* buf;
    if (vasprintf(&buf, frmt, args) == -1) {
        /* Not enough memory */
        err_set_not_enough_memory();
        va_end(args);
        /* Fail */
        return false;
    }

    /* Set the error message */
    __err_set_msg(str_concat(E_PREFIXES[type], buf));

    /* Succeed */
    xfree(buf);
    va_end(args);
    return true;
}

bool err_set_custom(
    char const* frmt,
    ...)
{
    /* Set the type of a error */
    __err_set_type(E_CUSTOM);

    va_list args;
    va_start(args, frmt);

    if (vasprintf(&__err_msg, frmt, args) == -1) {
        /* Not enough memory */
        err_set_not_enough_memory();
        va_end(args);
        /* Fail */
        return false;
    }

    /* Succeed */
    va_end(args);
    return true;
}

NoRet err_set_from_errno(void)
{
    switch (errno) {
        case EACCES:
            err_set(E_SYS, "permission denied");
            break;
        case ENOENT:
            err_set(E_SYS, "no such file or directory");
            break;
        default:
            err_set_unknown();
            LOG_DEBUG(
                "not handled \"errno\" case in a switch statement");
            break;
    }
}

char const* err_get_msg(void)
{
    return __err_msg;
}

NoRet err_reset(void)
{
    __err_type = E_OK;
    if (__err_msg) free(__err_msg);
    __err_msg = NULL;
}

bool err_is_ok(void)
{
    return __err_type == E_OK;
}

NoRet err_report(void)
{
    puts(__err_msg);
}
