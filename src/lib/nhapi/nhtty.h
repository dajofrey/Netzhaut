#ifndef NH_TTY_H
#define NH_TTY_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "nhcore.h"
#include "nhencoding.h"
#include "nhwsi.h"

#endif

/** @defgroup api_nhtty nhtty
 *  @brief Environment for running text based programs.
 *
 *  In this API, TTY stands for TeleType and provides an interface for
 *  writing text based programs. Using these programs, it processes input streams from either
 *  standard input or \ref nh_tty_sendInput and sends the result to either standard
 *  output or a \ref nh_terminal_Terminal. Text based programs can be either added
 *  by using \ref nh_tty_addDefaultProgram or \ref nh_tty_addCustomProgram.
 */

/** @addtogroup api_nhtty
 *  @{
 */

// ENUMS ===========================================================================================

    /**
     * Return values for functions.
     */
    typedef enum NH_TTY_RESULT {
        NH_TTY_SUCCESS, /**<Indicates that something worked as planned.*/ 
        NH_TTY_ERROR_NULL_POINTER,         
        NH_TTY_ERROR_BAD_STATE, 
        NH_TTY_ERROR_MEMORY_ALLOCATION,
        NH_TTY_ERROR_TERMINFO_DATA_CANNOT_BE_FOUND,
        NH_TTY_ERROR_UNKNOWN_TERMINAL_TYPE,
        NH_TTY_ERROR_TERMINAL_IS_HARDCOPY,
        NH_TTY_ERROR_UNKNOWN_COMMAND,
        NH_TTY_ERROR_INVALID_ARGUMENT,
    } NH_TTY_RESULT;

    typedef enum NH_TTY_PROGRAM_E {
        NH_TTY_PROGRAM_SHELL,
        NH_TTY_PROGRAM_LOGGER,
        NH_TTY_PROGRAM_EDITOR,
        NH_TTY_PROGRAM_TAGGER,
    } NH_TTY_PROGRAM_E;

    typedef enum NH_TTY_MARK_E {
        NH_TTY_MARK_LINE_VERTICAL   = 1 << 0, // Internal use.
        NH_TTY_MARK_LINE_HORIZONTAL = 1 << 1, // Internal use.
        NH_TTY_MARK_LINE_GRAPHICS   = 1 << 2,
        NH_TTY_MARK_ELEVATED        = 1 << 3,
        NH_TTY_MARK_ACCENT          = 1 << 4,
    } NH_TTY_MARK_E;

// TYPEDEFS ========================================================================================

    typedef struct nh_tty_TTY nh_tty_TTY;
    typedef struct nh_tty_Program nh_tty_Program;
    typedef struct nh_tty_ProgramPrototype nh_tty_ProgramPrototype;
    typedef struct nh_tty_Glyph nh_tty_Glyph;
    
    typedef void *(*nh_tty_init_f)(void *arg_p);
    typedef NH_TTY_RESULT (*nh_tty_draw_f)(nh_tty_Program *Program_p, nh_tty_Glyph *Glyphs_p, int width, int height, int row);
    typedef NH_TTY_RESULT (*nh_tty_drawTopbar_f)(nh_tty_Program *Program_p, nh_tty_Glyph *Glyphs_p, int width);
    typedef NH_TTY_RESULT (*nh_tty_getCursorPosition_f)(nh_tty_Program *Program_p, int *x_p, int *y_p);
    typedef NH_TTY_RESULT (*nh_tty_handleInput_f)(nh_tty_Program *Program_p, nh_wsi_Event Event);
    typedef NH_TTY_RESULT (*nh_tty_update_f)(nh_tty_Program *Program_p);
    typedef NH_TTY_RESULT (*nh_tty_handleCommand_f)(nh_tty_Program *Program_p, nh_List *Arguments_p);
    typedef void (*nh_tty_destroy_f)(void *p);
    typedef void (*nh_tty_destroyPrototype_f)(nh_tty_ProgramPrototype *Prototype_p);

// STRUCTS =========================================================================================

    typedef struct nh_tty_GlyphAttributes {
        NH_BOOL bold;
        NH_BOOL faint;
        NH_BOOL italic;
        NH_BOOL underline;
        NH_BOOL blink;
        NH_BOOL reverse;
        NH_BOOL invisible;
        NH_BOOL struck;
        NH_BOOL wrap;
        NH_BOOL wide;
    } nh_tty_GlyphAttributes;
    
    typedef struct nh_tty_GlyphColor {
        nh_Color Color;
        NH_BOOL custom;
    } nh_tty_GlyphColor;

    typedef struct nh_tty_Glyph {
        nh_tty_GlyphAttributes Attributes;
        nh_tty_GlyphColor Foreground;
        nh_tty_GlyphColor Background;
        NH_ENCODING_UTF32 codepoint;       /* character code */
        NH_TTY_MARK_E mark;
    } nh_tty_Glyph;
    
    typedef struct nh_tty_Row {
        nh_tty_Glyph *Glyphs_p;
        NH_BOOL *update_p;
    } nh_tty_Row;

    typedef struct nh_tty_ProgramCallbacks {
        nh_tty_init_f init_f;
        nh_tty_draw_f draw_f;
        nh_tty_drawTopbar_f drawTopbar_f;
        nh_tty_handleInput_f handleInput_f;
        nh_tty_getCursorPosition_f getCursorPosition_f;
        nh_tty_update_f update_f;
        nh_tty_handleCommand_f handleCommand_f;
        nh_tty_destroy_f destroy_f;
        nh_tty_destroyPrototype_f destroyPrototype_f;
    } nh_tty_ProgramCallbacks;
    
    typedef struct nh_tty_ProgramPrototype {
        nh_encoding_UTF32String Name;
        nh_encoding_UTF32String *CommandNames_p;
        int commands;
        nh_tty_ProgramCallbacks Callbacks;
        void *initArg_p;
    } nh_tty_ProgramPrototype;

    typedef struct nh_tty_Program {
        nh_tty_ProgramPrototype *Prototype_p;
        void *handle_p;
        NH_BOOL refresh;
        NH_BOOL close;
        NH_BOOL once;
        int command;
    } nh_tty_Program;

// FUNCTIONS =======================================================================================

    /**
     * @brief Get a new TTY handle. 
     * 
     * You can check for user-closure with @ref nh_core_getWorkload. If @ref nh_core_getWorkload returns NULL, 
     * the user probably closed the TTY or there was an error. You can force-close the TTY with 
     * @ref nh_tty_closeTTY.
     *
     * @return Pointer to a new TTY handle. NULL on failure.
     */
    nh_tty_TTY *nh_api_openTTY(
        NH_BYTE *config_p
    );

    /**
     * @brief Close the specified TTY.
     *
     * Only call this if you want to force-close the TTY. There is no need to call this if you expect
     * the user to close the TTY since this case is handled internally.
     *
     * @param Pointer to TTY. Must not be NULL.
     *
     * @return NH_TTY_SUCCESS on success.
     */
    NH_TTY_RESULT nh_api_closeTTY(
        nh_tty_TTY *TTY_p
    );

    /**
     * @brief Add a built-in program to TTY in a specific manner.
     *
     * The built-in programs are currently named "shell", "editor", "logger" and "tagger". Please see
     * the design document at TODO for further documentation.
     *
     * @param TTY_p  Pointer to TTY. Must not be NULL.
     * @param name_p Type of the program. 
     * @param once   If NH_TRUE, the program will only be added to the currently focused tile of the currently
     *               focused tab. 
     *               If NH_FALSE, it will be added to all current/future tiles of all current/future tabs until 
     *               the program is removed.
     * 
     * @return NH_TTY_SUCCESS on success.
     */
    NH_TTY_RESULT nh_api_addProgram(
        nh_tty_TTY *TTY_p, NH_TTY_PROGRAM_E type, NH_BOOL once 
    ); 

    /**
     * Todo.
     */
    NH_TTY_RESULT nh_api_claimStandardIO(
        nh_tty_TTY *TTY_p
    );
    
    /**
     * Todo.
     */
    NH_TTY_RESULT nh_api_unclaimStandardIO(
        nh_tty_TTY *TTY_p
    );

    /**
     * Todo.
     */
    NH_TTY_RESULT nh_api_sendEvent(
        nh_tty_TTY *TTY_p, nh_wsi_Event Event
    );

    /**
     * Todo.
     */
    NH_TTY_RESULT nh_api_sendCommandToShell(
        int pid, NH_TTY_PROGRAM_E type 
    );

/** @} */

#endif // NH_TTY_H
