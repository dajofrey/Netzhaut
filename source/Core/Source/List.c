// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/List.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

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

struct Nh_ListItem {
    void *data_p;
    struct Nh_ListItem *Next_p;
};

// LIST ============================================================================================

NH_RESULT Nh_addListItem( // TODO multithreading
    Nh_List *List_p, void *data_p)
{
NH_BEGIN()

    NH_CHECK_NULL(List_p, data_p)

    Nh_ListItem **Item_pp = &List_p->Head_p;
    while (*Item_pp != NULL) {Item_pp = &((*Item_pp)->Next_p);}

    *Item_pp = Nh_allocate(sizeof(Nh_ListItem));
    NH_CHECK_MEM(*Item_pp)
    
    (*Item_pp)->data_p = data_p;
    (*Item_pp)->Next_p = NULL;

    List_p->count++;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_prependListItem( // TODO multithreading
    Nh_List *List_p, void *data_p)
{
NH_BEGIN()

    NH_CHECK_NULL(List_p, data_p)

    if (List_p->Head_p == NULL) {NH_END(Nh_addListItem(List_p, data_p))}
    else {

        Nh_ListItem *Next_p = List_p->Head_p;
        List_p->Head_p = Nh_allocate(sizeof(Nh_ListItem));
        NH_CHECK_MEM(List_p->Head_p)

        List_p->Head_p->data_p = data_p;
        List_p->Head_p->Next_p = Next_p;

        List_p->count++;
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_replaceListItem(
    Nh_List *List_p, void *replace_p, void *replacement_p)
{
NH_BEGIN()

    NH_CHECK_NULL(List_p, replace_p, replacement_p)

    int index = 0;
    Nh_ListItem **Item_pp = &List_p->Head_p;
    while (*Item_pp != NULL && (*Item_pp)->data_p != replace_p) {
        Item_pp = &((*Item_pp)->Next_p);
        index++;
    }

    if (index == List_p->count) {NH_END(NH_ERROR_BAD_STATE)}
    (*Item_pp)->data_p = replacement_p;

NH_END(NH_SUCCESS)
}

void *Nh_getListItem(
    Nh_List *List_p, int index)
{
NH_BEGIN()

    if (List_p == NULL || List_p->Head_p == NULL) {NH_END(NULL)}

    Nh_ListItem **Item_pp = &List_p->Head_p;

    for (int i = 0; i < index; ++i) {
        if ((*Item_pp)->Next_p != NULL) {Item_pp = &(*Item_pp)->Next_p;}
        else {NH_END(NULL)}
    }
    
NH_END((*Item_pp)->data_p)
}

void Nh_destroyList( // TODO multithreading
    Nh_List *List_p, bool freeData)
{
NH_BEGIN()

    while (List_p->count > 0) {
        Nh_destroyListItem(List_p, List_p->count - 1, freeData);
    }

    NH_INIT_LIST((*List_p))
    
NH_SILENT_END()
}

void Nh_destroyListItem( // TODO multithreading
    Nh_List *List_p, int index, bool freeData)
{
NH_BEGIN()

    if (List_p == NULL) {NH_SILENT_END()}

    Nh_ListItem **Item_pp = &List_p->Head_p;
    Nh_ListItem **Previous_pp = NULL;

    for (int i = 0; i < index; ++i) 
    {
        if (*Item_pp != NULL) 
        {
            Previous_pp = Item_pp;
            Item_pp = &(*Item_pp)->Next_p;
        }
        else {NH_SILENT_END()}
    }

    Nh_ListItem *Next_p = (*Item_pp)->Next_p;

    if (freeData) {Nh_free((*Item_pp)->data_p);}
    Nh_free(*Item_pp);
   
    if (index == 0) {List_p->Head_p = Next_p;}
    else {(*Previous_pp)->Next_p = Next_p;}

    List_p->count -= 1;

NH_SILENT_END()
}

void Nh_destroyListItemFromPointer( // TODO multithreading
    Nh_List *List_p, void *pointer, bool freeData)
{
NH_BEGIN()

    if (List_p == NULL) {NH_SILENT_END()}

    Nh_ListItem **Item_pp = &List_p->Head_p;
    Nh_ListItem **Previous_pp = NULL;

    int index = 0;
    for (index = 0; index < List_p->count && Item_pp != NULL; ++index) 
    {
        if ((*Item_pp)->data_p == pointer) {break;}
        Previous_pp = Item_pp;
        Item_pp = &(*Item_pp)->Next_p;
    }

    if (Item_pp == NULL || index == List_p->count) {NH_SILENT_END()}

    Nh_ListItem *Next_p = (*Item_pp)->Next_p;

    if (freeData) {Nh_free((*Item_pp)->data_p);}
    Nh_free(*Item_pp);
   
    if (index == 0) {List_p->Head_p = Next_p;}
    else {(*Previous_pp)->Next_p = Next_p;}

    List_p->count -= 1;

NH_SILENT_END()
}

