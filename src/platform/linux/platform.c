/* File: platform.c */
/* Creation Date: 2016-12-21*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* This file implements all GNU/Linux platform */

#include "platform/platform.h"

#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>

#include "err.h"
#include "log.h"

#include "lib/types.h"
#include "lib/macros.h"
#include "lib/xmalloc.h"

#include "platform/path.h"

char** list_dir(
    const char* path,
    uint64* entries_count)
{
    if (!path || !entries_count) return NULL;
    DIR* dir;
    struct dirent* entry;
    char** filenames;

    /* Counting count of files in directory */
    if ((dir = opendir(path))) {
        entries_count = 0;
        while ((entry = readdir(dir))) {
            if (STREQ(entry->d_name, ".") || STREQ(entry->d_name, ".."))
                continue;
            ++entries_count;
        }
        closedir(dir);
    } else {
        /* Set appropriate error message */
        err_set_no_such_file_or_dir(path);
        /* Fail */
        return NULL;
    }

    /* Allocating array of filenames */
    filenames = xmalloc(*entries_count * PTR_SIZE);

    /* Appending filenames to array */
    dir = opendir(path);
    for (uint64 i = 0; (entry = readdir(dir));) {
        if (STREQ(entry->d_name, ".") || STREQ(entry->d_name, ".."))
            continue;
        filenames[i] = xmalloc(strlen(entry->d_name) * sizeof(char));
        strcpy(filenames[i], entry->d_name);
        ++i;
    }
    closedir(dir);

    return filenames;
}

bool file_copy(
    const char* src_path,
    const char* dst_path)
{
    /* Open the source file descriptor */
    int src = open(src_path, O_RDONLY);

    /* Open the destination file descriptor */
    int dst = open(dst_path, O_WRONLY);

    static const uint16 buf_size = 512;
    char buf[buf_size];

    for (;;) {
        /* Read from the source file descriptor */
        int res = read(src, buf, buf_size);

        /* If we got an EOF */
        if (res == 0) {
            /* Done */
            goto done;
        }

        /* If we got some error */
        if (res == -1) {
            switch (errno) {
                default:
                    LOG_DEBUG(
                        "not handled \"errno\" in switch on read (errno == %s)",
                        strerror(errno));
                    err_set_unknown();
            }

            goto out;
        }

        res = write(dst, buf, res);
        if (res == -1) {
            switch (errno) {
                default:
                    LOG_DEBUG(
                        "not handled \"errno\" in switch on write (errno == %s)",
                        strerror(errno));
                    err_set_unknown();
            }

            goto out;
        }
    }

done:
    close(src);
    close(dst);
    /* Succeed */
    return true;

out:
    close(src);
    close(dst);
    /* Fail */
    return false;
}

bool dir_copy(
    const char* src_path,
    const char* dst_path)
{
    if (!src_path || !dst_path) return false;

    /* Check that directory exists on source path */
    if (!path_exists(src_path)) {
        /* Set appropriate error message */
        err_set_no_such_file_or_dir(src_path);
        /* Fail */
        return false;
    }

    /* Check if directory at destination path exists */
    if (path_exists(dst_path)) {
        /* If so, call dir_copy on dst_path/dst_path_base and return it's result */
        char* dst_path_base = path_get_base(dst_path);
        char* dst_path_new = path_concat(dst_path, dst_path_base);
        bool res = dir_copy(src_path, dst_path_new);
        xfree(dst_path_base);
        xfree(dst_path_new);
        return res;
    }

    /* Create directory at destination path. If failed, return false */
    enum PathAccessMode src_access_mode;
    if (!get_path_access_mode(src_path, &src_access_mode))
        return false;
    if (!path_create(dst_path, src_access_mode, true))
        return false;

    /* Get list of files at source path into array of filenames */
    char** entries;
    uint64 entries_count;

    if (!(entries = list_dir(src_path, &entries_count))) {
        /* If we failed to list directory */
        return false;
    }

    /* For each filename in array of filenames: */
    while (entries_count != 0) {
        /* If thing at filename is directory: */
        char* src_entry_path;
        char* dst_entry_path;
        bool res;
        src_entry_path = path_concat(src_path, entries[entries_count]);
        dst_entry_path = path_concat(dst_path, entries[entries_count]);
        if (path_is_dir(src_entry_path)) {
            /* Copy directory */
            res = dir_copy(src_entry_path, dst_entry_path);
        } else {
            /* Copy file */
            res = file_copy(src_entry_path, dst_entry_path);
        }
        xfree(src_entry_path);
        xfree(dst_entry_path);
        /* If copy failed, deallocate entries array and return false */
        if (!res) {
            /* Deallocate each string in array */
            for (; entries_count > 0; --entries_count)
                xfree(entries[entries_count]);
            /* Deallocate actual array of pointers */
            xfree(entries);
            return false;
        }
        xfree(entries[entries_count]);
        --entries_count;
    }

    xfree(entries);
    return true;
}

#if TEST
#include <assert.h>

static NoRet test_path_concat(void)
{
    PASS();
}

static NoRet test_path_base(void)
{
    PASS();
}

static NoRet test_path_is_dir(void)
{
    PASS();
}

static NoRet test_path_exists(void)
{
    PASS();
}

NoRet test_platform(void)
{
    test_path_concat();
    printf("\t[\"test_path_concat\"]: passed\n");
    test_path_base();
    printf("\t[\"test_path_base\"]: passed\n");
    test_path_is_dir();
    printf("\t[\"test_path_is_dir\"]: passed\n");
    test_path_exists();
    printf("\t[\"test_path_exists\"]: passed\n");
}

#endif /* TEST */
