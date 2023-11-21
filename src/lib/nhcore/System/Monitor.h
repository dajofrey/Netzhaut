#ifndef NH_CORE_SYSTEM_MONITOR_H
#define NH_CORE_SYSTEM_MONITOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../TTY/Program.h"

#include "../../nhcore/System/Logger.h"
#include "../../nhcore/Util/String.h"
#include "../../nhcore/Util/List.h"
#include "../../nhcore/Util/Time.h"

#endif

/** @addtogroup lib_nhtty_structs
 *  @{
 */

    typedef struct nh_tty_LoggerNode {
        nh_LoggerNode *LoggerNode_p;
        NH_BOOL isOpen;
        NH_BOOL isCurrent;
        NH_BOOL isSelected;
        NH_BOOL hasFocus;
        long focusedRow;
        long offset;
        int peak;
        nh_List Children;
        struct nh_tty_LoggerNode *Parent_p;
    } nh_tty_LoggerNode;

    typedef struct nh_tty_LoggerView {
        int height;
        int screenCursor;
        int rowOffset;
    } nh_tty_LoggerView;

    typedef struct nh_tty_Logger {
        nh_tty_LoggerView View;
        NH_BOOL showCategories;
        int listingWidth;
        nh_tty_LoggerNode Root;
        nh_SystemTime LastUpdate;
        double updateIntervalInSeconds;
    } nh_tty_Logger;

/** @} */

/** @addtogroup lib_nhtty_functions
 *  @{
 */

    nh_tty_ProgramPrototype *nh_tty_createLoggerPrototype(
    );

/** @} */

#endif 
