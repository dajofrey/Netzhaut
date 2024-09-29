#ifndef NH_CORE_STACK_H
#define NH_CORE_STACK_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "List.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-core_typedefs
 *  @{
 */

    typedef nh_List nh_Stack;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_Stack nh_core_initStack(
        int chunkSize
    );
     
    NH_API_RESULT nh_pushStack(
        nh_Stack *Stack_p, void *handle_p
    );
     
    void *nh_peekStack(
        nh_Stack *Stack_p
    );
     
    void *nh_popStack(
        nh_Stack *Stack_p
    );

/** @} */

#endif 
