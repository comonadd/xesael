/* File: preced.c */
/* Creation Date: 2017-01-27 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Token precedance table */

#include "core/compiler/token/preced.h"

/********************/
/* Global variables */
/********************/

#define TOK_ASSIGN_PRECED 0
#define TOK_NON_OP_PRECED 0

const uint8 TOKS_PRECED[TOKS_COUNT] = {
  /* Value           Description                              */
  TOK_NON_OP_PRECED, /* Invalid token                         */

  /* Separators */
  TOK_NON_OP_PRECED, /* '\n'                                  */
  TOK_NON_OP_PRECED, /* ';'                                   */
  TOK_NON_OP_PRECED, /* ':'                                   */
  TOK_NON_OP_PRECED, /* ','                                   */
  TOK_NON_OP_PRECED, /* '['                                   */
  TOK_NON_OP_PRECED, /* ']'                                   */
  TOK_NON_OP_PRECED, /* '('                                   */
  TOK_NON_OP_PRECED, /* ')'                                   */
  TOK_NON_OP_PRECED, /* '{'                                   */
  TOK_NON_OP_PRECED, /* '}'                                   */

  /* Unary prefix operators */
  32, /* '-'                                    */
  32, /* '!'                                    */
  32, /* '~'                                    */
  32, /* '$'                                    */

  /* Unary postfix operators */
  31, /* '++'                                  */
  31, /* '--'                                  */

  /* Binary arithmetic operators */
  28, /* '+'                                   */
  28, /* '-'                                   */
  29, /* '*'                                   */
  29, /* '/'                                   */
  29, /* '%'                                   */
  30, /* '**'                                  */
  27, /* '&'                                   */
  27, /* '|'                                   */
  27, /* '^'                                   */
  26, /* '<<'                                  */
  26, /* '>>'                                  */

  /* Binary boolean operators */
  8, /* '&&'                                  */
  8, /* '||'                                  */

  /* Binary relational operators */
  27, /* '=='                                  */
  26, /* '!='                                  */
  26, /* '<'                                   */
  4,  /* '>'                                   */
  4,  /* '<='                                  */
  16, /* '>='                                  */
  16, /* '@'                                   */

  /* Assign operator */
  TOK_ASSIGN_PRECED, /* '='                                   */

  /* Arithmetic assign operator shorthands */
  TOK_ASSIGN_PRECED, /* '+='                                  */
  TOK_ASSIGN_PRECED, /* '-='                                  */
  TOK_ASSIGN_PRECED, /* '*='                                  */
  TOK_ASSIGN_PRECED, /* '/='                                  */
  TOK_ASSIGN_PRECED, /* '%='                                  */
  TOK_ASSIGN_PRECED, /* '**='                                 */
  TOK_ASSIGN_PRECED, /* '&='                                  */
  TOK_ASSIGN_PRECED, /* '|='                                  */
  TOK_ASSIGN_PRECED, /* '^='                                  */
  TOK_ASSIGN_PRECED, /* '<<='                                 */
  TOK_ASSIGN_PRECED, /* '>>='                                 */

  /* Boolean assign operator shorthands */
  TOK_ASSIGN_PRECED, /* '&&='                                 */
  TOK_ASSIGN_PRECED, /* '||='                                 */

  /* Keywords */
  TOK_NON_OP_PRECED, /* 'var'                                 */
  TOK_NON_OP_PRECED, /* 'const'                               */
  TOK_NON_OP_PRECED, /* 'enum'                                */
  TOK_NON_OP_PRECED, /* 'continue'                            */
  TOK_NON_OP_PRECED, /* 'break'                               */
  TOK_NON_OP_PRECED, /* 'scope'                               */
  TOK_NON_OP_PRECED, /* 'if'                                  */
  TOK_NON_OP_PRECED, /* 'elif'                                */
  TOK_NON_OP_PRECED, /* 'else'                                */
  TOK_NON_OP_PRECED, /* 'while'                               */
  TOK_NON_OP_PRECED, /* 'for'                                 */
  TOK_NON_OP_PRECED, /* 'function'                            */
  TOK_NON_OP_PRECED, /* 'true'                                */
  TOK_NON_OP_PRECED, /* 'false'                               */
  TOK_NON_OP_PRECED, /* 'return'                              */
  TOK_NON_OP_PRECED, /* 'import'                              */

  /* Not reserved */
  TOK_NON_OP_PRECED,
  TOK_NON_OP_PRECED,
  TOK_NON_OP_PRECED,
  TOK_NON_OP_PRECED,
};

#undef TOK_NON_OP_PRECED
#undef TOK_ASSIGN_PRECED
