/* File: dbl.c */
/* Creation Date: 2017-01-17 */
/* Creator: Dmitry Guzeev <dmitry.guzeev@yahoo.com> */
/* Description: */

#include "dbl.h"

inline double dbl_abs(const double d)
{
  return d > 0 ? d : -d;
}

inline bool dbl_eq(const double a, const double b)
{
  return dbl_abs(a - b) < DBL_CMP_EPSILON;
}

inline bool dbl_neq(const double a, const double b)
{
  return !dbl_eq(a, b);
}

inline bool dbl_lt(const double a, const double b)
{
  return a < b;
}

inline bool dbl_gt(const double a, const double b)
{
  return a > b;
}

inline bool dbl_lte(const double a, const double b)
{
  return dbl_eq(a, b) || (a < b);
}

inline bool dbl_gte(const double a, const double b)
{
  return dbl_eq(a, b) || (a > b);
}
