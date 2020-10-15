/* File: translator.c */
/* Creation Date: 2017-01-15 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The translator of AST into the bytecode */

#include "core/compiler/translator/translator.h"

#include "core/compiler/ast/ast.h"
#include "core/compiler/sym/sym.h"
#include "core/compiler/sym/symtable.h"
#include "core/compiler/translator/translator.h"
#include "core/vm/immediate/immediate.h"
#include "core/vm/immediate/immediate_list.h"
#include "err.h"
#include "lib/types.h"

/***********/
/* Methods */
/***********/

bool translator_init(struct Translator* T)
{
  /* Obvious check */
  DASSERT(T, "");

  T->st = st_new();
  if (!T->st) return false;

  immediate_list_init(&T->il);
  T->curr_rid = 0;
  T->buf      = bytecode_new();
  if (!T->buf) return false;

  /* Succeed */
  return true;
}

NoRet translator_deinit(struct Translator* T)
{
  /* Obvious check */
  DASSERT(T, "");

  st_delete(T->st);
  immediate_list_deinit(&T->il);
}

struct Translator* translator_new(void)
{
  struct Translator* T = xmalloc(sizeof(*T));
  translator_init(T);
  return T;
}

NoRet translator_delete(struct Translator* T)
{
  translator_deinit(T);
  xfree(T);
}

InstructionPosition translator_get_ip(struct Translator* T)
{
  return T->buf->instructions_count;
}

InstructionPosition translator_reserve_ip(struct Translator* T)
{
  return T->buf->instructions_count++;
}

RegisterID translator_get_free_reg(struct Translator* T)
{
  return T->curr_rid++;
}

NoRet translator_free_regs(struct Translator* T)
{
  T->curr_rid = 0;
}

bool translator_st_insert(struct Translator* T, struct Symbol* sym)
{
  return st_insert(T->st, sym);
}

struct Symbol* translator_st_lookup(struct Translator* T, char const* name)
{
  return st_lookup(T->st, name);
}

NoRet translator_append_instr(struct Translator* T, const Instruction i)
{
  bytecode_append_instr(T->buf, i);
}

struct Immediate* translator_immediate_insert(struct Translator* T,
                                              char* name,
                                              struct Object* obj)
{
  return immediate_list_insert(&T->il, name, obj);
}

struct Immediate* translator_immediate_lookup(struct Translator* T,
                                              char const* name)
{
  return immediate_list_lookup(&T->il, name);
}

NoRet translator_insert_instr(struct Translator* T,
                              const InstructionPosition pos,
                              const Instruction i)
{
  bytecode_insert_instr(T->buf, pos, i);
}

NoRet translator_enter_scope(struct Translator* T)
{
  T->st->prev = T->st;
  T->st       = st_new();
}

NoRet translator_leave_scope(struct Translator* T)
{
  struct SymbolTable* prev_st = T->st->prev;
  st_delete(T->st);
  T->st = prev_st;
}

/*******************/
/* Getters/Setters */
/*******************/

RegisterID translator_get_curr_rid(struct Translator* T)
{
  return T->curr_rid;
}

NoRet translator_set_curr_rid(struct Translator* T, const RegisterID rid)
{
  T->curr_rid = rid;
}

/**************/
/* Benchmarks */
/**************/

#if BENCH
NoRet bench_translator(void)
{
}
#endif /* BENCH */

/*********/
/* Tests */
/*********/

#if TEST
#include <assert.h>
NoRet test_compiler(void)
{
}
#endif /* TEST */
