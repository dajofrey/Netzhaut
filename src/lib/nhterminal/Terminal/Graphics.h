#ifndef NH_TERMINAL_TERMINAL_GRAPHICS_H
#define NH_TERMINAL_TERMINAL_GRAPHICS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Grid.h"

#include "../Vulkan/Text.h"
#include "../OpenGL/Foreground.h"
#include "../OpenGL/Background.h"
#include "../OpenGL/Boxes.h"

#include "../../nhcore/Util/HashMap.h"

#endif

/** @addtogroup lib_nhterminal_structs
 *  @{
 */

    typedef struct nh_terminal_AttributeRange {
        nh_tty_Glyph Glyph;
        int indices;
    } nh_terminal_AttributeRange;

    typedef struct nh_terminal_GraphicsAction {
        NH_BOOL init;
    } nh_terminal_GraphicsAction;

    /**
     * Text that is drawn on top of the background. Can be either normal or elevated,
     * which basically is the same but with different depth values.
     */
    typedef struct nh_terminal_GraphicsForeground {
        nh_terminal_GraphicsAction Action;
        nh_terminal_VulkanText Vulkan;
        nh_terminal_OpenGLForeground OpenGL;
        nh_Array Vertices;
        nh_Array Indices;
        nh_Array Vertices2;
        nh_Array Indices2;
        nh_Array Ranges;
        nh_Array Ranges2;
    } nh_terminal_GraphicsForeground;

    /**
     * The background behind normal text, boxes and elevated text.
     */
    typedef struct nh_terminal_GraphicsBackground {
        nh_terminal_GraphicsAction Action;
        nh_terminal_OpenGLBackground OpenGL;
        nh_Array Vertices;
        nh_Array Indices;
        nh_Array Ranges;
    } nh_terminal_GraphicsBackground;

    typedef struct nh_terminal_GraphicsBoxes {
        nh_terminal_GraphicsAction Action;
        nh_terminal_OpenGLBoxes OpenGL;
        nh_Array Vertices;
    } nh_terminal_GraphicsBoxes;
 
    typedef struct nh_terminal_GraphicsGradient {
        nh_Color Color;
        float ratio;
        int index;
        double interval;
        nh_SystemTime LastChange;
    } nh_terminal_GraphicsGradient;

    typedef struct nh_terminal_GraphicsData {
        nh_terminal_GraphicsForeground Foreground;
        nh_terminal_GraphicsBackground Background;
        nh_terminal_GraphicsBoxes Boxes;
    } nh_terminal_GraphicsData;

    /**
     * Blinking state.
     */
    typedef struct nh_terminal_GraphicsBlink {
        NH_BOOL on;              /**<If true, cursor is in visible period.*/
        nh_SystemTime LastBlink; /**<System time of last blink change.*/
    } nh_terminal_GraphicsBlink;

    typedef struct nh_terminal_GraphicsState {
        nh_gfx_Viewport *Viewport_p;
        nh_terminal_GraphicsBlink Blink;
        nh_terminal_GraphicsGradient Gradient;
        nh_HashMap Map;
        nh_gfx_FontInstance *FontInstance_p;
        nh_List Fonts;
        int font;
        nh_List Glyphs;
        nh_List Codepoints;
    } nh_terminal_GraphicsState;

    typedef struct nh_terminal_Graphics {
        nh_terminal_GraphicsState State;
        nh_terminal_GraphicsData Data1;
        nh_terminal_GraphicsData Data2;
    } nh_terminal_Graphics;

/** @} */

/** @addtogroup lib_nhterminal_functions
 *  @{
 */

    NH_TERMINAL_RESULT nh_terminal_initGraphics(
        nh_terminal_Graphics *Graphics_p
    );

    NH_TERMINAL_RESULT nh_terminal_freeGraphics(
        nh_terminal_Graphics *Graphics_p
    );

    NH_TERMINAL_RESULT nh_terminal_handleViewportChange(
        nh_terminal_Graphics *Graphics_p, nh_gfx_Viewport *Viewport_p
    );

    NH_TERMINAL_RESULT nh_terminal_updateGraphicsData(
        nh_terminal_GraphicsData *Data_p, nh_terminal_Grid *Grid_p
    );

    NH_TERMINAL_RESULT nh_terminal_renderGraphics(
        nh_terminal_Graphics *Graphics_p, nh_terminal_Grid *Grid_p, nh_terminal_Grid *Grid2_p
    );

    nh_Color nh_terminal_getGlyphColor(
        nh_terminal_GraphicsState *State_p, nh_tty_Glyph *Glyph_p, NH_BOOL foreground
    );

    NH_BOOL nh_terminal_updateBlinkOrGradient(
        nh_terminal_GraphicsState *State_p
    );

/** @} */

#endif 
