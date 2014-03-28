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
#include <errno.h>
#include "bitset.h"

struct bitset_s {
    uintptr_t length;
    unsigned int *map;
};

#define BITS_PER_INDEX (sizeof(unsigned int) * 8)

bitset_t * bitset_init(size_t length)
{
    bitset_t *bitset;
    size_t bitset_length;

    bitset = (bitset_t *)malloc(sizeof(bitset_t));

    if (bitset == NULL) {
        /* Unable to allocate bitset root */
        errno = ENOMEM;
        return (NULL);
    }

    /* Allocate as many bytes as needed for the bitset itself */
    bitset_length = (length + BITS_PER_INDEX - 1) / BITS_PER_INDEX;
    bitset->length = length;
    bitset->map = calloc(bitset_length, sizeof(unsigned int));

    if (!bitset->map) {
        /* Unable to allocate bitset. Free the bitset root and return NULL */
        free(bitset);
        errno = ENOMEM;
        return (NULL);
    }

    return (bitset);
}

void bitset_free(bitset_t *bitset)
{
    if (bitset) {
        if (bitset->map) {
            free(bitset->map);
            bitset->map = NULL;
        }
        bitset->length = 0;
        free(bitset);
    }
}

int bitset_set(bitset_t *bitset, size_t bit)
{
    size_t index = bit / BITS_PER_INDEX;
    size_t offset = bit % BITS_PER_INDEX;

    if (bitset) {
        if (bitset->length >= bit) {
            bitset->map[index] |= (1 << offset);
        } else {
            errno = ERANGE;
            return 1;
        }
    } else {
        errno = EINVAL;
        return 1;
    }

    return 0;
}

int bitset_clr(bitset_t *bitset, size_t bit)
{
    size_t index = bit / BITS_PER_INDEX;
    size_t offset = bit % BITS_PER_INDEX;

    if (bitset) {
        if (bitset->length >= bit) {
            bitset->map[index] &= ~(1 << offset);
        } else {
            errno = ERANGE;
            return 1;
        }
    } else {
        errno = EINVAL;
        return 1;
    }

    return 0;
}

int bitset_tgl(bitset_t *bitset, size_t bit)
{
    size_t index = bit / BITS_PER_INDEX;
    size_t offset = bit % BITS_PER_INDEX;

    if (bitset) {
        if (bitset->length >= bit) {
            bitset->map[index] ^= (1 << offset);
        } else {
            errno = ERANGE;
            return 1;
        }
    } else {
        errno = EINVAL;
        return 1;
    }

    return 0;
}

#define MIN(a,b) ((a) > (b) ? (b) : (a))

int bitset_and(bitset_t *dst, bitset_t *src)
{
    size_t comp_length;

    if (!dst || !src) {
        errno = EINVAL;
        return 1;
    }

    comp_length = MIN(dst->length, src->length);
    comp_length = (comp_length + BITS_PER_INDEX - 1) / BITS_PER_INDEX;

    return 0;
}
