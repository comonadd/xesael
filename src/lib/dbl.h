/* File: dbl.h */
/* Creation Date: 2017-01-17 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#ifndef DBL_H
#define DBL_H

#include "types.h"

#define DBL_CMP_EPSILON 0.0000001

extern double dbl_abs(const double d);

extern bool dbl_eq(const double a, const double b);

extern bool dbl_neq(const double a, const double b);

extern bool dbl_lt(const double a, const double b);

extern bool dbl_gt(const double a, const double b);

extern bool dbl_lte(const double a, const double b);

extern bool dbl_gte(const double a, const double b);

#endif /* DBL_H */
