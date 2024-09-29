#ifndef NH_CORE_ARRAY_LIST_H
#define NH_CORE_ARRAY_LIST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "List.h"
#include "../Common/Includes.h"

#endif

   typedef struct nh_ArrayList { 
       nh_List Arrays; 
       int elementSize;             /**<Number of items.*/ 
       int allocatedLengthPerChunk; 
   } nh_ArrayList; 

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_ArrayList nh_core_initArrayList(
        int elementSize, int allocatedLengthPerChunk
    );
    
    void *_nh_core_incrementArrayList(
        nh_ArrayList *ArrayList_p
    );
    
    void *nh_core_incrementArrayList(
        nh_ArrayList *ArrayList_p
    );
    
    void nh_core_freeArrayList(
        nh_ArrayList *ArrayList_p
    );

/** @} */

#endif 
