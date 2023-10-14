#ifndef NH_ECMASCRIPT_H
#define NH_ECMASCRIPT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhcore.h"

#endif

/** @defgroup api_nhecmascript nhecmascript
 *  @brief Parse scripts.
 */

/** @addtogroup api_nhecmascript 
 *  @{
 */

// TYPEDEFS ========================================================================================

    typedef struct nh_ecmascript_Script nh_ecmascript_Script;
    typedef struct nh_ecmascript_Realm nh_ecmascript_Realm;

// ENUMS ===========================================================================================

    /** 
     * Return values for functions. 
     */ 
    typedef enum NH_ECMASCRIPT_RESULT 
    { 
        NH_ECMASCRIPT_SUCCESS, /**<Indicates that something worked as planned.*/ 
        NH_ECMASCRIPT_ERROR_NULL_POINTER, 
        NH_ECMASCRIPT_ERROR_BAD_STATE, 
        NH_ECMASCRIPT_ERROR_MEMORY_ALLOCATION, 
 
    } NH_ECMASCRIPT_RESULT; 
 
// FUNCTIONS =======================================================================================

    /**
     * Todo.
     */
    nh_ecmascript_Script *nh_api_parseScript(
        NH_BYTE *sourceText_p, nh_ecmascript_Realm *Realm_p, int encoding
    );

/** @} */

#endif // NH_ECMASCRIPT_H
