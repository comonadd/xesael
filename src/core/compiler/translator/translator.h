/* File: translator.h */
/* Creation Date: 2017-01-15 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The translator of AST into the bytecode */

#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "lib/types.h"

#include "core/vm/bytecode/bytecode.h"
#include "core/vm/register.h"
#include "core/compiler/ast/ast.h"
#include "core/compiler/sym/symtable.h"
#include "core/vm/immediate/immediate_list.h"

/**
   @st - symbol table
   @il - immediate list
   @curr_rid - free register ID
   @bc - output bytecode
**/
struct Translator {
    struct SymbolTable* st;
    ImmediateList il;
    RegisterID curr_rid;
    struct ByteCode* buf;
};

/***********/
/* Methods */
/***********/

bool translator_init(struct Translator* T);

NoRet translator_deinit(struct Translator* T);

struct Translator* translator_new(void);

NoRet translator_delete(struct Translator* T);

InstructionPosition translator_get_ip(struct Translator* T);

InstructionPosition translator_reserve_ip(struct Translator* T);

NoRet translator_append_instr(struct Translator* T, const Instruction i);

NoRet translator_insert_instr(
    struct Translator* T,
    const InstructionPosition pos,
    const Instruction i);

RegisterID translator_get_free_reg(struct Translator* T);

NoRet translator_free_regs(struct Translator* T);

bool translator_st_insert(
    struct Translator* T,
    struct Symbol const* sym);

struct Symbol* translator_st_lookup(
    struct Translator* T,
    char const* name);

struct Immediate* translator_immediate_insert(
    struct Translator* T,
    char const* name,
    struct Object* obj);

struct Immediate* translator_immediate_lookup(
    struct Translator* T,
    char const* name);

NoRet translator_enter_scope(struct Translator* T);

NoRet translator_leave_scope(struct Translator* T);

/*******************/
/* Getters/Setters */
/*******************/

RegisterID translator_get_curr_rid(struct Translator* T);
NoRet translator_set_curr_rid(struct Translator* T, const RegisterID rid);

/**************/
/* Benchmarks */
/**************/

#if BENCH
NoRet bench_translator(void);
#endif /* BENCH */

/*********/
/* Tests */
/*********/

#if TEST
NoRet test_translator(void);
#endif /* TEST */

#endif /* TRANSLATOR_TRANSLATOR_H */
