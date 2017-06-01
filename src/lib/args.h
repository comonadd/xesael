/* File: args.h */
/* Creation Date: 2017-01-05*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#ifndef ARGS_H
#define ARGS_H

#include "lib/types.h"

struct Arguments {
    uint64 count;
    char** args;
};

struct ArgumentAction {
    char s;
    char* l;
    bool (*action)(uint8 i, struct Arguments const*);
};

#endif /* ARGS_H */
