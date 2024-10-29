// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "SourceTree.h"

#include "../Interfaces/StyleSheetList.h"
#include "../Interfaces/StyleSheet.h"
#include "../Interfaces/CounterStyleRule.h"

#include "../Properties/Properties.h"
#include "../Properties/Values.h"
#include "../Properties/Compute.h"

#include "../Common/Log.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"

#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-dom/Interfaces/Text.h"

#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// LIST ITEM =======================================================================================

/**
 * For example, handles "disc" list marker.
 */
static nh_encoding_UTF32String nh_css_getMarkerString(
    nh_css_StyleSheetListObject *StyleSheetList_p, nh_css_Value *ListStyleType_p)
{
    nh_encoding_UTF32String MarkerString = nh_encoding_initUTF32(16);
    nh_core_List *StyleSheets_p = nh_css_getStyleSheetListData(StyleSheetList_p);

    // First, find the default rule.
    nh_webidl_Object *CounterStyleRule_p =
        nh_css_findCounterStyleRule(NH_CSS_DEFAULT_STYLE_SHEET_P, ListStyleType_p->String.p);

    // Replace default if necessary.
    for (int i = 0; i < StyleSheets_p->size; ++i) {
        nh_css_StyleSheetObject *StyleSheet_p = StyleSheets_p->pp[i];
        nh_webidl_Object *Replace_p =
            nh_css_findCounterStyleRule(StyleSheet_p, ListStyleType_p->String.p);
        if (Replace_p) {
            CounterStyleRule_p = Replace_p;
        }
    }

    if (CounterStyleRule_p) {
        return nh_css_calculateMarkerString(CounterStyleRule_p);
    }

    return nh_encoding_initUTF32(16);
}

/**
 * Handles <li> elements.
 */
static NH_API_RESULT nh_css_handleListItem(
    nh_css_LogContext LogContext, nh_webidl_Object *Object_p, nh_css_StyleSheetListObject *StyleSheetList_p, 
    nh_css_Source **Source_pp, nh_css_Source *Parent_p, int depth)
{
    nh_dom_Node *Node_p = nh_dom_getNode(Object_p);
    nh_css_Value *ListStyleType_p = ((nh_core_List*)nh_dom_getComputedPropertyValues(Node_p))->pp[NH_CSS_PROPERTY_LIST_STYLE_TYPE];

    if (ListStyleType_p->Common.type == NH_CSS_VALUE_KEYWORD) 
    {
        nh_encoding_UTF32String MarkerString = nh_css_getMarkerString(StyleSheetList_p, ListStyleType_p);
        if (MarkerString.length <= 0) {return NH_API_ERROR_BAD_STATE;}

        nh_css_Source *Marker_p = nh_core_allocate(sizeof(nh_css_Source));
        NH_CORE_CHECK_NULL(Marker_p)

        Marker_p->type = NH_CSS_SOURCE_TEXT_NODE;
        Marker_p->pseudo = NH_CSS_PSEUDO_ELEMENT_MARKER;
        Marker_p->mark = false;
        Marker_p->Parent_p = Parent_p;
        Marker_p->ComputedValues_p = NULL;
        Marker_p->TextNode.String = MarkerString;

        nh_core_appendToList(&Parent_p->Element.Children, Marker_p);
    }
    else {return NH_API_ERROR_BAD_STATE;}

    return NH_API_SUCCESS;
}

// SOURCE NODE =====================================================================================

static bool nh_css_displayNode(
    nh_dom_Node *Node_p)
{
    nh_core_List *Values_p = nh_dom_getComputedPropertyValues(Node_p);
    if (Values_p->size && !strcmp(((nh_css_Value*)Values_p->pp[NH_CSS_PROPERTY_DISPLAY])->String.p, "none")) {
        return false;
    }
    if (nh_webidl_getObject(Node_p, "DOM", "Comment")) {
        return false;
    }

    return true;
}

static nh_css_LogContext nh_css_updateLogContext(
    nh_css_LogContext Previous, nh_dom_Element *Element_p, int depth)
{
    *Previous.nr_p += 1;

    nh_css_LogContext Context;
    Context.nr_p = Previous.nr_p;
    sprintf(Context.topLevelId_p, Previous.topLevelId_p);

    memset(Context.indent_p, 0, 1024);
    for (int i = 0; i < depth; ++i) {Context.indent_p[i] = ' ';}

    sprintf(Context.nodeId_p, ":%s%s#%d", Context.indent_p, nh_dom_getLocalName(Element_p)->p, *Context.nr_p);

    return Context;
}

static NH_API_RESULT nh_css_createSourceNode(
    nh_css_LogContext LogContext, nh_webidl_Object *Object_p, nh_css_StyleSheetListObject *StyleSheetList_p, 
    bool updateAll, nh_css_Source **Source_pp, nh_css_Source *Parent_p, int depth)
{
    nh_dom_Element *Element_p = nh_dom_getElement(Object_p);
    nh_dom_Node *Node_p = nh_dom_getNode(Object_p);
    NH_CORE_CHECK_NULL(Node_p)

    bool modified = false;
    if (Element_p && (updateAll || nh_dom_getUpdateAnnotationsFlag(Node_p)))
    {
        LogContext = nh_css_updateLogContext(LogContext, Element_p, depth);

        nh_css_freeComputedValues(Node_p);
        nh_css_freeSpecifiedValues(Node_p);

        NH_CORE_CHECK(nh_css_setSpecifiedValues(&LogContext, Element_p, StyleSheetList_p, nh_core_initList(0)))
        NH_CORE_CHECK(nh_css_computeSpecifiedValues(&LogContext, Node_p))

        nh_dom_setUpdateAnnotationsFlag(Node_p, false);
    }

    if (!strcmp(Object_p->Interface_p->name_p, "Text")) 
    {
        bool emptyText = true;

        nh_webidl_DOMString *String_p = nh_dom_getTextString((nh_dom_Text*)Object_p);
        NH_CORE_CHECK_NULL(String_p)

        nh_encoding_UTF32String Decoded = nh_encoding_decodeUTF8(String_p->p, String_p->length, NULL);
        for (int i = 0; i < Decoded.length; ++i) {
            if (!nh_encoding_isNonCharacter(Decoded.p[i]) && !nh_encoding_isControl(Decoded.p[i]) && Decoded.p[i] != ' ') {emptyText = false; break;}
        }

        if (!emptyText) {
            *Source_pp = nh_core_allocate(sizeof(nh_css_Source));
            NH_CORE_CHECK_MEM(*Source_pp)
            (*Source_pp)->type = NH_CSS_SOURCE_TEXT_NODE;
            (*Source_pp)->pseudo = NH_CSS_PSEUDO_ELEMENT_UNDEFINED;
            (*Source_pp)->mark = false;
            (*Source_pp)->Parent_p = Parent_p;
            (*Source_pp)->ComputedValues_p = nh_dom_getComputedPropertyValues(Node_p);
            (*Source_pp)->TextNode.String = Decoded;
        }
    }
    else if (nh_css_displayNode(Node_p)) 
    {
        *Source_pp = nh_core_allocate(sizeof(nh_css_Source));
        NH_CORE_CHECK_MEM(*Source_pp)

        (*Source_pp)->type = NH_CSS_SOURCE_ELEMENT;
        (*Source_pp)->pseudo = NH_CSS_PSEUDO_ELEMENT_UNDEFINED;
        (*Source_pp)->mark = false;
        (*Source_pp)->Parent_p = Parent_p;
        (*Source_pp)->ComputedValues_p = nh_dom_getComputedPropertyValues(Node_p);
        (*Source_pp)->Element.Children = nh_core_initList(nh_dom_getNodeChildren(Node_p)->size);

        nh_css_Value *Display_p = nh_core_getFromList(nh_dom_getComputedPropertyValues(Node_p), NH_CSS_PROPERTY_DISPLAY);
        if (Display_p && Display_p->Common.type == NH_CSS_VALUE_KEYWORD && !strcmp(Display_p->String.p, "list-item")) {
            NH_CORE_CHECK(nh_css_handleListItem(LogContext, Object_p, StyleSheetList_p, Source_pp, *Source_pp, depth))
        }
    }

    nh_core_List *Children_p = nh_dom_getNodeChildren(Node_p);

    for (int i = 0; i < Children_p->size && nh_css_displayNode(Node_p); ++i) 
    {
        nh_css_Source *Source_p = NULL;           
        NH_CORE_CHECK(nh_css_createSourceNode(LogContext, Children_p->pp[i], StyleSheetList_p, updateAll, &Source_p, *Source_pp, depth + 2))
        if (Source_p) {
            if (!Source_pp || !*Source_pp) {return NH_API_ERROR_BAD_STATE;}
            nh_core_appendToList(&(*Source_pp)->Element.Children, Source_p);
        }
    }

    return NH_API_SUCCESS;
}

// SOURCE TREE =====================================================================================

static nh_css_LogContext nh_css_initLogContext(
    void *Canvas_p)
{
    nh_css_LogContext Context;
    Context.nr_p = nh_core_allocate(sizeof(int));
    *Context.nr_p = -1;
    sprintf(Context.topLevelId_p, "nh-css:Canvas:%p:Cascade", Canvas_p);
    sprintf(Context.nodeId_p, "");
    return Context;
}

NH_API_RESULT nh_css_createSourceTree(
    nh_webidl_Object *HTMLElement_p, nh_css_StyleSheetListObject *StyleSheetList_p, bool updateAll,
    void *Canvas_p, nh_css_SourceTree *Result_p)
{
    NH_CORE_CHECK_NULL(Result_p)
    nh_css_LogContext LogContext = nh_css_initLogContext(Canvas_p);
    NH_CORE_CHECK(nh_css_createSourceNode(LogContext, HTMLElement_p, StyleSheetList_p, updateAll, &Result_p->Root_p, NULL, 0))
    nh_core_free(LogContext.nr_p);
    return NH_API_SUCCESS;
}

void nh_css_freeSource(
    nh_css_Source *Source_p)
{
    if (Source_p->type == NH_CSS_SOURCE_ELEMENT) {
        for (int i = 0; i < Source_p->Element.Children.size; ++i) {
            nh_css_freeSource(Source_p->Element.Children.pp[i]);
        }
        nh_core_freeList(&Source_p->Element.Children, false);
    }

    nh_core_free(Source_p);

    return;
}
