/* File: instruction.c */
/* Creation Date: 2017-01-17 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "lib/types.h"
#include "lib/xmalloc.h"

#include "core/vm/instruction/instruction.h"
#include "core/vm/register.h"
#include "core/vm/immediate/immediate.h"

/*************/
/* Constants */
/*************/

char const* INSTR_CODE_STR[IOP_COUNT] = {
    "MOV",
    "LOAD",

    "VARSET",
    "VARLOAD",

    "JMPR",
    "JMPRC",
    "JMPRCI",
    "JMPA",
    "JMPAC",
    "JMPACI",
    "CALL",
    "RET",

    "NEG",
    "NOT",
    "BNOT",
    "LEN",
    "INCR",
    "DECR",

    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "POW",
    "BAND",
    "BOR",
    "BXOR",
    "LS",
    "RS",

    "LAND",
    "LOR",

    "EQ",
    "NEQ",
    "LT",
    "GT",
    "LTE",
    "GTE",

    "IMPORT",
    "EXIT"
};

/***********/
/* Helpers */
/***********/

char const* instr_code2str(
    const enum InstructionOpCode code)
{
    return INSTR_CODE_STR[code];
}

NoRet instr_print(const Instruction instr)
{
    printf(
        "%s %d %d %d",
        instr_code2str(I_OP_GET(instr)),
        I_A_GET(instr),
        I_B_GET(instr),
        I_C_GET(instr));
}

/****************************/
/* Instruction constructors */
/****************************/

Instruction instr_new(
    const enum InstructionOpCode code)
{
    Instruction instr;
    I_OP_SET(instr, code);
    return instr;
}

Instruction instr_mov_new(
    const RegisterID rid1,
    const RegisterID rid2)
{
    Instruction instr = instr_new(IOP_MOV);
    I_A_SET(instr, rid1);
    I_B_SET(instr, rid2);
    return instr;
}

Instruction instr_load_new(
    const ImmediateID immediate_id,
    const RegisterID rid)
{
    Instruction instr = instr_new(IOP_LOAD);
    I_A_SET(instr, immediate_id);
    I_B_SET(instr, rid);
    return instr;
}

Instruction instr_varset_new(
    const ImmediateID var_name_immediate_id,
    const RegisterID val_rid)
{
    Instruction instr = instr_new(IOP_VARSET);
    I_A_SET(instr, var_name_immediate_id);
    I_B_SET(instr, val_rid);
    return instr;
}

Instruction instr_varload_new(
    const ImmediateID var_name_immediate_id,
    const RegisterID dst_rid)
{
    Instruction instr = instr_new(IOP_VARLOAD);
    I_A_SET(instr, var_name_immediate_id);
    I_B_SET(instr, dst_rid);
    return instr;
}

/* Relative jump */
Instruction instr_jmpr_new(
    const InstructionOffset offset)
{
    Instruction instr = instr_new(IOP_JMPR);
    I_A_SET(instr, offset);
    return instr;
}

Instruction instr_jmprc_new(
    const InstructionOffset offset,
    const RegisterID cond_rid)
{
    Instruction instr = instr_new(IOP_JMPRC);
    I_A_SET(instr, offset);
    I_B_SET(instr, cond_rid);
    return instr;
}

Instruction instr_jmprci_new(
    const InstructionOffset offset,
    const RegisterID cond_rid)
{
    Instruction instr = instr_new(IOP_JMPRCI);
    I_A_SET(instr, offset);
    I_B_SET(instr, cond_rid);
    return instr;
}

/* Absolute jump */
Instruction instr_jmpa_new(
    const InstructionPosition pos)
{
    Instruction instr = instr_new(IOP_JMPA);
    I_A_SET(instr, pos);
    return instr;
}

Instruction instr_jmpac_new(
    const InstructionPosition pos,
    const RegisterID rid)
{
    Instruction instr = instr_new(IOP_JMPAC);
    I_A_SET(instr, pos);
    I_B_SET(instr, rid);
    return instr;
}

Instruction instr_jmpaci_new(
    const InstructionPosition pos,
    const RegisterID rid)
{
    Instruction instr = instr_new(IOP_JMPACI);
    I_A_SET(instr, pos);
    I_B_SET(instr, rid);
    return instr;
}

/* Calling */
Instruction instr_call_new(
    const ImmediateID func_name_immediate_id,
    const RegisterID args_start_rid,
    const uint64 args_count)
{
    Instruction instr = instr_new(IOP_CALL);
    I_A_SET(instr, func_name_immediate_id);
    I_B_SET(instr, args_start_rid);
    I_C_SET(instr, args_count);
    return instr;
}

Instruction instr_ret_new(void)
{
    Instruction instr = instr_new(IOP_RET);
    return instr;
}

/* Unary operators */
Instruction instr_unop_new(
    const enum InstructionOpCode code,
    const RegisterID src_rid,
    const RegisterID dst_rid)
{
    Instruction instr = instr_new(code);
    I_A_SET(instr, src_rid);
    I_B_SET(instr, dst_rid);
    return instr;
}

/* Binary operators */
Instruction instr_binop_new(
    const enum InstructionOpCode code,
    const RegisterID src1_rid,
    const RegisterID src2_rid,
    const RegisterID dst_rid)
{
    Instruction instr = instr_new(code);
    I_A_SET(instr, src1_rid);
    I_B_SET(instr, src2_rid);
    I_C_SET(instr, dst_rid);
    return instr;
}

Instruction instr_import_new(
    const ImmediateID name_immediate_id)
{
    Instruction instr = instr_new(IOP_IMPORT);
    I_A_SET(instr, name_immediate_id);
    return instr;
}

Instruction instr_exit_new(void)
{
    Instruction instr = instr_new(IOP_EXIT);
    return instr;
}
