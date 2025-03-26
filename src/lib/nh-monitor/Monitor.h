#ifndef NH_MONITOR_MONITOR_H
#define NH_MONITOR_MONITOR_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Logger.h"
#include "../nh-core/Util/Time.h"

// STRUCTS =========================================================================================

typedef struct nh_monitor_MonitorNode {
    nh_monitor_LoggerNode *LoggerNode_p;
    bool isOpen;
    bool isCurrent;
    bool isSelected;
    bool hasFocus;
    long focusedRow;
    long offset;
    int peak;
    nh_core_List Children;
    struct nh_monitor_MonitorNode *Parent_p;
    int selectYCursor;
    int selectYOffset;
} nh_monitor_MonitorNode;

/**
 * Monitor data for a different Netzhaut process.
 */
typedef struct nh_monitor_PeerMonitor {
    nh_monitor_Logger Logger;
    nh_monitor_MonitorNode Root;
    int server_fd;
    int client_socket;
    int state;
} nh_monitor_PeerMonitor;

typedef struct nh_monitor_Monitor {
    nh_monitor_MonitorNode Root;
    nh_monitor_PeerMonitor Peer;
    nh_core_SystemTime LastUpdate;
    double updateIntervalInSeconds;
    int height;
    int viewHeight;
    int viewCursor;
    int viewOffset;
    int selectXOffset;
} nh_monitor_Monitor;

#endif
