#ifndef __TE_UTIL_H__
#define __TE_UTIL_H__

#define decrement_or_nullify(x) (x = (x > 0) ? x - 1 : 0)
#define increment_or_maximize(x, max) (x = (x > max) ? x + 1 : max)

#define max(a, b) (a > b) ? a : b
#define min(a, b) (a < b) ? a : b

#endif
