/* File: err.h */
/* Creation Date: 2017-01-14 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Error reporting system */

#ifndef XESAEL_ERR_INC
#define XESAEL_ERR_INC

#include "lib/types.h"

/**
 * Enumeration that is used for error reporting
 * @E_OK - no error, the default state
 * @E_ARG - argument error, thrown, at the argument parsing stage
 * @E_SYS - system error, thrown, for example, when there is no file at a given
 *          path
 * @E_INTERNAL - internal error, thrown when the error in the actual code
 * @E_COMPILE - compile error, thrown at the compile stage (compiler)
 *  @E_EXEC - execution error, thrown at the execution stage (VM)
 */
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

/**
 * Set the custom error
 * @return true on success, false if there is no enough memory
 */
bool err_set_custom(char const* frmt, ...);

/**
 * Set the error with the given type and message
 * @type - type of the error
 * @msg - error message
 */
NoRet err_set(const enum ErrorType type, char const* msg);

/**
 * Set the error with the given type and message.
 * Formatting is supported.
 * @type - type of the error
 * @return true on success, false if there is no enough memory
 */
bool err_setf(const enum ErrorType type, char const* frmt, ...);

/**
 * Set the error type and message from the errno
 */
NoRet err_set_from_errno(void);

// TODO: Document those 4 macros
#define err_set_not_enough_memory() err_set(E_SYS, "not enough memory")
#define err_set_path_not_exists() err_set(E_SYS, "path does not exist")
#define err_set_unknown() err_set(E_INTERNAL, "unknown error")
#define err_set_no_such_file_or_dir(___path) \
  err_setf(E_SYS, "no such file or directory \"%s\"", (___path))

/**
 * Get the error message that is set globally
 * @return the pointer to the globally set error message
 */
char const* err_get_msg(void);

/**
 * Reset the error status
 */
NoRet err_reset(void);

/**
 * Predicate. Check if error is E_OK
 * @return true if error is E_OK, false otherwise
 */
bool err_is_ok(void);

/**
 * Report the error that is set globally
 */
NoRet err_report(void);

#endif /* XESAEL_ERR_INC */
