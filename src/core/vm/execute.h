/* File: execute.h */
/* Creation Date: 2017-01-31 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The main virtual machine function */

#ifndef VM_EXECUTE_H
#define VM_EXECUTE_H

#include "lib/types.h"
#include "core/vm/bytecode/bytecode.h"

bool execute(struct ByteCode const* bc);

#endif /* VM_EXECUTE_H */
