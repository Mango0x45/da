/*
 * Simple & stupid dynamic array single-header implementation.  You can use the
 * macros defined in this file with any structure that has the following fields:
 *
 *  struct dyn_array {
 *      T *buf  // Array of items
 *      N  len  // Length of array
 *      N  cap  // Capacity of array
 *  }
 *
 * The type ‘T’ is whatever type you want to store.  The type ‘N’ is any numeric
 * type — most likely ‘size_t’ — but it could be sized as well.
 *
 * The daremove() macro also doesn’t bother with shrinking your array when the
 * length is far lower than the capacity.  If you care about that, do it
 * yourself.
 *
 * Remember to call free() on your dynamic arrays ‘buf’ field after use.
 *
 *
 * Macro Overview
 * ――――――――――――――
 * The argument ‘a’ to all of the below macros is a pointer to the dynamic array
 * structure.
 *
 * dainit(a, n)              Initialize the array with a capacity of ‘n’ items.
 * dapush(a, x)              Append the item ‘x’ to the array
 * daremove(a, x)            Remove the item at index ‘x’ from the array
 * da_remove_range(a, x, y)  Remove the items between the range [x, y)
 * da_foreach(a, p)          Iterate the pointer ‘p’ over each element of the
 *                           array.  The type of ‘p’ is inferred.
 *
 * The ‘dapush()’ macro will double the arrays capacity when it gets full.  If
 * you would like your arrays to grow with a different scale, edit this file.
 *
 *
 * Example
 * ―――――――
 *
 * struct {
 *     int *buf;
 *     size_t len, cap;
 * } nums;
 *
 * // Initialize nums with capacity == 4
 * dainit(&nums, 4);
 *
 * // Append 69, 1337, and 420 to nums
 * dapush(&nums, 69);
 * dapush(&nums, 1337);
 * dapush(&nums, 420);
 *
 * da_foreach (&nums, n) {
 *     int x = *n << 1;
 *     printf("n = %d; n² = %d\n", *n, x);
 * }
 *
 * // Remove 1337 and 420 from nums
 * da_remove_range(&nums, 1, 3);
 *
 * // Remove 69 from nums
 * daremove(&nums, 0);
 */

#ifndef MANGO_DA_H
#define MANGO_DA_H

#include <err.h>
#include <errno.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ >= 202311L
#	define DA_NULL nullptr
#else
#	define DA_NULL NULL
#endif

#define DA_ALLOC(p, n) \
	do { \
		if ((n) && SIZE_MAX / (n) < sizeof(*(p))) { \
			errno = EOVERFLOW; \
			err(EXIT_FAILURE, "realloc"); \
		} \
		if (!((p) = realloc((p), (n) * sizeof(*(p))))) \
			err(EXIT_FAILURE, "realloc"); \
	} while (0)

#define dainit(a, n) \
	do { \
		(a)->buf = DA_NULL; \
		(a)->cap = (n); \
		(a)->len = 0; \
		if (n) \
			DA_ALLOC((a)->buf, (a)->cap); \
	} while (0)

#define dapush(a, x) \
	do { \
		if ((a)->len >= (a)->cap) { \
			(a)->cap = (a)->cap ? (a)->cap * 2 : 1; \
			DA_ALLOC((a)->buf, (a)->cap); \
		} \
		(a)->buf[(a)->len++] = (x); \
	} while (0)

#define daremove(a, i) da_remove_range((a), (i), (i) + 1)

#define da_remove_range(a, i, j) \
	do { \
		memmove((a)->buf + (i), (a)->buf + (j), \
		        ((a)->len - (j)) * sizeof(*(a)->buf)); \
		(a)->len -= j - i; \
	} while (0)

#define da_foreach(a, p) \
	for (auto p = (a)->buf; (size_t)(p - (a)->buf) < (a)->len; p++)

#endif /* !MANGO_DA_H */
