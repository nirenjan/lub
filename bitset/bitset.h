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

#ifndef __BITSET_H
#define __BITSET_H

#include "cdecl.h"
#include <stdint.h>

__CDECL_BEGIN

/** @defgroup Bitset Bitset API */

/** @addtogroup Bitset
 * @{
 */

/** @brief Structure that holds all bitset data
 *
 * All the bitset APIs will be passed a pointer to this data type.
 */
typedef struct bitset_s bitset_t;

/** @brief Create a new bitset
 *
 * Create a new bitset of the given length.
 *
 * @param   length  The desired length of the bitset. This cannot be changed
 *                  during runtime.
 *
 * @returns Pointer to the allocated bitset with all bits set to zero. If it
 *          cannot allocate the necessary memory, it returns NULL and sets
 *          errno accordingly.
 */
bitset_t * bitset_init(size_t length);

/** @brief Free a previously allocated bitset
 *
 * @param   bitset  Pointer to an allocated bitset. The API will reset the
 *                  data structures and free the associated memory.
 *
 * @returns nothing
 */
void bitset_free(bitset_t *bitset);

/** @brief Set a bit in the bitset
 *
 * @param   bitset  Pointer to an allocated bitset
 * @param   bit     Index of the bit to set
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly
 */
int bitset_set(bitset_t *bitset, size_t bit);

/** @brief Clear a bit in the bitset
 *
 * @param   bitset  Pointer to an allocated bitset
 * @param   bit     Index of the bit to clear
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly
 */
int bitset_clr(bitset_t *bitset, size_t bit);

/** @brief Toggle a bit in the bitset
 *
 * @param   bitset  Pointer to an allocated bitset
 * @param   bit     Index of the bit to toggle
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly
 */
int bitset_tgl(bitset_t *bitset, size_t bit);

/** @brief Take the intersection of two bitsets
 *
 * Take the intersection of two bitsets and save it in the first bitset.
 * The intersection is the bits which are set in both bitsets. If the bitsets
 * are not of the same length, then the number of bits compared is the minimum
 * of the length of the two bitsets. The remaining bits are left unchanged.
 *
 * @param   dst     Pointer to the destination bitset
 * @param   src     Pointer to the source bitset which is bitwise AND'ed with
 *                  the destination bitset.
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly.
 */
int bitset_and(bitset_t *dst, bitset_t *src);

/** @brief Take the union of two bitsets
 *
 * Take the union of two bitsets and save it in the first bitset.
 * The union is the bits which are set in either bitset. If the bitsets
 * are not of the same length, then the number of bits compared is the minimum
 * of the length of the two bitsets. The remaining bits are left unchanged.
 *
 * @param   dst     Pointer to the destination bitset
 * @param   src     Pointer to the source bitset which is bitwise OR'ed with
 *                  the destination bitset.
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly.
 */
int bitset_or(bitset_t *dst, bitset_t *src);

/** @brief Take the symmetric difference of two bitsets
 *
 * Take the symmetric difference of two bitsets and save it in the first bitset.
 * The symmetric difference is the bits which are set in either bitset, but not
 * in both. If the bitsets are not of the same length, then the number of bits
 * compared is the minimum of the length of the two bitsets. The remaining bits
 * are left unchanged.
 *
 * @param   dst     Pointer to the destination bitset
 * @param   src     Pointer to the source bitset which is bitwise XOR'ed with
 *                  the destination bitset.
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly.
 */
int bitset_xor(bitset_t *dst, bitset_t *src);

/** @brief Take the complement of a bitset
 *
 * Take the complement of the given bitset and store it in the same location.
 * The complement inverts all bits, that is, all 0 bits become 1 bits and
 * vice versa.
 *
 * @param   dst     Pointer to the bitset
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly.
 */
int bitset_not(bitset_t *dst);


/** @} */

__CDECL_END

#endif /* !defined __BITSET_H */
