// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Properties.h"

#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/Array.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
	
// PROPERTY NAMES ==================================================================================

const NH_BYTE *NH_CSS_PROPERTY_NAMES_PP[] =
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

const NH_BOOL NH_CSS_PROPERTY_INHERITED_P[] =
{
    NH_TRUE,  // 'azimuth'
    NH_FALSE, // 'background-attachment'
    NH_FALSE, // 'background-color'
    NH_FALSE, // 'background-image'
    NH_FALSE, // 'background-position'
    NH_FALSE, // 'background-repeat'
    NH_FALSE, // 'background'
    NH_TRUE,  // 'border-collapse' 
    NH_FALSE, // 'border-color'
    NH_TRUE,  // 'border-spacing'
    NH_FALSE, // 'border-style'
    NH_FALSE, // 'border-radius'
    NH_FALSE, // 'border-top'
    NH_FALSE, // 'border-right'
    NH_FALSE, // 'border-bottom'
    NH_FALSE, // 'border-left'
    NH_FALSE, // 'border-top-color'
    NH_FALSE, // 'border-right-color'
    NH_FALSE, // 'border-bottom-color'
    NH_FALSE, // 'border-left-color'
    NH_FALSE, // 'border-top-style'
    NH_FALSE, // 'border-right-style'
    NH_FALSE, // 'border-bottom-style'
    NH_FALSE, // 'border-left-style'
    NH_FALSE, // 'border-top-width'
    NH_FALSE, // 'border-right-width'
    NH_FALSE, // 'border-bottom-width'
    NH_FALSE, // 'border-left-width'
    NH_FALSE, // 'border-width'
    NH_FALSE, // 'border-top-left-radius'
    NH_FALSE, // 'border-top-right-radius'
    NH_FALSE, // 'border-bottom-left-radius'
    NH_FALSE, // 'border-bottom-right-radius'
    NH_FALSE, // 'border'
    NH_FALSE, // 'bottom'
    NH_TRUE,  // 'caption-side'
    NH_FALSE, // 'clear'
    NH_FALSE, // 'clip'
    NH_TRUE,  // 'color'
    NH_FALSE, // 'content'
    NH_FALSE, // 'counter-increment'
    NH_FALSE, // 'counter-reset'
    NH_FALSE, // 'cue-after'
    NH_FALSE, // 'cue-before'
    NH_FALSE, // 'cue'
    NH_TRUE,  // 'cursor'
    NH_TRUE,  // 'direction'
    NH_FALSE, // 'display'
    NH_TRUE,  // 'elevation'
    NH_TRUE,  // 'empty-cells'
    NH_FALSE, // 'float'
    NH_TRUE,  // 'font-family'
    NH_TRUE,  // 'font-size'
    NH_TRUE,  // 'font-style'
    NH_TRUE,  // 'font-variant'
    NH_TRUE,  // 'font-weight'
    NH_TRUE,  // 'font'
    NH_FALSE, // 'height'
    NH_FALSE, // 'left'
    NH_TRUE,  // 'letter-spacing'
    NH_TRUE,  // 'line-height'
    NH_TRUE,  // 'list-style-image'
    NH_TRUE,  // 'list-style-position'
    NH_TRUE,  // 'list-style-type'
    NH_TRUE,  // 'list-style'
    NH_FALSE, // 'margin-right'
    NH_FALSE, // 'margin-left'
    NH_FALSE, // 'margin-top'
    NH_FALSE, // 'margin-bottom'
    NH_FALSE, // 'margin'
    NH_FALSE, // 'margin-block-start'
    NH_FALSE, // 'margin-block-end'
    NH_FALSE, // 'margin-inline-start'
    NH_FALSE, // 'margin-inline-end'
    NH_FALSE, // 'margin-block'
    NH_FALSE, // 'margin-inline'
    NH_FALSE, // 'max-height'
    NH_FALSE, // 'max-width'
    NH_FALSE, // 'min-height'
    NH_FALSE, // 'min-width'
    NH_TRUE,  // 'orphans'
    NH_FALSE, // 'outline-color'
    NH_FALSE, // 'outline-style'
    NH_FALSE, // 'outline-width'
    NH_FALSE, // 'outline'
    NH_FALSE, // 'overflow'
    NH_FALSE, // 'padding-top'
    NH_FALSE, // 'padding-right'
    NH_FALSE, // 'padding-bottom'
    NH_FALSE, // 'padding-left'
    NH_FALSE, // 'padding'
    NH_FALSE, // 'page-break-after'
    NH_FALSE, // 'page-break-before'
    NH_FALSE, // 'page-break-inside'
    NH_FALSE, // 'pause-after'
    NH_FALSE, // 'pause-before'
    NH_FALSE, // 'pause'
    NH_TRUE,  // 'pitch-range'
    NH_TRUE,  // 'pitch'
    NH_FALSE, // 'play-during'
    NH_FALSE, // 'position'
    NH_TRUE,  // 'quotes'
    NH_TRUE,  // 'richness'
    NH_FALSE, // 'right'
    NH_TRUE,  // 'speak-header'
    NH_TRUE,  // 'speak-numeral'
    NH_TRUE,  // 'speak-punctuation'
    NH_TRUE,  // 'speak'
    NH_TRUE,  // 'speech-rate'
    NH_TRUE,  // 'stress'
    NH_FALSE, // 'table-layout'
    NH_TRUE,  // 'text-align'
    NH_TRUE,  // 'text-align-all'
    NH_TRUE,  // 'text-align-last'
    NH_FALSE, // 'text-decoration'
    NH_TRUE,  // 'text-indent'
    NH_TRUE,  // 'text-orientation'
    NH_TRUE,  // 'text-transform'
    NH_FALSE, // 'top'
    NH_FALSE, // 'unicode-bidi'
    NH_FALSE, // 'vertical-align'
    NH_TRUE,  // 'visibility'
    NH_TRUE,  // 'voice-family'
    NH_TRUE,  // 'volume'
    NH_TRUE,  // 'white-space'
    NH_TRUE,  // 'widows'
    NH_FALSE, // 'width'
    NH_TRUE,  // 'word-break'
    NH_TRUE,  // 'word-spacing'
    NH_TRUE,  // 'writing-mode'
    NH_FALSE, // 'z-index'
};

// INITIAL PROPERTY VALUES =========================================================================

static NH_BYTE *NH_CSS_INITIAL_PROPERTY_VALUES_PP[] =
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

// INIT INITIAL PROEPRTY VALUES ====================================================================

NH_CSS_RESULT nh_css_createInitialPropertyValues()
{
NH_CSS_BEGIN()

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

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

nh_css_ComponentValueData *nh_css_getInitialComponentValueData(
    NH_CSS_PROPERTY property)
{
NH_CSS_BEGIN()
NH_CSS_END(&NH_CSS_INITIAL_PROPERTY_VALUES_P[property])
}

