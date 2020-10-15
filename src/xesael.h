/* File: xesael.h */
/* Creation Date: 2017-02-01 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The main entry point */

#ifndef XESAEL_XESAEL_INC
#define XESAEL_XESAEL_INC

#include "lib/args.h"
#include "lib/types.h"

/**
 * The main function of the interpreter.
 * It is called by the main() function that is executed by the platform.
 * @args - Command-line arguments
 * @return integer POSIX exit code
 */
int xesael_main(struct Arguments const* args);

#endif /* XESAEL_XESAEL_INC */
