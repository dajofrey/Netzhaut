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

    nh_webidl_Object *nh_html_createWindowProxy(
        nh_webidl_Object *Window_p
    );
    
    NH_HTML_RESULT nh_html_setWindowProxyWindow(
        nh_webidl_Object *WindowProxy_p, nh_webidl_Object *Window_p
    );

/** @} */

#endif
