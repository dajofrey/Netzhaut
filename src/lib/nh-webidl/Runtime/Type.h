#ifndef NH_WEBIDL_TYPE_H
#define NH_WEBIDL_TYPE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "Object.h"
#include "Parser.h"

#endif

/** @addtogroup lib_nh-webidl_functions Functions
 *  @{
 */

    NH_WEBIDL_TYPE nh_webidl_getType(
        nh_webidl_ParseNode *Type_p
    );

    size_t nh_webidl_getAllocationSize(
        NH_WEBIDL_TYPE type
    );

/** @} */

#endif
