#ifndef NH_WEBIDL_DEFINITIONS_H
#define NH_WEBIDL_DEFINITIONS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "Runtime.h"
#include "Parser.h"

#endif

/** @addtogroup lib_nhwebidl_structs
 *  @{
 */

    typedef struct nh_webidl_InterfaceMember {
        const NH_BYTE *name_p;
        nh_webidl_ParseNode *Node_p;
    } nh_webidl_InterfaceMember;

/** @} */

/** @addtogroup lib_nhwebidl_functions
 *  @{
 */

    NH_WEBIDL_RESULT nh_webidl_createInterfaces(
        nh_webidl_Fragment *Fragment_p
    );

    nh_webidl_Interface *nh_webidl_getInterfaceFromFragment(
        nh_webidl_Fragment *Fragment_p, NH_BYTE *className_p
    );

/** @} */

#endif
