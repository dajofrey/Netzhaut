// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Stack.h"

#include "../System/Memory.h"

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

nh_Stack nh_core_initStack(
    int chunkSize)
{
    return nh_core_initList(chunkSize);
}
 
NH_API_RESULT nh_core_pushStack(
    nh_Stack *Stack_p, void *handle_p)
{
    return nh_core_appendToList(Stack_p, handle_p); 
}
 
void *nh_core_peekStack(
    nh_Stack *Stack_p)
{
    return Stack_p->size <= 0 ? NULL : Stack_p->pp[Stack_p->size - 1];
}
 
void *nh_core_popStack(
    nh_Stack *Stack_p)
{
    if (Stack_p->size == 0) {return NULL;}

    void *handle_p = Stack_p->pp[Stack_p->size - 1];
    nh_core_removeFromList(Stack_p, false, Stack_p->size - 1);    

    return handle_p;
}
