/* File: sym.c */
/* Creation Date: 2017-01-16 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Symbol implementation */

#include "core/compiler/sym/sym.h"

#include "lib/xmalloc.h"

/***********/
/* Methods */
/***********/

struct Symbol* sym_new(const enum SymbolType type, char const* name)
{
  struct Symbol* sym = xmalloc(sizeof(*sym));
  sym->type          = type;
  sym->name          = name;
  sym->id            = -1;
  return sym;
}

struct Symbol* sym_var_new(char const* name,
                           const ImmediateID name_immediate_id)
{
  struct Symbol* sym              = sym_new(SYM_VAR, name);
  sym->data.var_name_immediate_id = name_immediate_id;
  return sym;
}

struct Symbol* sym_const_new(char const* name,
                             const ImmediateID name_immediate_id)
{
  struct Symbol* sym                = sym_new(SYM_CONST, name);
  sym->data.const_name_immediate_id = name_immediate_id;
  return sym;
}


struct Symbol* sym_func_new(char const* name,
                            const ImmediateID func_immediate_id)
{
  struct Symbol* sym          = sym_new(SYM_FUNC, name);
  sym->data.func_immediate_id = func_immediate_id;
  return sym;
}
