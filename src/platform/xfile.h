/* File: xfile.h */
/* Creation Date: 2017-02-01 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* This file defines the file API that Taste needs */

#ifndef PLATFORM_XFILE_H
#define PLATFORM_XFILE_H

#include "lib/types.h"

typedef int XFile;

/**
   $ Description:
   #   This enumeration describes file mode
   #   for files of type XFile
   @XFM_R - read-only
   @XFM_W - write-only
**/
enum XFileMode { XFM_R, XFM_W };

/**
   $ Description:
   #   This function opens a new file
   $ Input: file path, file mode, some flags
   $ Output:
   #   opened file with corresponding privileges (specified by the file mode)
   #   possibly, new created file
   @file - file to open
   @file_path - path to open
   @file_mode - mode to open file in
   @create_file_if_not_exists - boolean flag, that if set,
   #   file should be created if
   #   no file exists on given path
   @trunc - boolean flag, that if set,
   #   if file exists on given path, it will be truncated to length 0
**/
/* TODO: Handle more of possible errors */
bool xfile_open(XFile* file,
                const char* file_path,
                const enum XFileMode file_mode,
                const bool create_file_if_not_exists,
                const bool trunc);

/**
   $ Description:
   #   This function loads a file into memory and returns pointer to it
   $ Input: file to load
   $ Output: pointer to file location in memory and size of file
   @file - file to load
   @size - file size
**/
/* TODO: Handle errors */
bool xfile_load(XFile* const file,
                char** const file_content,
                ByteSize* const size);

/**
   $ Description:
   #   This function writes to a given opened file
   $ Input: opened file
   $ Output: file that contents given text
   @file - file to write to
   @input_s - text to write
**/
bool xfile_write(XFile* file, const char* input_s);

/**
   $ Description:
   #   This function closes the given opened file
   $ Input: opened file
   $ Output: closed file
   @file - file to close
**/
/* TODO: Handle errors */
bool xfile_close(XFile* file);

#endif /* PLATFORM_XFILE_H */
