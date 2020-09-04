#ifndef NH_CSS_FUNCTIONS_H
#define NH_CSS_FUNCTIONS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Parser.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    void Nh_CSS_rgba(
        char *str_p, float rgba_p[4]
    );
    
    void Nh_CSS_hsla(
        char *str_p, float rgba_p[4]
    );

/** @} */

#endif 
