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
#include "../Common/Macros.h"

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// DATA ============================================================================================

nh_webidl_Runtime NH_WEBIDL_RUNTIME;

// INIT ============================================================================================

NH_WEBIDL_RESULT nh_webidl_initRuntime()
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_RUNTIME.Specifications = nh_core_initArray(sizeof(nh_webidl_Specification), 32);

    for (int i = 0; i < NH_WEBIDL_FRAGMENT_NAMES_PP_COUNT; ++i) 
    {
        NH_BYTE specificationName_p[255] = {'\0'};
        strcpy(specificationName_p, NH_WEBIDL_FRAGMENT_NAMES_PP[i]);
        NH_BYTE *fragmentName_p = strstr(specificationName_p, "_");
        if (!fragmentName_p) {NH_WEBIDL_END(NH_WEBIDL_ERROR_BAD_STATE)}
        *fragmentName_p = '\0';
        fragmentName_p = fragmentName_p + 1;
         
        nh_webidl_FragmentParseResult ParseResult = nh_webidl_parse(NH_WEBIDL_FRAGMENT_NAMES_PP[i], NH_WEBIDL_FRAGMENTS_PP[i]);
        nh_webidl_load(specificationName_p, fragmentName_p, ParseResult);
    }

NH_WEBIDL_END(NH_WEBIDL_SUCCESS)
}

// SPECIFICATION ===================================================================================

nh_webidl_Specification *nh_webidl_getSpecification(
    NH_BYTE *specification_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Specification *Specification_p = NULL;

    for (int i = 0; i < NH_WEBIDL_RUNTIME.Specifications.length; ++i) {
        Specification_p = &((nh_webidl_Specification*)NH_WEBIDL_RUNTIME.Specifications.p)[i];
        if (!strcmp(Specification_p->name_p, specification_p)) {break;}
        Specification_p = NULL;
    }

    if (!Specification_p) {
        Specification_p = nh_core_incrementArray(&NH_WEBIDL_RUNTIME.Specifications);
        NH_WEBIDL_CHECK_MEM_2(NULL, Specification_p)
        Specification_p->Fragments = nh_core_initArray(sizeof(nh_webidl_Fragment), 64);
        strcpy(Specification_p->name_p, specification_p);
    }

NH_WEBIDL_END(Specification_p)
}

// FRAGMENT ========================================================================================

nh_webidl_Fragment *nh_webidl_getFragment(
    NH_BYTE *specificationName_p, NH_BYTE *fragmentName_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Specification *Specification_p = nh_webidl_getSpecification(specificationName_p);

    NH_WEBIDL_CHECK_NULL_2(NULL, fragmentName_p)
    NH_WEBIDL_CHECK_NULL_2(NULL, Specification_p)

    nh_webidl_Fragment *Fragment_p = NULL;
    for (int i = 0; i < Specification_p->Fragments.length; ++i) 
    {
        Fragment_p = &((nh_webidl_Fragment*)Specification_p->Fragments.p)[i];
        if (!strcmp(Fragment_p->name_p, fragmentName_p)) {break;}
        Fragment_p = NULL;
    }

NH_WEBIDL_END(Fragment_p)
}

NH_WEBIDL_RESULT nh_webidl_load(
    NH_BYTE *specification_p, NH_BYTE *fragmentName_p, nh_webidl_FragmentParseResult ParseResult)
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK_NULL(specification_p)

    nh_webidl_Specification *Specification_p = nh_webidl_getSpecification(specification_p);
    if (!Specification_p) {NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_ERROR_BAD_STATE)}

    nh_webidl_Fragment *Fragment_p = nh_core_incrementArray(&Specification_p->Fragments);
    NH_WEBIDL_CHECK_MEM(Fragment_p)

    Fragment_p->Specification_p = Specification_p;
    Fragment_p->ParseResult = ParseResult;
    NH_WEBIDL_CHECK(nh_webidl_createInterfaces(Fragment_p))
    strcpy(Fragment_p->name_p, fragmentName_p);

    nh_webidl_logFragment(specification_p, Fragment_p);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

// GET =============================================================================================

nh_webidl_Interface *nh_webidl_getInterface(
    NH_BYTE *specification_p, NH_BYTE *interface_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Specification *Specification_p = nh_webidl_getSpecification(specification_p);

    if (Specification_p) {
        for (int i = 0; i < Specification_p->Fragments.length; ++i) {
            nh_webidl_Fragment *Fragment_p = &((nh_webidl_Fragment*)Specification_p->Fragments.p)[i];
            if (Fragment_p) {
                nh_webidl_Interface *Interface_p = nh_webidl_getInterfaceFromFragment(Fragment_p, interface_p);
                if (Interface_p) {NH_WEBIDL_END(Interface_p)}
            }
        }
    }

NH_WEBIDL_END(NULL)
}

nh_List nh_webidl_getCompositeInterfaces( // TODO naive
    nh_webidl_Interface *PartialInterface_p)
{
NH_WEBIDL_BEGIN()

    nh_List Interfaces = nh_core_initList(8);

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

NH_WEBIDL_END(Interfaces)
}

