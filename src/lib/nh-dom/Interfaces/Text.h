#ifndef NH_DOM_TEXT_H
#define NH_DOM_TEXT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-dom_functions
 *  @{
 */

    nh_ecmascript_Object *nh_dom_createText(
        nh_encoding_UTF8String Data, nh_ecmascript_Object *NodeDocument_p
    );

    NH_API_RESULT nh_dom_appendToText(
        nh_ecmascript_Object *Text_p, nh_encoding_UTF8String DOMString
    );

    nh_encoding_UTF8String *nh_dom_getTextString(
        nh_ecmascript_Object *Text_p
    );

/** @} */

#endif
