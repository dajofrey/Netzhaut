#ifndef NH_WEBIDL_INTERFACE_H
#define NH_WEBIDL_INTERFACE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#include "Parser.h"

/** @addtogroup lib_nh-webidl_structs
 *  @{
 */

    typedef struct nh_webidl_Fragment nh_webidl_Fragment;

    typedef struct nh_webidl_InterfaceMember {
        const char *name_p;
        nh_webidl_ParseNode *Node_p;
    } nh_webidl_InterfaceMember;

    typedef struct nh_webidl_Specification {
        char name_p[255];
        nh_core_Array Fragments;
    } nh_webidl_Specification;
    
    typedef struct nh_webidl_InterfaceInheritance {
        const char *interface_p;
        const char *specification_p;
    } nh_webidl_InterfaceInheritance;
    
    typedef struct nh_webidl_Interface {
        bool partial;
        char *name_p;
        nh_core_Array Members;
        nh_webidl_Specification *Specification_p;
        nh_webidl_InterfaceInheritance *Inheritance_p;
    } nh_webidl_Interface;

/** @} */

/** @addtogroup lib_nh-webidl_functions
 *  @{
 */

    NH_API_RESULT nh_webidl_createInterfaces(
        nh_webidl_Fragment *Fragment_p
    );

    nh_webidl_Interface *nh_webidl_getInterfaceFromFragment(
        nh_webidl_Fragment *Fragment_p, char *className_p
    );

/** @} */

#endif
