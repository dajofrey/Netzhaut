// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/List.h"
#include "../Header/Macros.h"

#include "../../HTML/Header/Document.h"

#include NH_DEBUG

#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

inline static void Nh_CSS_setListStyleType(
    Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p
);

// IMPLEMENT =======================================================================================

void Nh_CSS_setDefaultListProperties(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_setDefaultListProperty(Node_p, NH_CSS_PROPERTY_LIST_STYLE_TYPE);

NH_SILENT_END()
}

void Nh_CSS_setDefaultListProperty(
    Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type)
{
NH_BEGIN()

    switch (type)
    {
         case NH_CSS_PROPERTY_LIST_STYLE_TYPE : 
             Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_DISC; 
             break;
    }

NH_SILENT_END()
}

NH_RESULT Nh_CSS_computeListProperties(
    Nh_HTML_Node *Node_p, NH_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    Nh_CSS_setListStyleType(Node_p, Properties_pp[NH_CSS_PROPERTY_LIST_STYLE_TYPE]);

NH_END(NH_SUCCESS)
}

// STYLE ===========================================================================================

inline static void Nh_CSS_setListStyleType(
    Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        char *style_p = Property_p->values_pp[0];
        
             if (!strcmp(style_p, "circle"))               {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_CIRCLE;}
        else if (!strcmp(style_p, "armenian"))             {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_ARMENIAN;}
        else if (!strcmp(style_p, "cjk-ideographic"))      {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_CJK_IDEOGRAPHIC;}       
        else if (!strcmp(style_p, "decimal"))              {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_DECIMAL;}
        else if (!strcmp(style_p, "decimal-leading-zero")) {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO;}
        else if (!strcmp(style_p, "georgian"))             {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_GEORGIAN;}
        else if (!strcmp(style_p, "hebrew"))               {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_HEBREW;}
        else if (!strcmp(style_p, "hiragana"))             {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_HIRAGANA;}
        else if (!strcmp(style_p, "hiragana-iroha"))       {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_HIRAGANA_IROHA;}
        else if (!strcmp(style_p, "katakana"))             {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_KATAKANA;}
        else if (!strcmp(style_p, "katakana-iroha"))       {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_KATAKANA_IROHA;}
        else if (!strcmp(style_p, "lower-alpha"))          {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_LOWER_ALPHA;}
        else if (!strcmp(style_p, "lower-greek"))          {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_LOWER_GREEK;}
        else if (!strcmp(style_p, "lower-latin"))          {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_LOWER_LATIN;}
        else if (!strcmp(style_p, "lower-roman"))          {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_LOWER_ROMAN;}
        else if (!strcmp(style_p, "none"))                 {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_NONE;}
        else if (!strcmp(style_p, "square"))               {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_SQUARE;}
        else if (!strcmp(style_p, "upper-alpha"))          {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_UPPER_ALPHA;}
        else if (!strcmp(style_p, "upper-greek"))          {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_UPPER_GREEK;}
        else if (!strcmp(style_p, "upper-latin"))          {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_UPPER_LATIN;}
        else if (!strcmp(style_p, "upper-roman"))          {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_UPPER_ROMAN;}
        else                                               {Node_p->Computed.Properties.List.styleType = NH_CSS_LIST_STYLE_TYPE_DISC;}
    }

NH_SILENT_END()
}

