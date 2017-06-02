/* File: main.c */
/* Creation Date: 2016-12-23*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* This file implements Taste entry point for GNU/Linux OS */

#include <termios.h>

#if DEBUG
#include <mcheck.h>
#endif /* DEBUG */

#include "lib/args.h"
#include "platform/platform.h"
#include "taste.h"

static struct termios saved_term_opts;
static struct termios term_opts;

int main(int argc, char** argv)
{
#if DEBUG
  mtrace();
#endif /* DEBUG */
  saved_term_opts = term_opts;

  struct Arguments args;
  args.count = argc;
  args.args  = argv;
  taste_main(&args);
  return 0;
}
