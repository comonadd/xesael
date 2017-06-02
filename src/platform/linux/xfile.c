/* File: xfile.c */
/* Creation Date: 2017-02-01 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "platform/xfile.h"

#include <dirent.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "err.h"
#include "lib/macros.h"
#include "lib/types.h"
#include "lib/xmalloc.h"
#include "log.h"

/**
   $ Description:
   #   This function opens a new file
   @file - file to open
   @file_path - path of a file
   @file_mode - mode to open with
   @create_file_if_not_exists - create a new file if it does not exists already
   @trunc - truncate
**/
bool xfile_open(XFile* file,
                const char* file_path,
                const enum XFileMode file_mode,
                const bool create_file_if_not_exists,
                const bool trunc)
{
  /* Obvious checks */
  DASSERT(file_path, "file_path is NULL");
  DASSERT(strlen(file_path) > 0, "file_path length is <= 0");

  int flags = 0;
  switch (file_mode) {
    case XFM_R: flags |= O_RDONLY; break;
    case XFM_W: flags |= O_WRONLY; break;
    default:
      LOG_DEBUG("unhandled file mode in the switch statement");
      UNLIKELY();
      /* Fail */
      return false;
  }

  if (create_file_if_not_exists) flags |= O_CREAT;
  if (trunc) flags |= O_TRUNC;
  if ((*file = open(file_path, flags)) == -1) {
    err_set_from_errno();
    /* Fail */
    return false;
  }

  /* Succeed */
  return true;
}

/**
   $ Description:
   #   This function loads a file into memory
   $ Input: file to load
   $ Output: pointer to file location in memory and size of read
   @file - file to load
   @size - file size
**/
bool xfile_load(XFile* const file, char** const content, ByteSize* const size)
{
  if (!(file && size)) return false;
  int fd = CAST(*file, int);
  if (fd < 0) {
    LOG_DEBUG("xfile_load(): tried to read from unopened file descriptor");
    err_set_unknown();
    /* Fail */
    return false;
  }

  static const size_t BUF_SIZE = 512;
  char buf[BUF_SIZE];
  ByteSize res_size = 0;
  *content          = NULL;
  char* res         = *content;
  do {
    /* Read the chunk from the file descriptor */
    ssize_t read_bytes = read(fd, buf, BUF_SIZE);

    /* If we got an EOF */
    if (read_bytes == 0) {
      /* Set an null-terminating character */
      res           = xrealloc(res, (res_size + 1) * sizeof(*res));
      res[res_size] = STR_END_CH;
      *size         = res_size;
      *content      = res;
      break;
    }

    /* If there was an error */
    if (read_bytes == -1) {
      err_set_from_errno();
      goto out;
    }

    /* Allocate more space for the new data */
    res = xrealloc(res, (res_size + read_bytes) * sizeof(*res));
    /* Copy the new data to the resulting string */
    memcpy(&res[res_size], buf, read_bytes);
    /* Increment the resulting size */
    res_size += read_bytes;
  } while (true);

  /* Succeed */
  return true;

out:
  if (res) xfree(res);
  *content = NULL;
  *size    = 0;
  /* Fail */
  return false;
}

bool xfile_write(XFile* file, const char* input_s)
{
  if (*file < 0) {
    LOG_DEBUG("tried to write into unopened file descriptor");
    err_set_unknown();
    /* Fail */
    return false;
  }

  int res = write(CAST(*file, int), input_s, strlen(input_s));
  if (res == -1) {
    err_set_from_errno();
    /* Fail */
    return false;
  }

  /* Succeed */
  return true;
}

bool xfile_close(XFile* file)
{
  if (CAST(*file, int) <= 0) {
    LOG_DEBUG("tried to close already closed file descriptor");
    err_set_unknown();
    /* Fail */
    return false;
  }

  int res = close(CAST(*file, int));
  if (res == -1) {
    err_set_from_errno();
    /* Fail */
    return false;
  }

  /* Succeed */
  return true;
}
