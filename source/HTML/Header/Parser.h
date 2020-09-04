#ifndef NH_HTML_PARSER_H
#define NH_HTML_PARSER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Document.h"

#include "../../API/Header/Netzhaut.h"

#endif

/** @addtogroup HTMLFunctions Functions
 *  \ingroup HTML
 *  @{
 */

    NH_RESULT Nh_HTML_parseDocument(
        Nh_HTML_Document *Document_p
    );

/** @} */

#endif 
