/* File: args.h */
/* Creation Date: 2017-02-02 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#ifndef TASTE_ARGS_H
#define TASTE_ARGS_H

#include "lib/types.h"
#include "lib/xmalloc.h"
#include "lib/args.h"

#define INCLUDES_MAX_COUNT 64
#define DEFAULT_OUTPUT_PATH "output.ttc"

/**
   @opt_lvl - optimization level
   @path - path
   @output_path - output path
   @includes - the array that represents a names to include (import at start)
**/
struct TasteArgumentsInfo {
    uint8 opt_lvl;
    char const* path;
    char const* output_path;
    char** includes;
};

/********/
/* Main */
/********/

struct TasteArgumentsInfo* taste_parse_args(
    struct Arguments const* args);

#endif /* TASTE_ARGS_H */
