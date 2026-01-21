#ifndef NH_CSS_PROPERTIES_FILTER_H
#define NH_CSS_PROPERTIES_FILTER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Properties.h"
#include "../Parser/Parser.h"
#include "../Common/Includes.h"

typedef enum NH_CSS_DECLARATION_ORIGIN {
    NH_CSS_DECLARATION_ORIGIN_AUTHOR,
    NH_CSS_DECLARATION_ORIGIN_USER,
    NH_CSS_DECLARATION_ORIGIN_USER_AGENT,
    NH_CSS_DECLARATION_ORIGIN_ANIMATION,
    NH_CSS_DECLARATION_ORIGIN_TRANSITION,
} NH_CSS_DECLARATION_ORIGIN;

extern const char *NH_CSS_DECLARATION_ORIGIN_NAMES_PP[];

typedef struct nh_css_Filter {
    nh_core_Array Candidates;
    nh_core_List CandidateLists;
    nh_core_Array CandidateListsArray;
} nh_css_Filter;

typedef struct nh_css_Candidate {
    bool direct;
    NH_CSS_DECLARATION_ORIGIN origin;
    nh_css_Declaration *Declaration_p;
    void *CSSStyleRule_p;
} nh_css_Candidate;

nh_css_Filter nh_css_filter(
    void *Element_p, void *AuthorStyleSheets_p, nh_core_List UserStyleSheets
);

#endif // NH_CSS_PROPERTIES_FILTER_H
