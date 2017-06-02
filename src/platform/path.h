/* File: path.h */
/* Creation Date: 2017-02-01 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#ifndef PLATFORM_PATH_H
#define PLATFORM_PATH_H

#include "lib/types.h"

/**
   $ Description:
   #   This enumeration describes access modes
   #   for files and directories
   @PAM_USER_ONLY - file can be accessed only by user that created it
   @PAM_ALL - file can be accessed by all users on system
**/
enum PathAccessMode { PAM_USER_ONLY, PAM_ALL };

/*************/
/* Constants */
/*************/

extern const char PLATFORM_PATH_SEPARATOR_CH;

/********/
/* Main */
/********/

/**
   $ Description:
   #   This function concatenates paths
   $ Input: two paths to concat
   $ Output: concatenated path allocated on heap
   @p1 - first path
   @p2 - second path
**/
char* path_concat(char const* p1, char const* p2);

/**
   $ Description:
   #   This function returns base of a given path
   $ Input: path to get base from
   $ Output: base of a given path
**/
char* path_get_base(char const* path);

/**
   $ Description:
   #   This function gives the basename of a given
   #   path
**/
char* path_get_basename(char const* path);

char* path_concat_with_ext(char const* path, char const* ext);
/**
   $ Description:
   #   This function returns true if thing at given path is a directory
   $ Input: path
   $ Output: true or false
   @path - path to check
**/
bool path_is_dir(char const* path);

/**
   $ Description:
   #   This function returns true if file/directory at given path exists
   $ Input: file/directory path
   $ Output: true or false
   @dir_path - file/directory path
**/
bool path_exists(char const* path);

/**
   $ Description:
   #   This function creates a new file/directory on a given path
   $ Input: path of a new file/directory
   $ Output: brand new file/directory at a given path
   @path - path of a new file/directory
   @access_mode - access mode in which create a new file/directory
**/
bool path_create(char const* path,
                 const enum PathAccessMode access_mode,
                 const bool dir);

/**
   $ Description:
   #   This function returns access mode of a file/directory at a given path
   $ Input: file/directory path
   $ Output: file/directory access mode
   @dir_path - path of a file/directory
**/
bool get_path_access_mode(const char* path, enum PathAccessMode* mode);

#endif /* PLATFORM_PATH_H */
