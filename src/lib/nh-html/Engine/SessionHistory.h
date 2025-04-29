#ifndef NH_HTML_MAIN_SESSION_HISTORY_H
#define NH_HTML_MAIN_SESSION_HISTORY_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "PolicyContainer.h"
#include "../Common/Includes.h"

// 7.4.1.2
typedef struct nh_html_DocumentState {
    nh_webidl_Object *Document_p;
    nh_html_PolicyContainer *PolicyContainer_p;
    NH_HTML_REFERRER_POLICY_E requestReferrerPolicy;
    nh_html_Origin *InitiatorOrigin_p;
    nh_html_Origin *Origin_p;
    nh_url_URL *AboutBaseURL_p;
    nh_core_List NestedHistories;
    bool reloadPending;
    bool everPopulated;
    nh_encoding_UTF8 NavigableTargetName;
    nh_html_NotRestoredReasons *NotRestoredReasons_p;
    // .. TODO
} nh_html_DocumentState;

// 7.4.1.1
typedef struct nh_html_SessionHistoryEntry {
    int step;
    nh_url_URL URL;
    nh_html_DocumentState DocumentState;
    // .. TODO
} nh_html_SessionHistoryEntry;

#endif
