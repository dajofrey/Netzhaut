#ifndef NH_TTY_COMMON_MACROS_H
#define NH_TTY_COMMON_MACROS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#ifdef NH_LOG_FLOW
    #include "Log.h"
#endif

#endif // DOXYGEN_SHOULD_SKIP_THIS

// HELPER ==========================================================================================

#define CTRL_KEY(k) ((k) & 0x1f)

#define NH_TTY_TILING_KEY CTRL_KEY('t')
#define NH_TTY_TOPBAR_KEY CTRL_KEY('e')
#define NH_TTY_QUIT_KEY CTRL_KEY('q')
#define NH_TTY_INSERT_TILE_LEFT_KEY   'a'
#define NH_TTY_INSERT_TILE_RIGHT_KEY  'd'
#define NH_TTY_INSERT_TILE_TOP_KEY    'w'
#define NH_TTY_INSERT_TILE_BOTTOM_KEY 's'
#define NH_TTY_SPLIT_KEY 'f'

#define NH_TTY_IS_MACRO_TILE(tile_pointer) (((nh_tty_Tile*)tile_pointer)->type == NH_TTY_TILE_TYPE_MACRO)
#define NH_TTY_IS_MICRO_TILE(tile_pointer) (((nh_tty_Tile*)tile_pointer)->type == NH_TTY_TILE_TYPE_MICRO)

#define NH_TTY_MACRO_TAB(tile_pointer) ((nh_tty_MacroTab*)((nh_tty_MacroTile*)((nh_tty_Tile*)tile_pointer)->p)->MacroTabs.pp[((nh_tty_MacroTile*)((nh_tty_Tile*)tile_pointer)->p)->current])
#define NH_TTY_MACRO_TAB_2(tile_pointer, custom_index) ((nh_tty_MacroTab*)((nh_tty_MacroTile*)((nh_tty_Tile*)tile_pointer)->p)->MacroTabs.pp[custom_index])

#define NH_TTY_MACRO_TILE(tile_pointer) ((nh_tty_MacroTile*)((nh_tty_Tile*)tile_pointer)->p)
#define NH_TTY_MICRO_TILE(tile_pointer) ((nh_tty_MicroTile*)((nh_tty_Tile*)tile_pointer)->p)

#define NH_TTY_MICRO_TAB(macro_tab_pointer) ((nh_tty_MicroTab*)((nh_tty_MacroTab*)macro_tab_pointer)->MicroWindow.Tabs_p->pp[((nh_tty_MacroTab*)macro_tab_pointer)->MicroWindow.current])

// FLOW LOGGING ====================================================================================

#ifdef NH_LOG_FLOW
    #define NH_TTY_BEGIN() {_nh_tty_logBegin(__FILE__, __func__);}
#else
    #define NH_TTY_BEGIN() 
#endif

#ifdef NH_LOG_FLOW
    #define NH_TTY_END(result) {_nh_tty_logEnd(__FILE__, __func__); return result;}
#else
    #define NH_TTY_END(result) {return result;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_TTY_SILENT_END() {_nh_tty_logEnd(__FILE__, __func__); return;}
#else
    #define NH_TTY_SILENT_END() {return;} 
#endif

#ifdef NH_LOG_FLOW
    #define NH_TTY_DIAGNOSTIC_END(result) {return _nh_tty_logDiagnosticEnd(__FILE__, __func__, result, __LINE__);}
#else
    #define NH_TTY_DIAGNOSTIC_END(result) {return result;} 
#endif

// ERROR LOGGING ===================================================================================

#define NH_TTY_CHECK(checkable)                                             \
{                                                                           \
    NH_TTY_RESULT checkResult = checkable;                                  \
    if (checkResult != NH_TTY_SUCCESS) {NH_TTY_DIAGNOSTIC_END(checkResult)} \
}

#define NH_TTY_CHECK_2(checkReturn, checkable)                   \
{                                                                \
    NH_TTY_RESULT checkResult = checkable;                       \
    if (checkResult != NH_TTY_SUCCESS) {NH_TTY_END(checkReturn)} \
}

#define NH_TTY_CHECK_NULL(checkable)                                              \
{                                                                                 \
    void *checkResult_p = checkable;                                              \
    if (checkResult_p == NULL) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_NULL_POINTER)} \
}

#define NH_TTY_CHECK_NULL_2(checkReturn, checkable)      \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_TTY_END(checkReturn)} \
}

#define NH_TTY_CHECK_MEM(checkable)                                                    \
{                                                                                      \
    void *checkResult_p = checkable;                                                   \
    if (checkResult_p == NULL) {NH_TTY_DIAGNOSTIC_END(NH_TTY_ERROR_MEMORY_ALLOCATION)} \
}

#define NH_TTY_CHECK_MEM_2(checkReturn, checkable)       \
{                                                        \
    void *checkResult_p = checkable;                     \
    if (checkResult_p == NULL) {NH_TTY_END(checkReturn)} \
}

#endif // NH_TTY_COMMON_MACROS_H
