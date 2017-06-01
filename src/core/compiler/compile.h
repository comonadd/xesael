/* File: compile.h */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* compile() and compile_file() functions */

#ifndef COMPILE_H
#define COMPILE_H

#include "core/vm/bytecode/bytecode.h"

/********/
/* Main */
/********/

struct ByteCode* compile(
    char const* src_name,
    char const* text);

struct ByteCode* compile_file(
    char const* path);

#endif /* COMPILE_H */
