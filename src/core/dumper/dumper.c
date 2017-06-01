/* File: dumper.c */
/* Creation Date: 2017-01-16 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* The GSh bytecode dumper */

#include "core/dumper/dumper.h"

#include "lib/types.h"
#include "lib/buffer.h"
#include "platform/xfile.h"
#include "core/vm/bytecode/bytecode.h"

struct {
    struct Buffer buf;
} __dumper;

/***********/
/* Helpers */
/***********/

static NoRet __dumper_init(void)
{
    buf_init(&__dumper.buf);
}

static NoRet __dumper_deinit(void)
{
    buf_deinit(&__dumper.buf);
}

static NoRet __dumper_write_instr(
    Instruction instr)
{
    UNUSED(instr);
    /* buf_append_ch(&__dumper.buf, instr); */
}

/********/
/* Main */
/********/

bool dump(
    struct ByteCode const* bc,
    char const* filename)
{
    __dumper_init();
    Instruction instr;
    for (uint64 i = 0; i < bc->instructions_count; ++i) {
        instr = bytecode_get_instr(bc, i);
        __dumper_write_instr(instr);
    }
    XFile file;
    if (!xfile_open(&file, filename, XFM_W, true, true))
        return false;
    xfile_write(&file, buf_get_val(&__dumper.buf));
    __dumper_deinit();

    /* Succeed */
    return true;
}
