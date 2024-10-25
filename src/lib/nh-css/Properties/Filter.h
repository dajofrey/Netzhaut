#ifndef NH_CSS_FILTER_H
#define NH_CSS_FILTER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Properties.h"

#include "../Parser/Parser.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_enums
 *  @{
 */

    typedef enum NH_CSS_DECLARATION_ORIGIN {
        NH_CSS_DECLARATION_ORIGIN_AUTHOR,
        NH_CSS_DECLARATION_ORIGIN_USER,
        NH_CSS_DECLARATION_ORIGIN_USER_AGENT,
        NH_CSS_DECLARATION_ORIGIN_ANIMATION,
        NH_CSS_DECLARATION_ORIGIN_TRANSITION,
    } NH_CSS_DECLARATION_ORIGIN;

/** @} */

/** @addtogroup lib_nh-css_vars
 *  @{
 */

    extern const char *NH_CSS_DECLARATION_ORIGIN_NAMES_PP[];

/** @} */

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_Filter {
        nh_core_Array DeclaredValues;
        nh_core_List DeclaredValueLists;
        nh_core_Array DeclaredValueListsArray;
    } nh_css_Filter;

    typedef struct nh_css_DeclaredValue {
        bool direct;
        NH_CSS_DECLARATION_ORIGIN origin;
        nh_css_Declaration *Declaration_p;
    } nh_css_DeclaredValue;

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_Filter nh_css_filterDeclarations(
        nh_dom_Element *Element_p, nh_css_StyleSheetListObject *AuthorStyleSheets_p, nh_core_List UserStyleSheets
    );

/** @} */

#endif
