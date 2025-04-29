// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "List.h"
#include "../System/Memory.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

nh_core_List _nh_core_initList(
    unsigned long chunkSize)
{
    nh_core_List List;
    List.chunkSize  = chunkSize;
    List.chunkCount = 0;
    List.size       = 0;
    List.pp         = NULL;
    return List;
}

nh_core_List nh_core_initList(
    unsigned long chunkSize)
{
    return _nh_core_initList(chunkSize);
}

NH_API_RESULT nh_core_appendToList(
    nh_core_List *List_p, void *handle_p)
{
    if (List_p->size >= List_p->chunkSize * List_p->chunkCount) 
    {
        if (List_p->pp == NULL) {
            List_p->pp = (void**)nh_core_allocate(sizeof(void*) * List_p->chunkSize);
            if (List_p->pp == NULL) {return NH_API_ERROR_BAD_STATE;}
        }
        else {
            List_p->pp = realloc(List_p->pp, sizeof(void*) * List_p->chunkSize * (List_p->chunkCount + 1));
            if (List_p->pp == NULL) {return NH_API_ERROR_BAD_STATE;}
        }
        List_p->chunkCount++;
    }

    List_p->pp[List_p->size] = handle_p;
    List_p->size++;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_appendItemsToList(
    nh_core_List *List_p, nh_core_List *Append_p)
{
    for (int i = 0; i < Append_p->size; ++i) {
        NH_CORE_CHECK(nh_core_appendToList(List_p, Append_p->pp[i]))
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_insertIntoList(
    nh_core_List *List_p, void *handle_p, unsigned long index)
{
    NH_CORE_CHECK_NULL(List_p)

    while (List_p->size <= index) {
        NH_CORE_CHECK(nh_core_appendToList(List_p, NULL))
    }

    for (unsigned long i = List_p->size - 1; i > index; --i) {
        List_p->pp[i] = List_p->pp[i - 1];
    }

    List_p->pp[index] = handle_p;

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_prependToList(
    nh_core_List *List_p, void *handle_p)
{
    return nh_core_insertIntoList(List_p, handle_p, 0);
}

void *_nh_core_getFromList(
    nh_core_List *List_p, unsigned long index)
{
    if (List_p == NULL || index < 0 || List_p->size <= index) {
        return NULL;
    }
    return List_p->pp[index];
}

void *nh_core_getFromList(
    nh_core_List *List_p, unsigned long index)
{
    return _nh_core_getFromList(List_p, index);
}

unsigned long nh_core_getListIndex(
    nh_core_List *List_p, void *handle_p)
{
    for (unsigned long i = 0; i < List_p->size; ++i) {
        if (List_p->pp[i] == handle_p) {return i;}
    }

    return 0;
}

void *nh_core_getNextListItem(
    nh_core_List *List_p, void *handle_p)
{
    for (unsigned long i = 0; i < List_p->size; ++i) {
        if (List_p->pp[i] == handle_p) {
            if (i + 1 < List_p->size) {return List_p->pp[i + 1];}
        }
    }

    return NULL;
}

void nh_core_freeList(
    nh_core_List *List_p, bool freeHandles)
{
    if (freeHandles) {
        for (int i = 0; i < List_p->size; ++i) {
            nh_core_free(List_p->pp[i]);
        }
    }
    nh_core_free(List_p->pp);

    List_p->chunkCount = 0;
    List_p->size = 0;
    List_p->pp = NULL;
}

NH_API_RESULT nh_core_removeFromList(
    nh_core_List *List_p, bool freeHandle, unsigned int index)
{
    if (index < 0 || List_p->size == 0) {return NH_API_ERROR_BAD_STATE;}
    if (index >= List_p->size) {index = List_p->size - 1;}

    if (freeHandle) {
        nh_core_free(List_p->pp[index]);
    }

    for (int i = index; i < List_p->size - 1; ++i) {
        List_p->pp[i] = List_p->pp[i + 1];
    }

    List_p->size--;

    if (List_p->size == 0) {
        nh_core_free(List_p->pp);
        List_p->pp = NULL;
        List_p->chunkCount = 0;
    }
    else if (List_p->size == List_p->chunkSize * (List_p->chunkCount - 1)) {
        List_p->pp = realloc(List_p->pp, sizeof(void*) * List_p->size);
        if (List_p->pp == NULL) {return NH_API_ERROR_BAD_STATE;}
        List_p->chunkCount--;
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_removeFromList2(
    nh_core_List *List_p, bool freeHandle, void *handle_p)
{
    bool ok = false;
    unsigned long index = 0;

    for (unsigned long i = 0; i < List_p->size; ++i) {
        if (List_p->pp[i] == handle_p) {
            index = i; 
            ok = true; 
            break;
        }
    }

    if (ok) {
        return nh_core_removeFromList(List_p, freeHandle, index);
    }

    return NH_API_ERROR_BAD_STATE;
}

bool nh_core_inList(
    nh_core_List *List_p, void *handle_p)
{
    for (int i = 0; i < List_p->size; ++i) {
        if (List_p->pp[i] == handle_p) {return true;}
    }

    return false;
}
