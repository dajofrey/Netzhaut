// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "StyleSheet.h"
#include "RuleList.h"
#include "Rule.h"
#include "StyleSheetList.h"
#include "Document.h"

#include "../Parser/Parser.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-webidl/Runtime/Object.h"

#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INITIALIZE ======================================================================================

NH_API_RESULT nh_css_initializeCSSStyleSheet(
    nh_webidl_Object *StyleSheet_p)
{
NH_CSS_BEGIN()

    StyleSheet_p->internal_p = nh_core_allocate(sizeof(nh_Array));
    NH_CSS_CHECK_MEM(StyleSheet_p->internal_p)

    StyleSheet_p->Attributes.pp[1] = nh_webidl_createObject("CSS", "CSSRuleList");
    NH_CSS_CHECK_NULL(StyleSheet_p->Attributes.pp[1])

NH_CSS_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// API =============================================================================================

nh_css_StyleSheetObject *nh_css_getStyleSheet(
    nh_webidl_Object *Object_p)
{
NH_CSS_BEGIN()
NH_CSS_END((nh_css_StyleSheetObject*)nh_webidl_getObject(Object_p, "CSS", "CSSStyleSheet"))
}

nh_css_RuleListObject *nh_css_getRuleList(
    nh_css_StyleSheetObject *StyleSheet_p)
{
NH_CSS_BEGIN()
NH_CSS_END(((nh_webidl_Object*)StyleSheet_p)->Attributes.pp[1])
}

void nh_css_setStyleSheetTokens(
    nh_css_StyleSheetObject *StyleSheet_p, nh_Array Tokens)
{
NH_CSS_BEGIN()

    *((nh_Array*)((nh_webidl_Object*)StyleSheet_p)->internal_p) = Tokens;

NH_CSS_SILENT_END()
}

// AT RULES ========================================================================================

static bool nh_css_compareCounterStyleRuleName(
    nh_Array *Prelude_p, char *name_p)
{
NH_CSS_BEGIN()

    bool match = false;

    for (int i = 0; i < Prelude_p->length; ++i) {
        nh_css_ComponentValue *ComponentValue_p = ((nh_css_ComponentValue*)Prelude_p->p)+i;
        if (ComponentValue_p->type == NH_CSS_COMPONENT_VALUE_PRESERVED_TOKEN) {
            nh_css_Token *Token_p = ComponentValue_p->PreservedToken.Token_p;
            if (Token_p->type == NH_CSS_TOKEN_IDENT) {
                nh_encoding_UTF8String Name = nh_encoding_encodeUTF8(Token_p->Other.Value.p, Token_p->Other.Value.length);
                match = !strcmp(Name.p, name_p);
                nh_encoding_freeUTF8(&Name);
                if (match) {break;}
            }
        }
    }

NH_CSS_END(match)
}

nh_webidl_Object *nh_css_findCounterStyleRule(
    nh_css_StyleSheetObject *StyleSheet_p, char *name_p)
{
NH_CSS_BEGIN()

    NH_ENCODING_UTF32 tmp_p[] = {99, 111, 117, 110, 116, 101, 114, 45, 115, 116, 121, 108, 101}; // counter-style
    nh_encoding_UTF32String Name = nh_encoding_initUTF32(13);
    nh_encoding_appendUTF32(&Name, tmp_p, 13);

    nh_css_RuleListObject *RuleList_p = ((nh_webidl_Object*)StyleSheet_p)->Attributes.pp[1];
    nh_List *List_p = nh_css_getRuleListData(RuleList_p);

    nh_webidl_Object *Object_p = NULL;
    for (int i = 0; i < List_p->size; ++i) {
        nh_css_RuleObject *Rule_p = nh_css_getRule(List_p->pp[i]);
        nh_css_Rule *RuleData_p = nh_css_getRuleData(Rule_p);
        if (RuleData_p->Name_p && nh_encoding_compareUTF32(RuleData_p->Name_p->p, Name.p)) {
            if (nh_css_compareCounterStyleRuleName(&RuleData_p->Prelude, name_p)) {
                Object_p = List_p->pp[i]; 
            }
        }
    }

    nh_encoding_freeUTF32(&Name);

NH_CSS_END(Object_p)
}

