/* File: vm.h */
/* Creation Date: 2017-01-14*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The virtual machine */

#ifndef VM_H
#define VM_H

#include "lib/types.h"
#include "lib/xvec.h"

#include "core/object/object.h"
#include "core/vm/bytecode/bytecode.h"
#include "core/vm/env.h"

struct VirtualMachine {
    struct Object const* registers[256];
};

/*************/
/* Constants */
/*************/

extern struct VirtualMachine vm;

/**************************************/
/* Helpers for working with registers */
/**************************************/

#define vm_set_reg(___id, ___obj) (vm.registers[(___id)] = (___obj))
#define vm_get_reg(___id) (vm.registers[(___id)])

#endif /* VM_H */
