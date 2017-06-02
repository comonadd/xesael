/* File: hash.c */
/* Creation Date: 2017-01-15*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Hash functions */

#include "hash.h"

#include "macros.h"
#include "types.h"

/* MurMur2 hash function implementation */
/* This function was originally created by Austin Appleby */
uint32 str_hash(const char* key, uint64 len)
{
  const uint32 m    = 0x5bd1e995;
  const uint32 seed = 0;
  const int r       = 24;

  uint32 h = seed ^ len;

  const unsigned char* data = (const unsigned char*)key;

  while (len >= 4) {
    uint32 k;
    k = data[0];
    k |= data[1] << 8;
    k |= data[2] << 16;
    k |= data[3] << 24;

    k *= m;
    k ^= k >> r;
    k *= m;

    h *= m;
    h ^= k;

    data += 4;
    len -= 4;
  }

  switch (len) {
    case 3: h ^= data[2] << 16;
    case 2: h ^= data[1] << 8;
    case 1: h ^= data[0]; h *= m;
    default: PASS();
  };

  h ^= h >> 13;
  h *= m;
  h ^= h >> 15;

  return h;
}

uint32 str_hash_f(const void* key)
{
  const char* k = CAST(key, const char*);
  return str_hash(k, strlen(k));
}
