#ifndef NH_MAKE_TEST_H
#define NH_MAKE_TEST_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Process.h"

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhmake_structs
 *  @{
 */

    typedef struct nh_make_ExternalFunction {
        NH_BYTE *libraryName_p;
        NH_BYTE *functionName_p;
    } nh_make_ExternalFunction;

    typedef struct nh_make_Test {
        nh_make_SourceContext *Context_p;
        NH_BYTE *name_p;
        void *dl_p;
        int arguments;
        nh_make_TestArgument *Arguments_p;
    } nh_make_Test;

    typedef struct nh_make_TestArray {
        int length;
        nh_make_Test *Tests_p;
    } nh_make_TestArray;

    typedef struct nh_make_TestEnvironment {
        nh_make_Process Process;
        nh_make_ExternalFunction *Initializer_p;
        nh_make_ExternalFunction *Loader_p;
        nh_make_TestArray *TestArray_p;
        NH_BYTE *target_p;
    } nh_make_TestEnvironment;

/** @} */

/** @addtogroup lib_nhmake_functions
 *  @{
 */

    void nh_make_initTestArray(
        nh_make_TestArray *Array_p
    );
    
    NH_MAKE_RESULT nh_make_addTest(
        nh_make_Runtime *Runtime_p, nh_make_TestArray *TestArray_p, nh_make_Function *Function_p
    );

    NH_MAKE_RESULT nh_make_test(
        nh_make_Runtime *Runtime_p, NH_BYTE *name_p
    );

/** @} */

#endif
