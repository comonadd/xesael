/* File: compiler.h */
/* Creation Date: 2017-02-01 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The compiler */

#ifndef COMPILER_H
#define COMPILER_H

#include "lib/types.h"

struct Compiler {
    char const* src_name;
};

/********************/
/* Global variables */
/********************/

extern struct Compiler compiler;

/***********/
/* Methods */
/***********/

/**
   $ Description:
   #   This function prepares the compiler
   #   to the work
**/
NoRet compiler_prepare(char const* src_name);

NoRet compiler_err_set(
    const LineNumber ln,
    char const* msg);

NoRet compiler_err_setf(
    const LineNumber ln,
    char const* frmt,
    ...);

#endif /* COMPILER_H */
