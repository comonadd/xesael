/* File: translate_func_def.c */
/* Creation Date: 2017-01-28 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* translate_func_def_node() function */

#include "core/compiler/translator/translate/translate_def/translate_func_def.h"

#include "err.h"

#include "lib/types.h"

#include "core/object/func/func.h"

#include "core/compiler/compiler.h"
#include "core/compiler/ast/ast_node.h"
#include "core/compiler/sym/sym.h"
#include "core/compiler/translator/translator.h"
#include "core/compiler/translator/translate/translate.h"

/**********************/
/* Private prototypes */
/**********************/

static bool fill_func_obj(
    struct XVector* nodes,
    struct Object* obj);

/*********************/
/* Private functions */
/*********************/

static bool fill_func_obj(
    struct XVector* nodes,
    struct Object* obj)
{
    /* For each node in the nodes vector */
    for (uint64 i = 0; i < nodes->size; ++i) {
        /* Retreive the node */
        struct ASTNode* body_node = xvec_get(nodes, i);


        switch (body_node->type) {
            case AST_NODE_RETURN:
                /* Compile the return expression */
                /* Append instuction "RETURN" */
                break;
            default:
                if (!translate_node(body_node))
                    return false;
        }
    }

    /* Succeed */
    return true;
}

bool translate_func_def_node(struct ASTNode* node)
{
    /* Get the name of a function */
    char const* name;
    name = ast_node_func_def_name(node)->data.str.val;

    /* Create a new function object */
    struct Object* obj = obj_func_new();

    /* Insert a new function immediate */
    ImmediateID func_immediate_id;
    func_immediate_id = translator_immediate_insert(name, obj)->id;

    /* Create a new function symbol */
    struct Symbol const* sym;
    sym = sym_func_new(name, func_immediate_id);

    /* Try to insert it to the symbol table */
    if (!translator_st_insert(sym)) {
        /* If such a name is already defined */
        /* Set the appropriate error message */
        compiler_err_setf(
            node->ln,
            "name \"%s\" is already taken",
            name);
        /* Fail */
        return false;
    }

    /* Allocate a new bytecode array for the function body */
    struct ByteCode* bc = bytecode_new();

    struct ASTNode* body = node->data.func_def.body;
    struct XVector* nodes = body->data.block.stmts;
    fill_func_obj(nodes, obj);

    /* Succeed */
    return true;
}
