// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Init.h"
#include "../../Common/Log.h"

#import <AppKit/AppKit.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_wsi_initializeCocoa()
{
    @autoreleasepool {
        // Make sure NSApplication is initialized
        [NSApplication sharedApplication];
        // Set activation policy so the app shows in Dock and gets focus
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        // Finish launching so we can start creating windows
        [NSApp finishLaunching];
        // Make sure the app is active and in front
        [NSApp activateIgnoringOtherApps:YES];
    }

    return NH_API_SUCCESS;
}
