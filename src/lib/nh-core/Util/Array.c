// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Array.h"

#include "../System/Memory.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// LIST ============================================================================================

nh_core_Array nh_core_initArray(
    int elementSize, int allocatedLengthPerChunk)
{
    nh_core_Array Array;

    Array.elementSize = elementSize;
    Array.allocatedLengthPerChunk = allocatedLengthPerChunk;
    Array.allocatedLength = 0;
    Array.length = 0;
    Array.p = NULL;

    return Array;
}

static NH_API_RESULT nh_core_updateArrayLength(
    nh_core_Array *Array_p, unsigned long count, size_t *offset_p)
{
    if (count <= 0) { return NH_API_SUCCESS; }

    Array_p->length += count;

    while (Array_p->allocatedLength < Array_p->length)
    {
        if (Array_p->allocatedLength == 0) {
            Array_p->p = nh_core_allocate(Array_p->allocatedLengthPerChunk * Array_p->elementSize);
            if (!Array_p->p) {
                return NH_API_ERROR_BAD_STATE;  // Memory allocation failed
            }
            memset(Array_p->p, 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);
        }
        else {
            void *new_p = nh_core_allocate((Array_p->allocatedLength + Array_p->allocatedLengthPerChunk) * Array_p->elementSize);
            if (!new_p) {
                return NH_API_ERROR_BAD_STATE;  // Memory allocation failed
            }

            memcpy(new_p, Array_p->p, Array_p->allocatedLength * Array_p->elementSize);
            nh_core_free(Array_p->p);  // Free the old buffer
            Array_p->p = new_p;
            memset(Array_p->p + (Array_p->allocatedLength * Array_p->elementSize), 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);
        }

        Array_p->allocatedLength += Array_p->allocatedLengthPerChunk;
    }

    // Ensure that the array is null-terminated
    if (Array_p->allocatedLength == Array_p->length) {
        void *new_p = nh_core_allocate((Array_p->allocatedLength + Array_p->allocatedLengthPerChunk) * Array_p->elementSize);
        if (!new_p) {
            return NH_API_ERROR_BAD_STATE;  // Memory allocation failed
        }

        memcpy(new_p, Array_p->p, Array_p->allocatedLength * Array_p->elementSize);
        nh_core_free(Array_p->p);  // Free old buffer
        Array_p->p = new_p;
        memset(Array_p->p + (Array_p->allocatedLength * Array_p->elementSize), 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);

        Array_p->allocatedLength += Array_p->allocatedLengthPerChunk;
    }

    *offset_p = (Array_p->length - count) * Array_p->elementSize;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_appendToArray(
    nh_core_Array *Array_p, void *p, unsigned long count)
{
    size_t offset = 0;
    NH_CORE_CHECK(nh_core_updateArrayLength(Array_p, count, &offset))

    memcpy((char *)Array_p->p + offset, p, Array_p->elementSize * count);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_appendToArrayRepeatedly(
    nh_core_Array *Array_p, void *p, unsigned long count)
{
    size_t offset = 0;
    NH_CORE_CHECK(nh_core_updateArrayLength(Array_p, count, &offset))

    for (int i = 0; i < count; ++i) {
        memcpy((char *)Array_p->p + offset + (i * Array_p->elementSize), p, Array_p->elementSize);
    }

    return NH_API_SUCCESS;
}

char *nh_core_getFromArray(
    nh_core_Array *Array_p, unsigned long index)
{
    if (index < 0) { index = 0; }

    while (Array_p->allocatedLength <= index)
    {
        if (Array_p->allocatedLength == 0) {
            Array_p->p = nh_core_allocate(Array_p->elementSize * Array_p->allocatedLengthPerChunk);
            if (!Array_p->p) {
                return NULL;  // Allocation failed
            }
            memset(Array_p->p, 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);
        }
        else {
            void *new_p = nh_core_allocate((Array_p->allocatedLength + Array_p->allocatedLengthPerChunk) * Array_p->elementSize);
            if (!new_p) {
                return NULL;  // Allocation failed
            }

            memcpy(new_p, Array_p->p, Array_p->allocatedLength * Array_p->elementSize);
            nh_core_free(Array_p->p);  // Free old buffer
            Array_p->p = new_p;
            memset(Array_p->p + (Array_p->allocatedLength * Array_p->elementSize), 0, Array_p->allocatedLengthPerChunk * Array_p->elementSize);
        }

        Array_p->allocatedLength += Array_p->allocatedLengthPerChunk;
    }

    if (index >= Array_p->length) { Array_p->length = index + 1; }

    return Array_p->p + (index * Array_p->elementSize);
}

char *nh_core_incrementArray(
    nh_core_Array *Array_p)
{
    return nh_core_getFromArray(Array_p, Array_p->length);
}

NH_API_RESULT nh_core_removeTailFromArray(
    nh_core_Array *Array_p, unsigned int count)
{
    Array_p->length -= count;

    if (Array_p->length < 0) { Array_p->length = 0; }

    while ((Array_p->allocatedLength - Array_p->allocatedLengthPerChunk) >= (int)Array_p->length)
    {
        if ((Array_p->allocatedLength - Array_p->allocatedLengthPerChunk) == 0) {
            nh_core_free(Array_p->p);
            Array_p->p = NULL;
        } else {
            Array_p->p = realloc(Array_p->p, (Array_p->allocatedLength - Array_p->allocatedLengthPerChunk) * Array_p->elementSize);
            NH_CORE_CHECK_NULL(Array_p->p)
        }
        Array_p->allocatedLength -= Array_p->allocatedLengthPerChunk;
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_removeFromArray(
    nh_core_Array *Array_p, int index, unsigned int count)
{
    if (index >= Array_p->length) { return NH_API_SUCCESS; }

    if (index + count >= Array_p->length) {
        return nh_core_removeTailFromArray(Array_p, count);
    }

    int cpyLength = Array_p->length - index - count;

    void *cpy_p = nh_core_allocate(Array_p->elementSize * cpyLength);
    NH_CORE_CHECK_MEM(cpy_p)
    memcpy(cpy_p, nh_core_getFromArray(Array_p, index + count), Array_p->elementSize * cpyLength);

    NH_CORE_CHECK(nh_core_removeTailFromArray(Array_p, cpyLength + count))
    NH_CORE_CHECK(nh_core_appendToArray(Array_p, cpy_p, cpyLength))

    nh_core_free(cpy_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_insertIntoArray(
    nh_core_Array *Array_p, int index, void *elements_p, int length)
{
    bool copy = true;
    if (index >= Array_p->length) {
        copy = false;
        index = Array_p->length;
    }

    int removeLength = Array_p->length - index;

    void *cpy_p = NULL;
    if (copy) {
        cpy_p = nh_core_allocate(Array_p->elementSize * removeLength);
        NH_CORE_CHECK_MEM(cpy_p)
        memcpy(cpy_p, nh_core_getFromArray(Array_p, index), Array_p->elementSize * removeLength);
    }

    NH_CORE_CHECK(nh_core_removeTailFromArray(Array_p, removeLength))
    NH_CORE_CHECK(nh_core_appendToArray(Array_p, elements_p, length))

    if (copy) {
        NH_CORE_CHECK(nh_core_appendToArray(Array_p, cpy_p, removeLength))
        nh_core_free(cpy_p);
    }

    return NH_API_SUCCESS;
}

void nh_core_freeArray(
    nh_core_Array *Array_p)
{
    if (Array_p->p != NULL) { nh_core_free(Array_p->p); }

    Array_p->allocatedLength = 0;
    Array_p->length = 0;
    Array_p->p = NULL;

    return;
}
