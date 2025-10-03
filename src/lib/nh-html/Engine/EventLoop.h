#ifndef NH_HTML_EVENT_LOOP_H
#define NH_HTML_EVENT_LOOP_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Common/Includes.h"

// ENUMS ===========================================================================================

typedef enum NH_HTML_CROSS_ORIGIN_ISOLATION_MODE {
    NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_NONE,
    NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_LOGICAL,
    NH_HTML_CROSS_ORIGIN_ISOLATION_MODE_CONCRETE,
} NH_HTML_CROSS_ORIGIN_ISOLATION_MODE;

typedef enum NH_HTML_AGENT_CLUSTER_KEY {
    NH_HTML_AGENT_CLUSTER_KEY_SITE,
    NH_HTML_AGENT_CLUSTER_KEY_TUPLE_ORIGIN,
} NH_HTML_AGENT_CLUSTER_KEY;

// STRUCTS =========================================================================================

typedef struct nh_html_EventLoop {
    nh_ecmascript_Agent *Agent_p;
    nh_core_Queue TaskQueue
} nh_html_EventLoop;

#endif
