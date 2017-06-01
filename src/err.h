/* File: err.h */
/* Creation Date: 2017-01-14 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Error reporting system */

#ifndef ERR_H
#define ERR_H

#include "lib/types.h"

/**
   $ Description:
   #   This enumeration used for error reporting
   @E_OK - no error, the default state
   @E_ARG - argument error, thrown, at the argument parsing stage
   @E_SYS - system error, thrown, for example, when there is no file at a given path
   @E_INTERNAL - internal error, thrown when the error in the actual code
   @E_COMPILE - compile error, thrown at the compile stage (compiler)
   @E_EXEC - execution error, thrown at the execution stage (VM)
**/
enum ErrorType {
    E_OK,
    E_ARG,
    E_SYS,
    E_INTERNAL,
    E_COMPILE,
    E_EXEC,
    E_CUSTOM,
    E_TYPES_COUNT
};

/********/
/* Main */
/********/

bool err_set_custom(
    char const* frmt,
    ...);

NoRet err_set(
    const enum ErrorType type,
    char const* msg);

bool err_setf(
    const enum ErrorType,
    char const* frmt,
    ...);

NoRet err_set_from_errno(void);

#define err_set_not_enough_memory() err_set(E_SYS, "not enough memory")
#define err_set_path_not_exists() err_set(E_SYS, "path does not exist")
#define err_set_unknown() err_set(E_INTERNAL, "unknown error")
#define err_set_no_such_file_or_dir(___path) \
    err_setf(E_SYS, "no such file or directory \"%s\"", (___path))

char const* err_get_msg(void);

NoRet err_reset(void);

bool err_is_ok(void);

NoRet err_report(void);

#endif /* ERR_H */
