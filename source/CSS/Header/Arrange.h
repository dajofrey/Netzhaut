#ifndef DOXYGEN_SHOULD_SKIP_THIS

#ifndef NH_CSS_ARRANGE_H
#define NH_CSS_ARRANGE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Box.h"

#endif

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_arrange(
        Nh_Tab *Tab_p, NH_BOOL unformatted
    );

    char *Nh_CSS_stringifyArrangement(
        Nh_HTML_Document *Document_p, NH_BOOL unformatted
    );

/** @} */

#endif
