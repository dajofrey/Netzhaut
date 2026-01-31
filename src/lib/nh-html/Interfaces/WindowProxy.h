#ifndef NH_HTML_WINDOW_PROXY_H
#define NH_HTML_WINDOW_PROXY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    nh_ecmascript_Object *nh_html_createWindowProxy(
        nh_ecmascript_Object *Window_p
    );
    
    NH_API_RESULT nh_html_setWindowProxyWindow(
        nh_ecmascript_Object *WindowProxy_p, nh_ecmascript_Object *Window_p
    );

/** @} */

#endif
