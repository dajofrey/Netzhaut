#ifndef NH_NET_URL_H
#define NH_NET_URL_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../Core/Header/URI.h"
#include "../../API/Header/Netzhaut.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup NetworkDefinitions Definitions
 *  \ingroup Network
 *  @{
 */

    typedef struct Nh_Net_URL Nh_Net_URL;

/** @} */

/** @addtogroup NetworkFunctions Functions
 *  \ingroup Network
 *  @{
 */

    Nh_Net_URL *Nh_Net_createURL(
        char *chars_p, NH_URI_SCHEME *scheme_p
    );
    
    void Nh_Net_freeURL(
        Nh_URI URI
    );
    
    Nh_URI Nh_Net_copyURL(
        Nh_URI *URI_p
    );
    
    bool Nh_Net_equivalentURL(
        Nh_URI URI1, Nh_URI URI2
    );
    
    NH_MEDIA Nh_Net_getMediaType(
        Nh_URI URI
    );
    
    void *Nh_Net_getDataFromURL(
        Nh_Net_URL *URL_p, size_t *size_p, int index
    );

    void Nh_Net_freeData(
        Nh_URI URI
    );

/** @} */

#endif 
