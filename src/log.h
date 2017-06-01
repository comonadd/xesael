/* File: log.h */
/* Creation Date: 2017-01-24 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Logging system */

#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#include "lib/types.h"

#define LOG_STREAM stdout

#if DEBUG
#  define LOG_DEBUG(...) do {                                           \
        fprintf(LOG_STREAM, "[%s:%d]: %s: ", __FILE__, __LINE__, __PRETTY_FUNCTION__); \
        fprintf(LOG_STREAM, __VA_ARGS__);                               \
        fprintf(LOG_STREAM, "\n");                                      \
    } while (0)
#else
#  define LOG_DEBUG(...)
#endif /* DEBUG */

#endif /* LOG_H */
