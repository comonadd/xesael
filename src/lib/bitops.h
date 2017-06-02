/* File: bit.h */
/* Creation Date: 2017-01-09*/
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */
/* Helpful things for working with bits */

#ifndef BITOPS_H
#define BITOPS_H

#include "lib/types.h"

#define MASK1(type, pos, n) ((~((~(type)0) << (n))) << (pos))
#define MASK0(type, pos, n) (~MASK1(type, pos, n))
#define RESET_BIT(bits, pos) (bits &= ~(1 << pos))
#define SET_BIT(bits, pos) (bits |= (1 << pos))
#define GET_BIT(bits, pos) \
  (CAST(bits, uint64) & (CAST(2, uint64) << CAST(pos, uint64)))

#define RESET_BITS(bits, pos, n)                            \
  do {                                                      \
    for (uint8 i = 0; i < n; ++i) reset_bit(bits, pos + i); \
  } while (0)

#define SET_BITS(___bits, ___pos, ___n, ___bits_to_set)              \
  do {                                                               \
    uint64 __bits              = CAST((___bits), uint64);            \
    uint64 __bits_to_set       = CAST((___bits_to_set), uint64);     \
    uint64 mask0               = MASK0(uint64, (___pos), (___n));    \
    uint64 masked_bits0        = __bits & mask0;                     \
    uint64 shifted_bits_to_set = (__bits_to_set << (___pos));        \
    ___bits                    = masked_bits0 | shifted_bits_to_set; \
  } while (0)

#define GET_BITS(bits, pos, n) (MASK1(uint64, pos, n) & bits)
#define PRINTN_BITS(bits, n)                        \
  do {                                              \
    uint64 __bits = CAST(bits, uint64);             \
    for (int32 i = n; i >= 0; --i)                  \
      putc(!!GET_BIT(__bits, i - 1) + '0', stdout); \
    putc('\n', stdout);                             \
  } while (0)

#define PRINT_BITS(bits) PRINTN_BITS(CAST(bits, uint64), 8 * sizeof(uint64))

#endif /* BITOPS_H */
