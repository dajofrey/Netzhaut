#ifndef NH_CSS_VALUES_H
#define NH_CSS_VALUES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Filter.h"

#include "../Main/SourceTree.h"
#include "../Parser/Parser.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_enums
 *  @{
 */

    typedef enum NH_CSS_VALUE {
        NH_CSS_VALUE_UNDEFINED,
        NH_CSS_VALUE_NUMBER,
        NH_CSS_VALUE_EM,
        NH_CSS_VALUE_EX,
        NH_CSS_VALUE_CAP,
        NH_CSS_VALUE_CH,
        NH_CSS_VALUE_IC,
        NH_CSS_VALUE_REM,
        NH_CSS_VALUE_LH,
        NH_CSS_VALUE_RLH,
        NH_CSS_VALUE_VW,
        NH_CSS_VALUE_VH,
        NH_CSS_VALUE_VI,
        NH_CSS_VALUE_VB,
        NH_CSS_VALUE_VMIN,
        NH_CSS_VALUE_VMAX,
        NH_CSS_VALUE_IN,
        NH_CSS_VALUE_CM,
        NH_CSS_VALUE_MM,
        NH_CSS_VALUE_PT,
        NH_CSS_VALUE_PC,
        NH_CSS_VALUE_PX,
        NH_CSS_VALUE_Q,
        NH_CSS_VALUE_S,
        NH_CSS_VALUE_MS,
        NH_CSS_VALUE_HZ,
        NH_CSS_VALUE_KHZ,
        NH_CSS_VALUE_DPI,
        NH_CSS_VALUE_DPCM,
        NH_CSS_VALUE_DPPX,
        NH_CSS_VALUE_DEG,
        NH_CSS_VALUE_GRAD,
        NH_CSS_VALUE_RAD,
        NH_CSS_VALUE_TURN,
        NH_CSS_VALUE_PERCENTAGE,
        NH_CSS_VALUE_KEYWORD,
        NH_CSS_VALUE_FUNCTION,
        NH_CSS_VALUE_HEX,
        NH_CSS_VALUE_URL,
        NH_CSS_VALUE_COUNT,
        NH_CSS_VALUE_COLOR,
        NH_CSS_VALUE_STRING,
        NH_CSS_VALUE_KEYWORD_INITIAL,
        NH_CSS_VALUE_KEYWORD_INHERIT,
        NH_CSS_VALUE_KEYWORD_UNSET,
        NH_CSS_VALUE_KEYWORD_REVERT,
    } NH_CSS_VALUE;

    typedef enum NH_CSS_VALUE_SCOPE {
        NH_CSS_VALUE_SCOPE_SPECIFIED,
        NH_CSS_VALUE_SCOPE_COMPUTED,
        NH_CSS_VALUE_SCOPE_USED,
        NH_CSS_VALUE_SCOPE_ACTUAL,
    } NH_CSS_VALUE_SCOPE;

/** @} */

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_Value nh_css_Value;

    typedef struct nh_css_ValueCommon {
        NH_CSS_VALUE type;
        NH_CSS_VALUE_SCOPE scope;
        nh_css_Candidate *Origin_p;
        struct nh_css_Value *Next_p;
    } nh_css_ValueCommon;

    typedef struct nh_css_Number {
        nh_css_ValueCommon Common;
        NH_WEBIDL_DOUBLE value;
    } nh_css_Number;

    typedef struct nh_css_Value {
        nh_css_ValueCommon Common;
        NH_WEBIDL_DOUBLE number;
        nh_encoding_UTF8String String; 
    } nh_css_Value;

/** @} */

/** @addtogroup lib_nh-css_vars
 *  @{
 */

    extern const char *NH_CSS_TYPE_NAMES_PP[];

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_Value nh_css_initValue(
        NH_CSS_VALUE_SCOPE scope, NH_CSS_VALUE type
    );

    NH_API_RESULT nh_css_setSpecifiedValues(
        nh_css_LogContext *LogContext_p, nh_webidl_Object *Element_p, nh_webidl_Object *AuthorStyleSheets_p, 
        nh_core_List UserStyleSheets
    );

    NH_API_RESULT nh_css_freeSpecifiedValues(
        nh_webidl_Object *Node_p
    );

    bool nh_css_isStringValue(
        nh_css_Value *Value_p
    );

    bool nh_css_isLengthValue(
        nh_css_Value *Value_p
    );
    
    bool nh_css_isLengthPercentageValue(
        nh_css_Value *Value_p
    );

/** @} */

#endif
