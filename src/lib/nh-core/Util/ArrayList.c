// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "ArrayList.h"
#include "Array.h"
#include "List.h"

#include "../System/Memory.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// LIST ============================================================================================

nh_core_ArrayList nh_core_initArrayList(
    int elementSize, int allocatedLengthPerChunk)
{
    nh_core_ArrayList ArrayList;

    ArrayList.Arrays = nh_core_initList(8);
    ArrayList.elementSize = elementSize;
    ArrayList.allocatedLengthPerChunk = allocatedLengthPerChunk;

    return ArrayList;
}

static void *nh_core_incrementArrayListRecursion(
    nh_core_ArrayList *ArrayList_p, int array)
{
    nh_core_Array *Array_p = nh_core_getFromList(&ArrayList_p->Arrays, array);

    if (!Array_p) {
        Array_p = nh_core_allocate(sizeof(nh_core_Array));
        if (!Array_p) {return NULL;}
        *Array_p = nh_core_initArray(ArrayList_p->elementSize, ArrayList_p->allocatedLengthPerChunk);
        if (nh_core_appendToList(&ArrayList_p->Arrays, Array_p) != NH_API_SUCCESS) {return NULL;}
    }

    if (Array_p->length < ArrayList_p->allocatedLengthPerChunk - 1) {
        return nh_core_getFromArray(Array_p, Array_p->length);
    }
    else {
        return nh_core_incrementArrayListRecursion(ArrayList_p, array + 1);
    }
}

void *nh_core_incrementArrayList(
    nh_core_ArrayList *ArrayList_p)
{
    return nh_core_incrementArrayListRecursion(ArrayList_p, 0);
}

void nh_core_freeArrayList(
    nh_core_ArrayList *ArrayList_p)
{
    for (int i = 0; i < ArrayList_p->Arrays.size; ++i) {
        nh_core_freeArray(ArrayList_p->Arrays.pp[i]);
    }

    nh_core_freeList(&ArrayList_p->Arrays, true);

    ArrayList_p->allocatedLengthPerChunk = 0;
    ArrayList_p->elementSize = 0;
}

