// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "LinkedList.h"

#include "../System/Memory.h"
#include "../Common/Macros.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#ifdef __unix__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

// DECLARE =========================================================================================

struct nh_LinkedListItem {
    void *data_p;
    struct nh_LinkedListItem *Next_p;
};

// LINKED LIST =====================================================================================

nh_LinkedList nh_core_initLinkedList()
{
NH_CORE_BEGIN()

    nh_LinkedList List;
    List.count  = 0;
    List.Head_p = NULL;

NH_CORE_END(List)
}

NH_CORE_RESULT _nh_core_appendToLinkedList( // TODO multithreading
    nh_LinkedList *List_p, void *data_p)
{
    if (List_p == NULL || data_p == NULL) {return NH_CORE_ERROR_BAD_STATE;}

    nh_LinkedListItem **Item_pp = &List_p->Head_p;
    while (*Item_pp != NULL) {Item_pp = &((*Item_pp)->Next_p);}

    *Item_pp = nh_core_allocate(sizeof(nh_LinkedListItem));
    if (*Item_pp == NULL) {return NH_CORE_ERROR_BAD_STATE;}
    
    (*Item_pp)->data_p = data_p;
    (*Item_pp)->Next_p = NULL;

    List_p->count++;

    return NH_CORE_SUCCESS;
}

NH_CORE_RESULT nh_core_appendToLinkedList( // TODO multithreading
    nh_LinkedList *List_p, void *data_p)
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL(List_p)
    NH_CORE_CHECK_NULL(data_p)

    nh_LinkedListItem **Item_pp = &List_p->Head_p;
    while (*Item_pp != NULL) {Item_pp = &((*Item_pp)->Next_p);}

    *Item_pp = nh_core_allocate(sizeof(nh_LinkedListItem));
    NH_CORE_CHECK_MEM(*Item_pp)
    
    (*Item_pp)->data_p = data_p;
    (*Item_pp)->Next_p = NULL;

    List_p->count++;

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_insertIntoLinkedList( // TODO multithreading
    nh_LinkedList *List_p, void *data_p, int index)
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL(List_p)
    NH_CORE_CHECK_NULL(data_p)

    if (index >= List_p->count) {
        NH_CORE_DIAGNOSTIC_END(nh_core_appendToLinkedList(List_p, data_p))
    }
    if (index <= 0) {
        NH_CORE_DIAGNOSTIC_END(nh_core_prependToLinkedList(List_p, data_p))
    }

    int tmp = 0;
    nh_LinkedListItem **Item_pp = &List_p->Head_p;
    nh_LinkedListItem **Prev_pp = NULL;

    while (*Item_pp != NULL && tmp < index) {
        Prev_pp = Item_pp;
        Item_pp = &((*Item_pp)->Next_p);
        tmp++;
    }
    
    nh_LinkedListItem *New_p = nh_core_allocate(sizeof(nh_LinkedListItem));
    NH_CORE_CHECK_MEM(New_p)
    
    New_p->data_p = data_p;
    New_p->Next_p = *Item_pp;

    (*Prev_pp)->Next_p = New_p;

    List_p->count++;

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_prependToLinkedList( // TODO multithreading
    nh_LinkedList *List_p, void *data_p)
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL(List_p)
    NH_CORE_CHECK_NULL(data_p)

    if (List_p->Head_p == NULL) {NH_CORE_END(nh_core_appendToLinkedList(List_p, data_p))}
    else {

        nh_LinkedListItem *Next_p = List_p->Head_p;
        List_p->Head_p = nh_core_allocate(sizeof(nh_LinkedListItem));
        NH_CORE_CHECK_MEM(List_p->Head_p)

        List_p->Head_p->data_p = data_p;
        List_p->Head_p->Next_p = Next_p;

        List_p->count++;
    }

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_core_setInLinkedList(
    nh_LinkedList *List_p, int index, void *data_p)
{
NH_CORE_BEGIN()

    nh_LinkedListItem **Item_pp = &List_p->Head_p;

    for (int i = 0; i < index; ++i) {
        if ((*Item_pp)->Next_p != NULL) {Item_pp = &(*Item_pp)->Next_p;}
        else {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}
    }

    (*Item_pp)->data_p = data_p;

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

NH_CORE_RESULT nh_replaceInLinkedList(
    nh_LinkedList *List_p, void *replace_p, void *replacement_p)
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL(List_p)
    NH_CORE_CHECK_NULL(replace_p) 
    NH_CORE_CHECK_NULL(replacement_p)

    int index = 0;
    nh_LinkedListItem **Item_pp = &List_p->Head_p;
    while (*Item_pp != NULL && (*Item_pp)->data_p != replace_p) {
        Item_pp = &((*Item_pp)->Next_p);
        index++;
    }

    if (index == List_p->count) {NH_CORE_DIAGNOSTIC_END(NH_CORE_ERROR_BAD_STATE)}
    (*Item_pp)->data_p = replacement_p;

NH_CORE_DIAGNOSTIC_END(NH_CORE_SUCCESS)
}

void *_nh_core_getFromLinkedList(
    nh_LinkedList *List_p, int index)
{
    if (List_p == NULL || List_p->Head_p == NULL) {return NULL;}

    nh_LinkedListItem **Item_pp = &List_p->Head_p;

    for (int i = 0; i < index; ++i) {
        if ((*Item_pp)->Next_p != NULL) {Item_pp = &(*Item_pp)->Next_p;}
        else {return NULL;}
    }

    return (*Item_pp)->data_p;
}

void *nh_core_getFromLinkedList(
    nh_LinkedList *List_p, int index)
{
NH_CORE_BEGIN()

    if (List_p == NULL || List_p->Head_p == NULL) {NH_CORE_END(NULL)}

    nh_LinkedListItem **Item_pp = &List_p->Head_p;

    for (int i = 0; i < index; ++i) {
        if ((*Item_pp)->Next_p != NULL) {Item_pp = &(*Item_pp)->Next_p;}
        else {NH_CORE_END(NULL)}
    }
    
NH_CORE_END((*Item_pp)->data_p)
}

void nh_core_destroyLinkedList( // TODO multithreading
    nh_LinkedList *List_p, bool freeData)
{
NH_CORE_BEGIN()

    while (List_p->count > 0) {
        nh_core_removeFromLinkedList(List_p, List_p->count - 1, freeData);
    }

    *List_p = nh_core_initLinkedList();
    
NH_CORE_SILENT_END()
}

void nh_core_removeFromLinkedList( // TODO multithreading
    nh_LinkedList *List_p, int index, bool freeData)
{
NH_CORE_BEGIN()

    if (List_p == NULL) {NH_CORE_SILENT_END()}

    nh_LinkedListItem **Item_pp = &List_p->Head_p;
    nh_LinkedListItem **Prev_pp = NULL;

    for (int i = 0; i < index; ++i) 
    {
        if (*Item_pp != NULL) 
        {
            Prev_pp = Item_pp;
            Item_pp = &(*Item_pp)->Next_p;
        }
        else {NH_CORE_SILENT_END()}
    }

    nh_LinkedListItem *Next_p = (*Item_pp)->Next_p;

    if (freeData) {nh_core_free((*Item_pp)->data_p);}
    nh_core_free(*Item_pp);
   
    if (index == 0) {List_p->Head_p = Next_p;}
    else {(*Prev_pp)->Next_p = Next_p;}

    List_p->count -= 1;

NH_CORE_SILENT_END()
}

void nh_core_removeFromLinkedList2( // TODO multithreading
    nh_LinkedList *List_p, void *pointer, NH_BOOL freeData)
{
NH_CORE_BEGIN()

    _nh_core_removeFromLinkedList2(List_p, pointer, freeData);

NH_CORE_SILENT_END()
}

void _nh_core_removeFromLinkedList2( // TODO multithreading
    nh_LinkedList *List_p, void *pointer, NH_BOOL freeData)
{
    if (List_p == NULL) {return;}

    nh_LinkedListItem *Item_p = List_p->Head_p;
    nh_LinkedListItem *Previous_p = NULL;

    int index = 0;
    for (index = 0; index < List_p->count && Item_p != NULL; ++index) 
    {
        if (Item_p->data_p == pointer) {break;}
        Previous_p = Item_p;
        Item_p = Item_p->Next_p;
    }

    if (Item_p == NULL || index == List_p->count) {return;}

    nh_LinkedListItem *Next_p = Item_p->Next_p;

    if (freeData) {nh_core_free(Item_p->data_p);}
    nh_core_free(Item_p);
   
    if (index == 0) {List_p->Head_p = Next_p;}
    else {Previous_p->Next_p = Next_p;}

    List_p->count -= 1;
}

NH_BOOL nh_inLinkedList(
    nh_LinkedList *List_p, void *pointer)
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL_2(NH_FALSE, List_p)
    NH_CORE_CHECK_NULL_2(NH_FALSE, pointer)

    for (int i = 0; i < List_p->count; ++i) {
        if (nh_core_getFromLinkedList(List_p, i) == pointer) {NH_CORE_END(NH_TRUE)}
    }

NH_CORE_END(NH_FALSE)
}

int nh_core_getLinkedListIndex(
    nh_LinkedList *List_p, void *pointer)
{
NH_CORE_BEGIN()

    NH_CORE_CHECK_NULL_2(NH_FALSE, List_p)
    NH_CORE_CHECK_NULL_2(NH_FALSE, pointer)

    for (int i = 0; i < List_p->count; ++i) {
        if (nh_core_getFromLinkedList(List_p, i) == pointer) {NH_CORE_END(i)}
    }

NH_CORE_END(-1)
}

