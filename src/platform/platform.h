/* File: core.h */
/* Creation Date: 2016-12-21*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* This file defines all the things that Taste needs from platform */

#ifndef PLATFORM_H
#define PLATFORM_H

#include "lib/types.h"

/********/
/* Main */
/********/

/**
   $ Description:
   #   This function lists directory at given path
   $ Input: path of a directory
   $ Output: array of entry names and size of this array
   @path - path of a directory
   @entries_count - count of entries in directory
**/
char** list_dir(
    const char* path,
    uint64* entries_count);

/**
   $ Description:
   #   This function copies the file from src_path to dst_path
   $ Input: source and destination paths
   $ Output: file with the same content on both locations
   @src_path - source path
   @dst_path - destination path
**/
bool file_copy(
    const char* src_path,
    const char* dst_path);

/**
   $ Description:
   #   This function copies the directory from src_path to dst_path
   $ Input: source and destination paths
   $ Output: directories with the same content on both locations
   @src_path - source path
   @dst_path - destination path
**/
bool dir_copy(
    const char* src_path,
    const char* dst_path);

/*********/
/* Tests */
/*********/

#if TEST
NoRet test_platform(void);
#endif /* TEST */

#endif /* PLATFORM_H */
