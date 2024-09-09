#ifndef NH_NETWORK_RESULT_H
#define NH_NETWORK_RESULT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#endif

/** @{ */

    /**
     * Return values for functions.
     */
    typedef enum NH_NETWORK_RESULT 
    {
        NH_NETWORK_SUCCESS, /**<Indicates that something worked as planned.*/                   
        NH_NETWORK_ERROR_NULL_POINTER,         
        NH_NETWORK_ERROR_BAD_STATE, 
        NH_NETWORK_ERROR_MEMORY_ALLOCATION,

    } NH_NETWORK_RESULT;

/** @} */
/** @{ */

    extern const char *NH_NETWORK_RESULTS_PP[];
    extern unsigned int NH_NETWORK_RESULTS_PP_COUNT;

/** @} */

#endif
