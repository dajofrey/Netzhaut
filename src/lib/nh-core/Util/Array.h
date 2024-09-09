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

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_Array nh_core_initArray(
        int elementSize, int allocatedLengthPerChunk
    );

    NH_CORE_RESULT nh_core_appendToArray(
        nh_Array *Array_p, void *p, unsigned long count
    );

    NH_CORE_RESULT nh_core_appendToArrayRepeatedly(
        nh_Array *Array_p, void *p, unsigned long count
    );

    NH_CORE_RESULT nh_core_insertIntoArray(
        nh_Array *Array_p, int index, void *elements_p, int length
    );

    void *_nh_core_getFromArray(
        nh_Array *Array_p, unsigned long index
    );

    void *nh_core_getFromArray(
        nh_Array *Array_p, unsigned long index
    );
   
    void *_nh_core_incrementArray(
        nh_Array *Array_p
    );
    
    void *nh_core_incrementArray(
        nh_Array *Array_p
    );

    NH_CORE_RESULT nh_core_removeTailFromArray(
        nh_Array *Array_p, unsigned int count
    );

    NH_CORE_RESULT nh_core_removeFromArray(
        nh_Array *Array_p, int index, unsigned int count
    );

    void nh_core_freeArray(
        nh_Array *Array_p
    );

/** @} */

#endif 
