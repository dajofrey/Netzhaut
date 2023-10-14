#ifndef NH_HTML_REALMS_AND_ENVIRONMENTS_H
#define NH_HTML_REALMS_AND_ENVIRONMENTS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhhtml_structs
 *  @{
 */

    typedef struct nh_html_Environment {
        nh_ASCIIString Id;
        nh_url *CreationURL_p;
        nh_url *TopLevelCreationURL_p;
        nh_html_Origin *TopLevelOrigin_p;
        nh_html_BrowsingContext *TargetBrowsingContext_p;
        void *ActiveServiceWorker_p;
        NH_BOOL executionReady;
    } nh_html_Environment;

    typedef struct nh_html_WindowEnvironmentSettings {
    } WindowEnvironmentSettings;

/** @} */

/** @addtogroup lib_nhhtml_functions
 *  @{
 */

    nh_ecmascript_ExecutionContext *nh_html_createJavaScriptRealm(
        nh_ecmascript_Agent *Agent_p, nh_ecmascript_Object *GlobalObject_p, nh_ecmascript_Object *ThisValue_p
    );

/** @} */

#endif
