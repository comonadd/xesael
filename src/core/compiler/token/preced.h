/* File: preced.h */
/* Creation Date: 2017-01-27 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Token precedance table */

#ifndef TOKEN_PRECED_H
#define TOKEN_PRECED_H

#include "core/compiler/token/token.h"

/********************/
/* Global variables */
/********************/

extern const uint8 TOKS_PRECED[TOKS_COUNT];

/***********/
/* Helpers */
/***********/

#define tok_preced(tok) TOKS_PRECED[tok->type]

#endif /* TOKEN_PRECED_H */
