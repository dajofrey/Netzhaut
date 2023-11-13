#ifndef NH_CORE_LINKED_LIST_H
#define NH_CORE_LINKED_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup lib_nhcore_macros
 *  @{
 */

    #define NH_INIT_LINKED_LIST(List) List.Head_p = NULL; List.count = 0;

/** @} */

/** @addtogroup lib_nhcore_functions
 *  @{
 */

    nh_LinkedList nh_core_initLinkedList(
    );

    /**
     * Adds \p data_p to the list pointed by \p List_p.
     *
     * @param  List_p A pointer to the list to which \p data_p will be added.
     * @param  data_p A pointer to data that you want to add. Must be allocated beforehand.
     * @return        @ref NH_CORE_SUCCESS when \p data_p was added, various error codes if something went wrong. 
     */
    NH_CORE_RESULT nh_core_appendToLinkedList(
        nh_LinkedList *List_p, void *data_p
    );
   
    NH_CORE_RESULT _nh_core_appendToLinkedList(
        nh_LinkedList *List_p, void *data_p
    );

    /**
     * Prepends \p data_p to the list pointed by \p List_p.
     *
     * @param  List_p A pointer to the list to which \p data_p will be prepended.
     * @param  data_p A pointer to data that you want to prepend. Must be allocated beforehand.
     * @return        @ref NH_CORE_SUCCESS when \p data_p was prepended, various error codes if something went wrong. 
     */
    NH_CORE_RESULT nh_core_prependToLinkedList(
        nh_LinkedList *List_p, void *data_p
    );

    NH_CORE_RESULT nh_replaceInLinkedList(
        nh_LinkedList *List_p, void *replace_p, void *replacement_p
    );

    NH_CORE_RESULT nh_core_insertIntoLinkedList(
        nh_LinkedList *List_p, void *data_p, int index
    );

    /**
     * Get item data specified by \p index from \p List_p.
     *
     * @param  List_p A pointer to the list that you want to get data from.
     * @param  index  Item location. 0 would be the index of the first item in \p List_p.
     * @return        A pointer to the data from the specified list-item, or NULL if some error occured.
     */ 
    void *nh_core_getFromLinkedList(
        nh_LinkedList *List_p, int index
    );
   
    void *_nh_core_getFromLinkedList(
        nh_LinkedList *List_p, int index
    );

    NH_CORE_RESULT nh_core_setInLinkedList(
        nh_LinkedList *List_p, int index, void *data_p
    );

    /**
     * Destroy the list pointed to by \p List_p. This operation always destroys list internal data.
     *
     * @param List_p   A pointer to the list that you want to destroy.
     * @param freeData Flag that indicates if the data that was added using @ref nh_core_appendToLinkedList should be freed.
     * @return         Nothing.
     */ 
    void nh_core_destroyLinkedList(
        nh_LinkedList *List_p, bool freeData
    );

    void nh_core_removeFromLinkedList(
        nh_LinkedList *List_p, int index, bool freeData
    );

    void nh_core_removeFromLinkedList2(
        nh_LinkedList *List_p, void *pointer, NH_BOOL freeData
    );

    void _nh_core_removeFromLinkedList2( // TODO multithreading
        nh_LinkedList *List_p, void *pointer, NH_BOOL freeData
    );

    NH_BOOL nh_inLinkedList(
        nh_LinkedList *List_p, void *pointer
    );

    int nh_core_getLinkedListIndex(
        nh_LinkedList *List_p, void *pointer
    );

/** @} */

#endif 
