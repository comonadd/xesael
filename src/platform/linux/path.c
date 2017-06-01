/* File: path.c */
/* Creation Date: 2017-02-01 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* This file implements a path functions */
/* that Taste needs for a Linux platform */

#include "platform/path.h"

#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "err.h"
#include "log.h"

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"

/*************/
/* Constants */
/*************/

const char PLATFORM_PATH_SEPARATOR_CH = '/';

/********/
/* Main */
/********/

char* path_concat(
    char const* p1,
    char const* p2)
{
    /* If we got an wrong inputs */
    if (!(p1 && p2)) {
        err_set_unknown();
        LOG_DEBUG(
            "wrong inputs, bool(p1) == %d, bool(p2) == %d",
            !!p1, !!p2);
        return NULL;
    }

    /* Retreive the lengths */
    uint64 p1_len = strlen(p1);
    uint64 p2_len = strlen(p2);
    uint64 new_len = p1_len + p2_len + 1;

    /* Allocate enough space for new path and null-terminator */
    char* new_path = xmalloc(new_len + 1);

    /* Copy the contents of a first path to the resulting memory */
    memcpy(new_path, p1, p1_len);

    /* Place the separator */
    new_path[p1_len] = PLATFORM_PATH_SEPARATOR_CH;

    /* Copy the contents of a second path to the resulting memory */
    memcpy(&new_path[p1_len + 1], p2, p2_len);
    new_path[new_len] = STR_END_CH;

    /* Succeed */
    return new_path;
}

char* path_get_base(
    char const* path)
{
    if (!path) {
        err_set_unknown();
        LOG_DEBUG("wrong input, path is NULL");
        return NULL;
    }

    /* Retreive the path length */
    const uint64 path_len = strlen(path);

    /* If length of path is 0 or 1, return nothing */
    if (path_len < 2) return NULL;
    if (path[path_len - 1] == PLATFORM_PATH_SEPARATOR_CH)
        return NULL;

    /* path_base_length = 0 */
    uint64 path_base_len = 0;

    /* current position = length of a path - 1 */
    uint64 curr_pos = path_len - 1;

    /* Until we see a path separator in path at current position: */
    while (path[curr_pos] != PLATFORM_PATH_SEPARATOR_CH) {
        if (curr_pos == 0) {
            /* If we not found any path separators, */
            /* return the path without any changes */
            return strdup(path);
        }
        ++path_base_len;
        --curr_pos;
    }

    /* Allocate path_base string on heap of size path_base_length */
    char* path_base = xmalloc(path_base_len * sizeof(*path_base));

    /* While current position is less than path length: */
    for (uint64 i = 0; curr_pos < path_len; ++i, ++curr_pos) {
        /* Append character in path at current position to path base */
        path_base[i] = path[curr_pos];
    }

    /* Succeed */
    return path_base;
}

char* path_get_basename(
    char const* path)
{
    /* If we have invalid input */
    if (!path) {
        err_set_unknown();
        LOG_DEBUG("wrong input, path is NULL");
        return NULL;
    }

    /* Retreive the path base */
    char* path_base = path_get_base(path);

    /* Save the path length */
    uint64 path_len = strlen(path);

    /* Current position in a path = len of a path - 1 */
    uint64 curr_pos = path_len - 1;

    /* Go from the end of a given path until char at current pos in text is not a '.' */
    while (path[curr_pos] != DOT_CH) {
        if (curr_pos == 0) {
            /* If current position equals to 0, */
            /* we have no any extension in a given path, */
            /* so, just return the base */
            return path_base;
        }
        /* Decrement current position */
        --curr_pos;
    }

    /* Replace the '.' with the null terminator */
    path_base[curr_pos] = STR_END_CH;

    /* Succeed */
    return path_base;
}

char* path_concat_with_ext(
    char const* path,
    char const* ext)
{
    /* If we got wrong inputs */
    if (!(path && ext)) {
        LOG_DEBUG(
            "wrong inputs, bool(path) == %d, bool(ext) == %d",
            !!path, !!ext);
        err_set_unknown();
        /* Fail */
        return NULL;
    }

    /* Retreive the lengths */
    const uint64 path_len = strlen(path);
    const uint64 ext_len = strlen(ext);

    /* Allocate memory for the resulting string */
    char* new_path = xmalloc((path_len + ext_len + 1) * sizeof(*new_path));
    if (!new_path) {
        err_set_not_enough_memory();
        return NULL;
    }

    memcpy(new_path, path, path_len);
    new_path[path_len] = DOT_CH;
    memcpy(new_path, ext, ext_len);
    new_path[strlen(new_path)] = STR_END_CH;

    /* Suceed */
    return new_path;
}

bool path_is_dir(char const* path)
{
    if (!path) {
        err_set_unknown();
        LOG_DEBUG("wrong input, path is NULL");
        return false;
    }

    struct stat stat_;
    stat(path, &stat_);
    return S_ISREG(stat_.st_mode);
}

bool path_exists(char const* path)
{
    if (!path) {
        err_set_unknown();
        LOG_DEBUG("wrong input, path is NULL");
        return false;
    }

    struct stat stat_;
    return !((stat(path, &stat_) == -1) && errno == ENOENT);
}

bool get_path_access_mode(
    char const* path,
    enum PathAccessMode* mode)
{
    if (!path) {
        err_set_unknown();
        LOG_DEBUG("wrong input, path is NULL");
        return false;
    }

    if (!path_exists(path)) {
        err_set_path_not_exists();
        return false;
    }

    struct stat stat_;
    stat(path, &stat_);

    switch (stat_.st_mode) {
        case 0700: case 1700:
            *mode = PAM_USER_ONLY; break;
        case 0777: case 1777:
            *mode = PAM_ALL; break;
        default:
            err_set_unknown();
            LOG_DEBUG("not handled ");
            return false;
    }

    /* Succeed */
    return true;
}

bool path_create(
    char const* path,
    const enum PathAccessMode access_mode,
    const bool dir)
{
    if (!path) {
        err_set_unknown();
        LOG_DEBUG("wrong input, path is NULL");
        return false;
    }

    if (path_exists(path)) return true;

    mode_t mode;
    switch (access_mode) {
        case PAM_ALL: mode = 0777; break;
        case PAM_USER_ONLY: mode = 0700; break;
        default:
            IMPOSSIBLE();
            return false;
    }

    int res;

    /* Create a file/directory */
    if (dir) {
        res = mkdir(path, mode);
    } else {
        res = creat(path, mode);
    }

    if (res == -1) {
        switch (errno) {
            default:
                LOG_DEBUG(
                    "not handled \"errno\" case switch statement, strerror(errno) == \"%s\"",
                    strerror(errno));
                err_set_unknown();
        }

        /* Fail */
        return false;
    }

    /* Succeed */
    return true;
}
