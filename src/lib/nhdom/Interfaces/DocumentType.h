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

/** @addtogroup lib_nhdom_functions
 *  @{
 */

    nh_webidl_Object *nh_dom_createDocumentType(
        nh_webidl_DOMString *Name_p, nh_webidl_DOMString *PublicId_p, nh_webidl_DOMString *SystemId_p
    );

/** @} */

#endif
