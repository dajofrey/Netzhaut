// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TreeConstructionHelper.h"
#include "Algorithms.h"

#include "../Common/Macros.h"
#include "../Interfaces/HTMLElement.h"

#include "../../nh-dom/Interfaces/Attr.h"
#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-dom/Interfaces/NodeList.h"
#include "../../nh-dom/Interfaces/Comment.h"
#include "../../nh-dom/Interfaces/Document.h"
#include "../../nh-dom/Interfaces/DocumentType.h"
#include "../../nh-dom/Interfaces/Text.h"
#include "../../nh-dom/Common/Macros.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// HELPER ==========================================================================================

// https://html.spec.whatwg.org/multipage/parsing.html#create-an-element-for-the-token
nh_webidl_Object *nh_html_createElementForToken(
    nh_html_Token *Token_p, nh_webidl_DOMString *Namespace_p, nh_webidl_Object *IntendedParent_p)
{
NH_HTML_BEGIN()

    nh_webidl_Object *Document_p = nh_dom_getNodeDocument(nh_dom_getNode(IntendedParent_p));
    nh_webidl_DOMString *LocalName_p = &Token_p->StartOrEndTag.TagName;

    nh_webidl_DOMString *Is_p = NULL;

    for (int i = 0; i < Token_p->StartOrEndTag.Attributes.length; ++i) {
        nh_html_Attribute *Attribute_p = &((nh_html_Attribute*)Token_p->StartOrEndTag.Attributes.p)[i];
        if (!strcmp(Attribute_p->Name.p, "is")) {
            Is_p = &Attribute_p->Value;
        }
    }

    nh_webidl_Interface *Interface_p = NULL;
    if (Namespace_p == &NH_WEBIDL_HTML_NAMESPACE) {
        Interface_p = nh_html_getElementInterface(LocalName_p->p);
    }

    NH_BOOL willExecuteScript = NH_FALSE;

    if (willExecuteScript) {
        // TODO
    }

    nh_webidl_Object *Object_p = nh_dom_createElement(
        Document_p, LocalName_p, Namespace_p, NULL, Is_p, NH_FALSE, Interface_p 
    );
    nh_dom_setParentNode(nh_dom_getNode(Object_p), IntendedParent_p);
    nh_html_setToken(nh_html_getHTMLElement(Object_p), Token_p);

    for (int i = 0; i < Token_p->StartOrEndTag.Attributes.length; ++i) {
        nh_html_Attribute *Attribute_p = &((nh_html_Attribute*)Token_p->StartOrEndTag.Attributes.p)[i];
        nh_dom_Attr *Attr_p = nh_dom_createAttr(
            Document_p, &Attribute_p->Name, &NH_WEBIDL_HTML_NAMESPACE, NULL, nh_dom_getElement(Object_p), &Attribute_p->Value
        );
        nh_dom_appendAttr(nh_dom_getElement(Object_p), Attr_p);
    }

    if (willExecuteScript) {
        // TODO
    }

    // TODO xmlns
    // TODO resettable
    // TODO form-associated

NH_HTML_END(Object_p)
}

// https://html.spec.whatwg.org/multipage/parsing.html#appropriate-place-for-inserting-a-node
NH_WEBIDL_UNSIGNED_LONG nh_html_getAppropriatePlaceForInsertingNode(
    nh_html_Parser *Parser_p, nh_webidl_Object **Target_pp)
{
NH_HTML_BEGIN()

    if (*Target_pp == NULL) {*Target_pp = nh_html_getCurrentNode(Parser_p);}

    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionPosition = 0;

    if (Parser_p->fosterParenting && (!strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableElement") || !strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableSectionElement") || !strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableRowElement"))) {
        puts("TODO foster parenting");
        exit(0);
    }
    else {
        adjustedInsertionPosition = nh_dom_getNodeListLength(nh_webidl_getAttribute(*Target_pp, "childNodes"));
    }

    if (!strcmp((*Target_pp)->Interface_p->name_p, "HTMLTemplateElement")) {
        // TODO
    }

NH_HTML_END(adjustedInsertionPosition)
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-comment
NH_HTML_RESULT nh_html_insertCommentAtPosition(
    nh_html_Parser *Parser_p, nh_webidl_Object *Target_p, NH_WEBIDL_UNSIGNED_LONG position)
{
NH_HTML_BEGIN()

    nh_dom_Comment *Comment_p = nh_dom_createComment(Parser_p->Token_p->CommentOrCharacter.Data);
    NH_HTML_CHECK_MEM(Comment_p)
    NH_DOM_CHECK_2(NH_HTML_ERROR_BAD_STATE, nh_dom_insertIntoNode(nh_dom_getNode(Target_p), (nh_webidl_Object*)Comment_p, position))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

NH_HTML_RESULT nh_html_insertComment(
    nh_html_Parser *Parser_p, nh_webidl_Object *Node_p)
{
NH_HTML_BEGIN()
NH_HTML_DIAGNOSTIC_END(nh_html_insertCommentAtPosition(Parser_p, Node_p, nh_html_getAppropriatePlaceForInsertingNode(Parser_p, &Node_p)))
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-foreign-element
nh_webidl_Object *nh_html_insertForeignElement(
    nh_html_Parser *Parser_p, nh_html_Token *Token_p, nh_webidl_DOMString *Namespace_p)
{
NH_HTML_BEGIN()

    nh_webidl_Object *Target_p = NULL;
    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionLocation = nh_html_getAppropriatePlaceForInsertingNode(Parser_p, &Target_p);

    nh_webidl_Object *Element_p = nh_html_createElementForToken(Token_p, Namespace_p, Target_p);
    nh_dom_insertIntoNode(nh_dom_getNode(Target_p), Element_p, adjustedInsertionLocation);

    nh_pushStack(&Parser_p->OpenElements, Element_p);

NH_HTML_END(Element_p)
}

nh_webidl_Object *nh_html_insertHTMLElement(
    nh_html_Parser *Parser_p, nh_html_Token *Token_p)
{
NH_HTML_BEGIN()
NH_HTML_END(nh_html_insertForeignElement(Parser_p, Token_p, &NH_WEBIDL_HTML_NAMESPACE))
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-character
NH_HTML_RESULT nh_html_insertCharacter(
    nh_html_Parser *Parser_p, nh_webidl_Object *Target_p, nh_webidl_DOMString *Data_p)
{
NH_HTML_BEGIN()

    if (!Data_p) {Data_p = &Parser_p->Token_p->CommentOrCharacter.Data;}

    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionLocation = nh_html_getAppropriatePlaceForInsertingNode(Parser_p, &Target_p);

    if (!strcmp(Target_p->Interface_p->name_p, "Document")) {
        NH_HTML_END(NH_HTML_SUCCESS)
    } 

    if (adjustedInsertionLocation > 0) {
        nh_webidl_Object *Sibling_p = nh_dom_getFromNodeList(nh_webidl_getAttribute(Target_p, "childNodes"), adjustedInsertionLocation - 1);
        if (!strcmp(Sibling_p->Interface_p->name_p, "Text")) {
            NH_DOM_CHECK_2(NH_HTML_ERROR_BAD_STATE, nh_dom_appendToText((nh_dom_Text*)Sibling_p, *Data_p))
            NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
        }
    }

    nh_dom_Node *TargetNode_p = nh_dom_getNode(Target_p);
    NH_HTML_CHECK_NULL(TargetNode_p)

    nh_dom_Text *Text_p = nh_dom_createText(*Data_p, nh_dom_getNodeDocument(TargetNode_p)); 
    NH_HTML_CHECK_MEM(Text_p)

    NH_DOM_CHECK_2(NH_HTML_ERROR_BAD_STATE, nh_dom_insertIntoNode(TargetNode_p, (nh_webidl_Object*)Text_p, adjustedInsertionLocation))

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// https://html.spec.whatwg.org/multipage/parsing.html#generic-rcdata-element-parsing-algorithm
NH_HTML_RESULT nh_html_parseRAWTEXTOrRCDATA(
    nh_html_Parser *Parser_p, nh_html_Token *Token_p, NH_BOOL RAWTEXT)
{
NH_HTML_BEGIN()

    NH_HTML_CHECK_MEM(nh_html_insertHTMLElement(Parser_p, Token_p))

    Parser_p->Tokenizer_p->state = RAWTEXT ? NH_HTML_TOKENIZATION_STATE_RAWTEXT : NH_HTML_TOKENIZATION_STATE_RCDATA;
    Parser_p->originalInsertionMode = Parser_p->insertionMode;
    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_TEXT;

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

nh_html_Token *nh_html_getEmptyStartTagToken(
    NH_HTML_TAG tag)
{
NH_HTML_BEGIN()

    if (tag >= 255) {NH_HTML_END(NULL)}

    static nh_html_Token Tokens_p[255];

    Tokens_p[tag].type                          = NH_HTML_TOKEN_START_TAG;
    Tokens_p[tag].StartOrEndTag.TagName.p = (NH_BYTE*)NH_HTML_TAG_NAMES_PP[tag];
    Tokens_p[tag].StartOrEndTag.TagName.length  = strlen(NH_HTML_TAG_NAMES_PP[tag]);
    Tokens_p[tag].StartOrEndTag.Attributes      = nh_core_initArray(1, 1);
    Tokens_p[tag].StartOrEndTag.selfClosing     = NH_FALSE;

NH_HTML_END(&Tokens_p[tag])
}

// ACTIVE FORMATTING ELEMENTS ======================================================================
// https://html.spec.whatwg.org/multipage/parsing.html#the-list-of-active-formatting-elements

// https://html.spec.whatwg.org/multipage/parsing.html#push-onto-the-list-of-active-formatting-elements
NH_HTML_RESULT nh_html_pushActiveFormattingElement(
    nh_html_Parser *Parser_p, nh_webidl_Object *Element_p)
{
NH_HTML_BEGIN()

    // TODO

    nh_core_appendToList(&Parser_p->ActiveFormattingElements, Element_p);

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

NH_HTML_RESULT nh_html_insertMarker(
    nh_html_Parser *Parser_p)
{
NH_HTML_BEGIN()

    nh_core_appendToList(&Parser_p->ActiveFormattingElements, NULL);

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// https://html.spec.whatwg.org/multipage/parsing.html#reconstruct-the-active-formatting-elements
NH_HTML_RESULT nh_html_reconstructActiveFormattingElements(
    nh_html_Parser *Parser_p)
{
NH_HTML_BEGIN()

    if (Parser_p->ActiveFormattingElements.size == 0) {
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    int index = Parser_p->ActiveFormattingElements.size - 1;
    nh_webidl_Object *Entry_p = Parser_p->ActiveFormattingElements.pp[index];

    if (Entry_p == NULL) { // marker
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }
    if (nh_inList(&Parser_p->OpenElements, Entry_p)) {
        NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
    }

    RECONSTRUCT_REWIND: {
        if (index == 0) {goto RECONSTRUCT_CREATE;}
    
        Entry_p = Parser_p->ActiveFormattingElements.pp[--index];
       
        if (Entry_p != NULL && !nh_inList(&Parser_p->OpenElements, Entry_p)) {
            goto RECONSTRUCT_REWIND;
        }
    }

    RECONSTRUCT_ADVANCE: {
        Entry_p = Parser_p->ActiveFormattingElements.pp[--index];
    }

    RECONSTRUCT_CREATE: {
        nh_html_Token *Token_p = nh_html_getToken(nh_html_getHTMLElement(Entry_p)); 
        nh_webidl_Object *NewElement_p = nh_html_insertHTMLElement(Parser_p, Token_p);
        NH_HTML_CHECK_NULL(NewElement_p)
      
        Parser_p->ActiveFormattingElements.pp[index] = NewElement_p;
    
        if (index > 0) {goto RECONSTRUCT_ADVANCE;}
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// IMPLIED END TAGS ================================================================================

NH_HTML_RESULT nh_html_generateImpliedEndTags(
    nh_html_Parser *Parser_p, NH_BYTE *exclude_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *elements_pp[] = 
    {
        "dd",
        "dt",
        "li",
        "optgroup",
        "option",
        "p",
        "rb",
        "rp",
        "rt",
        "rtc",
    };

    nh_webidl_Object *CurrentNode_p = nh_html_getCurrentNode(Parser_p);

    while (CurrentNode_p)
    {
        nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(nh_dom_getElement(CurrentNode_p));
        NH_BOOL pop = NH_FALSE;

        for (int i = 0; i < sizeof(elements_pp)/sizeof(elements_pp[i]); ++i) {
            if (!strcmp(LocalName_p->p, elements_pp[i])) {
                if (exclude_p == NULL || strcmp(elements_pp[i], exclude_p)) {
                    nh_webidl_Object *O_p = nh_html_popCurrentNode(Parser_p);
                    pop = NH_TRUE;
                    break;
                }
            }
        }

        if (!pop) {break;}

        CurrentNode_p = nh_html_getCurrentNode(Parser_p);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

NH_HTML_RESULT nh_html_generateAllImpliedEndTags(
    nh_html_Parser *Parser_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *elements_pp[] = 
    {
        "caption",
        "colgroup",
        "dd",
        "dt",
        "li",
        "optgroup",
        "option",
        "p",
        "rb",
        "rp",
        "rt",
        "rtc",
        "tbody",
        "td",
        "tfoot",
        "th",
        "thead",
        "tr",
    };

    nh_webidl_Object *CurrentNode_p = nh_html_getCurrentNode(Parser_p);

    while (CurrentNode_p)
    {
        nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(nh_dom_getElement(CurrentNode_p));
        NH_BOOL pop = NH_FALSE;

        for (int i = 0; i < sizeof(elements_pp)/sizeof(elements_pp[i]); ++i) {
            if (!strcmp(LocalName_p->p, elements_pp[i])) {
                nh_html_popCurrentNode(Parser_p);
                pop = NH_TRUE;
                break;
            }
        }

        if (!pop) {break;}

        CurrentNode_p = nh_html_getCurrentNode(Parser_p);
    }

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

// STACK OF OPEN ELEMENTS ==========================================================================
// https://html.spec.whatwg.org/multipage/parsing.html#the-stack-of-open-elements

nh_webidl_Object *nh_html_getCurrentNode(
    nh_html_Parser *Parser_p)
{
NH_HTML_BEGIN()
NH_HTML_END(nh_peekStack(&Parser_p->OpenElements))
}

nh_webidl_Object *nh_html_getAdjustedCurrentNode(
    nh_html_Parser *Parser_p)
{
NH_HTML_BEGIN()

    // TODO

NH_HTML_END(nh_html_getCurrentNode(Parser_p))
}

NH_HTML_RESULT nh_html_pushOpenElement(
    nh_html_Parser *Parser_p, nh_webidl_Object *Object_p)
{
NH_HTML_BEGIN()

    nh_pushStack(&Parser_p->OpenElements, Object_p);

NH_HTML_DIAGNOSTIC_END(NH_HTML_SUCCESS)
}

nh_webidl_Object *nh_html_popCurrentNode(
    nh_html_Parser *Parser_p)
{
NH_HTML_BEGIN()

    nh_webidl_Object *Object_p = nh_popStack(&Parser_p->OpenElements);

    nh_webidl_DOMString *TagName_p = nh_dom_getTagName(nh_dom_getElement(Object_p));

    if (!strcmp(TagName_p->p, "STYLE")) {
        nh_html_updateAStyleBlock(Object_p);
    }

NH_HTML_END(Object_p)
}

// https://html.spec.whatwg.org/multipage/parsing.html#special
NH_BOOL nh_html_inSpecialCategory(
    nh_webidl_Object *Node_p)
{
NH_HTML_BEGIN()

    nh_dom_Element *Element_p = nh_dom_getElement(Node_p);
    if (!Element_p) {NH_HTML_END(NH_FALSE)}

    nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(Element_p);
    int tag = nh_html_getTagIndex(LocalName_p->p);

    if (!strcmp(Node_p->Interface_p->Specification_p->name_p, "HTML"))
    {
        switch (tag)
        {
            case NH_HTML_TAG_ADDRESS    : 
            case NH_HTML_TAG_APPLET     :
            case NH_HTML_TAG_AREA       :
            case NH_HTML_TAG_ARTICLE    :
            case NH_HTML_TAG_ASIDE      :
            case NH_HTML_TAG_BASE       :
            case NH_HTML_TAG_BASEFONT   : 
            case NH_HTML_TAG_BGSOUND    :
            case NH_HTML_TAG_BLOCKQUOTE : 
            case NH_HTML_TAG_BODY       :
            case NH_HTML_TAG_BR         :
            case NH_HTML_TAG_BUTTON     :
            case NH_HTML_TAG_CAPTION    :
            case NH_HTML_TAG_CENTER     :
            case NH_HTML_TAG_COL        :
            case NH_HTML_TAG_COLGROUP   :
            case NH_HTML_TAG_DD         :
            case NH_HTML_TAG_DETAILS    :
            case NH_HTML_TAG_DIR        :
            case NH_HTML_TAG_DIV        :
            case NH_HTML_TAG_DL         :
            case NH_HTML_TAG_DT         :
            case NH_HTML_TAG_EMBED      :
            case NH_HTML_TAG_FIELDSET   :
            case NH_HTML_TAG_FIGCAPTION :
            case NH_HTML_TAG_FIGURE     :
            case NH_HTML_TAG_FOOTER     :
            case NH_HTML_TAG_FORM       :
            case NH_HTML_TAG_FRAME      :
            case NH_HTML_TAG_FRAMESET   : 
            case NH_HTML_TAG_H1         : 
            case NH_HTML_TAG_H2         : 
            case NH_HTML_TAG_H3         : 
            case NH_HTML_TAG_H4         : 
            case NH_HTML_TAG_H5         : 
            case NH_HTML_TAG_H6         : 
            case NH_HTML_TAG_HEAD       :
            case NH_HTML_TAG_HEADER     :
            case NH_HTML_TAG_HGROUP     :
            case NH_HTML_TAG_HR         :
            case NH_HTML_TAG_HTML       :
            case NH_HTML_TAG_IFRAME     :
            case NH_HTML_TAG_IMG        :
            case NH_HTML_TAG_INPUT      :
            case NH_HTML_TAG_KEYGEN     :
            case NH_HTML_TAG_LI         :
            case NH_HTML_TAG_LINK       :
            case NH_HTML_TAG_LISTING    :
            case NH_HTML_TAG_MAIN       :
            case NH_HTML_TAG_MARQUEE    :
            case NH_HTML_TAG_MENU       :
            case NH_HTML_TAG_META       :
            case NH_HTML_TAG_NAV        :
            case NH_HTML_TAG_NOEMBED    :
            case NH_HTML_TAG_NOFRAMES   :
            case NH_HTML_TAG_NOSCRIPT   :
            case NH_HTML_TAG_OBJECT     :
            case NH_HTML_TAG_OL         :
            case NH_HTML_TAG_P          :
            case NH_HTML_TAG_PARAM      :
            case NH_HTML_TAG_PLAINTEXT  :
            case NH_HTML_TAG_PRE        :
            case NH_HTML_TAG_SCRIPT     :
            case NH_HTML_TAG_SECTION    :
            case NH_HTML_TAG_SELECT     :
            case NH_HTML_TAG_SOURCE     :
            case NH_HTML_TAG_STYLE      :
            case NH_HTML_TAG_SUMMARY    :
            case NH_HTML_TAG_TABLE      :
            case NH_HTML_TAG_TBODY      :
            case NH_HTML_TAG_TD         :
            case NH_HTML_TAG_TEMPLATE   :
            case NH_HTML_TAG_TEXTAREA   :
            case NH_HTML_TAG_TFOOT      :
            case NH_HTML_TAG_TH         :
            case NH_HTML_TAG_THEAD      :
            case NH_HTML_TAG_TITLE      :
            case NH_HTML_TAG_TR         :
            case NH_HTML_TAG_TRACK      :
            case NH_HTML_TAG_UL         :
            case NH_HTML_TAG_WBR        :
            case NH_HTML_TAG_XMP        : NH_HTML_END(NH_TRUE)
            default                     : break;
        }
    }

    // TODO MathML, SVG

NH_HTML_END(NH_FALSE)
}

static NH_BOOL nh_html_hasElementInSpecificScope(
    nh_html_Parser *Parser_p, NH_BYTE *target_p, NH_BYTE **scope_pp, unsigned int items)
{
NH_HTML_BEGIN()

    for (int i = Parser_p->OpenElements.size - 1; i >= 0; --i)
    {
        nh_webidl_Object *Node_p = nh_core_getFromList(&Parser_p->OpenElements, i);
        nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(nh_dom_getElement(Node_p));

        if (!strcmp(LocalName_p->p, target_p)) {NH_HTML_END(NH_TRUE)}

        for (int j = 0; j < items; ++j) {
            if (!strcmp(LocalName_p->p, scope_pp[j])) {
                NH_HTML_END(NH_FALSE)
            }
        }
    }

NH_HTML_END(NH_FALSE)
}

NH_BOOL nh_html_hasElementInScope(
    nh_html_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *defaultScope_pp[] =
    {
        "applet",
        "caption",
        "html",
        "table",
        "td",
        "th",
        "marquee",
        "object",
        "template",
        "mi",
        "mo",
        "mn",
        "ms",
        "mtext",
        "annotation-xml",
        "foreignObject",
        "desc",
        "title",
    };

NH_HTML_END(nh_html_hasElementInSpecificScope(Parser_p, target_p, defaultScope_pp, sizeof(defaultScope_pp)/sizeof(defaultScope_pp[0])))
}

NH_BOOL nh_html_hasElementInListItemScope(
    nh_html_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *listItemScope_pp[] =
    {
        "applet",
        "caption",
        "html",
        "table",
        "td",
        "th",
        "marquee",
        "object",
        "template",
        "mi",
        "mo",
        "mn",
        "ms",
        "mtext",
        "annotation-xml",
        "foreignObject",
        "desc",
        "title",
        "ol",
        "ul",
    };

NH_HTML_END(nh_html_hasElementInSpecificScope(Parser_p, target_p, listItemScope_pp, sizeof(listItemScope_pp)/sizeof(listItemScope_pp[0])))
}

NH_BOOL nh_html_hasElementInButtonScope(
    nh_html_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *buttonScope_pp[] =
    {
        "applet",
        "caption",
        "html",
        "table",
        "td",
        "th",
        "marquee",
        "object",
        "template",
        "mi",
        "mo",
        "mn",
        "ms",
        "mtext",
        "annotation-xml",
        "foreignObject",
        "desc",
        "title",
        "button",
    };

NH_HTML_END(nh_html_hasElementInSpecificScope(Parser_p, target_p, buttonScope_pp, sizeof(buttonScope_pp)/sizeof(buttonScope_pp[0])))
}

NH_BOOL nh_html_hasElementInTableScope(
    nh_html_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *tableScope_pp[] =
    {
        "html",
        "table",
        "template",
    };

NH_HTML_END(nh_html_hasElementInSpecificScope(Parser_p, target_p, tableScope_pp, sizeof(tableScope_pp)/sizeof(tableScope_pp[0])))
}

NH_BOOL nh_html_hasElementInSelectScope(
    nh_html_Parser *Parser_p, NH_BYTE *target_p)
{
NH_HTML_BEGIN()

    static NH_BYTE *selectScope_pp[] =
    {
        "optgroup",
        "option",
    };

NH_HTML_END(nh_html_hasElementInSpecificScope(Parser_p, target_p, selectScope_pp, sizeof(selectScope_pp)/sizeof(selectScope_pp[0])))
}

