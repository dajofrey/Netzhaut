#ifndef NH_LIST_H
#define NH_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    /**
     * Opaque structure holding the actual data of @ref Nh_List.
     */
    typedef struct Nh_ListItem Nh_ListItem;

    /**
     * Generic linked-list which holds data pointers. 
     */
    typedef struct Nh_List {
        int count;           /**<Number of items.*/
        Nh_ListItem *Head_p; /**<Opaque pointer to the first item in the list. A list with \ref count == 0 should have this set as NULL. The actual data should always be queried using @ref Nh_getListItem.*/
    } Nh_List;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    /**
     * Adds \p data_p to the list pointed by \p List_p.
     *
     * @param  List_p A pointer to the list to which \p data_p will be added.
     * @param  data_p A pointer to data that you want to add. Must be allocated beforehand.
     * @return        @ref NH_SUCCESS when \p data_p was added, various error codes if something went wrong. 
     */
    NH_RESULT Nh_addListItem(
        Nh_List *List_p, void *data_p
    );
   
    /**
     * Prepends \p data_p to the list pointed by \p List_p.
     *
     * @param  List_p A pointer to the list to which \p data_p will be prepended.
     * @param  data_p A pointer to data that you want to prepend. Must be allocated beforehand.
     * @return        @ref NH_SUCCESS when \p data_p was prepended, various error codes if something went wrong. 
     */
    NH_RESULT Nh_prependListItem(
        Nh_List *List_p, void *data_p
    );

    NH_RESULT Nh_replaceListItem(
        Nh_List *List_p, void *replace_p, void *replacement_p
    );

    /**
     * Get item data specified by \p index from \p List_p.
     *
     * @param  List_p A pointer to the list that you want to get data from.
     * @param  index  Item location. 0 would be the index of the first item in \p List_p.
     * @return        A pointer to the data from the specified list-item, or NULL if some error occured.
     */ 
    void *Nh_getListItem(
        Nh_List *List_p, int index
    );
   
    /**
     * Destroy the list pointed to by \p List_p. This operation always destroys list internal data.
     *
     * @param List_p   A pointer to the list that you want to destroy.
     * @param freeData Flag that indicates if the data that was added using @ref Nh_addListItem should be freed.
     * @return         Nothing.
     */ 
    void Nh_destroyList(
        Nh_List *List_p, bool freeData
    );

    void Nh_destroyListItem(
        Nh_List *List_p, int index, bool freeData
    );

    void Nh_destroyListItemFromPointer(
        Nh_List *List_p, void *pointer, bool freeData
    );

/** @} */

#endif 
