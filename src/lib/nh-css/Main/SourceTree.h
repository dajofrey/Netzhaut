#ifndef NH_CSS_MAIN_SOURCE_TREE_H
#define NH_CSS_MAIN_SOURCE_TREE_H

// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "../Parser/Parser.h"
#include "../Common/Includes.h"

// ENUMS ===========================================================================================

typedef enum NH_CSS_SOURCE {
    NH_CSS_SOURCE_ELEMENT,
    NH_CSS_SOURCE_TEXT_NODE,
} NH_CSS_SOURCE;

typedef enum NH_CSS_PSEUDO_ELEMENT {
    NH_CSS_PSEUDO_ELEMENT_UNDEFINED,
    NH_CSS_PSEUDO_ELEMENT_MARKER,
} NH_CSS_PSEUDO_ELEMENT;

// STRUCTS =========================================================================================

typedef struct nh_css_LogContext {
    int *nr_p;
    char topLevelId_p[256];
    char nodeId_p[1024];
    char indent_p[1024];
} nh_css_LogContext;

typedef struct nh_css_TextNode {
    nh_encoding_UTF32String String;
} nh_css_TextNode;

typedef struct nh_css_Element {
    nh_core_List Children;
} nh_css_Element;

typedef struct nh_css_Source {
    NH_CSS_SOURCE type;
    bool pseudo;
    bool mark;
    nh_css_Element Element;
    nh_css_TextNode TextNode;
    nh_core_List *ComputedValues_p;
    struct nh_css_Source *Parent_p;
} nh_css_Source;

typedef struct nh_css_SourceTree {
    nh_css_Source *Root_p;
} nh_css_SourceTree;

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_css_createSourceTree(
    nh_webidl_Object *HTML_p, nh_webidl_Object *StyleSheets_p, bool updateAll,
    void *canvas_p, nh_css_SourceTree *Result_p
);

void nh_css_freeSource(
    nh_css_Source *Source_p
);

#endif // NH_CSS_MAIN_SOURCE_TREE_H

