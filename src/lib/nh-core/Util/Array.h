#ifndef NH_CORE_ARRAY_H
#define NH_CORE_ARRAY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

    /** 
     * Generic linked-list which holds data pointers. 
     */ 
    typedef struct nh_core_Array { 
        int elementSize;             /**<Number of items.*/ 
        int allocatedLengthPerChunk; 
        int allocatedLength; 
        unsigned long length; 
        char *p;
    } nh_core_Array; 

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_core_Array nh_core_initArray(
        int elementSize, int allocatedLengthPerChunk
    );

    NH_API_RESULT nh_core_appendToArray(
        nh_core_Array *Array_p, void *p, unsigned long count
    );

    NH_API_RESULT nh_core_appendToArrayRepeatedly(
        nh_core_Array *Array_p, void *p, unsigned long count
    );

    NH_API_RESULT nh_core_insertIntoArray(
        nh_core_Array *Array_p, int index, void *elements_p, int length
    );

    void *nh_core_getFromArray(
        nh_core_Array *Array_p, unsigned long index
    );
   
    void *nh_core_incrementArray(
        nh_core_Array *Array_p
    );

    NH_API_RESULT nh_core_removeTailFromArray(
        nh_core_Array *Array_p, unsigned int count
    );

    NH_API_RESULT nh_core_removeFromArray(
        nh_core_Array *Array_p, int index, unsigned int count
    );

    void nh_core_freeArray(
        nh_core_Array *Array_p
    );

/** @} */

#endif 
