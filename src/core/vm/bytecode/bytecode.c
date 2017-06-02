/* File: bytecode.c */
/* Creation Date: 2017-01-15 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* VM bytecode */

#include "core/vm/bytecode/bytecode.h"

#include "err.h"

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xmalloc.h"

#include "core/object/object.h"
#include "core/vm/instruction/instruction.h"

/********************/
/* ByteCode helpers */
/********************/

NoRet bytecode_init(struct ByteCode* bc)
{
  bc->immediates            = NULL;
  bc->instructions_count    = 0;
  bc->instructions_capacity = BYTECODE_INIT_CAPACITY;
  bc->instructions = xmalloc(BYTECODE_INIT_CAPACITY * sizeof(Instruction));
}

NoRet bytecode_deinit(struct ByteCode* bc)
{
  if (bc->immediates) {
    for (uint64 i = 0; i < bc->immediates_count; ++i) {
      obj_decr_ref_count(bc->immediates[i]);
    }
    xfree(CAST(bc->immediates, void*));
  }
  xfree(bc->instructions);
}

struct ByteCode* bytecode_new(void)
{
  struct ByteCode* bc = xmalloc(sizeof(*bc));
  bytecode_init(bc);
  return bc;
}

NoRet bytecode_delete(struct ByteCode* bc)
{
  bytecode_deinit(bc);
  xfree(bc);
}

Instruction bytecode_get_instr(const struct ByteCode* bc, const uint64 i)
{
  DASSERT(i < bc->instructions_count,
          "tried to access non-existant instruction in bytecode");
  return bc->instructions[i];
}

bool bytecode_append_instr(struct ByteCode* bc, const Instruction instr)
{
  if (bc->instructions_count >= bc->instructions_capacity) {
    bc->instructions_capacity += BYTECODE_ALLOC_SIZE;
    bc->instructions = xrealloc(
      bc->instructions, bc->instructions_capacity * sizeof(Instruction));

    if (!bc->instructions) {
      err_set_not_enough_memory();
      /* Fail */
      return false;
    }
  }

  bc->instructions[bc->instructions_count++] = instr;
  /* Succeed */
  return true;
}

bool bytecode_insert_instr(struct ByteCode* bc,
                           const uint64 pos,
                           const Instruction instr)
{
  if ((pos + 1) >= bc->instructions_capacity) {
    bc->instructions_capacity = pos + 1 + BYTECODE_ALLOC_SIZE;
    bc->instructions          = xrealloc(
      bc->instructions, bc->instructions_capacity * sizeof(*bc->instructions));

    if (!bc->instructions) {
      err_set_not_enough_memory();
      /* Fail */
      return false;
    }
    bc->instructions_count = bc->instructions_capacity;
  }

  bc->instructions[pos] = instr;
  return true;
}

NoRet bytecode_print(struct ByteCode* bc)
{
  printf("Bytecode dump:\n");

  /* Print the immediates */
  printf("\tImmediates:\n");
  printf("\t{\n");
  if (bc->immediates) {
    printf("\t\tCount: %lu\n", bc->immediates_count);
    for (uint64 i = 0; i < bc->immediates_count; ++i) {
      printf("\t\t[%lu] %-8s", i, " ");
      struct Object* obj = bc->immediates[i];
      obj_print(obj);
      printf("%-4s", " ");

      switch (obj->type) {
        case OBJT_INUM: printf("\"%li\"\n", obj->data.inum.val); break;

        case OBJT_DNUM: printf("\"%fl\"\n", obj->data.dnum.val); break;

        case OBJT_STR: printf("\"%s\"\n", obj->data.str.val); break;

        default: IMPOSSIBLE(); return;
      }
    }
  } else {
    printf("\t\tCount: 0\n");
  }
  printf("\t}\n");

  /* Print the instructions */
  printf("\tInstructions:\n");
  printf("\t{\n");
  printf("\t\tCount: %lu\n", bc->instructions_count);
  printf("\t\tSize of one instruction (in bytes): %lu\n", sizeof(Instruction));
  for (uint64 i = 0; i < bc->instructions_count; ++i) {
    Instruction instr        = bytecode_get_instr(bc, i);
    char const* instr_code_s = instr_code2str(I_OP_GET(instr));
    printf("\t\t[%lu] %-8s %-8s %d  %d  %d\n",
           i,
           " ",
           instr_code_s,
           I_A_GET(instr),
           I_B_GET(instr),
           I_C_GET(instr));
  }
  printf("\t}\n");
}
