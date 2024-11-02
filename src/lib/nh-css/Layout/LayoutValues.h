#ifndef NH_CSS_LAYOUT_VALUES_H
#define NH_CSS_LAYOUT_VALUES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../Properties/Color.h"

#include "../../nh-gfx/Fonts/FontManager.h"
#include "../../nh-gfx/Fonts/Text.h"

typedef struct nh_css_Fragment nh_css_Fragment;

#endif

/** @addtogroup lib_nh-css_enums
 *  @{
 */

    typedef enum NH_CSS_POSITION {
        NH_CSS_POSITION_STATIC,
        NH_CSS_POSITION_RELATIVE,
        NH_CSS_POSITION_ABSOLUTE,
        NH_CSS_POSITION_FIXED,
    } NH_CSS_POSITION;

    typedef enum NH_CSS_LINE_STYLE {
        NH_CSS_LINE_STYLE_NONE,
        NH_CSS_LINE_STYLE_HIDDEN,
        NH_CSS_LINE_STYLE_DOTTED,
        NH_CSS_LINE_STYLE_DASHED,
        NH_CSS_LINE_STYLE_SOLID,
        NH_CSS_LINE_STYLE_DOUBLE,
        NH_CSS_LINE_STYLE_GROOVE,
        NH_CSS_LINE_STYLE_RIDGE,
        NH_CSS_LINE_STYLE_INSET,
        NH_CSS_LINE_STYLE_OUTSET,
    } NH_CSS_LINE_STYLE;

    typedef enum NH_CSS_BOX_SIZING {
        NH_CSS_BOX_SIZING_AUTO, 
        NH_CSS_BOX_SIZING_NONE, 
        NH_CSS_BOX_SIZING_MIN_CONTENT,
        NH_CSS_BOX_SIZING_MAX_CONTENT,
        NH_CSS_BOX_SIZING_FIT_CONTENT,
        NH_CSS_BOX_SIZING_LENGTH,
        NH_CSS_BOX_SIZING_PERCENTAGE,
    } NH_CSS_BOX_SIZING;

    typedef enum NH_CSS_TEXT_ALIGN {
        NH_CSS_TEXT_ALIGN_START,
        NH_CSS_TEXT_ALIGN_AUTO,
        NH_CSS_TEXT_ALIGN_END, 
        NH_CSS_TEXT_ALIGN_LEFT, 
        NH_CSS_TEXT_ALIGN_RIGHT, 
        NH_CSS_TEXT_ALIGN_CENTER, 
        NH_CSS_TEXT_ALIGN_JUSTIFY, 
        NH_CSS_TEXT_ALIGN_MATCH_PARENT,
        NH_CSS_TEXT_ALIGN_JUSTIFY_ALL, 
    } NH_CSS_TEXT_ALIGN;

    typedef enum NH_CSS_FLOAT {
        NH_CSS_FLOAT_LEFT,
        NH_CSS_FLOAT_RIGHT,
        NH_CSS_FLOAT_NONE, 
    } NH_CSS_FLOAT;

    typedef enum NH_CSS_WORD_BREAK {
        NH_CSS_WORD_BREAK_NORMAL,
        NH_CSS_WORD_BREAK_KEEP_ALL,
        NH_CSS_WORD_BREAK_BREAK_ALL, 
        NH_CSS_WORD_BREAK_BREAK_WORD,
    } NH_CSS_WORD_BREAK;

/** @} */

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_TextValues {
        nh_css_Color Color;
        int fontSize;
        int textWidth;
        int textHeight;
        nh_gfx_Text Text;
        nh_core_Array FontFamilies;
        nh_gfx_FontStyle FontStyle;
        NH_CSS_WORD_BREAK wordBreak;
    } nh_css_TextValues;

    typedef struct nh_css_BoxSizing {
        NH_CSS_BOX_SIZING type;
        int value;
        float percentage;
    } nh_css_BoxSizing;

    typedef struct nh_css_BoxValues {
        NH_CSS_POSITION position;
        NH_CSS_FLOAT _float;
        nh_css_Color BackgroundColor;
        nh_css_BoxSizing Width;
        nh_css_BoxSizing Height;
        nh_css_BoxSizing MinWidth;
        nh_css_BoxSizing MinHeight;
        nh_css_BoxSizing MaxWidth;
        nh_css_BoxSizing MaxHeight;
        int marginTop;
        nh_css_BoxSizing MarginRight;
        int marginBottom;
        nh_css_BoxSizing MarginLeft;
        int paddingTop;
        int paddingRight;
        int paddingBottom;
        int paddingLeft;
        int borderTop;
        int borderRight;
        int borderBottom;
        int borderLeft;
        nh_css_Color BorderTopColor;
        nh_css_Color BorderRightColor;
        nh_css_Color BorderBottomColor;
        nh_css_Color BorderLeftColor;
        int borderTopLeftRadius;
        int borderTopRightRadius;
        int borderBottomLeftRadius;
        int borderBottomRightRadius;
        NH_CSS_LINE_STYLE borderTopStyle;
        NH_CSS_LINE_STYLE borderRightStyle;
        NH_CSS_LINE_STYLE borderBottomStyle;
        NH_CSS_LINE_STYLE borderLeftStyle;
        NH_CSS_TEXT_ALIGN textAlignAll;
        NH_CSS_TEXT_ALIGN textAlignLast;
    } nh_css_BoxValues;

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    NH_API_RESULT nh_css_computeTextValues(
        nh_css_Fragment *Fragment_p
    );

    void nh_css_freeTextValues(
        nh_css_Fragment *Fragment_p
    );

    NH_API_RESULT nh_css_computeBoxValues(
        nh_css_Fragment *Fragment_p
    );

/** @} */

#endif
