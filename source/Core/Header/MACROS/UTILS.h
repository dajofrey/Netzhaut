/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_UTILITY_MACROS_H
#define NH_UTILITY_MACROS_H

#define NH_START_TAT() Nh_Time TAT = Nh_getTime();
#define NH_STOP_TAT() Thread_p->Timing.turnAround = Nh_getTimeDiffInSeconds(TAT, Nh_getTime());

#define NH_RUN_DELAY() NH_sleepMs(Thread_p->Timing.runDelay);
#define NH_IDLE_DELAY() NH_sleepMs(Thread_p->Timing.idleDelay);

#define NH_INIT_LIST(List) \
    List.Head_p = NULL;    \
    List.count  = 0;

#define NH_THREAD_SWITCH_POSSIBLE(SwitchTo_p)                 \
(                                                             \
    SwitchTo_p->Info.isVirtual &&                             \
   ((SwitchTo_p->Info.owner == Nh_getThread()->Info.owner) || \
    (SwitchTo_p->Info.owner == Nh_getThread()->id))           \
)

#endif 
