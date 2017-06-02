/* File: symtable.h */
/* Creation Date: 2017-01-15*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The GSh symbol table implementation */
/* Symbol table contains a variables, functions */

/******************************************/
/* Example:                               */
/* -------------------------------------- */
/* | ID |  Value                        | */
/* |----|-------------------------------| */
/* | 35 | Symbol(SYM_VAR, 33)           | */
/* | 15 | Symbol(SYM_FUNC, 62)          | */
/* -------------------------------------- */
/******************************************/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include "core/compiler/sym/sym.h"
#include "lib/dict.h"
#include "lib/macros.h"
#include "lib/types.h"

#define SYMBOL_TABLE_DICT_SIZE 256

struct SymbolTable {
  struct Dictionary symbols;
  struct SymbolTable* prev;
};

/***********/
/* Methods */
/***********/

NoRet st_init(struct SymbolTable* st);

NoRet st_deinit(struct SymbolTable* st);

struct SymbolTable* st_new(void);

NoRet st_delete(struct SymbolTable* st);

bool st_insert(struct SymbolTable* st, struct Symbol const* sym);

struct Symbol* st_lookup(struct SymbolTable const* st, char const* name);

#endif /* SYMTABLE_H */
