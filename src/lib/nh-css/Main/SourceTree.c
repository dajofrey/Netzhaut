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
#include "../Common/Macros.h"

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

static nh_encoding_UTF32String nh_css_getMarkerString(
    nh_css_StyleSheetListObject *StyleSheetList_p, nh_css_Value *ListStyleType_p)
{
NH_CSS_BEGIN()

    nh_encoding_UTF32String MarkerString = nh_encoding_initUTF32(16);
    nh_List *StyleSheets_p = nh_css_getStyleSheetListData(StyleSheetList_p);

    nh_css_CounterStyleRuleObject *CounterStyleRule_p = (nh_css_CounterStyleRuleObject*)
        nh_css_findCounterStyleRule(NH_CSS_DEFAULT_STYLE_SHEET_P, ListStyleType_p->String.p);

    for (int i = 0; i < StyleSheets_p->size; ++i) {
        nh_css_StyleSheetObject *StyleSheet_p = StyleSheets_p->pp[i];
        CounterStyleRule_p = (nh_css_CounterStyleRuleObject*)
            nh_css_findCounterStyleRule(StyleSheet_p, ListStyleType_p->String.p);
    }

    if (CounterStyleRule_p) {
        NH_CSS_END(nh_css_calculateMarkerString(CounterStyleRule_p))
    }

NH_CSS_END(nh_encoding_initUTF32(16))
}

static NH_CSS_RESULT nh_css_handleListItem(
    nh_css_LogContext LogContext, nh_webidl_Object *Object_p, nh_css_StyleSheetListObject *StyleSheetList_p, 
    nh_css_Source **Source_pp, nh_css_Source *Parent_p, int depth)
{
NH_CSS_BEGIN()

    nh_dom_Node *Node_p = nh_dom_getNode(Object_p);
    nh_css_Value *ListStyleType_p = ((nh_List*)nh_dom_getComputedPropertyValues(Node_p))->pp[NH_CSS_PROPERTY_LIST_STYLE_TYPE];

    if (ListStyleType_p->Common.type == NH_CSS_VALUE_KEYWORD) 
    {
        nh_encoding_UTF32String MarkerString = nh_css_getMarkerString(StyleSheetList_p, ListStyleType_p);
        if (MarkerString.length <= 0) {NH_CSS_END(NH_CSS_ERROR_BAD_STATE)}

        nh_css_Source *Marker_p = nh_core_allocate(sizeof(nh_css_Source));
        NH_CSS_CHECK_NULL(Marker_p)

        Marker_p->type = NH_CSS_SOURCE_TEXT_NODE;
        Marker_p->pseudo = NH_CSS_PSEUDO_ELEMENT_MARKER;
        Marker_p->mark = NH_FALSE;
        Marker_p->Parent_p = Parent_p;
        Marker_p->ComputedValues_p = NULL;
        Marker_p->TextNode.String = MarkerString;

        nh_core_appendToList(&Parent_p->Element.Children, Marker_p);
    }
    else {NH_CSS_END(NH_CSS_ERROR_BAD_STATE)}

NH_CSS_END(NH_CSS_SUCCESS)
}

// SOURCE NODE =====================================================================================

static NH_BOOL nh_css_displayNode(
    nh_dom_Node *Node_p)
{
NH_CSS_BEGIN()

    nh_List *Values_p = nh_dom_getComputedPropertyValues(Node_p);
    if (Values_p->size && !strcmp(((nh_css_Value*)Values_p->pp[NH_CSS_PROPERTY_DISPLAY])->String.p, "none")) {
        NH_CSS_END(NH_FALSE)
    }

NH_CSS_END(NH_TRUE)
}

static nh_css_LogContext nh_css_updateLogContext(
    nh_css_LogContext Previous, nh_dom_Element *Element_p, int depth)
{
NH_CSS_BEGIN()

    *Previous.nr_p += 1;

    nh_css_LogContext Context;
    Context.nr_p = Previous.nr_p;
    sprintf(Context.topLevelId_p, Previous.topLevelId_p);

    memset(Context.indent_p, 0, 1024);
    for (int i = 0; i < depth; ++i) {Context.indent_p[i] = ' ';}

    sprintf(Context.nodeId_p, ":%s%s#%d", Context.indent_p, nh_dom_getLocalName(Element_p)->p, *Context.nr_p);

NH_CSS_END(Context)
}

static NH_CSS_RESULT nh_css_createSourceNode(
    nh_css_LogContext LogContext, nh_webidl_Object *Object_p, nh_css_StyleSheetListObject *StyleSheetList_p, 
    NH_BOOL updateAll, nh_css_Source **Source_pp, nh_css_Source *Parent_p, int depth)
{
NH_CSS_BEGIN()

    nh_dom_Element *Element_p = nh_dom_getElement(Object_p);
    nh_dom_Node *Node_p = nh_dom_getNode(Object_p);
    NH_CSS_CHECK_NULL(Node_p)

    NH_BOOL modified = NH_FALSE;
    if (Element_p && (updateAll || nh_dom_getUpdateAnnotationsFlag(Node_p)))
    {
        LogContext = nh_css_updateLogContext(LogContext, Element_p, depth);

        nh_css_freeComputedValues(Node_p);
        nh_css_freeSpecifiedValues(Node_p);

        NH_CSS_CHECK(nh_css_setSpecifiedValues(&LogContext, Element_p, StyleSheetList_p, nh_core_initList(0)))
        NH_CSS_CHECK(nh_css_computeSpecifiedValues(&LogContext, Node_p))

        nh_dom_setUpdateAnnotationsFlag(Node_p, NH_FALSE);
    }

    if (!strcmp(Object_p->Interface_p->name_p, "Text")) 
    {
        NH_BOOL emptyText = NH_TRUE;

        nh_webidl_DOMString *String_p = nh_dom_getTextString((nh_dom_Text*)Object_p);
        NH_CSS_CHECK_NULL(String_p)

        nh_encoding_UTF32String Decoded = nh_encoding_decodeUTF8(String_p->p, String_p->length, NULL);
        for (int i = 0; i < Decoded.length; ++i) {
            if (!nh_encoding_isNonCharacter(Decoded.p[i]) && !nh_encoding_isControl(Decoded.p[i]) && Decoded.p[i] != ' ') {emptyText = NH_FALSE; break;}
        }

        if (!emptyText) {
            *Source_pp = nh_core_allocate(sizeof(nh_css_Source));
            NH_CSS_CHECK_MEM(*Source_pp)
            (*Source_pp)->type = NH_CSS_SOURCE_TEXT_NODE;
            (*Source_pp)->pseudo = NH_CSS_PSEUDO_ELEMENT_UNDEFINED;
            (*Source_pp)->mark = NH_FALSE;
            (*Source_pp)->Parent_p = Parent_p;
            (*Source_pp)->ComputedValues_p = nh_dom_getComputedPropertyValues(Node_p);
            (*Source_pp)->TextNode.String = Decoded;
        }
    }
    else if (nh_css_displayNode(Node_p)) 
    {
        *Source_pp = nh_core_allocate(sizeof(nh_css_Source));
        NH_CSS_CHECK_MEM(*Source_pp)

        (*Source_pp)->type = NH_CSS_SOURCE_ELEMENT;
        (*Source_pp)->pseudo = NH_CSS_PSEUDO_ELEMENT_UNDEFINED;
        (*Source_pp)->mark = NH_FALSE;
        (*Source_pp)->Parent_p = Parent_p;
        (*Source_pp)->ComputedValues_p = nh_dom_getComputedPropertyValues(Node_p);
        (*Source_pp)->Element.Children = nh_core_initList(nh_dom_getNodeChildren(Node_p)->size);

        nh_css_Value *Display_p = nh_core_getFromList(nh_dom_getComputedPropertyValues(Node_p), NH_CSS_PROPERTY_DISPLAY);
        if (Display_p && Display_p->Common.type == NH_CSS_VALUE_KEYWORD && !strcmp(Display_p->String.p, "list-item")) {
            NH_CSS_CHECK(nh_css_handleListItem(LogContext, Object_p, StyleSheetList_p, Source_pp, *Source_pp, depth))
        }
    }

    nh_List *Children_p = nh_dom_getNodeChildren(Node_p);

    for (int i = 0; i < Children_p->size && nh_css_displayNode(Node_p); ++i) 
    {
        nh_css_Source *Source_p = NULL;           
        NH_CSS_CHECK(nh_css_createSourceNode(LogContext, Children_p->pp[i], StyleSheetList_p, updateAll, &Source_p, *Source_pp, depth + 2))
        if (Source_p) {
            if (!Source_pp || !*Source_pp) {NH_CSS_DIAGNOSTIC_END(NH_CSS_ERROR_BAD_STATE)}
            nh_core_appendToList(&(*Source_pp)->Element.Children, Source_p);
        }
    }

NH_CSS_DIAGNOSTIC_END(NH_CSS_SUCCESS)
}

// SOURCE TREE =====================================================================================

static nh_css_LogContext nh_css_initLogContext(
    void *canvas_p)
{
NH_CSS_BEGIN()

    nh_css_LogContext Context;
    Context.nr_p = nh_core_allocate(sizeof(int));
    *Context.nr_p = -1;
    sprintf(Context.topLevelId_p, "nh-css:Canvas:%p:Cascade", canvas_p);
    sprintf(Context.nodeId_p, "");

NH_CSS_END(Context)
}

nh_css_SourceTree nh_css_createSourceTree(
    nh_webidl_Object *HTMLElement_p, nh_css_StyleSheetListObject *StyleSheetList_p, NH_BOOL updateAll,
    void *canvas_p)
{
NH_CSS_BEGIN()

    nh_css_SourceTree Tree;
    nh_css_LogContext LogContext = nh_css_initLogContext(canvas_p);
    nh_css_createSourceNode(LogContext, HTMLElement_p, StyleSheetList_p, updateAll, &Tree.Root_p, NULL, 0);
    nh_core_free(LogContext.nr_p);

NH_CSS_END(Tree)
}

void nh_css_freeSource(
    nh_css_Source *Source_p)
{
NH_CSS_BEGIN()

    if (Source_p->type == NH_CSS_SOURCE_ELEMENT) {
        for (int i = 0; i < Source_p->Element.Children.size; ++i) {
            nh_css_freeSource(Source_p->Element.Children.pp[i]);
        }
        nh_core_freeList(&Source_p->Element.Children, NH_FALSE);
    }

    nh_core_free(Source_p);

NH_CSS_SILENT_END()
}

