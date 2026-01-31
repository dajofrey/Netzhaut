#ifndef NH_DOM_DOCUMENT_TYPE_H
#define NH_DOM_DOCUMENT_TYPE_H

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

    nh_ecmascript_Object *nh_dom_createDocumentType(
        nh_encoding_UTF8String *Name_p, nh_encoding_UTF8String *PublicId_p, nh_encoding_UTF8String *SystemId_p
    );

/** @} */

#endif
