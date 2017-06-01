/* File: execute.c */
/* Creation Date: 2017-01-31 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The main virtual machine function */

#include "core/vm/execute.h"

#include "lib/types.h"

#include "core/object/object.h"
#include "core/object/bool/bool.h"
#include "core/object/cfunc/cfunc.h"
#include "core/object/ops/unop.h"
#include "core/object/ops/binop.h"
#include "core/vm/vm.h"
#include "core/vm/env.h"
#include "core/vm/bytecode/bytecode.h"
#include "core/vm/instruction/instruction.h"

/****************************/
/* Global private variables */
/****************************/

static bool __executing = false;
static struct Environment* __env = NULL;

/***********************/
/* Environment helpers */
/***********************/

#define __var_set(___name, ___obj) env_set_var(__env, (___name), (___obj))
#define __var_get(___name) env_get_var(__env, (___name))

#define __env_leave() do {                              \
        /* Save pointer to the previous environment */  \
        struct Environment* prev_env = __env->prev;     \
        /* Delete the current environment */            \
        env_delete(__env);                              \
        __env = prev_env;                               \
    } while (0)

#define __env_enter() do {                      \
        struct Environment* prev_env = __env;   \
        __env = env_new();                      \
        __env->prev = prev_env;                 \
    } while (0)

/******************************************************/
/* Macros for retreiving the object that is specified */
/* by register ID in the instruction argument         */
/******************************************************/

#define I_RA(___instr) vm_get_reg(I_A_GET((___instr)))
#define I_RB(___instr) vm_get_reg(I_B_GET((___instr)))
#define I_RC(___instr) vm_get_reg(I_C_GET((___instr)))

/******************************************************/
/* Macros for retreive the object that is specified   */
/* by immediate ID in the instruction argument        */
/******************************************************/

#define I_IA(___instr) immediates[I_A_GET((___instr))]
#define I_IB(___instr) immediates[I_B_GET((___instr))]
#define I_IC(___instr) immediates[I_C_GET((___instr))]

/**********************************************/
/* This macro dispatches the next instruction */
/**********************************************/

#define __dispatch() do {                       \
        instr = *(++ip);                        \
        goto *jmp_tbl[I_OP_GET(instr)];         \
    } while (0)

/***********************************************/
/* This macro jumps relative to the current IP */
/***********************************************/

#define __jmpr(___offset) do {                  \
        ip += (___offset);                      \
        instr = *ip;                            \
        goto *jmp_tbl[I_OP_GET(instr)];         \
    } while (0)

    /**********************************************************/
/* This macro jumps relative to the start of instructions */
/**********************************************************/

#define __jmpa(___pos) do {                     \
        ip = bc->instructions + (___pos);       \
        instr = *ip;				\
        goto *jmp_tbl[I_OP_GET(instr)];         \
    } while (0)

/*******************/
/* MOV instruction */
/*******************/
#define mov_instr do {                                          \
        /* Get the actual object that we want to move */        \
        /* and the register ID that we wanna to move to */      \
        struct Object const* obj = I_RA(instr);                 \
        const RegisterID dst_rid = I_B_GET(instr);              \
        /* Actually move the object to a register */            \
        vm_set_reg(dst_rid, obj);                               \
        /* Go to the next instruction */                        \
        __dispatch();                                           \
    } while (0)

/********************/
/* LOAD instruction */
/********************/
#define load_instr do {                                 \
        /* Get the immediate and the register ID */     \
        struct Object const* obj = I_IA(instr);         \
        const RegisterID rid = I_B_GET(instr);          \
        /* Actually change the register */              \
        vm_set_reg(rid, obj);                           \
        /* Go to the next instruction */                \
        __dispatch();                                   \
    } while (0)

/**********************/
/* VARSET instruction */
/**********************/
#define varset_instr do {                                               \
        /* Retreive the name of a variable to set value to */           \
        struct Object const* var_name_obj = I_IA(instr);                \
        char const* var_name = var_name_obj->data.str.val;              \
        /* Retreive the object that we want to set this variable to */  \
        struct Object const* var_obj = I_RB(instr);                     \
        /* Actually set the variable to new value */                    \
        __var_set(var_name, var_obj);                                  \
        DASSERT(__var_get(var_name) == var_obj, "");                   \
        /* Go to the next instruction */                                \
        __dispatch();                                                   \
    } while (0)

/***********************/
/* VARLOAD instruction */
/***********************/
#define varload_instr do {                                      \
        /* Get the name of a variable that we wanna to load */  \
        struct Object* var_name_obj = I_IA(instr);              \
        char* var_name = var_name_obj->data.str.val;            \
        /* Get the register ID that we wanna to load */         \
        /* variable value in */                                 \
        const RegisterID rid = I_B_GET(instr);                  \
        /* Get the variable value */                            \
        struct Object* var_obj = __var_get(var_name);          \
        vm_set_reg(rid, var_obj);                               \
        /* Go to the next instruction */                        \
        __dispatch();                                           \
    } while (0)

/********************/
/* JMPR instruction */
/********************/
#define jmpr_instr do {                         \
        /* Do the actual jump */                \
        __jmpr(I_A_GET(instr));                 \
    } while (0)

/*********************/
/* JMPRC instruction */
/*********************/
#define jmprc_instr do {                        \
        /* If the condition is true */          \
        if (obj_is_true(I_RB(instr))) {         \
            /* Do the jump */                   \
            __jmpr(I_A_GET(instr));             \
        }                                       \
        /* Go to the next instruction */        \
        __dispatch();                           \
    } while (0)

/**********************/
/* JMPRCI instruction */
/**********************/
#define jmprci_instr do {                       \
        /* If the condition is false */         \
        if (obj_is_false(I_RB(instr))) {        \
            /* Do the jump */                   \
            __jmpr(I_A_GET(instr));             \
        }                                       \
        /* Go to the next instruction */        \
        __dispatch();                           \
    } while (0)

/********************/
/* JMPA instruction */
/********************/
#define jmpa_instr do {                         \
        /* Do the actual jump */                \
        __jmpa(I_A_GET(instr));                 \
    } while (0)

/*********************/
/* JMPAC instruction */
/*********************/
#define jmpac_instr do {                        \
        /* If the condition is false */         \
        if (obj_is_true(I_RB(instr))) {         \
            /* Do the jump */                   \
            __jmpa(I_A_GET(instr));             \
        }                                       \
        /* Go to the next instruction */        \
        __dispatch();                           \
    } while (0)

/**********************/
/* JMPACI instruction */
/**********************/
#define jmpaci_instr do {                       \
        /* If the condition is false */         \
        if (obj_is_false(I_RB(instr))) {        \
            /* Do the jump */                   \
            __jmpa(I_A_GET(instr));             \
        }                                       \
        /* Go to the next instruction */        \
        __dispatch();                           \
    } while (0)

/********************/
/* CALL instruction */
/********************/
#define call_instr do {                                                 \
        /* Retreive the function object from */                         \
        /* the first instruction argument */                            \
        struct Object const* f = I_IA(instr);                           \
        /* Retreive the register which contains */                      \
        /* the first function argument */                               \
        const RegisterID args_start_rid = I_B_GET(instr);               \
        /* Retreive the arguments count */                              \
        const uint64 args_count = I_C_GET(instr);                       \
        /* If function is a C function */                               \
        if (f->type == OBJT_CFUNC) {                                    \
            /* Retreive the pointer to the actual C function */         \
            const CFuncImpl f_impl = f->data.cfunc.f;                   \
            /* Allocate a new arguments array */                        \
            struct CFuncArguments* args = cfunc_args_new();             \
            /* Extract the arguments from registers into the array */   \
            const RegisterID args_end_rid  = args_start_rid + args_count; \
            /* Append arguments from the registers */                   \
            /* into the arguments array */                              \
            {                                                           \
                RegisterID rid = args_start_rid;                        \
                while (rid != args_end_rid) {                           \
                    /* Append a object that register at this ID contains */ \
                    cfunc_args_append(args, vm_get_reg(rid));           \
                    /* Next register */                                 \
                    ++rid;                                              \
                }                                                       \
            }                                                           \
            /* Actually execute the function */                         \
            struct Object* res = f_impl(args);                          \
            /* The result of a function call is stored in the */        \
            /* first argument register */                               \
            vm_set_reg(args_start_rid, res);                            \
            /* Go to the next instruction */                            \
            __dispatch();                                               \
        }                                                               \
        /* Enter a new environment */                                   \
        __env_enter();                                                  \
        /* Execute GSh function */                                      \
        execute(f->data.func.bc);                                       \
        /* Go to the next instruction */                                \
        __dispatch();                                                   \
   } while (0)

/*******************/
/* RET instruction */
/*******************/
#define ret_instr do {                                  \
        /* Leave the current function environment */    \
        __env_leave();                                  \
        /* Go to the next instruction */                \
        __dispatch();                                   \
    } while (0)

/*******************************/
/* Unary operation instruction */
/*******************************/
#define unop_instr do {                                                 \
        /* Get the operand, destination register ID and the opcode */   \
        struct Object const* op = I_RA(instr);                          \
        const RegisterID dst_rid = I_B_GET(instr);                      \
        /* Compute the result */                                        \
        struct Object* res = instr_compute_unop(I_OP_GET(instr), op);   \
        /* If we failed to compute the operation */                     \
        if (!res) return false;                                         \
        /* Set the resulting value to the destination register */       \
        vm_set_reg(dst_rid, res);                                       \
        /* Go to the next instruction */                                \
        __dispatch();                                                   \
    } while (0)

/*********************************/
/* Binary operation instructions */
/*********************************/
#define binop_instr do {                                                \
        /* Get the left and right operands and the destination RID */   \
        struct Object const* lop = I_RA(instr);                         \
        struct Object const* rop = I_RB(instr);                         \
        const RegisterID dst_rid = I_C_GET(instr);                      \
        /* Compute the result */                                        \
        struct Object* res = instr_compute_binop(I_OP_GET(instr), lop, rop); \
        /* If we failed to compute the result */                        \
        if (!res) return false;                                         \
        /* Set the value to the destination register */                 \
        vm_set_reg(dst_rid, res);                                       \
        /* Go to the next instruction */                                \
        __dispatch();                                                   \
    } while (0)

/********************/
/* Exit instruction */
/********************/
#define exit_instr do {                         \
        __executing = false;                    \
        return true;                            \
    } while (0)

/********/
/* Main */
/********/

bool execute(struct ByteCode const* bc)
{
    /* Setup environment if first run */
    if (!__executing) {
        __executing = true;
        __env = env_new();
    }

    /* The jump table */
    static void* jmp_tbl[IOP_COUNT] = {
        &&IOP_MOV_L, &&IOP_LOAD_L,

        &&IOP_VARSET_L, &&IOP_VARLOAD_L,

        &&IOP_JMPR_L, &&IOP_JMPRC_L, &&IOP_JMPRCI_L,
        &&IOP_JMPA_L, &&IOP_JMPAC_L, &&IOP_JMPACI_L,
        &&IOP_CALL_L, &&IOP_RET_L,

        &&IOP_UNOP, &&IOP_UNOP,
        &&IOP_UNOP, &&IOP_UNOP,
        &&IOP_UNOP, &&IOP_UNOP,

        &&IOP_BINOP, &&IOP_BINOP,
        &&IOP_BINOP, &&IOP_BINOP,
        &&IOP_BINOP, &&IOP_BINOP,
        &&IOP_BINOP, &&IOP_BINOP,
        &&IOP_BINOP,
        &&IOP_BINOP, &&IOP_BINOP,

        &&IOP_BINOP, &&IOP_BINOP,

        &&IOP_BINOP, &&IOP_BINOP,
        &&IOP_BINOP, &&IOP_BINOP,
        &&IOP_BINOP, &&IOP_BINOP,

        &&IOP_EXIT_L
    };

    /* Retreive the immediates used by this bytecode */
    struct Object* const* immediates = bc->immediates;

    /* Retreive the ip to execute */
    Instruction const* ip = bc->instructions;
    Instruction instr = *ip;

    /* Go to the first instruction */
    goto *jmp_tbl[I_OP_GET(instr)];

IOP_MOV_L: mov_instr;
IOP_LOAD_L: load_instr;
IOP_VARSET_L: varset_instr;
IOP_VARLOAD_L: varload_instr;
IOP_JMPR_L: jmpr_instr;
IOP_JMPRC_L: jmprc_instr;
IOP_JMPRCI_L: jmprci_instr;
IOP_JMPA_L: jmpa_instr;
IOP_JMPAC_L: jmpac_instr;
IOP_JMPACI_L: jmpaci_instr;
IOP_CALL_L: call_instr;
IOP_RET_L: ret_instr;

IOP_UNOP: unop_instr;
IOP_BINOP: binop_instr;

IOP_EXIT_L: exit_instr;
}

#undef __dispatch
#undef __jmpr
#undef __jmpa
