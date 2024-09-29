// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Stack.h"

#include "../System/Memory.h"
#include "../Common/Macros.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// STACK ===========================================================================================

nh_Stack nh_core_initStack(
    int chunkSize)
{
NH_CORE_BEGIN()
NH_CORE_END(nh_core_initList(chunkSize))
}
 
NH_API_RESULT nh_pushStack(
    nh_Stack *Stack_p, void *handle_p)
{
NH_CORE_BEGIN()
NH_CORE_DIAGNOSTIC_END(nh_core_appendToList(Stack_p, handle_p)) 
}
 
void *nh_peekStack(
    nh_Stack *Stack_p)
{
NH_CORE_BEGIN()
NH_CORE_END(Stack_p->size <= 0 ? NULL : Stack_p->pp[Stack_p->size - 1])
}
 
void *nh_popStack(
    nh_Stack *Stack_p)
{
NH_CORE_BEGIN()

    if (Stack_p->size == 0) {NH_CORE_END(NULL)}

    void *handle_p = Stack_p->pp[Stack_p->size - 1];
    nh_core_removeFromList(Stack_p, false, Stack_p->size - 1);    

NH_CORE_END(handle_p)
}

