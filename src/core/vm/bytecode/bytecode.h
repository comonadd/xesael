/* File: bytecode.h */
/* Creation Date: 2017-01-14 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* VM bytecode */

#ifndef VM_BYTECODE_H
#define VM_BYTECODE_H

#include "lib/dict.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xvec.h"

#include "core/vm/instruction/instruction.h"

#define BYTECODE_ALLOC_SIZE 128
#define BYTECODE_INIT_CAPACITY (BYTECODE_ALLOC_SIZE + BYTECODE_ALLOC_SIZE)

/**
   $ Description:
   #   This structure represents an GSh byte-code
   @immediates - array of immediates (literals) represented as a objects
   @raw - the actual code of a program
**/
struct ByteCode {
  struct Object* const* immediates;
  uint64 immediates_count;
  Instruction* instructions;
  uint64 instructions_count;
  uint64 instructions_capacity;
};

/********************/
/* Bytecode helpers */
/********************/

NoRet bytecode_init(struct ByteCode* bc);

NoRet bytecode_deinit(struct ByteCode* bc);

struct ByteCode* bytecode_new(void);

NoRet bytecode_delete(struct ByteCode* bc);

Instruction bytecode_get_instr(const struct ByteCode* bc, const uint64 i);

bool bytecode_append_instr(struct ByteCode* bc, const Instruction instr);

bool bytecode_insert_instr(struct ByteCode* bc,
                           const uint64 pos,
                           const Instruction instr);

NoRet bytecode_print(struct ByteCode* bc);

#endif /* VM_BYTECODE_H */
