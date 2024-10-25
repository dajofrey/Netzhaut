// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Properties.h"

#include "../Common/Log.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/Array.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// PROPERTY NAMES ==================================================================================

const char *NH_CSS_PROPERTY_NAMES_PP[] =
{
    "azimuth",
    "background-attachment",
    "background-color",
    "background-image",
    "background-position",
    "background-repeat",
    "background",
    "border-collapse",
    "border-color",
    "border-spacing",
    "border-style",
    "border-radius",
    "border-top", 
    "border-right",
    "border-bottom", 
    "border-left",
    "border-top-color",
    "border-right-color",
    "border-bottom-color", 
    "border-left-color",
    "border-top-style",
    "border-right-style",
    "border-bottom-style",
    "border-left-style",
    "border-top-width",
    "border-right-width",
    "border-bottom-width",
    "border-left-width",
    "border-width",
    "border-top-left-radius",
    "border-top-right-radius",
    "border-bottom-left-radius",
    "border-bottom-right-radius",
    "border",
    "bottom",
    "caption-side",
    "clear",
    "clip",
    "color",
    "content",
    "counter-increment",
    "counter-reset",
    "cue-after",
    "cue-before",
    "cue",
    "cursor",
    "direction",
    "display",
    "elevation",
    "empty-cells",
    "float",
    "font-family",
    "font-size",
    "font-style",
    "font-variant",
    "font-weight",
    "font",
    "height",
    "left",
    "letter-spacing",
    "line-height",
    "list-style-image",
    "list-style-position",
    "list-style-type",
    "list-style",
    "margin-right",
    "margin-left",
    "margin-top",
    "margin-bottom",
    "margin",
    "margin-block-start", 
    "margin-block-end", 
    "margin-inline-start", 
    "margin-inline-end",
    "margin-block", 
    "margin-inline",
    "max-height",
    "max-width",
    "min-height",
    "min-width",
    "orphans",
    "outline-color",
    "outline-style",
    "outline-width",
    "outline",
    "overflow",
    "padding-top",
    "padding-right",
    "padding-bottom",
    "padding-left",
    "padding",
    "page-break-after",
    "page-break-before",
    "page-break-inside",
    "pause-after",
    "pause-before",
    "pause",
    "pitch-range",
    "pitch",
    "play-during",
    "position",
    "quotes",
    "richness",
    "right",
    "speak-header",
    "speak-numeral",
    "speak-punctuation",
    "speak",
    "speech-rate",
    "stress",
    "table-layout",
    "text-align",
    "text-align-all",
    "text-align-last",
    "text-decoration",
    "text-indent",
    "text-orientation",
    "text-transform",
    "top",
    "unicode-bidi",
    "vertical-align",
    "visibility",
    "voice-family",
    "volume",
    "white-space",
    "widows",
    "width",
    "word-break",
    "word-spacing",
    "writing-mode",
    "z-index",
};

// PROPERTY INHERITANCE ============================================================================

const bool NH_CSS_PROPERTY_INHERITED_P[] =
{
    true,  // 'azimuth'
    false, // 'background-attachment'
    false, // 'background-color'
    false, // 'background-image'
    false, // 'background-position'
    false, // 'background-repeat'
    false, // 'background'
    true,  // 'border-collapse' 
    false, // 'border-color'
    true,  // 'border-spacing'
    false, // 'border-style'
    false, // 'border-radius'
    false, // 'border-top'
    false, // 'border-right'
    false, // 'border-bottom'
    false, // 'border-left'
    false, // 'border-top-color'
    false, // 'border-right-color'
    false, // 'border-bottom-color'
    false, // 'border-left-color'
    false, // 'border-top-style'
    false, // 'border-right-style'
    false, // 'border-bottom-style'
    false, // 'border-left-style'
    false, // 'border-top-width'
    false, // 'border-right-width'
    false, // 'border-bottom-width'
    false, // 'border-left-width'
    false, // 'border-width'
    false, // 'border-top-left-radius'
    false, // 'border-top-right-radius'
    false, // 'border-bottom-left-radius'
    false, // 'border-bottom-right-radius'
    false, // 'border'
    false, // 'bottom'
    true,  // 'caption-side'
    false, // 'clear'
    false, // 'clip'
    true,  // 'color'
    false, // 'content'
    false, // 'counter-increment'
    false, // 'counter-reset'
    false, // 'cue-after'
    false, // 'cue-before'
    false, // 'cue'
    true,  // 'cursor'
    true,  // 'direction'
    false, // 'display'
    true,  // 'elevation'
    true,  // 'empty-cells'
    false, // 'float'
    true,  // 'font-family'
    true,  // 'font-size'
    true,  // 'font-style'
    true,  // 'font-variant'
    true,  // 'font-weight'
    true,  // 'font'
    false, // 'height'
    false, // 'left'
    true,  // 'letter-spacing'
    true,  // 'line-height'
    true,  // 'list-style-image'
    true,  // 'list-style-position'
    true,  // 'list-style-type'
    true,  // 'list-style'
    false, // 'margin-right'
    false, // 'margin-left'
    false, // 'margin-top'
    false, // 'margin-bottom'
    false, // 'margin'
    false, // 'margin-block-start'
    false, // 'margin-block-end'
    false, // 'margin-inline-start'
    false, // 'margin-inline-end'
    false, // 'margin-block'
    false, // 'margin-inline'
    false, // 'max-height'
    false, // 'max-width'
    false, // 'min-height'
    false, // 'min-width'
    true,  // 'orphans'
    false, // 'outline-color'
    false, // 'outline-style'
    false, // 'outline-width'
    false, // 'outline'
    false, // 'overflow'
    false, // 'padding-top'
    false, // 'padding-right'
    false, // 'padding-bottom'
    false, // 'padding-left'
    false, // 'padding'
    false, // 'page-break-after'
    false, // 'page-break-before'
    false, // 'page-break-inside'
    false, // 'pause-after'
    false, // 'pause-before'
    false, // 'pause'
    true,  // 'pitch-range'
    true,  // 'pitch'
    false, // 'play-during'
    false, // 'position'
    true,  // 'quotes'
    true,  // 'richness'
    false, // 'right'
    true,  // 'speak-header'
    true,  // 'speak-numeral'
    true,  // 'speak-punctuation'
    true,  // 'speak'
    true,  // 'speech-rate'
    true,  // 'stress'
    false, // 'table-layout'
    true,  // 'text-align'
    true,  // 'text-align-all'
    true,  // 'text-align-last'
    false, // 'text-decoration'
    true,  // 'text-indent'
    true,  // 'text-orientation'
    true,  // 'text-transform'
    false, // 'top'
    false, // 'unicode-bidi'
    false, // 'vertical-align'
    true,  // 'visibility'
    true,  // 'voice-family'
    true,  // 'volume'
    true,  // 'white-space'
    true,  // 'widows'
    false, // 'width'
    true,  // 'word-break'
    true,  // 'word-spacing'
    true,  // 'writing-mode'
    false, // 'z-index'
};

// INITIAL PROPERTY VALUES =========================================================================

static char *NH_CSS_INITIAL_PROPERTY_VALUES_PP[] =
{
    "center",        // 'azimuth'
    "scroll",        // 'background-attachment'
    "transparent",   // 'background-color'
    "none",          // 'background-image'
    "0% 0%",         // 'background-position'
    "repeat",        // 'background-repeat'
    "",              // 'background'
    "separate",      // 'border-collapse' 
    "",              // 'border-color'
    "0",             // 'border-spacing'
    "",              // 'border-style'
    "",              // 'border-radius'
    "",              // 'border-top'
    "",              // 'border-right'
    "",              // 'border-bottom'
    "",              // 'border-left'
    "currentColor",  // 'border-top-color'
    "currentColor",  // 'border-right-color'
    "currentColor",  // 'border-bottom-color'
    "currentColor",  // 'border-left-color'
    "none",          // 'border-top-style'
    "none",          // 'border-right-style'
    "none",          // 'border-bottom-style'
    "none",          // 'border-left-style'
    "medium",        // 'border-top-width'
    "medium",        // 'border-right-width'
    "medium",        // 'border-bottom-width'
    "medium",        // 'border-left-width'
    "",              // 'border-width'
    "0",             // 'border-top-left-radius'
    "0",             // 'border-top-right-radius'
    "0",             // 'border-bottom-left-radius'
    "0",             // 'border-bottom-right-radius'
    "",              // 'border'
    "auto",          // 'bottom'
    "top",           // 'caption-side'
    "none",          // 'clear'
    "auto",          // 'clip'
    "black",         // 'color'
    "normal",        // 'content'
    "none",          // 'counter-increment'
    "none",          // 'counter-reset'
    "none",          // 'cue-after'
    "none",          // 'cue-before'
    "",              // 'cue'
    "auto",          // 'cursor'
    "ltr",           // 'direction'
    "inline",        // 'display'
    "level",         // 'elevation'
    "show",          // 'empty-cells'
    "none",          // 'float'
    "sans-serif",    // 'font-family'
    "medium",        // 'font-size'
    "normal",        // 'font-style'
    "normal",        // 'font-variant'
    "normal",        // 'font-weight'
    "",              // 'font'
    "auto",          // 'height'
    "auto",          // 'left'
    "normal",        // 'letter-spacing'
    "normal",        // 'line-height'
    "none",          // 'list-style-image'
    "outside",       // 'list-style-position'
    "disc",          // 'list-style-type'
    "",              // 'list-style'
    "0",             // 'margin-right'
    "0",             // 'margin-left'
    "0",             // 'margin-top'
    "0",             // 'margin-bottom'
    "",              // 'margin'
    "0",             // 'margin-block-start'
    "0",             // 'margin-block-end'
    "0",             // 'margin-inline-start'
    "0",             // 'margin-inline-end'
    "",              // 'margin-block'
    "",              // 'margin-inline'
    "none",          // 'max-height'
    "none",          // 'max-width'
    "0",             // 'min-height'
    "0",             // 'min-width'
    "2",             // 'orphans'
    "invert",        // 'outline-color'
    "none",          // 'outline-style'
    "medium",        // 'outline-width'
    "",              // 'outline'
    "visible",       // 'overflow'
    "0",             // 'padding-top'
    "0",             // 'padding-right'
    "0",             // 'padding-bottom'
    "0",             // 'padding-left'
    "",              // 'padding'
    "auto",          // 'page-break-after'
    "auto",          // 'page-break-before'
    "auto",          // 'page-break-inside'
    "0",             // 'pause-after'
    "0",             // 'pause-before'
    "",              // 'pause'
    "50",            // 'pitch-range'
    "medium",        // 'pitch'
    "auto",          // 'play-during'
    "static",        // 'position'
    "auto",          // 'quotes'
    "50",            // 'richness'
    "auto",          // 'right'
    "once",          // 'speak-header'
    "continuous",    // 'speak-numeral'
    "none",          // 'speak-punctuation'
    "normal",        // 'speak'
    "medium",        // 'speech-rate'
    "50",            // 'stress'
    "auto",          // 'table-layout'
    "start",         // 'text-align'
    "start",         // 'text-align-all'
    "auto",          // 'text-align-last'
    "none",          // 'text-decoration'
    "0",             // 'text-indent'
    "mixed",         // 'text-orientation'
    "none",          // 'text-transform'
    "auto",          // 'top'
    "normal",        // 'unicode-bidi'
    "baseline",      // 'vertical-align'
    "visible",       // 'visibility'
    "neutral",       // 'voice-family'
    "medium",        // 'volume'
    "normal",        // 'white-space'
    "2",             // 'widows'
    "auto",          // 'width'
    "normal",        // 'word-break'
    "normal",        // 'word-spacing'
    "horizontal-tb", // 'writing-mode'
    "auto",          // 'z-index'
};

static nh_css_ComponentValueData NH_CSS_INITIAL_PROPERTY_VALUES_P[NH_CSS_PROPERTY_COUNT];

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_css_createInitialPropertyValues()
{
    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) 
    {
        if (strlen(NH_CSS_INITIAL_PROPERTY_VALUES_PP[i]) == 0) {
            NH_CSS_INITIAL_PROPERTY_VALUES_P[i].ComponentValues = nh_core_initArray(0, 0);
            NH_CSS_INITIAL_PROPERTY_VALUES_P[i].Tokens = nh_core_initArray(0, 0);
            continue;
        }

        nh_css_ComponentValueData Data = nh_css_parseComponentValuesFromUTF8Codepoints(
            NULL, NH_CSS_INITIAL_PROPERTY_VALUES_PP[i], strlen(NH_CSS_INITIAL_PROPERTY_VALUES_PP[i])
        );

        NH_CSS_INITIAL_PROPERTY_VALUES_P[i] = Data;
    }

    return NH_API_SUCCESS;
}

nh_css_ComponentValueData *nh_css_getInitialComponentValueData(
    NH_CSS_PROPERTY property)
{
    return &NH_CSS_INITIAL_PROPERTY_VALUES_P[property];
}
