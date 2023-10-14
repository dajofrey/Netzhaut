// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "CounterStyleRule.h"

#include "../Parser/TokenParser.h"
#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/List.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhencoding/Encodings/UTF8.h"

#include "../../nhwebidl/Runtime/Object.h"

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// API =============================================================================================

nh_css_CounterStyleRuleObject *nh_css_createCounterStyleRule(
    nh_Array Declaration)
{
NH_CSS_BEGIN()

    nh_webidl_Object *CounterStyleRule_p = nh_webidl_createObject("CSS", "CSSCounterStyleRule");
    NH_CSS_CHECK_MEM_2(NULL, CounterStyleRule_p)

    CounterStyleRule_p->internal_p = nh_core_allocate(sizeof(nh_Array));
    NH_CSS_CHECK_MEM_2(NULL, CounterStyleRule_p->internal_p)

    *((nh_Array*)CounterStyleRule_p->internal_p) = Declaration;

NH_CSS_END((nh_css_CounterStyleRuleObject*)CounterStyleRule_p)
}

nh_css_CounterStyleRuleObject *nh_css_getCounterStyleRule(
    nh_webidl_Object *Object_p)
{
NH_CSS_BEGIN()
NH_CSS_END((nh_css_CounterStyleRuleObject*)nh_webidl_getObject(Object_p, "CSS", "CSSCounterStyleRule"))
}

// MARKER CALCULATION ==============================================================================

static nh_css_Declaration *nh_css_getCounterStyleRuleDeclaration(
    nh_css_CounterStyleRuleObject *CounterStyleRule_p, NH_BYTE *declaration_p)
{
NH_CSS_BEGIN()

    nh_Array *Declarations_p = ((nh_webidl_Object*)CounterStyleRule_p)->internal_p;
    for (int i = 0; i < Declarations_p->length; ++i) {
        nh_css_Declaration *Declaration_p = ((nh_css_Declaration*)Declarations_p->p)+i;
        if (!strcmp(Declaration_p->Name.p, declaration_p)) {
            NH_CSS_END(Declaration_p)
        }
    }

NH_CSS_END(NULL)
}

nh_encoding_UTF32String nh_css_calculateMarkerString(
    nh_css_CounterStyleRuleObject *CounterStyleRule_p)
{
NH_CSS_BEGIN()

    nh_encoding_UTF32String Marker = nh_encoding_initUTF32(16);

    nh_css_Declaration *Symbols_p = nh_css_getCounterStyleRuleDeclaration(CounterStyleRule_p, "symbols");
    if (Symbols_p) {
        for (int j = 0; j < Symbols_p->ComponentValues.length; ++j) {
            nh_css_ComponentValue *Value_p = ((nh_css_ComponentValue*)Symbols_p->ComponentValues.p)+j;
            if (Value_p->type == NH_CSS_COMPONENT_VALUE_PRESERVED_TOKEN) {
                nh_css_Token *Token_p = Value_p->PreservedToken.Token_p;
                if (Token_p->type == NH_CSS_TOKEN_IDENT || Token_p->type == NH_CSS_TOKEN_STRING) {
                    nh_encoding_appendUTF32(&Marker, Token_p->Other.Value.p, Token_p->Other.Value.length);
                    break;
                }
            }
        }
    }

    nh_css_Declaration *Suffix_p = nh_css_getCounterStyleRuleDeclaration(CounterStyleRule_p, "suffix");
    if (Suffix_p) {
        for (int j = 0; j < Suffix_p->ComponentValues.length; ++j) {
            nh_css_ComponentValue *Value_p = ((nh_css_ComponentValue*)Suffix_p->ComponentValues.p)+j;
            if (Value_p->type == NH_CSS_COMPONENT_VALUE_PRESERVED_TOKEN) {
                nh_css_Token *Token_p = Value_p->PreservedToken.Token_p;
                if (Token_p->type == NH_CSS_TOKEN_IDENT || Token_p->type == NH_CSS_TOKEN_STRING) {
                    nh_encoding_appendUTF32(&Marker, Token_p->Other.Value.p, Token_p->Other.Value.length);
                    break;
                }
            }
        }
    }

NH_CSS_END(Marker)
}

