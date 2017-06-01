/* File: parse.h */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* parse() function */

#ifndef PARSE_H
#define PARSE_H

#include "lib/xvec.h"

#include "core/compiler/ast/ast.h"

/********/
/* Main */
/********/

struct AST* parse(struct XVector const* toks);

#endif /* PARSE_H */
