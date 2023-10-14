#ifndef NH_MAKE_RUNTIME_H
#define NH_MAKE_RUNTIME_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Source.h"

#include "../Common/Includes.h"
#include "../Parser/Parser.h"
#include "../Parser/Variables.h"
#include "../Test/Test.h"

#endif

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef struct nh_make_Runtime {
        NH_MAKE_BOOL quiet;
        NH_MAKE_BOOL GUI;
        NH_MAKE_BOOL showParseTree;
        NH_BYTE *prefix_p;
        nh_make_ParserArray ParserArray;
        nh_make_SourceArray SourceArray; 
        nh_make_TestArray TestArray;
        nh_make_SourceContextArray SourceContextArray; 
        nh_make_VariableArray VariableArray; 
        nh_make_FileArray FileArray; 
        nh_make_functionCallback_f functionCallback_f;
        nh_make_sourceContextCallback_f sourceContextCallback_f;
        nh_make_sourceContextCallback_f beforeBuildCallback_f;
        nh_make_sourceContextCallback_f afterBuildCallback_f;
    } nh_make_Runtime;

/** @} */

/** @addtogroup lib_nhmake_vars
 *  @{
 */

    extern nh_make_Runtime NH_MAKE_DEFAULT_RUNTIME;

/** @} */

/** @addtogroup lib_nhmake_typedefs
 *  @{
 */

    typedef NH_MAKE_RESULT (*nh_make_initialize_f)(
    );
    
    typedef void (*nh_make_terminate_f)(
    );

    typedef nh_make_Runtime *(*nh_make_createRuntime_f)(
        NH_BYTE *name_p
    );
    
    typedef void (*nh_make_destroyRuntime_f)(
        nh_make_Runtime *Runtime_p
    );

    typedef NH_MAKE_RESULT (*nh_make_run_f)(
        nh_make_Runtime *Runtime_p, int argc, NH_BYTE **argv_pp
    );

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    NH_MAKE_RESULT nh_make_executeRuntime(
        nh_make_Runtime *Runtime_p, NH_BYTE **args_pp, int args
    );

/** @} */

#endif
