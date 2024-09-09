#ifndef NH_WEBIDL_OBJECT_H
#define NH_WEBIDL_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    nh_webidl_Object *nh_webidl_createObject(
        NH_BYTE *specification_p, NH_BYTE *interface_p
    );

    nh_webidl_Object *nh_webidl_createObjectFromInterface(
       nh_webidl_Interface *Interface_p 
    );

    nh_webidl_Object *nh_webidl_createCompositeObject(
        NH_BYTE *specification_p, NH_BYTE *interface_p, ...
    );

    nh_webidl_Object *nh_webidl_getObject(
        nh_webidl_Object *Object_p, NH_BYTE *specification_p, NH_BYTE *interface_p
    );

    void *nh_webidl_getAttribute(
        nh_webidl_Object *Object_p, NH_BYTE *attribute_p
    );

/** @} */

#endif
