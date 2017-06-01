/* File: dumper.h */
/* Creation Date: 2017-01-16 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The GSh bytecode dumper */

#ifndef GSH_DUMPER_H
#define GSH_DUMPER_H

#include "lib/types.h"
#include "core/vm/bytecode/bytecode.h"

bool dump(
    struct ByteCode const* bc,
    char const* filename);

#endif /* GSH_DUMPER_H */
