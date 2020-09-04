#ifndef NH_CSS_PARSER_H
#define NH_CSS_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Sheet.h"

#include "../../API/Header/Netzhaut.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_parseSheet(
        NH_CSS_Sheet *Sheet_p
    );

/** @} */

#endif 
