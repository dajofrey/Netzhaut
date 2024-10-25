#ifndef NH_CORE_COMMON_TERMINATE_H
#define NH_CORE_COMMON_TERMINATE_H

// LICENSE =========================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Includes.h"
#include "../Loader/Loader.h"

// FUNCTIONS =======================================================================================

typedef void (*nh_core_terminate_f)(
    nh_core_Loader *Loader_p
);

#endif // NH_CORE_COMMON_TERMINATE_H
