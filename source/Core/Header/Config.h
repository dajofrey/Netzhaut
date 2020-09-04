#ifndef NH_CONFIG_H
#define NH_CONFIG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "System.h"
#include "Thread.h"
#include "Font.h"
#include "URI.h"

#include <stddef.h>
#include <stdbool.h>

#endif

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_Config {

        Nh_List InternalURNSyntax;

        struct Flags {

            struct Internal {
                bool shutdown;
            } Internal;

            struct Log {
                bool init;
                bool mouse;
                bool vulkan;
                bool script;
                bool listener;
                bool event;
                bool jsinterpreter;
                bool jsparser;
                bool sheet;
                bool cssparser;
                bool document;
                bool htmlparser;
                NH_BOOL receive;
                NH_BOOL send;
                NH_BOOL margin;
            } Log;

        } Flags;

        struct Settings {
            double gamma;
            int inputBufferSize;
            char *internalURNSchemeName_p;
            char *startpage_p;
        } Settings;

    } Nh_Config;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    NH_RESULT Nh_initConfig(
    );

    Nh_Config *Nh_getConfig(
    );

    void Nh_freeConfig(
    );

    void Nh_initLogFlags(
    );

/** @} */

#endif
