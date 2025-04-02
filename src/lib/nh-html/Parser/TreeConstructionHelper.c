// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TreeConstructionHelper.h"
#include "Algorithms.h"

#include "../Interfaces/HTMLElement.h"

#include "../../nh-dom/Interfaces/Attr.h"
#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-dom/Interfaces/NodeList.h"
#include "../../nh-dom/Interfaces/Comment.h"
#include "../../nh-dom/Interfaces/Document.h"
#include "../../nh-dom/Interfaces/DocumentType.h"
#include "../../nh-dom/Interfaces/Text.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

// https://html.spec.whatwg.org/multipage/parsing.html#create-an-element-for-the-token
nh_webidl_Object *nh_html_createElementForToken(
    nh_html_Token *Token_p, nh_webidl_DOMString *Namespace_p, nh_webidl_Object *IntendedParent_p)
{
    nh_webidl_Object *Document_p = nh_dom_getNodeDocument(NH_WEBIDL_GET_DOM_NODE(IntendedParent_p));
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

    bool willExecuteScript = false;

    if (willExecuteScript) {
        // TODO
    }

    nh_webidl_Object *Object_p = nh_dom_createElement(
        Document_p, LocalName_p, Namespace_p, NULL, Is_p, false, Interface_p 
    );
    if (Object_p == NULL) {return NULL;}

    nh_dom_setParentNode(NH_WEBIDL_GET_DOM_NODE(Object_p), IntendedParent_p);
    nh_html_setToken(NH_WEBIDL_GET_HTML_ELEMENT(Object_p), Token_p);

    for (int i = 0; i < Token_p->StartOrEndTag.Attributes.length; ++i) {
        nh_html_Attribute *Attribute_p = &((nh_html_Attribute*)Token_p->StartOrEndTag.Attributes.p)[i];
        nh_webidl_Object *Attr_p = nh_dom_createAttr(
            Document_p, &Attribute_p->Name, &NH_WEBIDL_HTML_NAMESPACE, NULL, NH_WEBIDL_GET_DOM_ELEMENT(Object_p), &Attribute_p->Value
        );
        nh_dom_appendAttr(NH_WEBIDL_GET_DOM_ELEMENT(Object_p), Attr_p);
    }

    if (willExecuteScript) {
        // TODO
    }

    // TODO xmlns
    // TODO resettable
    // TODO form-associated

    return Object_p;
}

// https://html.spec.whatwg.org/multipage/parsing.html#appropriate-place-for-inserting-a-node
NH_WEBIDL_UNSIGNED_LONG nh_html_getAppropriatePlaceForInsertingNode(
    nh_html_Parser *Parser_p, nh_webidl_Object **Target_pp)
{
    if (*Target_pp == NULL) {*Target_pp = nh_html_getCurrentNode(Parser_p);}

    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionPosition = 0;

    if (Parser_p->fosterParenting && (!strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableElement") || !strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableSectionElement") || !strcmp((*Target_pp)->Interface_p->name_p, "HTMLTableRowElement"))) {
        puts("TODO foster parenting");
        exit(0);
    }
    else {
        adjustedInsertionPosition = NH_WEBIDL_GET_DOM_NODEListLength(nh_webidl_getAttribute(*Target_pp, "childNodes"));
    }

    if (!strcmp((*Target_pp)->Interface_p->name_p, "HTMLTemplateElement")) {
        // TODO
    }

    return adjustedInsertionPosition;
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-comment
NH_API_RESULT nh_html_insertCommentAtPosition(
    nh_html_Parser *Parser_p, nh_webidl_Object *Target_p, NH_WEBIDL_UNSIGNED_LONG position)
{
    nh_webidl_Object *Comment_p = nh_dom_createComment(Parser_p->Token_p->CommentOrCharacter.Data);
    NH_CORE_CHECK_MEM(Comment_p)
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_dom_insertIntoNode(NH_WEBIDL_GET_DOM_NODE(Target_p), (nh_webidl_Object*)Comment_p, position))

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_html_insertComment(
    nh_html_Parser *Parser_p, nh_webidl_Object *Node_p)
{
    return nh_html_insertCommentAtPosition(Parser_p, Node_p, nh_html_getAppropriatePlaceForInsertingNode(Parser_p, &Node_p));
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-foreign-element
nh_webidl_Object *nh_html_insertForeignElement(
    nh_html_Parser *Parser_p, nh_html_Token *Token_p, nh_webidl_DOMString *Namespace_p)
{
    nh_webidl_Object *Target_p = NULL;
    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionLocation = nh_html_getAppropriatePlaceForInsertingNode(Parser_p, &Target_p);

    nh_webidl_Object *Element_p = nh_html_createElementForToken(Token_p, Namespace_p, Target_p);
    nh_dom_insertIntoNode(NH_WEBIDL_GET_DOM_NODE(Target_p), Element_p, adjustedInsertionLocation);

    nh_core_pushStack(&Parser_p->OpenElements, Element_p);

    return Element_p;
}

nh_webidl_Object *nh_html_insertHTMLElement(
    nh_html_Parser *Parser_p, nh_html_Token *Token_p)
{
    return nh_html_insertForeignElement(Parser_p, Token_p, &NH_WEBIDL_HTML_NAMESPACE);
}

// https://html.spec.whatwg.org/multipage/parsing.html#insert-a-character
NH_API_RESULT nh_html_insertCharacter(
    nh_html_Parser *Parser_p, nh_webidl_Object *Target_p, nh_webidl_DOMString *Data_p)
{
    if (!Data_p) {Data_p = &Parser_p->Token_p->CommentOrCharacter.Data;}

    NH_WEBIDL_UNSIGNED_LONG adjustedInsertionLocation = nh_html_getAppropriatePlaceForInsertingNode(Parser_p, &Target_p);

    if (!strcmp(Target_p->Interface_p->name_p, "Document")) {
        // No text nodes directly attached to document.  
        return NH_API_SUCCESS;
    } 

    if (adjustedInsertionLocation > 0) {
        nh_webidl_Object *Sibling_p = nh_dom_getFromNodeList(nh_webidl_getAttribute(Target_p, "childNodes"), adjustedInsertionLocation - 1);
        if (!strcmp(Sibling_p->Interface_p->name_p, "Text")) {
            // Append to text node.
            NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_dom_appendToText(Sibling_p, *Data_p))
            return NH_API_SUCCESS;
        }
    }

    // Create new text node.
    nh_webidl_Object *TargetNode_p = NH_WEBIDL_GET_DOM_NODE(Target_p);
    NH_CORE_CHECK_NULL(TargetNode_p)

    nh_webidl_Object *Text_p = nh_dom_createText(*Data_p, nh_dom_getNodeDocument(TargetNode_p)); 
    NH_CORE_CHECK_MEM(Text_p)

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_dom_insertIntoNode(TargetNode_p, Text_p, adjustedInsertionLocation))

    return NH_API_SUCCESS;
}

// https://html.spec.whatwg.org/multipage/parsing.html#generic-rcdata-element-parsing-algorithm
NH_API_RESULT nh_html_parseRAWTEXTOrRCDATA(
    nh_html_Parser *Parser_p, nh_html_Token *Token_p, bool RAWTEXT)
{
    NH_CORE_CHECK_MEM(nh_html_insertHTMLElement(Parser_p, Token_p))

    Parser_p->Tokenizer_p->state = RAWTEXT ? NH_HTML_TOKENIZATION_STATE_RAWTEXT : NH_HTML_TOKENIZATION_STATE_RCDATA;
    Parser_p->originalInsertionMode = Parser_p->insertionMode;
    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_TEXT;

    return NH_API_SUCCESS;
}

nh_html_Token *nh_html_getEmptyStartTagToken(
    NH_HTML_TAG tag)
{
    if (tag >= 255) {return NULL;}

    static nh_html_Token Tokens_p[255];

    Tokens_p[tag].type                          = NH_HTML_TOKEN_START_TAG;
    Tokens_p[tag].StartOrEndTag.TagName.p = (char*)NH_HTML_TAG_NAMES_PP[tag];
    Tokens_p[tag].StartOrEndTag.TagName.length  = strlen(NH_HTML_TAG_NAMES_PP[tag]);
    Tokens_p[tag].StartOrEndTag.Attributes      = nh_core_initArray(1, 1);
    Tokens_p[tag].StartOrEndTag.selfClosing     = false;

    return &Tokens_p[tag];
}

// ACTIVE FORMATTING ELEMENTS ======================================================================
// https://html.spec.whatwg.org/multipage/parsing.html#the-list-of-active-formatting-elements

// https://html.spec.whatwg.org/multipage/parsing.html#push-onto-the-list-of-active-formatting-elements
NH_API_RESULT nh_html_pushActiveFormattingElement(
    nh_html_Parser *Parser_p, nh_webidl_Object *Element_p)
{
    // TODO

    nh_core_appendToList(&Parser_p->ActiveFormattingElements, Element_p);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_html_insertMarker(
    nh_html_Parser *Parser_p)
{
    nh_core_appendToList(&Parser_p->ActiveFormattingElements, NULL);

    return NH_API_SUCCESS;
}

// https://html.spec.whatwg.org/multipage/parsing.html#reconstruct-the-active-formatting-elements
NH_API_RESULT nh_html_reconstructActiveFormattingElements(
    nh_html_Parser *Parser_p)
{
    if (Parser_p->ActiveFormattingElements.size == 0) {
        return NH_API_SUCCESS;
    }

    int index = Parser_p->ActiveFormattingElements.size - 1;
    nh_webidl_Object *Entry_p = Parser_p->ActiveFormattingElements.pp[index];

    if (Entry_p == NULL) { // marker
        return NH_API_SUCCESS;
    }
    if (nh_core_inList(&Parser_p->OpenElements, Entry_p)) {
        return NH_API_SUCCESS;
    }

    RECONSTRUCT_REWIND: {
        if (index == 0) {goto RECONSTRUCT_CREATE;}
    
        Entry_p = Parser_p->ActiveFormattingElements.pp[--index];
       
        if (Entry_p != NULL && !nh_core_inList(&Parser_p->OpenElements, Entry_p)) {
            goto RECONSTRUCT_REWIND;
        }
    }

    RECONSTRUCT_ADVANCE: {
        Entry_p = Parser_p->ActiveFormattingElements.pp[--index];
    }

    RECONSTRUCT_CREATE: {
        nh_html_Token *Token_p = nh_html_getToken(NH_WEBIDL_GET_HTML_ELEMENT(Entry_p)); 
        nh_webidl_Object *NewElement_p = nh_html_insertHTMLElement(Parser_p, Token_p);
        NH_CORE_CHECK_NULL(NewElement_p)
      
        Parser_p->ActiveFormattingElements.pp[index] = NewElement_p;
    
        if (index > 0) {goto RECONSTRUCT_ADVANCE;}
    }

    return NH_API_SUCCESS;
}

// IMPLIED END TAGS ================================================================================

NH_API_RESULT nh_html_generateImpliedEndTags(
    nh_html_Parser *Parser_p, char *exclude_p)
{
    static char *elements_pp[] = 
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
        nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(CurrentNode_p));
        bool pop = false;

        for (int i = 0; i < sizeof(elements_pp)/sizeof(elements_pp[i]); ++i) {
            if (!strcmp(LocalName_p->p, elements_pp[i])) {
                if (exclude_p == NULL || strcmp(elements_pp[i], exclude_p)) {
                    nh_webidl_Object *O_p = nh_html_popCurrentNode(Parser_p);
                    pop = true;
                    break;
                }
            }
        }

        if (!pop) {break;}

        CurrentNode_p = nh_html_getCurrentNode(Parser_p);
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_html_generateAllImpliedEndTags(
    nh_html_Parser *Parser_p)
{
    static char *elements_pp[] = 
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
        nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(CurrentNode_p));
        bool pop = false;

        for (int i = 0; i < sizeof(elements_pp)/sizeof(elements_pp[i]); ++i) {
            if (!strcmp(LocalName_p->p, elements_pp[i])) {
                nh_html_popCurrentNode(Parser_p);
                pop = true;
                break;
            }
        }

        if (!pop) {break;}

        CurrentNode_p = nh_html_getCurrentNode(Parser_p);
    }

    return NH_API_SUCCESS;
}

// STACK OF OPEN ELEMENTS ==========================================================================
// https://html.spec.whatwg.org/multipage/parsing.html#the-stack-of-open-elements

nh_webidl_Object *nh_html_getCurrentNode(
    nh_html_Parser *Parser_p)
{
    return nh_core_peekStack(&Parser_p->OpenElements);
}

nh_webidl_Object *nh_html_getAdjustedCurrentNode(
    nh_html_Parser *Parser_p)
{
    // TODO

    return nh_html_getCurrentNode(Parser_p);
}

NH_API_RESULT nh_html_pushOpenElement(
    nh_html_Parser *Parser_p, nh_webidl_Object *Object_p)
{
    nh_core_pushStack(&Parser_p->OpenElements, Object_p);
    return NH_API_SUCCESS;
}

nh_webidl_Object *nh_html_popCurrentNode(
    nh_html_Parser *Parser_p)
{
    nh_webidl_Object *Object_p = nh_core_popStack(&Parser_p->OpenElements);
    nh_webidl_DOMString *TagName_p = nh_dom_getTagName(NH_WEBIDL_GET_DOM_ELEMENT(Object_p));

    if (!strcmp(TagName_p->p, "STYLE")) {
        // ... must run the update a style block algorithm whenever ...
        // ... The element is popped off the stack of open elements ...
        nh_html_updateAStyleBlock(Object_p);
    }

    return Object_p;
}

// https://html.spec.whatwg.org/multipage/parsing.html#special
bool nh_html_inSpecialCategory(
    nh_webidl_Object *Node_p)
{
    nh_webidl_Object *Element_p = NH_WEBIDL_GET_DOM_ELEMENT(Node_p);
    if (!Element_p) {return false;}

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
            case NH_HTML_TAG_XMP        : return true;
            default                     : break;
        }
    }

    // TODO MathML, SVG

    return false;
}

static bool nh_html_hasElementInSpecificScope(
    nh_html_Parser *Parser_p, char *target_p, char **scope_pp, unsigned int items)
{
    for (int i = Parser_p->OpenElements.size - 1; i >= 0; --i)
    {
        nh_webidl_Object *Node_p = nh_core_getFromList(&Parser_p->OpenElements, i);
        nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Node_p));

        if (!strcmp(LocalName_p->p, target_p)) {return true;}

        for (int j = 0; j < items; ++j) {
            if (!strcmp(LocalName_p->p, scope_pp[j])) {
                return false;
            }
        }
    }

    return false;
}

bool nh_html_hasElementInScope(
    nh_html_Parser *Parser_p, char *target_p)
{
    static char *defaultScope_pp[] =
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

    return nh_html_hasElementInSpecificScope(Parser_p, target_p, defaultScope_pp, sizeof(defaultScope_pp)/sizeof(defaultScope_pp[0]));
}

bool nh_html_hasElementInListItemScope(
    nh_html_Parser *Parser_p, char *target_p)
{
    static char *listItemScope_pp[] =
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

    return nh_html_hasElementInSpecificScope(Parser_p, target_p, listItemScope_pp, sizeof(listItemScope_pp)/sizeof(listItemScope_pp[0]));
}

bool nh_html_hasElementInButtonScope(
    nh_html_Parser *Parser_p, char *target_p)
{
    static char *buttonScope_pp[] =
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

    return nh_html_hasElementInSpecificScope(Parser_p, target_p, buttonScope_pp, sizeof(buttonScope_pp)/sizeof(buttonScope_pp[0]));
}

bool nh_html_hasElementInTableScope(
    nh_html_Parser *Parser_p, char *target_p)
{
    static char *tableScope_pp[] =
    {
        "html",
        "table",
        "template",
    };

    return nh_html_hasElementInSpecificScope(Parser_p, target_p, tableScope_pp, sizeof(tableScope_pp)/sizeof(tableScope_pp[0]));
}

bool nh_html_hasElementInSelectScope(
    nh_html_Parser *Parser_p, char *target_p)
{
    static char *selectScope_pp[] =
    {
        "optgroup",
        "option",
    };

    return nh_html_hasElementInSpecificScope(Parser_p, target_p, selectScope_pp, sizeof(selectScope_pp)/sizeof(selectScope_pp[0]));
}
