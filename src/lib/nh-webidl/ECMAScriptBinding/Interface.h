#ifndef NH_WEBIDL_INTERFACE_H
#define NH_WEBIDL_INTERFACE_H

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

typedef struct nh_webidl_Specification { 
    char name_p[255]; 
    nh_Array Fragments; 
} nh_webidl_Specification; 

typedef struct nh_webidl_InterfaceInheritance { 
    const char *interface_p; 
    const char *specification_p; 
} nh_webidl_InterfaceInheritance; 

typedef struct nh_webidl_Interface { 
    bool partial; 
    char *name_p; 
    nh_Array Members; 
    nh_webidl_Specification *Specification_p; 
    nh_webidl_InterfaceInheritance *Inheritance_p; 
} nh_webidl_Interface; 

/** @} */

#endif
