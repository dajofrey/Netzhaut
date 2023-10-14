// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Graphics.h"

#include "../Vulkan/Render.h"
#include "../OpenGL/Render.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"
#include "../Common/Config.h"

#include "../../nhgfx/Base/Viewport.h"
#include "../../nhgfx/Fonts/HarfBuzz.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/Math.h"
#include "../../nhcore/Util/Array.h"
#include "../../nhcore/Util/List.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// HELPER ==========================================================================================

static NH_TERMINAL_RESULT nh_terminal_getInternalMonospaceFonts(
    nh_List *Fonts_p)
{
NH_TERMINAL_BEGIN()
 
    nh_gfx_FontStyle Style;
    nh_gfx_parseFontStyle(&Style, "normal");

    nh_gfx_FontFamily Family = nh_gfx_initFontFamily(NULL);
    Family.generic_p[NH_GFX_GENERIC_FONT_FAMILY_MONOSPACE] = NH_TRUE;

    *Fonts_p = nh_gfx_getFonts(Family, Style, NH_TRUE);
    if (Fonts_p->size <= 0) {NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)}

    nh_gfx_freeFontStyle(&Style);

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

static nh_terminal_GraphicsAction nh_terminal_initGraphicsAction()
{
NH_TERMINAL_BEGIN()

    nh_terminal_GraphicsAction Action;
    Action.init = NH_TRUE;

NH_TERMINAL_END(Action)
}

static nh_Color nh_terminal_getGradientColor(
    nh_terminal_Config *Config_p, nh_terminal_GraphicsGradient *Gradient_p)
{
NH_TERMINAL_BEGIN()

    if (Config_p->accents == 1) {
        NH_TERMINAL_END(Config_p->Accents_p[0])
    }

    nh_Color Color1 = Config_p->Accents_p[Gradient_p->index];
    nh_Color Color2 = Gradient_p->index == Config_p->accents-1 ? Config_p->Accents_p[0] : Config_p->Accents_p[Gradient_p->index+1];

    nh_Color Result;
    Result.r = Color1.r + Gradient_p->ratio * (Color2.r - Color1.r);
    Result.g = Color1.g + Gradient_p->ratio * (Color2.g - Color1.g);
    Result.b = Color1.b + Gradient_p->ratio * (Color2.b - Color1.b);
    Result.a = 1.0f;

    Gradient_p->ratio += 0.01f;

NH_TERMINAL_END(Result)
}

// INIT/FREE =======================================================================================

static NH_TERMINAL_RESULT nh_terminal_initGraphicsData(
    nh_terminal_GraphicsData *Data_p)
{
NH_TERMINAL_BEGIN()

    Data_p->Foreground.Action = nh_terminal_initGraphicsAction();
    Data_p->Foreground.Vertices = nh_core_initArray(sizeof(float), 1024);
    Data_p->Foreground.Indices  = nh_core_initArray(sizeof(uint32_t), 1024);
    Data_p->Foreground.Vertices2 = nh_core_initArray(sizeof(float), 1024);
    Data_p->Foreground.Indices2  = nh_core_initArray(sizeof(uint32_t), 1024);
    Data_p->Foreground.Ranges = nh_core_initArray(sizeof(nh_terminal_AttributeRange), 32);
    Data_p->Foreground.Ranges2 = nh_core_initArray(sizeof(nh_terminal_AttributeRange), 32);

    nh_terminal_initOpenGLForeground(&Data_p->Foreground.OpenGL);

    Data_p->Background.Action = nh_terminal_initGraphicsAction();
    Data_p->Background.Vertices = nh_core_initArray(sizeof(float), 1024);
    Data_p->Background.Indices  = nh_core_initArray(sizeof(uint32_t), 1024);
    Data_p->Background.Ranges = nh_core_initArray(sizeof(nh_terminal_AttributeRange), 32);

    nh_terminal_initOpenGLBackground(&Data_p->Background.OpenGL);

    Data_p->Boxes.Action = nh_terminal_initGraphicsAction();
    nh_terminal_initOpenGLBoxes(&Data_p->Boxes.OpenGL);
 
NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_initGraphicsState(
    nh_terminal_GraphicsState *State_p)
{
NH_TERMINAL_BEGIN()

    nh_terminal_Config Config = nh_terminal_getConfig();

    NH_TERMINAL_CHECK(nh_terminal_getInternalMonospaceFonts(&State_p->Fonts))

    State_p->Map = nh_core_createHashMap();
    State_p->Glyphs = nh_core_initList(128);
    State_p->Codepoints = nh_core_initList(128);

    State_p->Gradient.Color = nh_terminal_getGradientColor(&Config, &State_p->Gradient);
    State_p->Gradient.interval = 0.1;
    State_p->Gradient.index = 0;
    State_p->Gradient.ratio = 0.0f;
    State_p->Gradient.LastChange = nh_core_getSystemTime();

    State_p->Blink.LastBlink = nh_core_getSystemTime();
    State_p->Blink.on = NH_FALSE;
 
NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_initGraphics(
    nh_terminal_Graphics *Graphics_p)
{
NH_TERMINAL_BEGIN()

    memset(Graphics_p, 0, sizeof(nh_terminal_Graphics));

    NH_TERMINAL_CHECK(nh_terminal_initGraphicsState(&Graphics_p->State))
    NH_TERMINAL_CHECK(nh_terminal_initGraphicsData(&Graphics_p->Data1))
    NH_TERMINAL_CHECK(nh_terminal_initGraphicsData(&Graphics_p->Data2))
 
NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_freeGraphicsData(
    nh_terminal_GraphicsData *Data_p)
{
NH_TERMINAL_BEGIN()

    nh_core_freeArray(&Data_p->Foreground.Vertices);
    nh_core_freeArray(&Data_p->Foreground.Indices);
    nh_core_freeArray(&Data_p->Foreground.Ranges);
    nh_core_freeArray(&Data_p->Foreground.Vertices2);
    nh_core_freeArray(&Data_p->Foreground.Indices2);
    nh_core_freeArray(&Data_p->Foreground.Ranges2);

    nh_terminal_freeOpenGLForeground(&Data_p->Foreground.OpenGL);

    nh_core_freeArray(&Data_p->Background.Vertices);
    nh_core_freeArray(&Data_p->Background.Indices);
    nh_core_freeArray(&Data_p->Background.Ranges);
 
    nh_terminal_freeOpenGLBackground(&Data_p->Background.OpenGL);
    nh_terminal_freeOpenGLBoxes(&Data_p->Boxes.OpenGL);

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_freeGraphics(
    nh_terminal_Graphics *Graphics_p)
{
NH_TERMINAL_BEGIN()

    NH_TERMINAL_CHECK(nh_terminal_freeGraphicsData(&Graphics_p->Data1))
    NH_TERMINAL_CHECK(nh_terminal_freeGraphicsData(&Graphics_p->Data2))

    nh_core_freeList(&Graphics_p->State.Fonts, NH_FALSE);
    nh_core_freeList(&Graphics_p->State.Glyphs, NH_TRUE);
    nh_core_freeList(&Graphics_p->State.Codepoints, NH_TRUE);

    nh_core_freeHashMap(Graphics_p->State.Map);

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

// RANGES ==========================================================================================

static int nh_terminal_getCurrentAttributeRangeForLineGraphics(
    nh_terminal_Grid *Grid_p, nh_tty_Glyph *Current_p, int *col_p, int *row_p)
{
NH_TERMINAL_BEGIN()

    int total = 0;

    for (int row = *row_p; row < Grid_p->rows; ++row) {
        for (int col = *col_p; col < Grid_p->cols; ++col) {
            nh_tty_Glyph Glyph = ((nh_terminal_Tile*)((nh_List*)Grid_p->Rows.pp[row])->pp[col])->Glyph;
            if (!(Glyph.mark & NH_TTY_MARK_LINE_GRAPHICS)) {
                continue;
            }
            if (nh_terminal_compareForegroundAttributes(Current_p, &Glyph)) {
                *col_p = col;
                *row_p = row;
                *Current_p = Glyph;
                NH_TERMINAL_END(total)
            }
            ++total;
        }
        *col_p = 0;
    }

    *col_p = Grid_p->cols;
    *row_p = Grid_p->rows;

NH_TERMINAL_END(total)
}

static int nh_terminal_getCurrentAttributeRange(
    nh_terminal_Grid *Grid_p, nh_tty_Glyph *Current_p, int *col_p, int *row_p, NH_BOOL foreground)
{
NH_TERMINAL_BEGIN()

    int total = 0;

    for (int row = *row_p; row < Grid_p->rows; ++row) {
        for (int col = *col_p; col < Grid_p->cols; ++col) {
            nh_tty_Glyph Glyph = ((nh_terminal_Tile*)((nh_List*)Grid_p->Rows.pp[row])->pp[col])->Glyph;
            if (foreground && (!Glyph.codepoint || Glyph.codepoint == ' ' || Glyph.mark & NH_TTY_MARK_LINE_GRAPHICS)) {
                continue;
            }
            if (!foreground && (!Glyph.Background.custom && !Glyph.Attributes.reverse && !Glyph.Attributes.blink)) {
                continue;
            }
            if ((foreground && nh_terminal_compareForegroundAttributes(Current_p, &Glyph))
            || (!foreground && nh_terminal_compareBackgroundAttributes(Current_p, &Glyph))) {
                *col_p = col;
                *row_p = row;
                *Current_p = Glyph;
                NH_TERMINAL_END(total)
            }
            ++total;
        }
        *col_p = 0;
    }

    *col_p = Grid_p->cols;
    *row_p = Grid_p->rows;

NH_TERMINAL_END(total)
}

static NH_TERMINAL_RESULT nh_terminal_computeRangeForLineGraphics(
    nh_terminal_GraphicsData *Data_p, nh_terminal_Grid *Grid_p)
{
NH_TERMINAL_BEGIN()

    nh_core_freeArray(&Data_p->Foreground.Ranges2);
    Data_p->Foreground.Ranges2 = nh_core_initArray(sizeof(nh_terminal_AttributeRange), 32);

    if (Grid_p->rows <= 0 || Grid_p->Rows.size == 0) {
        NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
    }

    int total = 0;
    int row = 0;
    int col = 0;

    nh_tty_Glyph Glyph = ((nh_terminal_Tile*)((nh_List*)Grid_p->Rows.pp[0])->pp[0])->Glyph;
    NH_BOOL begin = NH_TRUE;

    while (NH_TRUE)
    {
        nh_tty_Glyph NextGlyph = Glyph;
        total = nh_terminal_getCurrentAttributeRangeForLineGraphics(Grid_p, &NextGlyph, &col, &row) * 12;
        if (!total && !begin) {break;}
        begin = NH_FALSE;

        nh_terminal_AttributeRange *Range_p = nh_core_incrementArray(&Data_p->Foreground.Ranges2);

        Range_p->Glyph = Glyph;
        Range_p->indices = total;

        Glyph = NextGlyph;
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_computeRange(
    nh_terminal_GraphicsData *Data_p, nh_terminal_Grid *Grid_p, NH_BOOL foreground)
{
NH_TERMINAL_BEGIN()

    if (foreground) {
        nh_core_freeArray(&Data_p->Foreground.Ranges);
        Data_p->Foreground.Ranges = nh_core_initArray(sizeof(nh_terminal_AttributeRange), 32);
    } else {
        nh_core_freeArray(&Data_p->Background.Ranges);
        Data_p->Background.Ranges = nh_core_initArray(sizeof(nh_terminal_AttributeRange), 32);
    }

    if (Grid_p->rows <= 0 || Grid_p->Rows.size == 0) {
        NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
    }

    int total = 0;
    int row = 0;
    int col = 0;

    nh_tty_Glyph Glyph = ((nh_terminal_Tile*)((nh_List*)Grid_p->Rows.pp[0])->pp[0])->Glyph;
    NH_BOOL begin = NH_TRUE;

    // Default Foreground/Background.
    while (NH_TRUE)
    {
        nh_tty_Glyph NextGlyph = Glyph;
        total = nh_terminal_getCurrentAttributeRange(Grid_p, &NextGlyph, &col, &row, foreground) * 6;
        if (!total && !begin) {break;}
        begin = NH_FALSE;

        nh_terminal_AttributeRange *Range_p = nh_core_incrementArray(
            foreground ? &Data_p->Foreground.Ranges : &Data_p->Background.Ranges);

        Range_p->Glyph = Glyph;
        Range_p->indices = total;

        Glyph = NextGlyph;
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

// UPDATE ==========================================================================================

static NH_TERMINAL_RESULT nh_terminal_updateForegroundData(
    nh_terminal_Grid *Grid_p, nh_terminal_GraphicsForeground *Foreground_p)
{
NH_TERMINAL_BEGIN()

    nh_core_freeArray(&Foreground_p->Vertices);
    nh_core_freeArray(&Foreground_p->Indices);
    nh_core_freeArray(&Foreground_p->Vertices2);
    nh_core_freeArray(&Foreground_p->Indices2);

    nh_Array Vertices = nh_core_initArray(sizeof(float), 2000);
    nh_Array Indices = nh_core_initArray(sizeof(uint32_t), 1024);
    nh_Array Vertices2 = nh_core_initArray(sizeof(float), 2000);
    nh_Array Indices2 = nh_core_initArray(sizeof(uint32_t), 1024);

    int offset1 = 0;
    int offset2 = 0;

    for (int i = 0; i < Grid_p->Rows.size; ++i) {
        nh_List *Row_p = Grid_p->Rows.pp[i];

        for (int j = 0; j < Row_p->size; ++j) {
            nh_terminal_Tile *Tile_p = Row_p->pp[j];
            if (!Tile_p || !Tile_p->Glyph.codepoint || Tile_p->Glyph.codepoint == ' ') {continue;}

            if (Tile_p->Glyph.mark & NH_TTY_MARK_LINE_GRAPHICS) {
                nh_core_appendToArray(&Vertices2, Tile_p->Foreground.vertices_p, 24);
                uint32_t indices_p[12] = {
                    offset1, offset1 + 1, offset1 + 2, offset1, offset1 + 2, offset1 + 3,
                    offset1 + 4, offset1 + 5, offset1 + 6, offset1 + 4, offset1 + 6, offset1 + 7
                };
                nh_core_appendToArray(&Indices2, indices_p, 12);
                offset1 += 8;
            } else {
                nh_core_appendToArray(&Vertices, Tile_p->Foreground.vertices_p, 20);
                uint32_t indices_p[6] = {offset2, offset2 + 1, offset2 + 2, offset2, offset2 + 2, offset2 + 3};
                nh_core_appendToArray(&Indices, indices_p, 6);
                offset2 += 4;
            }
        }
    }

    Foreground_p->Vertices = Vertices;
    Foreground_p->Indices = Indices;
    Foreground_p->Vertices2 = Vertices2;
    Foreground_p->Indices2 = Indices2;

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_updateBackgroundData(
    nh_terminal_Grid *Grid_p, nh_terminal_GraphicsBackground *Background_p)
{
NH_TERMINAL_BEGIN()

    nh_core_freeArray(&Background_p->Vertices);
    nh_core_freeArray(&Background_p->Indices);

    nh_Array Vertices = nh_core_initArray(sizeof(float), 2000);
    nh_Array Indices = nh_core_initArray(sizeof(uint32_t), 1024);

    int offset = 0;

    for (int i = 0; i < Grid_p->Rows.size; ++i) {
        nh_List *Row_p = Grid_p->Rows.pp[i];
        for (int j = 0; j < Row_p->size; ++j) {
            nh_terminal_Tile *Tile_p = Row_p->pp[j];
            if (!Tile_p->Glyph.Background.custom && !Tile_p->Glyph.Attributes.reverse && !Tile_p->Glyph.Attributes.blink) {continue;}
            nh_core_appendToArray(&Vertices, Tile_p->Background.vertices_p, 12);
            uint32_t indices_p[6] = {offset, offset + 1, offset + 2, offset, offset + 2, offset + 3};
            nh_core_appendToArray(&Indices, indices_p, 6);
            offset += 4;
        }
    }

    Background_p->Vertices = Vertices;
    Background_p->Indices = Indices;

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

static NH_TERMINAL_RESULT nh_terminal_updateBoxesData(
    nh_terminal_Grid *Grid_p, nh_terminal_GraphicsBoxes *Boxes_p)
{
NH_TERMINAL_BEGIN()

    nh_core_freeArray(&Boxes_p->Vertices);
    Boxes_p->Vertices = nh_core_initArray(sizeof(float), 64);
 
    for (int i = 0; i < Grid_p->Boxes.length; ++i) {
        nh_core_appendToArray(
            &Boxes_p->Vertices, ((nh_terminal_Box*)Grid_p->Boxes.p)[i].innerVertices_p, 18);
        nh_core_appendToArray(
            &Boxes_p->Vertices, ((nh_terminal_Box*)Grid_p->Boxes.p)[i].outerVertices_p, 18);
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_TERMINAL_RESULT nh_terminal_updateGraphicsData(
    nh_terminal_GraphicsData *Data_p, nh_terminal_Grid *Grid_p)
{
NH_TERMINAL_BEGIN()

    NH_TERMINAL_CHECK(nh_terminal_updateForegroundData(Grid_p, &Data_p->Foreground))
    NH_TERMINAL_CHECK(nh_terminal_updateBackgroundData(Grid_p, &Data_p->Background))
    NH_TERMINAL_CHECK(nh_terminal_updateBoxesData(Grid_p, &Data_p->Boxes))

    NH_TERMINAL_CHECK(nh_terminal_computeRange(Data_p, Grid_p, NH_TRUE))
    NH_TERMINAL_CHECK(nh_terminal_computeRange(Data_p, Grid_p, NH_FALSE))
    NH_TERMINAL_CHECK(nh_terminal_computeRangeForLineGraphics(Data_p, Grid_p))

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

NH_BOOL nh_terminal_updateBlinkOrGradient(
    nh_terminal_GraphicsState *State_p)
{
NH_TERMINAL_BEGIN()

    NH_BOOL update = NH_FALSE;
    nh_SystemTime Time = nh_core_getSystemTime();

    nh_terminal_Config Config = nh_terminal_getConfig();
    if (nh_core_getSystemTimeDiffInSeconds(State_p->Blink.LastBlink, Time) >= Config.blinkFrequency) {
        update = NH_TRUE;
        State_p->Blink.LastBlink = Time;
        State_p->Blink.on = !State_p->Blink.on;
    }

    if (Config.accents > 1 && nh_core_getSystemTimeDiffInSeconds(State_p->Gradient.LastChange, Time) >= State_p->Gradient.interval) { 
        update = NH_TRUE; 
        State_p->Gradient.LastChange = Time; 
        if (State_p->Gradient.ratio >= 1.0f) {
            State_p->Gradient.index = State_p->Gradient.index == Config.accents-1 ? 0 : State_p->Gradient.index+1; 
            State_p->Gradient.ratio = 0.0f;
        }
        State_p->Gradient.Color = nh_terminal_getGradientColor(&Config, &State_p->Gradient); 
    } 

NH_TERMINAL_END(update)
}

// VIEWPORT ========================================================================================

NH_TERMINAL_RESULT nh_terminal_handleViewportChange(
    nh_terminal_Graphics *Graphics_p, nh_gfx_Viewport *Viewport_p)
{
NH_TERMINAL_BEGIN()

    // Check if it's the initial call.
    if (!Graphics_p->State.Viewport_p) 
    {
        switch (Viewport_p->Surface_p->api)
        {
            case NH_GFX_API_VULKAN :
//                nh_terminal_initVulkanText(Viewport_p->Surface_p->Vulkan.GPU_p, 
//                    &Graphics_p->Foreground.Vulkan);
                break;
            case NH_GFX_API_OPENGL :
                break;
            default :
                NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)
        }
    }

    nh_terminal_Config Config = nh_terminal_getConfig();
 
    Viewport_p->Settings.ClearColor = Config.Background;
    Graphics_p->State.Viewport_p = Viewport_p;

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

// RENDER ==========================================================================================

NH_TERMINAL_RESULT nh_terminal_renderGraphics(
    nh_terminal_Graphics *Graphics_p, nh_terminal_Grid *Grid_p, nh_terminal_Grid *Grid2_p)
{
NH_TERMINAL_BEGIN()

    switch (Graphics_p->State.Viewport_p->Surface_p->api)
    {
        case NH_GFX_API_VULKAN :
//            NH_TERMINAL_CHECK(nh_terminal_renderUsingVulkan(Graphics_p))
            break;
       case NH_GFX_API_OPENGL :
            NH_TERMINAL_CHECK(nh_terminal_renderUsingOpenGL(Graphics_p, Grid_p, Grid2_p))
            break;
        default :
            NH_TERMINAL_END(NH_TERMINAL_ERROR_BAD_STATE)
    }

NH_TERMINAL_END(NH_TERMINAL_SUCCESS)
}

// COLOR ===========================================================================================

nh_Color nh_terminal_getGlyphColor(
    nh_terminal_GraphicsState *State_p, nh_tty_Glyph *Glyph_p, NH_BOOL foreground)
{
NH_TERMINAL_BEGIN()

    nh_terminal_Config Config = nh_terminal_getConfig();
 
    if (foreground) {
        if (Glyph_p->Attributes.reverse || (Glyph_p->Attributes.blink && State_p->Blink.on)) {
            if (Glyph_p->Background.custom) {
                NH_TERMINAL_END(Glyph_p->Background.Color)
            }
            NH_TERMINAL_END(Config.Background)
        }
        if (Glyph_p->mark & NH_TTY_MARK_ACCENT) {
            NH_TERMINAL_END(State_p->Gradient.Color)
        }
        if (Glyph_p->Foreground.custom) {
            NH_TERMINAL_END(Glyph_p->Foreground.Color)
        }
        NH_TERMINAL_END(Config.Foreground)
    }

    // Background.
    if ((Glyph_p->Attributes.reverse && !(Glyph_p->Attributes.blink && State_p->Blink.on)) 
    || (!Glyph_p->Attributes.reverse &&   Glyph_p->Attributes.blink && State_p->Blink.on)) {
        if (Glyph_p->mark & NH_TTY_MARK_ACCENT) {
            NH_TERMINAL_END(State_p->Gradient.Color)
        }
        if (Glyph_p->Foreground.custom) {
            NH_TERMINAL_END(Glyph_p->Foreground.Color)
        }
        NH_TERMINAL_END(Config.Foreground)
    }
    if (Glyph_p->Background.custom) {
        NH_TERMINAL_END(Glyph_p->Background.Color)
    }

NH_TERMINAL_END(Config.Background)
}

