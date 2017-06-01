/* File: hash.h */
/* Creation Date: 2017-01-15*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Hash functions */

#ifndef HASH_H
#define HASH_H

#include "types.h"

/* String hashing function */
uint32 str_hash(
    const char* key,
    uint64 len);

uint32 str_hash_f(const void* key);

#endif /* HASH_H */
