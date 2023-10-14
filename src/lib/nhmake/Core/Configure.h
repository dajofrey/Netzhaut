#ifndef NH_MAKE_CONFIGURE_H
#define NH_MAKE_CONFIGURE_H

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

#endif

/** @addtogroup lib_nhmake_typedefs
 *  @{
 */

    typedef NH_MAKE_RESULT (*nh_make_addFile_f)(
        nh_make_Runtime *Runtime_p, NH_BYTE *path_p
    );

    typedef void (*nh_make_setFunctionCallback_f)(
        nh_make_Runtime *Runtime_p, nh_make_functionCallback_f functionCallback_f
    );
    
    typedef void (*nh_make_setSourceContextCallback_f)(
        nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f
    );
    
    typedef void (*nh_make_setBeforeBuildCallback_f)(
        nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f
    );
    
    typedef void (*nh_make_setAfterBuildCallback_f)(
        nh_make_Runtime *Runtime_p, nh_make_sourceContextCallback_f sourceContextCallback_f
    );
    
    typedef void (*nh_make_setQuiet_f)(
        nh_make_Runtime *Runtime_p, NH_MAKE_BOOL quiet
    );
    
    typedef void (*nh_make_setShowParseTree_f)(
        nh_make_Runtime *Runtime_p, NH_MAKE_BOOL showParseTree
    );
    
    typedef NH_MAKE_RESULT (*nh_make_setVariable_f)(
        nh_make_Runtime *Runtime_p, NH_BYTE *variable_p, NH_BYTE **values_pp, int valueCount
    );
    
/** @} */

#endif
