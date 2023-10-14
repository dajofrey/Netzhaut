#ifndef NH_WEBIDL_RUNTIME_H
#define NH_WEBIDL_RUNTIME_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Parser.h"
#include "../Common/Includes.h"
#include "../../nhcore/Util/Array.h"

#endif

/** @addtogroup lib_nhwebidl_structs
 *  @{
 */

    typedef struct nh_webidl_Fragment {
        NH_BYTE name_p[255];
        nh_webidl_Specification *Specification_p;
        nh_webidl_FragmentParseResult ParseResult;
        nh_Array Interfaces;
    } nh_webidl_Fragment;

    typedef struct nh_webidl_Runtime {
        nh_Array Specifications;
    } nh_webidl_Runtime;

/** @} */

/** @addtogroup lib_nhwebidl_vars
 *  @{
 */

    extern nh_webidl_Runtime NH_WEBIDL_RUNTIME;

/** @} */

/** @addtogroup lib_nhwebidl_functions
 *  @{
 */

    NH_WEBIDL_RESULT nh_webidl_initRuntime(
    );

    NH_WEBIDL_RESULT nh_webidl_load(
        NH_BYTE *specification_p, NH_BYTE *fragmentName_p, nh_webidl_FragmentParseResult ParseResult
    );

    nh_webidl_Specification *nh_webidl_getSpecification(
        NH_BYTE *specification_p
    );

    nh_webidl_Fragment *nh_webidl_getFragment(
        NH_BYTE *specification_p, NH_BYTE *fragment_p
    );

    nh_webidl_Interface *nh_webidl_getInterface(
        NH_BYTE *specification_p, NH_BYTE *interface_p
    );

    nh_List nh_webidl_getCompositeInterfaces(
        nh_webidl_Interface *PartialInterface_p
    );

/** @} */

#endif
