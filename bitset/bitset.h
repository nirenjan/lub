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
struct bitset;

/** @name Memory management
 * @{
 */

/** @brief Create a new bitset
 *
 * Create a new bitset of the given length.
 *
 * @param   length  The desired length of the bitset.
 *
 * @returns Pointer to the allocated bitset. If it cannot allocate the
 *          necessary memory, it returns NULL and sets errno accordingly. No
 *          guarantee is made on the state of the bits.
 */
struct bitset * bitset_malloc(size_t length);

/** @brief Resize an initialized bitset
 *
 * Resizes an allocated bitset to the new size. The bit state is copied to
 * the resized bitset, but truncated if the new bitset is smaller. If the
 * new bitset is larger, the state of the new bits is undefined.
 *
 * @param   set     Pointer to the allocated bitset
 * @param   length  The desired length of the bitset.
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly.
 */
int bitset_resize(struct bitset *set, size_t length);

/** @brief Free a previously allocated bitset
 *
 * @param   set     Pointer to an allocated bitset. The API will reset the
 *                  data structures and free the associated memory.
 *
 * @returns nothing
 */
void bitset_free(struct bitset *set);

/** @brief Create a copy of a bitset.
 *
 * This can be used to make a copy of a bitset. However, if there is
 * insufficient memory to make the copy, then it will fail.
 *
 * @param   set     Pointer to the bitset to duplicate
 *
 * @returns Pointer to the duplicated bitset. If it cannot allocate the
 *          necessary memory, it returns NULL and sets errno accordingly.
 */
struct bitset * bitset_dup(struct bitset *set);

/** @} */

/** @name Bitset Update Functions
 * @{
 */

/** @brief Set a bit in the bitset
 *
 * @param   set     Pointer to an allocated bitset
 * @param   bit     Index of the bit to set
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly
 */
int bitset_set(struct bitset *set, size_t bit);

/** @brief Reset a bit in the bitset
 *
 * @param   set     Pointer to an allocated bitset
 * @param   bit     Index of the bit to clear
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly
 */
int bitset_reset(struct bitset *set, size_t bit);

/** @brief Toggle a bit in the bitset
 *
 * @param   set     Pointer to an allocated bitset
 * @param   bit     Index of the bit to toggle
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly
 */
int bitset_toggle(struct bitset *set, size_t bit);

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
int bitset_not(struct bitset *dst);

/** @brief Clear all the bits in a bitset
 *
 * Clear all bits in the bitset. This is typically used after @ref
 * bitset_malloc to get to a known state.
 *
 * @param   set     Pointer to the bitset
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly.
 */
int bitset_clear(struct bitset *set);

/** @} */

/** @name Bitset Merge Functions
 * @{
 */

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
int bitset_and(struct bitset *dst, struct bitset *src);

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
int bitset_or(struct bitset *dst, struct bitset *src);

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
int bitset_xor(struct bitset *dst, struct bitset *src);

/** @name Bitset Access Functions
 * @{
 */

/** @brief Test the value of a bit
 *
 * Return the status of the bit in the given pointer.
 *
 * @param   set     Pointer to the bitset
 * @param   bit     Index of the bit to test
 * @param   value   Pointer to the return value. If this is NULL, then the
 *                  call will fail. The value is either 0 or 1 corresponding
 *                  to the actual bit value.
 *
 * @returns 0 on success, 1 on failure and sets errno accordingly.
 */
int bitset_test(struct bitset *set, size_t bit, unsigned int *value);

/** @} */

/** @} */

/** @} */

__CDECL_END

#endif /* !defined __BITSET_H */
