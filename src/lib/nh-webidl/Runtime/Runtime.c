// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Runtime.h"
#include "Definitions.h"
#include "Builtin.h"
#include "Object.h"

#include "../Common/Log.h"

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DATA ============================================================================================

nh_webidl_Runtime NH_WEBIDL_RUNTIME;

// INIT ============================================================================================

NH_API_RESULT nh_webidl_initRuntime()
{
    NH_WEBIDL_RUNTIME.Specifications = nh_core_initArray(sizeof(nh_webidl_Specification), 32);

    for (int i = 0; i < NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT; ++i) 
    {
        char specificationName_p[255] = {'\0'};
        strcpy(specificationName_p, NH_WEBIDL_FRAGMENT_NAMES_PP[i]);
        char *fragmentName_p = strstr(specificationName_p, "_");
        if (!fragmentName_p) {return NH_API_ERROR_BAD_STATE;}
        *fragmentName_p = '\0';
        fragmentName_p = fragmentName_p + 1;
         
        nh_webidl_FragmentParseResult ParseResult = nh_webidl_parse(NH_WEBIDL_FRAGMENT_NAMES_PP[i], NH_WEBIDL_FRAGMENTS_PP[i]);
        nh_webidl_load(specificationName_p, fragmentName_p, ParseResult);
    }

    return NH_API_SUCCESS;
}

// SPECIFICATION ===================================================================================

nh_webidl_Specification *nh_webidl_getSpecification(
    char *specification_p)
{
    nh_webidl_Specification *Specification_p = NULL;

    for (int i = 0; i < NH_WEBIDL_RUNTIME.Specifications.length; ++i) {
        Specification_p = &((nh_webidl_Specification*)NH_WEBIDL_RUNTIME.Specifications.p)[i];
        if (!strcmp(Specification_p->name_p, specification_p)) {break;}
        Specification_p = NULL;
    }

    if (!Specification_p) {
        Specification_p = nh_core_incrementArray(&NH_WEBIDL_RUNTIME.Specifications);
        NH_CORE_CHECK_MEM_2(NULL, Specification_p)
        Specification_p->Fragments = nh_core_initArray(sizeof(nh_webidl_Fragment), 64);
        strcpy(Specification_p->name_p, specification_p);
    }

    return Specification_p;
}

// FRAGMENT ========================================================================================

nh_webidl_Fragment *nh_webidl_getFragment(
    char *specificationName_p, char *fragmentName_p)
{
    nh_webidl_Specification *Specification_p = nh_webidl_getSpecification(specificationName_p);

    NH_CORE_CHECK_NULL_2(NULL, fragmentName_p)
    NH_CORE_CHECK_NULL_2(NULL, Specification_p)

    nh_webidl_Fragment *Fragment_p = NULL;
    for (int i = 0; i < Specification_p->Fragments.length; ++i) 
    {
        Fragment_p = &((nh_webidl_Fragment*)Specification_p->Fragments.p)[i];
        if (!strcmp(Fragment_p->name_p, fragmentName_p)) {break;}
        Fragment_p = NULL;
    }

    return Fragment_p;
}

NH_API_RESULT nh_webidl_load(
    char *specification_p, char *fragmentName_p, nh_webidl_FragmentParseResult ParseResult)
{
    NH_CORE_CHECK_NULL(specification_p)

    nh_webidl_Specification *Specification_p = nh_webidl_getSpecification(specification_p);
    if (!Specification_p) {return NH_API_ERROR_BAD_STATE;}

    nh_webidl_Fragment *Fragment_p = nh_core_incrementArray(&Specification_p->Fragments);
    NH_CORE_CHECK_MEM(Fragment_p)

    Fragment_p->Specification_p = Specification_p;
    Fragment_p->ParseResult = ParseResult;
    NH_CORE_CHECK(nh_webidl_createInterfaces(Fragment_p))
    strcpy(Fragment_p->name_p, fragmentName_p);

    nh_webidl_logFragment(specification_p, Fragment_p);

    return NH_API_SUCCESS;
}

// GET =============================================================================================

nh_webidl_Interface *nh_webidl_getInterface(
    char *specification_p, char *interface_p)
{
    nh_webidl_Specification *Specification_p = nh_webidl_getSpecification(specification_p);

    if (Specification_p) {
        for (int i = 0; i < Specification_p->Fragments.length; ++i) {
            nh_webidl_Fragment *Fragment_p = &((nh_webidl_Fragment*)Specification_p->Fragments.p)[i];
            if (Fragment_p) {
                nh_webidl_Interface *Interface_p = nh_webidl_getInterfaceFromFragment(Fragment_p, interface_p);
                if (Interface_p) {return Interface_p;}
            }
        }
    }

    return NULL;
}

nh_core_List nh_webidl_getCompositeInterfaces( // TODO naive
    nh_webidl_Interface *PartialInterface_p)
{
    nh_core_List Interfaces = nh_core_initList(8);

    for (int j = 0; j < NH_WEBIDL_RUNTIME.Specifications.length; ++j)
    {
        nh_webidl_Specification *Specification_p = &((nh_webidl_Specification*)NH_WEBIDL_RUNTIME.Specifications.p)[j];
    
        if (Specification_p) {
            for (int i = 0; i < Specification_p->Fragments.length; ++i) {
                nh_webidl_Fragment *Fragment_p = &((nh_webidl_Fragment*)Specification_p->Fragments.p)[i];
                if (Fragment_p) {
                    nh_webidl_Interface *Interface_p = nh_webidl_getInterfaceFromFragment(Fragment_p, PartialInterface_p->name_p);
                    if (Interface_p) {nh_core_appendToList(&Interfaces, Interface_p);}
                }
            }
        }
    }

    return Interfaces;
}

