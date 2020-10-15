/* File: sym.h */
/* Creation Date: 2017-01-16 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Symbol implementation */

#ifndef SYM_H
#define SYM_H

#include "core/object/object.h"
#include "core/vm/instruction/instruction.h"
#include "lib/types.h"

typedef int64 SymbolID;

enum SymbolType { SYM_STR, SYM_INUM, SYM_DNUM, SYM_VAR, SYM_CONST, SYM_FUNC };

union SymbolData {
  ImmediateID var_name_immediate_id;
  ImmediateID const_name_immediate_id;
  ImmediateID func_immediate_id;
};

struct Symbol {
  SymbolID id;
  char* name;
  enum SymbolType type;
  union SymbolData data;
};

/***********/
/* Methods */
/***********/

struct Symbol* sym_new(const enum SymbolType type, char* name);

struct Symbol* sym_var_new(char* name, const ImmediateID name_immediate_id);

struct Symbol* sym_const_new(char* name, const ImmediateID name_immediate_id);

struct Symbol* sym_func_new(char* name, const ImmediateID func_immediate_id);

#endif /* SYM_H */
