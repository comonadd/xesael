#include <termios.h>

#include <stdio.h>
#include <stdlib.h>
#include "lib/args.h"
#include "platform/platform.h"
#include "xesael.h"

static struct termios saved_term_opts;
static struct termios term_opts;

int main(int argc, char** argv)
{
  saved_term_opts = term_opts;
  struct Arguments args;
  args.count = argc;
  args.args  = argv;
  xesael_main(&args);
  return 0;
}
