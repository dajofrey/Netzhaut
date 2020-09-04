// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/RingIterator.h"
#include "../Header/Macros.h"
#include "../Header/String.h"
#include "../Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

// ITERATOR ========================================================================================

NH_RESULT Nh_initRingIterator(
    Nh_RingIterator *Iterator_p, int itemCount, int itemByteSize)
{
NH_BEGIN()

    Iterator_p->itemCount = itemCount;
    Iterator_p->itemByteSize = itemByteSize;

    Iterator_p->overflow = 0;
    Iterator_p->index   = -1;
 
    Iterator_p->data_p = Nh_allocate(itemCount * itemByteSize);
    NH_CHECK_MEM(Iterator_p->data_p)

NH_END(NH_SUCCESS)
}

void *Nh_advanceRingIterator(
    Nh_RingIterator *Iterator_p)
{
NH_BEGIN()

    Iterator_p->index++;
    if (Iterator_p->index == Iterator_p->itemCount) {Iterator_p->index = 0; Iterator_p->overflow += 1;}
    
NH_END(Iterator_p->data_p + (Iterator_p->index * Iterator_p->itemByteSize))
}

void Nh_freeRingIterator(
    Nh_RingIterator *Iterator_p)
{
NH_BEGIN()

    Nh_free(Iterator_p->data_p);

NH_SILENT_END()
}

// MARK ============================================================================================

void Nh_initRingIteratorMark(
    Nh_RingIteratorMark *Mark_p)
{
NH_BEGIN()

    Mark_p->overflow = 0;
    Mark_p->index = -1;

NH_SILENT_END()
}

void *Nh_incrementRingIteratorMark(
    Nh_RingIterator *Iterator_p, Nh_RingIteratorMark *Mark_p)
{
NH_BEGIN()

    if (Iterator_p->index == Mark_p->index && Iterator_p->overflow == Mark_p->overflow) {NH_END(NULL)}

    Mark_p->index++;

    if (Iterator_p->overflow != Mark_p->overflow && Mark_p->index == Iterator_p->itemCount) {
        Mark_p->index = 0; Mark_p->overflow = Iterator_p->overflow;
    }

NH_END(Iterator_p->data_p + (Mark_p->index * Iterator_p->itemByteSize))
}

