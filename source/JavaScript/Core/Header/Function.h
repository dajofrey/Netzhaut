#ifndef NH_JS_FUNCTION_H
#define NH_JS_FUNCTION_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include <stdbool.h>

#include "Object.h"

#include "../../Header/Data.h"
#include "../../Header/Parser.h"

typedef struct Nh_JS_Result Nh_JS_Result;
typedef struct Nh_JS_Script Nh_JS_Script;
typedef struct Nh_JS_Object Nh_JS_Object;

#endif

#ifdef DOXYGEN_JS_API

/** @addtogroup JSCoreFunction Function
 *  \ingroup JSCore
 *  @{
 */

    Constructor Function();                                   /**<Not yet implemented.*/

    Instance Property arguments;                              /**<Not yet implemented.*/
    Instance Property caller;                                 /**<Not yet implemented.*/
    Instance Property displayName;                            /**<Not yet implemented.*/
    Instance Property length;                                 /**<Not yet implemented.*/
    Instance Property name;                                   /**<Not yet implemented.*/

    Instance Method apply(thisArg [, argsArray]);             /**<Not yet implemented.*/
    Instance Method bind(thisArg[, arg1[, arg2[, ...argN]]]); /**<Not yet implemented.*/
    Instance Method call(thisArg[, arg1, arg2, ...argN]);     /**<Not yet implemented.*/
    Instance Method toString();                               /**<Not yet implemented.*/

/** @} */

#else

/** @addtogroup JSCoreStructs Structs
 *  \ingroup JSCore
 *  @{
 */

    typedef struct Nh_JS_Function {
        char *name_p;
        Nh_JS_FunctionPointer internal_p;
        Nh_JS_Parsable *external_p;
    } Nh_JS_Function;

/** @} */

/** @addtogroup JSCoreFunctions Functions
 *  \ingroup JSCore
 *  @{
 */

    Nh_JS_Result Nh_JS_Function_Function(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_Function_arguments(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Function_caller(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Function_displayName(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Function_length(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Function_name(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    NH_RESULT Nh_JS_createFunctionData(
        Nh_JS_Function *Function_p, Nh_JS_Object *Owner_p, int index
    );

/** @} */

#endif

#endif 
