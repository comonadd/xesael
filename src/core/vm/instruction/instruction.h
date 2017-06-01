/* File: instruction.h */
/* Creation Date: 2017-01-14 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The GSh VM instruction */
/* Instruction represented as a 64-bit number: */
/* 0000000 0000000000000000000 0000000000000000000 0000000000000000000 */
/* The first 7 bits (0-6) is the instruction code */
/* The next 19 bits (7-25) is the 1st argument */
/* The next 19 bits (26-45) is the 2nd argument */
/* The next 19 bits (46-64) is the 3rd argument */

#ifndef VM_INSTRUCTION_H
#define VM_INSTRUCTION_H

#include "lib/types.h"
#include "lib/macros.h"

#include "core/object/ops/unop.h"
#include "core/object/ops/binop.h"

#include "core/vm/register.h"
#include "core/vm/immediate/immediate.h"

/**
   $ Description:
   #   GSh VM instruction codes
   #   R - register ID
   #   C - object ID
   #   A - address (represented as a number)
   #   P - just the constant number
   #   N - nothing
**/
/* NOTE: Change of the order in this enumeration should be followed by */
/*       the appropriate chang in the constants below */
enum InstructionOpCode {
    /****************************************************************************/
    /* Enum Name     Name    Arguments  Description                             */
    /*                         1 2 3                                            */
    /****************************************************************************/
    IOP_MOV,      /* MOV       R R N    Move value from one register to another */
    IOP_LOAD,     /* LOAD      C R N    Load constant into the register         */

    IOP_VARSET,   /* VARSET    C R N    Set variable to value that R contains   */
    IOP_VARLOAD,  /* VARLOAD   C R N    Load variable value to a register value */

    IOP_JMPR,     /* JMPR      A N N    Relative jump                           */
    IOP_JMPRC,    /* JMPRC     A R N    Relative jump conditional               */
    IOP_JMPRCI,   /* JMPRCI    A R N    Relative jump conditional inversed      */
    IOP_JMPA,     /* JMPA      A N N    Absolute jump                           */
    IOP_JMPAC,    /* JMPAC     A R N    Absolute jump conditional               */
    IOP_JMPACI,   /* JMPACI    A R N    Absolute jump conditional inversed      */
    IOP_CALL,     /* CALL      C R P    Call the function with the given name   */
    IOP_RET,      /* RET       N N N    Return to a saved position              */

    IOP_NEG,      /* NEG       R R N    Negation                                */
    IOP_NOT,      /* NOT       R R N    Boolean NOT                             */
    IOP_BNOT,     /* BNOT      R R N    Bitwise NOT                             */
    IOP_LEN,      /* LEN       R R N    Length                                  */
    IOP_INCR,     /* INCR      R R N    Increment                               */
    IOP_DECR,     /* DECR      R R N    Decrement                               */

    IOP_ADD,      /* ADD       R R R    Add two registers                       */
    IOP_SUB,      /* SUB       R R R    Substract one register from another     */
    IOP_MUL,      /* MUL       R R R    Multiply one register by another        */
    IOP_DIV,      /* DIV       R R R    Divide one register by another          */
    IOP_MOD,      /* MOD       R R R    Modulus operation                       */
    IOP_POW,      /* POW       R R R    Power                                   */
    IOP_BAND,     /* POW       R R R    Bitwise AND                             */
    IOP_BOR,      /* BOR       R R R    Bitwise OR                              */
    IOP_BXOR,     /* BXOR      R R R    Bitwise XOR                             */
    IOP_LS,       /* LS        R R R    Bitwise LS                              */
    IOP_RS,       /* RS        R R R    Bitwise RS                              */

    IOP_LAND,     /* LAND      R R R    Boolean AND                             */
    IOP_LOR,      /* LOR       R R R    Boolean OR                              */

    IOP_EQ,       /* EQ        R R R    Relational EQUALS                       */
    IOP_NEQ,      /* NEQ       R R R    Relational NOT-EQUALS                   */
    IOP_LT,       /* LT        R R R    Relational LESS-THAN                    */
    IOP_GT,       /* GT        R R R    Relational GREATER-THAN                 */
    IOP_LTE,      /* LTE       R R R    Relational LESS-THAN-OR-EQUALS          */
    IOP_GTE,      /* GTE       R R R    Relational GREATER-THAN-OR-EQUALS       */

    IOP_IMPORT,   /* IMPORT    C N N    Import name                             */
    IOP_EXIT,     /* EXIT      N N N    Exit from program                       */

    IOP_COUNT
};

#define IOP_UNOP_FIRST IOP_NEG
#define IOP_UNOP_LAST IOP_DECR

#define IOP_BINOP_FIRST IOP_ADD
#define IOP_BINOP_LAST IOP_LOR

#define IOP_BINOP_ARITH_FIRST IOP_BINOP_FIRST
#define IOP_BINOP_ARITH_LAST IOP_RS

#define IOP_BINOP_BOOL_FIRST IOP_LAND
#define IOP_BINOP_BOOL_LAST IOP_LOR

#define INSTR_CODE_IS_UNOP(code) ((code >= IOP_UNOP_FIRST) && (code <= IOP_UNOP_LAST))
#define INSTR_CODE_IS_BINOP(code) ((code >= IOP_BINOP_FIRST) && (code <= IOP_BINOP_LAST))

#define INSTR_CODE_BINOP_IS_ARITH(code) \
    ((code >= IOP_BINOP_ARITH_FIRST) && (code <= IOP_BINOP_ARITH_LAST))

#define INSTR_CODE_BINOP_IS_BOOL(code) \
    ((code >= IOP_BINOP_BOOL_FIRST) && (code <= IOP_BINOP_BOOL_LAST))

typedef int32 InstructionSArgument;
typedef uint32 InstructionUArgument;

typedef uint64 InstructionPosition;
typedef int16 InstructionOffset;

/**
   @code - code of a instruction
   @a - first argument
   @b - second argument
   @c - third argument
**/
#define INSTR_OP_SIZE 8
/* NOTE: Each argument field has exact same size */
#define INSTR_ARG_SIZE 16
typedef struct {
    enum InstructionOpCode code : INSTR_OP_SIZE;
    InstructionSArgument a : INSTR_ARG_SIZE;
    InstructionUArgument b : INSTR_ARG_SIZE;
    InstructionUArgument c : INSTR_ARG_SIZE;
} Instruction;
#undef INSTR_OP_SIZE
#undef INSTR_ARG_SIZE

/*************/
/* Constants */
/*************/

extern char const* INSTR_CODE_STR[IOP_COUNT];

/**********************************/
/* Instruction code field helpers */
/**********************************/

#define I_OP_SET(___instr, ___code) ((___instr).code = (___code))
#define I_OP_GET(___instr) (___instr).code

#define binop_instr_code_from_ast_node(___node)				\
    CAST(CAST(								\
             (___node)->type - AST_NODE_BINOP_FIRST,			\
             enum InstructionOpCode) + IOP_BINOP_FIRST, enum InstructionOpCode)

#define unop_instr_code_from_ast_node(___node)				\
    CAST(CAST(								\
             (___node)->type - AST_NODE_UNOP_FIRST,			\
             enum InstructionOpCode) + IOP_UNOP_FIRST, enum InstructionOpCode)

/********************************/
/* Instruction argument helpers */
/********************************/

#define I_A_SET(___instr, ___arg) ((___instr).a = (___arg))
#define I_B_SET(___instr, ___arg) ((___instr).b = (___arg))
#define I_C_SET(___instr, ___arg) ((___instr).c = (___arg))

#define I_A_GET(___instr) (___instr).a
#define I_B_GET(___instr) (___instr).b
#define I_C_GET(___instr) (___instr).c

/**********************************/
/* Assert macros for instructions */
/**********************************/

#define INSTR_ASSERT_CODE(___instr, ___code) DASSERT(I_OP_GET(___instr) == (___code), "")

/***********/
/* Helpers */
/***********/

char const* instr_code2str(
    const enum InstructionOpCode code);

NoRet instr_print(const Instruction instr);

#define instr_compute_binop(___opcode, ___lop, ___rop)          \
    obj_binop(___opcode - IOP_BINOP_FIRST, (___lop), (___rop))

#define instr_compute_unop(___opcode, ___op)            \
    obj_unop(___opcode - IOP_UNOP_FIRST, (___op))

/****************************/
/* Instruction constructors */
/****************************/

Instruction instr_new(
    const enum InstructionOpCode code);

Instruction instr_mov_new(
    const RegisterID rid1,
    const RegisterID rid2);

Instruction instr_load_new(
    const ImmediateID immediate_id,
    const RegisterID rid);

Instruction instr_varset_new(
    const ImmediateID var_name_immediate_id,
    const RegisterID val_rid);

Instruction instr_varload_new(
    const ImmediateID var_name_immediate_id,
    const RegisterID dst_rid);

/* Relative jump */
Instruction instr_jmpr_new(
    const InstructionOffset offset);

Instruction instr_jmprc_new(
    const InstructionOffset offset,
    const RegisterID rid);

Instruction instr_jmprci_new(
    const InstructionOffset offset,
    const RegisterID rid);

/* Absolute jump */
Instruction instr_jmpa_new(
    const InstructionPosition pos);

Instruction instr_jmpac_new(
    const InstructionPosition pos,
    const RegisterID cond_rid);

Instruction instr_jmpaci_new(
    const InstructionPosition pos,
    const RegisterID cond_rid);

/* Calling */
Instruction instr_call_new(
    const ImmediateID func_name_immediate_id,
    const RegisterID args_start_rid,
    const uint64 args_count);

Instruction instr_ret_new(void);

/* Unary operators */
Instruction instr_unop_new(
    const enum InstructionOpCode code,
    const RegisterID src_rid,
    const RegisterID dst_rid);

/* Binary operators */
Instruction instr_binop_new(
    const enum InstructionOpCode code,
    const RegisterID src1_rid,
    const RegisterID src2_rid,
    const RegisterID dst_rid);

Instruction instr_import_new(
    const ImmediateID name_immediate_id);

Instruction instr_exit_new(void);

#endif /* VM_INSTRUCTION_H */
