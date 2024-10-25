#ifndef NH_WEBIDL_OBJECT_H
#define NH_WEBIDL_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../ECMAScriptBinding/Interface.h"

#endif

typedef struct nh_webidl_Object { 
    nh_webidl_Interface *Interface_p; 
    struct nh_webidl_Object *Parent_p; 
    struct nh_webidl_Object *Child_p; 
    nh_core_List Attributes; 
    nh_core_List Parts; 
    void *internal_p; 
} nh_webidl_Object; 

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    nh_webidl_Object *nh_webidl_createObject(
        char *specification_p, char *interface_p
    );

    nh_webidl_Object *nh_webidl_createObjectFromInterface(
       nh_webidl_Interface *Interface_p 
    );

    nh_webidl_Object *nh_webidl_createCompositeObject(
        char *specification_p, char *interface_p, ...
    );

    nh_webidl_Object *nh_webidl_getObject(
        nh_webidl_Object *Object_p, char *specification_p, char *interface_p
    );

    void *nh_webidl_getAttribute(
        nh_webidl_Object *Object_p, char *attribute_p
    );

    nh_core_String nh_webidl_stringifyObjectForDebugging( 
        nh_webidl_Object *Object_p
    );

/** @} */

#endif
