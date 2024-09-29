#ifndef NH_CORE_LIST_H
#define NH_CORE_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

 /** 
    * Dynamic list implementation using a two dimensional array. 
    */ 
   typedef struct nh_List { 
       unsigned long chunkSize; 
       unsigned long chunkCount; 
       unsigned long size; 
       void **pp; 
   } nh_List; 

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_List _nh_core_initList(
        unsigned long chunkSize
    );

    nh_List nh_core_initList(
        unsigned long chunkSize
    );
    
    NH_API_RESULT nh_core_appendToList(
        nh_List *List_p, void *handle_p
    );

    NH_API_RESULT nh_core_appendItemsToList(
        nh_List *List_p, nh_List *Append_p
    );

    NH_API_RESULT nh_core_prependToList(
        nh_List *List_p, void *handle_p
    );
   
    NH_API_RESULT nh_core_insertIntoList(
        nh_List *List_p, void *handle_p, unsigned long index
    );

    void *_nh_core_getFromList(
        nh_List *List_p, unsigned long index
    );

    void *nh_core_getFromList(
        nh_List *List_p, unsigned long index
    );

    unsigned long nh_core_getListIndex(
        nh_List *List_p, void *handle_p
    );

    void *nh_core_getNextListItem(
        nh_List *List_p, void *handle_p
    );

    void nh_core_freeList(
        nh_List *List_p, bool freeHandles
    );
    
    NH_API_RESULT nh_core_removeFromList(
        nh_List *List_p, bool freeHandle, unsigned int index
    );

    NH_API_RESULT nh_core_removeFromList2(
        nh_List *List_p, bool freeHandle, void *handle_p
    );

    bool nh_inList(
        nh_List *List_p, void *handle_p
    );

/** @} */

#endif 
