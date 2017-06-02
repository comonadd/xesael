/* File: args.c */
/* Creation Date: 2017-02-02 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "args.h"

#include <ctype.h>
#include <stdio.h>

#include "err.h"

#include "lib/args.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xmalloc.h"

#include "core/compiler/compile.h"
#include "core/dumper/dumper.h"
#include "core/vm/execute.h"

static bool __arg_action_show_version(const uint8 i,
                                      struct Arguments const *args);

static bool __arg_action_show_help(const uint8 i, struct Arguments const *args);

static bool __arg_action_set_optimization_lvl(const uint8 i,
                                              struct Arguments const *args);

static bool __arg_action_set_path(const uint8 i, struct Arguments const *args);

static bool __arg_action_set_output_path(const uint8 i,
                                         struct Arguments const *args);

static bool __arg_action_include(const uint8 i, struct Arguments const *args);

static bool __arg_action_compile(const uint8 i, struct Arguments const *args);

static bool __arg_action_execute(const uint8 i, struct Arguments const *args);

static bool __arg_action_execute_str(const uint8 i,
                                     struct Arguments const *args);

#define __ARGS_ACTIONS_COUNT 9
static struct ArgumentAction const __ARGS_ACTIONS[__ARGS_ACTIONS_COUNT] = {
  {'v', "version", __arg_action_show_version},
  {'h', "help", __arg_action_show_help},
  {'O', "optimization", __arg_action_set_optimization_lvl},
  {'P', "path", __arg_action_set_path},
  {'I', "include", __arg_action_include},
  {'o', "output", __arg_action_set_output_path},
  {'c', "compile", __arg_action_compile},
  {'e', "execute", __arg_action_execute},
  {'E', "execute_str", __arg_action_execute_str}};

/*************/
/* Variables */
/*************/

struct TasteArgumentsInfo *__res;

/********/
/* Main */
/********/

static bool __arg_action_show_version(const uint8 i,
                                      struct Arguments const *args)
{
  UNUSED(i);
  UNUSED(args);
  printf(VERSION);
  return true;
}

static bool __arg_action_show_help(const uint8 i, struct Arguments const *args)
{
  UNUSED(i);
  UNUSED(args);

#define print_arg_help(___short, ___long, ___desc) \
  printf("\t-%c\t--%s\t\t%s\n", (___short), (___long), (___desc));

  printf("Help for the Taste " VERSION "\n");
  print_arg_help('h', "help", "show help");
  print_arg_help('v', "version", "show version");
  print_arg_help('c', "compile", "compile given file");

/* Succeed */
#undef print_arg_help
  return true;
}

static bool __arg_action_set_optimization_lvl(const uint8 i,
                                              struct Arguments const *args)
{
  /* Retreive the level */
  char lvl_ch = args->args[i + 1][0];

  if (!isdigit(lvl_ch)) goto out;

  /* Convert the character into the number */
  uint8 lvl = ASCII_DIGIT2NORMAL_DIGIT(lvl_ch);

  if (lvl > 3) goto out;

  /* Actually set the optimization level */
  __res->opt_lvl = lvl;

  /* Succeed */
  return true;

out:
  err_setf(E_ARG,
           "optimization level should be specified as a number (0 <= l <= 3)",
           lvl_ch);
  return false;
}

static bool __arg_action_set_path(const uint8 i, struct Arguments const *args)
{
  /* Retreive the specified path */
  char const *path = args->args[i + 1];

  /* Set the path */
  __res->path = path;

  /* Succeed */
  return true;
}

static bool __arg_action_set_output_path(const uint8 i,
                                         struct Arguments const *args)
{
  /* Retreive the actual path */
  char const *path = args->args[i + 1];

  /* Set the actual output path */
  __res->output_path = path;

  /* Succeed */
  return true;
}

/* TODO: Implement */
static bool __arg_action_include(const uint8 i, struct Arguments const *args)
{
  char const *includes_s = args->args[i + 1];

  /* The resulting array */
  char **includes       = xmalloc(INCLUDES_MAX_COUNT * sizeof(*includes));
  uint64 includes_count = 0;

#define append_include(___include)                                        \
  do {                                                                    \
    if (includes_count >= INCLUDES_MAX_COUNT) goto out_too_many_includes; \
    (includes[includes_count++] = (___include));                          \
  } while (0)

  char *include;
  uint64 include_len = 0;
  for (uint64 j = 0; j < strlen(includes_s); ++j) {
    switch (includes_s[j]) {
      case ',':
        append_include(include);
        include     = NULL;
        include_len = 0;
        break;
      default: include[include_len++] = includes_s[j];
    }
  }

  /* Append the last include */
  append_include(include);

  /* Set the value to the result */
  __res->includes = includes;

  /* Succeed */
  return true;

out_too_many_includes:
  /* Fail */
  err_setf(E_ARG, "too many includes specified (> %lu)", INCLUDES_MAX_COUNT);
  return false;
}

static bool __arg_action_compile(const uint8 i, struct Arguments const *args)
{
  /* Retreive the file_path to the file to compile */
  char const *file_path = args->args[i + 1];

  if (file_path[0] == '-') {
    err_set(E_ARG, "invalid argument");
    return false;
  }

  /* Compile the file */
  struct ByteCode *bc = compile_file(file_path);
  if (!bc) return false;

  char const *output_path =
    __res->output_path ? __res->output_path : DEFAULT_OUTPUT_PATH;
  dump(bc, output_path);

  /* Succeed */
  return true;
}

static bool __arg_action_execute(uint8 i, struct Arguments const *args)
{
  /* Retreive the file path to execute */
  char *file_path = args->args[i + 1];

  if (file_path[0] == '-') return true;

  /* Compile the file */
  struct ByteCode *bc = compile_file(file_path);
  if (!bc) return false;

  /* Execute the bytecode */
  if (!execute(bc)) return false;

  /* Succeed */
  return true;
}

static bool __arg_action_execute_str(const uint8 i,
                                     struct Arguments const *args)
{
  /* Compile the file */
  struct ByteCode *bc = compile("<stdin>", args->args[i + 1]);
  if (!bc) return false;

  /* Execute the bytecode */
  if (!execute(bc)) return false;

  /* Succeed */
  return true;
}

struct TasteArgumentsInfo *taste_parse_args(struct Arguments const *args)
{
  /* Obvious check */
  DASSERT(args, "");

  /* If there is nothing to parse */
  if (args->count < 2) return NULL;

  /* Allocate space for the result */
  __res = xmalloc(sizeof(*__res));

  /* For each argument in the arguments array */
  for (uint64 i = 1; i < args->count; ++i) {
    /* Retreive the argument */
    char *arg = args->args[i];

    if ((arg[0] == '-') && (arg[1] == '-')) {
      /* Parse long argument */

      arg += 2;
      for (uint64 j = 0; j < __ARGS_ACTIONS_COUNT; ++j) {
        if (STREQ(__ARGS_ACTIONS[j].l, arg)) {
          if (!__ARGS_ACTIONS[j].action(j, args)) return NULL;
        }
      }
    } else if (arg[0] == '-') {
      /* Parse short argument */

      char arg_ch = *(arg + 1);
      for (uint64 j = 0; j < __ARGS_ACTIONS_COUNT; ++j) {
        if (arg_ch == __ARGS_ACTIONS[j].s) {
          if (!__ARGS_ACTIONS[j].action(j, args)) return NULL;
        }
      }
    } else {
      /* Execute file */
    }
  }

  /* Succeed */
  return __res;
}
