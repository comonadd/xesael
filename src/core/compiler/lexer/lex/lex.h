/* File: lex.h */
/* Creation date: 2017-02-04 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* lex() function */
/* TODO: Add handling for '\\' */
/* TODO: Add handling for '\'' */
/* TODO: Add handling for '?' */

#ifndef LEX_H
#define LEX_H

#include "lib/xvec.h"

#include "core/compiler/compiler.h"

/**
   $ Description:
   #   This function does the lexical analysis
   #   of the given text. It translates the
   #   text into the tokens. In this case,
   #   tokens are stored in the vector
   @text - text to analyse to
**/
struct XVector* lex(char const* text);

#endif /* LEX_H */
