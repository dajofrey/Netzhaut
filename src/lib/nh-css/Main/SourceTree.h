#ifndef NH_CSS_SOURCE_TREE_H
#define NH_CSS_SOURCE_TREE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Parser/Parser.h"
#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-css_enums
 *  @{
 */

    typedef enum NH_CSS_SOURCE {
        NH_CSS_SOURCE_ELEMENT,
        NH_CSS_SOURCE_TEXT_NODE,
    } NH_CSS_SOURCE;

    typedef enum NH_CSS_PSEUDO_ELEMENT {
        NH_CSS_PSEUDO_ELEMENT_UNDEFINED,
        NH_CSS_PSEUDO_ELEMENT_MARKER,
    } NH_CSS_PSEUDO_ELEMENT;

/** @} */

/** @addtogroup lib_nh-css_structs
 *  @{
 */

    typedef struct nh_css_LogContext {
        int *nr_p;
        NH_BYTE topLevelId_p[256];
        NH_BYTE nodeId_p[1024];
        NH_BYTE indent_p[1024];
    } nh_css_LogContext;

    typedef struct nh_css_TextNode {
        nh_encoding_UTF32String String;
    } nh_css_TextNode;

    typedef struct nh_css_Element {
        nh_List Children;
    } nh_css_Element;

    typedef struct nh_css_Source {
        NH_CSS_SOURCE type;
        NH_BOOL pseudo;
        NH_BOOL mark;
        nh_css_Element Element;
        nh_css_TextNode TextNode;
        nh_List *ComputedValues_p;
        struct nh_css_Source *Parent_p;
    } nh_css_Source;

    typedef struct nh_css_SourceTree {
        nh_css_Source *Root_p;
    } nh_css_SourceTree;

/** @} */

/** @addtogroup lib_nh-css_functions
 *  @{
 */

    nh_css_SourceTree nh_css_createSourceTree(
        nh_webidl_Object *HTML_p, nh_css_StyleSheetListObject *StyleSheets_p, NH_BOOL updateAll,
        void *canvas_p
    );

    void nh_css_freeSource(
        nh_css_Source *Source_p
    );

/** @} */

#endif
