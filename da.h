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
 * You should include ‘err.h’ and ‘stdlib.h’ along with this file.  If you want
 * to use da_remove(), include ‘string.h’.  The da_remove() macro also doesn’t
 * bother with shrinking your array when the length is far lower than the
 * capacity.  If you care about that, do it yourself.  To use the da_foreach()
 * macro, you need to have ptrdiff_t defined; to be absolutely sure you can
 * include ‘stddef.h’.
 *
 * Remember to call free() on your dynamic arrays ‘buf’ field after use.
 *
 * Macro Overview
 * ――――――――――――――
 * The argument ‘a’ to all of the below macros is a pointer to the dynamic array
 * structure.
 *
 * dainit(a, n)              Initialize the array with a capacity of ‘n’ items.
 * dapush(a, x)              Append the item ‘x’ to the array
 * daremove(a, x)            Remove the item ‘x’ from the array
 * da_remove_range(a, x, y)  Remove the items between the range [x, y)
 * da_foreach(a, p)          Iterate the pointer ‘p’ over each element of the
 *                           array.  The type of ‘p’ is inferred.
 *
 * The ‘dapush()’ macro will by double the arrays capacity when it gets full.
 * If you would like your arrays to grow with a different scale, edit this file.
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

#define __da_s(a) (sizeof(*(a)->buf))

#define dainit(a, n) \
	do { \
		(a)->cap = n; \
		(a)->len = 0; \
		if (!((a)->buf = malloc((a)->cap * __da_s(a)))) \
			err(EXIT_FAILURE, "malloc"); \
	} while (0)

#define dapush(a, x) \
	do { \
		if ((a)->len >= (a)->cap) { \
			(a)->cap = (a)->cap ? (a)->cap * 2 : 1; \
			if (!((a)->buf = realloc((a)->buf, (a)->cap * __da_s(a)))) \
				err(EXIT_FAILURE, "realloc"); \
		} \
		(a)->buf[(a)->len++] = (x); \
	} while (0)

#define daremove(a, i) da_remove_range((a), (i), (i) + 1)

#define da_remove_range(a, i, j) \
	do { \
		memmove((a)->buf + (i), (a)->buf + (j), ((a)->len - (j)) * __da_s(a)); \
		(a)->len -= j - i; \
	} while (0)

#define da_foreach(a, p) \
	for (auto p = (a)->buf; p - (a)->buf < (ptrdiff_t)(a)->len; p++)

#endif /* !MANGO_DA_H */
