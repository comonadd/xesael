/* File: symtable.c */
/* Creation Date: 2017-01-15*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The GSh symbol table implementation */

#include "core/compiler/sym/symtable.h"

#include "lib/hash.h"
#include "lib/types.h"

#include "core/compiler/sym/sym.h"
#include "core/object/object.h"

NoRet st_init(struct SymbolTable* st)
{
  dict_init(&st->symbols, SYMBOL_TABLE_DICT_SIZE, NULL, NULL, NULL, obj_free_f);
  st->prev = NULL;
}

NoRet st_deinit(struct SymbolTable* st)
{
  dict_deinit(&st->symbols);
}

struct SymbolTable* st_new(void)
{
  struct SymbolTable* st = xmalloc(sizeof(*st));
  st_init(st);
  return st;
}

NoRet st_delete(struct SymbolTable* st)
{
  st_deinit(st);
  xfree(st);
}

bool st_insert(struct SymbolTable* st, struct Symbol* sym)
{
  sym->id = dict_get_key_index(&st->symbols, sym->name);
  return dict_insert_raw(&st->symbols, sym->name, sym, sym->id);
}

struct Symbol* st_lookup(struct SymbolTable const* st, char const* name)
{
  struct SymbolTable const* curr_st = st;
  struct Symbol* sym;

repeat:
  if (!curr_st) return NULL;
  sym = dict_find(&st->symbols, name);
  if (!sym) {
    curr_st = st->prev;
    goto repeat;
  }
  return sym;
}
