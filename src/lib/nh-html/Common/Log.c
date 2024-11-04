// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"

#include "../../nh-core/Logger/Logger.h"
#include "../../nh-dom/Interfaces/Attr.h"
#include "../../nh-dom/Interfaces/Text.h"
#include "../../nh-dom/Interfaces/NamedNodeMap.h"
#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/NodeList.h"
#include "../../nh-webidl/Runtime/Object.h"
#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <stdio.h>
#include <string.h>

// FUNCTIONS =======================================================================================

#define MAX_INDENT 2047
#define MAX_MESSAGE 4095

static NH_API_RESULT nh_html_logDocumentRecursively(
    char *node_p, nh_webidl_Object *Object_p, bool lastChild, int depth, bool *branch_p, 
    char *indent_p)
{
    nh_core_String Message = nh_core_initString(255);
    nh_core_String Attributes = nh_core_initString(128);

    if (depth >= MAX_INDENT) {return NH_API_ERROR_BAD_STATE;}

    for (int i = 0, offset = 0; i < depth * 2; ++i) {
        if (i % 2) {offset++, indent_p[i] = ' '; continue;}
        indent_p[i] = branch_p[offset] == true ? '|' : ' ';
    }
    indent_p[depth * 2 - 1] = '-';

    nh_dom_Element *Element_p = nh_dom_getElement(Object_p);

    if (Element_p) {
        nh_core_List *Attributes_p = nh_dom_getAttrList(nh_dom_getNamedNodeMap(Element_p));
        for (int i = 0; i < Attributes_p->size; ++i) {
            nh_webidl_DOMString *LocalName_p = nh_dom_getAttrLocalName(Attributes_p->pp[i]);
            nh_webidl_DOMString *Value_p = nh_dom_getAttrValue(Attributes_p->pp[i]);
            nh_core_appendToString(&Attributes, LocalName_p->p, LocalName_p->length);
            nh_core_appendToString(&Attributes, "=\"", 2);
            nh_core_appendToString(&Attributes, Value_p->p, Value_p->length);
            nh_core_appendToString(&Attributes, "\" ", 2);
        } 
    }

    char tag_p[64] = {0};
    sprintf(tag_p, Element_p ? "<%s>" : "%s", Element_p ? nh_dom_getLocalName(Element_p)->p : "node");

    if (Attributes.length) {
        nh_core_appendFormatToString(&Message, "%s%s (%s) %s", indent_p, tag_p, Object_p->Interface_p->name_p, Attributes.p);
    }
    else if (!Element_p && !strcmp(Object_p->Interface_p->name_p, "Text")) {
        nh_webidl_DOMString *DOMString_p = nh_dom_getTextString(((nh_dom_Text*)Object_p));
        nh_encoding_UTF32String String = nh_encoding_decodeUTF8(DOMString_p->p, DOMString_p->length, NULL);
        nh_encoding_UTF32String NewString = nh_encoding_replaceNonCharactersExpressively(&String);
        nh_encoding_UTF8String ReplaceString = nh_encoding_encodeUTF8(NewString.p, NewString.length);
        nh_core_appendFormatToString(&Message, "%s%s \"%s\"", indent_p, Object_p->Interface_p->name_p, ReplaceString.p);
        nh_encoding_freeUTF8(&ReplaceString);
        nh_encoding_freeUTF32(&String);
        nh_encoding_freeUTF32(&NewString);
    }
    else if (!Element_p) {
        nh_core_appendFormatToString(&Message, "%s%s", indent_p, Object_p->Interface_p->name_p);
    }
    else {
        nh_core_appendFormatToString(&Message, "%s%s (%s)", indent_p, tag_p, Object_p->Interface_p->name_p);
    }

    nh_core_sendLogMessage(node_p, NULL, Message.p);
    nh_core_freeString(&Attributes);
    nh_core_freeString(&Message);

    branch_p[depth] = true;
    if (lastChild) {branch_p[depth - 1] = false;}

    memset(indent_p, 0, MAX_INDENT);
    
    nh_dom_NodeList *NodeList_p = nh_webidl_getAttribute(Object_p, "childNodes");
    NH_CORE_CHECK_NULL(NodeList_p)
    NH_WEBIDL_UNSIGNED_LONG length = nh_dom_getNodeListLength(NodeList_p);

    for (NH_WEBIDL_UNSIGNED_LONG i = 0; i < length; ++i) {
        NH_CORE_CHECK(nh_html_logDocumentRecursively(
            node_p, nh_dom_getFromNodeList(NodeList_p, i), i == length - 1, depth + 1, branch_p, 
            indent_p
        ))
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_html_logDocument(
    char *logId_p, nh_webidl_Object *Document_p)
{
    char node_p[255] = {0};
    sprintf(node_p, "nh-html:Parser:%s:DOMTree", logId_p);

    char indent_p[MAX_INDENT] = {'\0'};

    bool branch_p[MAX_INDENT];
    memset(branch_p, false, MAX_INDENT);
    nh_html_logDocumentRecursively(node_p, Document_p, false, 0, branch_p, indent_p);

    return NH_API_SUCCESS;
}
