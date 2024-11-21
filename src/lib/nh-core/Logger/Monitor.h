#ifndef NH_CORE_LOGGER_MONITOR_H
#define NH_CORE_LOGGER_MONITOR_H

#include "Logger.h"
#include "../Util/Time.h"

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

typedef struct nh_core_MonitorNode {
    nh_core_LoggerNode *LoggerNode_p;
    bool isOpen;
    bool isCurrent;
    bool isSelected;
    bool hasFocus;
    long focusedRow;
    long offset;
    int peak;
    nh_core_List Children;
    struct nh_core_MonitorNode *Parent_p;
    int selectYCursor;
    int selectYOffset;
} nh_core_MonitorNode;

/**
 * Monitor data for a different Netzhaut process.
 */
typedef struct nh_core_PeerMonitor {
    nh_core_Logger Logger;
    nh_core_MonitorNode Root;
    int server_fd;
    int client_socket;
    int state;
} nh_core_PeerMonitor;

typedef struct nh_core_Monitor {
    nh_core_MonitorNode Root;
    nh_core_PeerMonitor Peer;
    nh_core_SystemTime LastUpdate;
    double updateIntervalInSeconds;
    int height;
    int viewHeight;
    int viewCursor;
    int viewOffset;
    int selectXOffset;
} nh_core_Monitor;

#endif // NH_CORE_LOGGER_MONITOR_H
