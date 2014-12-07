/**********************************************************************
 * Bitset wrapper library
 **********************************************************************
 * Copyright (C) 2014 Nirenjan Krishnan (nirenjan@gmail.com)
 *
 * This library is used to manage a bitset of arbitrary size. The
 * bitsets can be manipulated using the API in this file. The size
 * of the bitsets are fixed at initialization time.
 *********************************************************************/
/** @file */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "bitset.h"

/**********************************************************************
 * Merge Operators
 *
 * These utility functions are used for the corresponding merge
 * operations by the update and merge wrappers
 **********************************************************************/
static unsigned int bitset_merge_and(unsigned int a, unsigned int b)
{
    return (a & b);
}

static unsigned int bitset_merge_andn(unsigned int a, unsigned int b)
{
    return (a & ~b);
}

static unsigned int bitset_merge_or(unsigned int a, unsigned int b)
{
    return (a | b);
}

static unsigned int bitset_merge_xor(unsigned int a, unsigned int b)
{
    return (a ^ b);
}

static unsigned int bitset_merge_not(unsigned int a, unsigned int b)
{
    b = b; /* To avoid unused variable warnings */
    return (~a);
}

/* This is the merge utility function type */
typedef unsigned int (*bitset_merge_fn)(unsigned int, unsigned int);

/*
 * This function is used to find the minimum of two size_t's. It's used
 * in preference to a macro to avoid potential pitfalls.
 */
static inline size_t min(size_t a, size_t b)
{
    return ((a > b) ? b : a);
}

/*
 * The bitset is stored as a struct with a length (in number of bits),
 * and a pointer to the actual map. The map is an array of unsigned int's,
 * rather than a fixed size, thereby allowing the compiler to optimize
 * for the architecture being built for.
 */
struct bitset {
    size_t length;      /* Number of bits in the bitset */
    size_t map_length;  /* Number of ints in the map array */
    unsigned int *map;  /* Pointer to the map array */
};

#define BITS_PER_INDEX (sizeof(unsigned int) * 8)

/*
 * Utility function to compute the map size for a given bitset size
 */
static inline size_t bitset_map_size(size_t bitset_size)
{
    return ((bitset_size + BITS_PER_INDEX - 1) / BITS_PER_INDEX);
}

/*
 * bitset_malloc
 *
 * Allocate a bitset of the given size
 */
struct bitset * bitset_malloc(size_t length)
{
    struct bitset *set;
    size_t bitset_length;

    set = (struct bitset *)malloc(sizeof(struct bitset));

    if (set == NULL) {
        /* Unable to allocate bitset root */
        errno = ENOMEM;
        return (NULL);
    }

    /* Allocate as many bytes as needed for the bitset itself */
    bitset_length = bitset_map_size(length);
    set->length = length;
    set->map_length = bitset_length;
    set->map = malloc(bitset_length * sizeof(unsigned int));

    if (!set->map) {
        /* Unable to allocate bitset. Free the bitset root and return NULL */
        free(set);
        errno = ENOMEM;
        return (NULL);
    }

    return (set);
}

/*
 * bitset_dup
 *
 * Make a duplicate of a bitset
 */
struct bitset * bitset_dup(struct bitset *set)
{
    struct bitset * new_bitset;

    if (!set || !set->map) {
        errno = EINVAL;
        return NULL;
    }

    new_bitset = bitset_malloc(set->length);
    if (!new_bitset) {
        /* Errno has already been set to ENOMEM by bitset_malloc */
        return NULL;
    }

    /* Copy the contents of the original bitset to the new bitset */
    memcpy(new_bitset->map, set->map,
            set->map_length * sizeof(unsigned int));

    return new_bitset;
}

/*
 * bitset_resize
 *
 * Resize a given bitset to the new size
 */
int bitset_resize(struct bitset *set, size_t length)
{
    size_t new_map_length;
    unsigned int *new_map;
    unsigned int *old_map;

    if (!set || !set->map) {
        errno = EINVAL;
        return 1;
    }

    new_map_length = bitset_map_size(length);
    if (new_map_length == set->map_length) {
        /* 
         * No need to resize the array itself, just go ahead
         * and update the length
         */
        set->length = length;
        return 0;
    }

    old_map = set->map;
    new_map = realloc(old_map, new_map_length * sizeof(unsigned int));
    if (!new_map) {
        /* Failed to allocate memory. Retain the existing map */
        errno = ENOMEM;
        return 1;
    }

    set->length = length;
    set->map_length = new_map_length;
    set->map = new_map;

    return 0;
}

void bitset_free(struct bitset *set)
{
    if (set) {
        if (set->map) {
            free(set->map);
            set->map = NULL;
        }
        set->length = 0;
        set->map_length = 0;
        free(set);
    }
}

static int bitset_update(struct bitset *set, size_t bit, bitset_merge_fn fn)
{
    size_t index = bit / BITS_PER_INDEX;
    size_t offset = bit % BITS_PER_INDEX;
    int retval = 0;

    if (set && set->map) {
        if (set->length >= bit) {
            set->map[index] = fn(set->map[index], (1 << offset));
        } else {
            errno = ERANGE;
            retval = 1;
        }
    } else {
        errno = EINVAL;
        retval = 1;
    }

    return retval;
}

int bitset_set(struct bitset *set, size_t bit)
{
    return bitset_update(set, bit, bitset_merge_or);
}

int bitset_reset(struct bitset *set, size_t bit)
{
    return bitset_update(set, bit, bitset_merge_andn);
}

int bitset_toggle(struct bitset *set, size_t bit)
{
    return bitset_update(set, bit, bitset_merge_xor);
}

static int bitset_merge(struct bitset *dst, struct bitset *src, bitset_merge_fn fn)
{
    size_t comp_length;
    size_t i;

    if (!dst || !src || !dst->map || !src->map) {
        errno = EINVAL;
        return 1;
    }

    comp_length = min(dst->map_length, src->map_length);

    for (i = 0; i < comp_length; i++) {
        /* Merge the two bitsets */
        dst->map[i] = fn(dst->map[i], src->map[i]);
    }

    return 0;
}

int bitset_and(struct bitset *dst, struct bitset *src)
{
    return bitset_merge(dst, src, bitset_merge_and);
}

int bitset_or(struct bitset *dst, struct bitset *src)
{
    return bitset_merge(dst, src, bitset_merge_or);
}

int bitset_xor(struct bitset *dst, struct bitset *src)
{
    return bitset_merge(dst, src, bitset_merge_xor);
}

int bitset_andn(struct bitset *dst, struct bitset *src)
{
    return bitset_merge(dst, src, bitset_merge_andn);
}

int bitset_not(struct bitset *dst)
{
    /*
     * The src operand is dummy in this case, it's just required by the
     * bitset_merge function, but ignored by the bitset_merge_not function
     */
    return bitset_merge(dst, dst, bitset_merge_not);
}

int bitset_clear(struct bitset *set)
{
    if (!set || !set->map) {
        errno = EINVAL;
        return 1;
    }

    memset(set->map, 0, set->map_length * sizeof(unsigned int));

    return 0;
}

int bitset_test(struct bitset *set, size_t bit, unsigned int *value)
{
    size_t index = bit / BITS_PER_INDEX;
    size_t offset = bit % BITS_PER_INDEX;

    if (!set || !set->map || !value) {
        errno = EINVAL;
        return 1;
    }

    *value = !!(set->map[index] & (1 << offset));
    return 0;
}
