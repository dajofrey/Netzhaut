// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TreeConstruction.h"
#include "TreeConstructionHelper.h"
#include "AdoptionAgencyAlgorithm.h"

#include "../Interfaces/HTMLImageElement.h"

#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-dom/Interfaces/NodeList.h"
#include "../../nh-dom/Interfaces/Comment.h"
#include "../../nh-dom/Interfaces/Document.h"
#include "../../nh-dom/Interfaces/DocumentType.h"
#include "../../nh-dom/Interfaces/Text.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

static NH_API_RESULT nh_html_reprocessToken(
    nh_html_Parser *Parser_p, NH_HTML_INSERTION_MODE insertionMode
);

// PARSE ERROR =====================================================================================

static NH_API_RESULT nh_html_newTreeConstructionError(
    nh_html_Parser *Parser_p, NH_HTML_PARSE_ERROR error)
{
return nh_html_newParseError(Parser_p, 0, error);
}

// FUNCTIONS =======================================================================================

static bool nh_html_startsWith(
    char *a, char *b)
{
   if(strncmp(a, b, strlen(b)) == 0) {return true;}

return false;
}

static char *doctypeIdentifierPrefixes_pp[] =
{
    "+//Silmaril//dtd html Pro v0r11 19970101//",
    "-//AS//DTD HTML 3.0 asWedit + extensions//",
    "-//AdvaSoft Ltd//DTD HTML 3.0 asWedit + extensions//",
    "-//IETF//DTD HTML 2.0 Level 1//",
    "-//IETF//DTD HTML 2.0 Level 2//",
    "-//IETF//DTD HTML 2.0 Strict Level 1//",
    "-//IETF//DTD HTML 2.0 Strict Level 2//",
    "-//IETF//DTD HTML 2.0 Strict//",
    "-//IETF//DTD HTML 2.0//",
    "-//IETF//DTD HTML 2.1E//",
    "-//IETF//DTD HTML 3.0//",
    "-//IETF//DTD HTML 3.2 Final//",
    "-//IETF//DTD HTML 3.2//",
    "-//IETF//DTD HTML 3//",
    "-//IETF//DTD HTML Level 0//",
    "-//IETF//DTD HTML Level 1//",
    "-//IETF//DTD HTML Level 2//",
    "-//IETF//DTD HTML Level 3//",
    "-//IETF//DTD HTML Strict Level 0//",
    "-//IETF//DTD HTML Strict Level 1//",
    "-//IETF//DTD HTML Strict Level 2//",
    "-//IETF//DTD HTML Strict Level 3//",
    "-//IETF//DTD HTML Strict//",
    "-//IETF//DTD HTML//",
    "-//Metrius//DTD Metrius Presentational//",
    "-//Microsoft//DTD Internet Explorer 2.0 HTML Strict//",
    "-//Microsoft//DTD Internet Explorer 2.0 HTML//",
    "-//Microsoft//DTD Internet Explorer 2.0 Tables//",
    "-//Microsoft//DTD Internet Explorer 3.0 HTML Strict//",
    "-//Microsoft//DTD Internet Explorer 3.0 HTML//",
    "-//Microsoft//DTD Internet Explorer 3.0 Tables//",
    "-//Netscape Comm. Corp.//DTD HTML//",
    "-//Netscape Comm. Corp.//DTD Strict HTML//",
    "-//O'Reilly and Associates//DTD HTML 2.0//",
    "-//O'Reilly and Associates//DTD HTML Extended 1.0//",
    "-//O'Reilly and Associates//DTD HTML Extended Relaxed 1.0//",
    "-//SQ//DTD HTML 2.0 HoTMetaL + extensions//",
    "-//SoftQuad Software//DTD HoTMetaL PRO 6.0::19990601::extensions to HTML 4.0//",
    "-//SoftQuad//DTD HoTMetaL PRO 4.0::19971010::extensions to HTML 4.0//",
    "-//Spyglass//DTD HTML 2.0 Extended//",
    "-//Sun Microsystems Corp.//DTD HotJava HTML//",
    "-//Sun Microsystems Corp.//DTD HotJava Strict HTML//",
    "-//W3C//DTD HTML 3 1995-03-24//",
    "-//W3C//DTD HTML 3.2 Draft//",
    "-//W3C//DTD HTML 3.2 Final//",
    "-//W3C//DTD HTML 3.2//",
    "-//W3C//DTD HTML 3.2S Draft//",
    "-//W3C//DTD HTML 4.0 Frameset//",
    "-//W3C//DTD HTML 4.0 Transitional//",
    "-//W3C//DTD HTML Experimental 19960712//",
    "-//W3C//DTD HTML Experimental 970421//",
    "-//W3C//DTD W3 HTML//",
    "-//W3O//DTD W3 HTML 3.0//",
    "-//WebTechs//DTD Mozilla HTML 2.0//",
    "-//WebTechs//DTD Mozilla HTML//",
    "-//W3C//DTD HTML 4.01 Frameset//",
    "-//W3C//DTD HTML 4.01 Transitional//",
};

static bool nh_html_setDocumentToQuirksModeFromDOCTYPE(
    nh_html_Parser *Parser_p)
{
    if (Parser_p->Token_p->DOCTYPE.forceQuirks 
    ||  Parser_p->Token_p->DOCTYPE.Name_p == NULL 
    ||  strcmp(Parser_p->Token_p->DOCTYPE.Name_p->p, "html")) 
    {
        return true; 
    }
    if (Parser_p->Token_p->DOCTYPE.SystemIdentifier_p != NULL 
    && !strcmp(Parser_p->Token_p->DOCTYPE.SystemIdentifier_p->p, "http://www.ibm.com/data/dtd/v11/ibmxhtml1-transitional.dtd")) 
    {
        return true; 
    }
    if (Parser_p->Token_p->DOCTYPE.PublicIdentifier_p != NULL)
    {
        if (!strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "-//W3O//DTD W3 HTML Strict 3.0//EN//")
        ||  !strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "-/W3C/DTD HTML 4.0 Transitional/EN")
        ||  !strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "HTML")) 
        {
            return true; 
        }
        for (int i = 0; i < sizeof(doctypeIdentifierPrefixes_pp) / sizeof(doctypeIdentifierPrefixes_pp[0]); ++i) 
        {
            if (nh_html_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, doctypeIdentifierPrefixes_pp[i]))
            {
                return true;
            }
        }
        if (Parser_p->Token_p->DOCTYPE.SystemIdentifier_p == NULL) 
        {
            if (nh_html_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "-//W3C//DTD HTML 4.01 Frameset//")
            ||  nh_html_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "-//W3C//DTD HTML 4.01 Transitional//")) 
            {
                return true; 
            }
        }
    }

    return false;
}

static bool nh_html_setDocumentToLimitedQuirksModeFromDOCTYPE(
    nh_html_Parser *Parser_p)
{
    if (Parser_p->Token_p->DOCTYPE.PublicIdentifier_p != NULL)
    {
        if (!strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "-//W3C//DTD XHTML 1.0 Frameset//" )
        ||  !strcmp(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "-//W3C//DTD XHTML 1.0 Transitional//"))
        {
            return true; 
        }
        if (Parser_p->Token_p->DOCTYPE.SystemIdentifier_p == NULL) 
        {
            if (nh_html_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "-//W3C//DTD HTML 4.01 Frameset//")
            ||  nh_html_startsWith(Parser_p->Token_p->DOCTYPE.PublicIdentifier_p->p, "-//W3C//DTD HTML 4.01 Transitional//")) 
            {
                return true; 
            }
        }
    }

    return false;
}

static NH_API_RESULT nh_html_processInitial(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
            { 
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            goto INITIAL_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_CORE_CHECK(nh_html_insertComment(Parser_p, Parser_p->Document_p))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            if (Parser_p->Token_p->DOCTYPE.Name_p == NULL 
            ||  strcmp(Parser_p->Token_p->DOCTYPE.Name_p->p, "html") 
            ||  Parser_p->Token_p->DOCTYPE.PublicIdentifier_p != NULL
            ||  Parser_p->Token_p->DOCTYPE.SystemIdentifier_p != NULL
            &&  strcmp(Parser_p->Token_p->DOCTYPE.Name_p->p, "about:legacy-compat"))
            {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            }

            nh_webidl_Object *DocumentType_p = nh_dom_createDocumentType(
                Parser_p->Token_p->DOCTYPE.Name_p, Parser_p->Token_p->DOCTYPE.PublicIdentifier_p, 
                Parser_p->Token_p->DOCTYPE.SystemIdentifier_p
            );

            nh_dom_appendToNode(NH_WEBIDL_GET_DOM_NODE(Parser_p->Document_p), DocumentType_p);
            nh_dom_setDocumentType(Parser_p->Document_p, DocumentType_p);

            if (nh_html_setDocumentToQuirksModeFromDOCTYPE(Parser_p)) {
                nh_dom_setDocumentMode(Parser_p->Document_p, &NH_DOM_DOCUMENT_MODE_QUIRKS);
            }
            else if (nh_html_setDocumentToLimitedQuirksModeFromDOCTYPE(Parser_p)) {
                nh_dom_setDocumentMode(Parser_p->Document_p, &NH_DOM_DOCUMENT_MODE_LIMITED_QUIRKS);
            }

            Parser_p->insertionMode = NH_HTML_INSERTION_MODE_BEFORE_HTML;
            break;
        }
        default : INITIAL_DEFAULT :
        {
            NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_dom_setDocumentMode(Parser_p->Document_p, &NH_DOM_DOCUMENT_MODE_QUIRKS))
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_BEFORE_HTML))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processBeforeHTML(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_CORE_CHECK(nh_html_insertComment(Parser_p, Parser_p->Document_p))
            break;
        }
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
            { 
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            goto BEFORE_HTML_DEFAULT;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) 
            {
                nh_webidl_Object *HTMLHtmlElement_p = nh_html_createElementForToken(Parser_p->Token_p, &NH_WEBIDL_HTML_NAMESPACE, Parser_p->Document_p);
                NH_CORE_CHECK_MEM(HTMLHtmlElement_p)
                NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_dom_appendToNode(NH_WEBIDL_GET_DOM_NODE(Parser_p->Document_p), HTMLHtmlElement_p))
                NH_CORE_CHECK(nh_html_pushOpenElement(Parser_p, HTMLHtmlElement_p))
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_BEFORE_HEAD;
                break; 
            }
            goto BEFORE_HTML_DEFAULT;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HEAD
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BODY
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HTML
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BR) 
            {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            // fallthrough
        }
        default : BEFORE_HTML_DEFAULT :
        {
            nh_webidl_Object *HTMLHtmlElement_p = nh_html_createElementForToken(nh_html_getEmptyStartTagToken(NH_HTML_TAG_HTML), &NH_WEBIDL_HTML_NAMESPACE, Parser_p->Document_p);
            NH_CORE_CHECK_MEM(HTMLHtmlElement_p)
            NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_dom_appendToNode(NH_WEBIDL_GET_DOM_NODE(Parser_p->Document_p), HTMLHtmlElement_p))
            NH_CORE_CHECK(nh_html_pushOpenElement(Parser_p, HTMLHtmlElement_p))
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_BEFORE_HEAD))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processBeforeHead(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
            { 
                break;
            }
            goto BEFORE_HEAD_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_CORE_CHECK(nh_html_insertComment(Parser_p, NULL))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML)
            {
                Parser_p->HeadElement_p = nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p);
                NH_CORE_CHECK_MEM(Parser_p->HeadElement_p)
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD;
                break; 
            }
            goto BEFORE_HEAD_DEFAULT;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HEAD
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BODY
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_HTML
            &&  Parser_p->Token_p->StartOrEndTag.tag != NH_HTML_TAG_BR) 
            {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            // fallthrough
        }
        default : BEFORE_HEAD_DEFAULT :
        {
            Parser_p->HeadElement_p = nh_html_insertHTMLElement(Parser_p, nh_html_getEmptyStartTagToken(NH_HTML_TAG_HEAD));
            NH_CORE_CHECK_MEM(Parser_p->HeadElement_p)
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_HEAD))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processInBody(
    nh_html_Parser *Parser_p
);

static NH_API_RESULT nh_html_processInHead(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
            { 
                NH_CORE_CHECK(nh_html_insertCharacter(Parser_p, NULL, NULL))
                break;
            }
            goto IN_HEAD_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_CORE_CHECK(nh_html_insertComment(Parser_p, NULL))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_HTML : 
                {
                    NH_CORE_CHECK(nh_html_processInBody(Parser_p))
                    break;
                }
                case NH_HTML_TAG_BASE     :
                case NH_HTML_TAG_BASEFONT :
                case NH_HTML_TAG_BGSOUND  :
                case NH_HTML_TAG_LINK     :
                {
                    NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                    // TODO Acknowledge the token's self-closing flag, if it is set.
                    break; 
                }
                case NH_HTML_TAG_META :
                {
                    NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                    // TODO
                    break;
                }
                case NH_HTML_TAG_TITLE : 
                {
                    NH_CORE_CHECK(nh_html_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, false))
                    break;
                }
                case NH_HTML_TAG_NOFRAMES :
                case NH_HTML_TAG_STYLE    :
                {
                    NH_CORE_CHECK(nh_html_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, true))
                    break;
                }
                case NH_HTML_TAG_SCRIPT :
                {
                    // TODO
                    break;
                }
                case NH_HTML_TAG_TEMPLATE :
                {
                    NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    NH_CORE_CHECK(nh_html_insertMarker(Parser_p))
                    Parser_p->framesetOk = false;
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_TEMPLATE;
                    // TODO Push "in template" onto the stack of template insertion modes so that it is the new current template insertion mode.
                    break;
                }
                case NH_HTML_TAG_HEAD :
                {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                case NH_HTML_TAG_NOSCRIPT :
                {
                    if (Parser_p->scripting) {
                        NH_CORE_CHECK(nh_html_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, true))
                        break;
	    	    }

                    NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD_NOSCRIPT;
                    break;
                }
                default :
                {
                    goto IN_HEAD_DEFAULT;
                }
            }
            break;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_HEAD :
                {
                    NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_AFTER_HEAD;
                    break; 
                }
                case NH_HTML_TAG_BODY :
                case NH_HTML_TAG_HTML :
                case NH_HTML_TAG_BR   :
                {
                    goto IN_HEAD_DEFAULT;
                }
                case NH_HTML_TAG_TEMPLATE :
                {
                    // TODO
                    break;
                }
                default :
                {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
            }
            break;
        }
        default : IN_HEAD_DEFAULT :
        {
            NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_AFTER_HEAD))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processInHeadNoscript(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
            { 
                NH_CORE_CHECK(nh_html_processInHead(Parser_p))
                break;
            }
            goto IN_HEAD_NOSCRIPT_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_CORE_CHECK(nh_html_processInHead(Parser_p))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_HTML     :
                {
                    NH_CORE_CHECK(nh_html_processInBody(Parser_p))
                    break;
                }
                case NH_HTML_TAG_BASEFONT :
                case NH_HTML_TAG_BGSOUND  :
                case NH_HTML_TAG_LINK     :
                case NH_HTML_TAG_META     :
                case NH_HTML_TAG_NOFRAMES :
                case NH_HTML_TAG_STYLE    :
                {
                    NH_CORE_CHECK(nh_html_processInHead(Parser_p))
                    break;
                }
                case NH_HTML_TAG_HEAD     :
                case NH_HTML_TAG_NOSCRIPT :
                {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                default :
                {
                    goto IN_HEAD_NOSCRIPT_DEFAULT;
                }
            }
            break;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_NOSCRIPT :
                {
                    NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_HEAD;
                    break; 
                }
                case NH_HTML_TAG_BR :
                {
                    goto IN_HEAD_NOSCRIPT_DEFAULT;
                }
                default :
                {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
            }
            break;
        }
        default : IN_HEAD_NOSCRIPT_DEFAULT :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_HEAD))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processAfterHead(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
            { 
                NH_CORE_CHECK(nh_html_insertCharacter(Parser_p, NULL, NULL))
                break;
            }
            goto AFTER_HEAD_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_CORE_CHECK(nh_html_insertComment(Parser_p, NULL))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_HTML : 
                {
                    NH_CORE_CHECK(nh_html_processInBody(Parser_p))
                    break;
                }
                case NH_HTML_TAG_BODY :
                {
                    NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    Parser_p->framesetOk = false;
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_BODY;
                    break; 
                }
                case NH_HTML_TAG_FRAMESET :
                {
                    NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_FRAMESET;
                    break; 
                }
                case NH_HTML_TAG_BASE     :
                case NH_HTML_TAG_BASEFONT :
                case NH_HTML_TAG_BGSOUND  :
                case NH_HTML_TAG_LINK     :
                case NH_HTML_TAG_META     :
                case NH_HTML_TAG_NOFRAMES :
                case NH_HTML_TAG_SCRIPT   :
                case NH_HTML_TAG_STYLE    :
                case NH_HTML_TAG_TEMPLATE :
                case NH_HTML_TAG_TITLE    :
                {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                    NH_CORE_CHECK(nh_html_pushOpenElement(Parser_p, Parser_p->HeadElement_p))
                    NH_CORE_CHECK(nh_html_processInHead(Parser_p)) 
                    nh_core_removeFromList2(&Parser_p->OpenElements, false, Parser_p->HeadElement_p);
                    break;
                }
                case NH_HTML_TAG_HEAD :
                {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                default :
                {
                    goto AFTER_HEAD_DEFAULT;
                }
            }
            break;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            switch (Parser_p->Token_p->StartOrEndTag.tag)
            {
                case NH_HTML_TAG_TEMPLATE :
                {
                    NH_CORE_CHECK(nh_html_processInHead(Parser_p))
                    break;
                }
                case NH_HTML_TAG_BODY :
                case NH_HTML_TAG_HTML :
                case NH_HTML_TAG_BR   :
                {
                    goto AFTER_HEAD_DEFAULT;
                }
                default :
                {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
            }
            break;
        }
        default : AFTER_HEAD_DEFAULT :
        {
            NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, nh_html_getEmptyStartTagToken(NH_HTML_TAG_BODY)))
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_BODY))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_closeElement(
    nh_html_Parser *Parser_p, char *tagName_p)
{
    NH_CORE_CHECK(nh_html_generateImpliedEndTags(Parser_p, tagName_p))

    if (strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(nh_html_getCurrentNode(Parser_p)))->p, tagName_p)) {
        // parse error
    }

    nh_webidl_Object *Pop_p = nh_html_popCurrentNode(Parser_p);
    while (Pop_p != NULL && strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Pop_p))->p, tagName_p)) {
        Pop_p = nh_html_popCurrentNode(Parser_p);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_closePElement(
    nh_html_Parser *Parser_p)
{
    return nh_html_closeElement(Parser_p, "p");
}

static bool nh_html_hasOtherOpenElements(
    nh_html_Parser *Parser_p)
{
    static char *elements_pp[] = {
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
        "body",
        "html",
    };

    for (int i = 0; i < Parser_p->OpenElements.size; ++i) 
    {
        nh_webidl_Object *Element_p = NH_WEBIDL_GET_DOM_ELEMENT(Parser_p->OpenElements.pp[i]);
        if (!Element_p) {continue;}

        nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(Element_p);

        bool match = false;
        for (int j = 0; j < sizeof(elements_pp)/sizeof(elements_pp[0]); ++j) {
            if (!strcmp(LocalName_p->p, elements_pp[j])) {match = true; break;}
        }

        if (!match) {return true;}
    }

    return false;
}

static bool nh_html_hasOpenElement(
    nh_html_Parser *Parser_p, char *target_p)
{
    for (int i = Parser_p->OpenElements.size - 1; i >= 0; --i)
    {
        nh_webidl_Object *Node_p = nh_core_getFromList(&Parser_p->OpenElements, i);
        nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Node_p));
        if (!strcmp(LocalName_p->p, target_p)) {return true;}
    }

    return false;
}

static NH_API_RESULT nh_html_processInBody(
    nh_html_Parser *Parser_p)
{
    if (Parser_p->Token_p->type == NH_HTML_TOKEN_CHARACTER)
    {
        if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0)
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
        }
        else
        if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
        ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
        ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
        ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
        ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
        { 
            NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
            NH_CORE_CHECK(nh_html_insertCharacter(Parser_p, NULL, NULL))
        }
        else 
        {
            NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
            NH_CORE_CHECK(nh_html_insertCharacter(Parser_p, NULL, NULL))
            Parser_p->framesetOk = false;
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_COMMENT) {
        NH_CORE_CHECK(nh_html_insertComment(Parser_p, NULL))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_DOCTYPE) {
        NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_HTML :
            {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))

                for (int i = 0; i < Parser_p->OpenElements.size; ++i) {
                    nh_webidl_Object *Element_p = Parser_p->OpenElements.pp[i];
                    if (!strcmp(Element_p->Interface_p->name_p, "HTMLTemplateElement")) {
                        return NH_API_SUCCESS;
                    }
                }

                // TODO

                break;
            }
            case NH_HTML_TAG_BASE     :
            case NH_HTML_TAG_BASEFONT :
            case NH_HTML_TAG_BGSOUND  :
            case NH_HTML_TAG_LINK     :
            case NH_HTML_TAG_META     :
            case NH_HTML_TAG_NOFRAMES :
            case NH_HTML_TAG_SCRIPT   :
            case NH_HTML_TAG_STYLE    :
            case NH_HTML_TAG_TEMPLATE :
            case NH_HTML_TAG_TITLE    :
            {
                NH_CORE_CHECK(nh_html_processInHead(Parser_p)) 
                break;
            }
            case NH_HTML_TAG_BODY :
            {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                if (Parser_p->OpenElements.size > 1 && !strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Parser_p->OpenElements.pp[1]))->p, "body") && !nh_html_hasOpenElement(Parser_p, "template")) {
                    Parser_p->framesetOk = false;
                    // TODO
                }
                break;
            }
            case NH_HTML_TAG_FRAMESET :
            {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                if (Parser_p->OpenElements.size > 1 && !strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Parser_p->OpenElements.pp[1]))->p, "body") && Parser_p->framesetOk) {
                    // TODO
                }
                break;
            }
            case NH_HTML_TAG_ADDRESS    :
            case NH_HTML_TAG_ARTICLE    :
            case NH_HTML_TAG_ASIDE      :
            case NH_HTML_TAG_BLOCKQUOTE :
            case NH_HTML_TAG_CENTER     :
            case NH_HTML_TAG_DETAILS    :
            case NH_HTML_TAG_DIALOG     :
            case NH_HTML_TAG_DIR        :
            case NH_HTML_TAG_DIV        :
            case NH_HTML_TAG_DL         :
            case NH_HTML_TAG_FIELDSET   :
            case NH_HTML_TAG_FIGCAPTION :
            case NH_HTML_TAG_FIGURE     :
            case NH_HTML_TAG_FOOTER     :
            case NH_HTML_TAG_HEADER     :
            case NH_HTML_TAG_HGROUP     :
            case NH_HTML_TAG_MAIN       :
            case NH_HTML_TAG_MENU       :
            case NH_HTML_TAG_NAV        :
            case NH_HTML_TAG_OL         :
            case NH_HTML_TAG_P          :
            case NH_HTML_TAG_SECTION    :
            case NH_HTML_TAG_SUMMARY    :
            case NH_HTML_TAG_UL         :
            {
                if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                }
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_H1 :
            case NH_HTML_TAG_H2 :
            case NH_HTML_TAG_H3 :
            case NH_HTML_TAG_H4 :
            case NH_HTML_TAG_H5 :
            case NH_HTML_TAG_H6 :
            {
                if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                }

                nh_webidl_Object *Current_p = nh_html_getCurrentNode(Parser_p);
                nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Current_p));

                if (!strcmp(LocalName_p->p, "h1")
                ||  !strcmp(LocalName_p->p, "h2")
                ||  !strcmp(LocalName_p->p, "h3")
                ||  !strcmp(LocalName_p->p, "h4")
                ||  !strcmp(LocalName_p->p, "h5")
                ||  !strcmp(LocalName_p->p, "h6")) 
                {
                    // parse error
                    NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                }

                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_PRE     :
            case NH_HTML_TAG_LISTING :
            {
                if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                }
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                // TODO If the next token is a U+000A LINE FEED (LF) character token, then ignore that token and move on to the next one. (Newlines at the start of pre blocks are ignored as an authoring convenience.)
                Parser_p->framesetOk = false;
                break;
            }
            case NH_HTML_TAG_FORM :
            {
                if (Parser_p->FormElement_p && !nh_html_hasOpenElement(Parser_p, "template")) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                }
                else {
                    if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                        NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                    }
                    nh_webidl_Object *Form_p = nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p);
                    if (!nh_html_hasOpenElement(Parser_p, "template")) {
                        Parser_p->FormElement_p = Form_p;
                    }
                }
                break;
            }
            case NH_HTML_TAG_LI :
            {
                Parser_p->framesetOk = false;
                nh_webidl_Object *Node_p = nh_html_getCurrentNode(Parser_p);

                while (1)
                {
                    if (!strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Node_p))->p, "li")) {
                        NH_CORE_CHECK(nh_html_closeElement(Parser_p, "li"))
                        break;
                    }
                    nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Node_p));
                    if (nh_html_inSpecialCategory(Node_p) && strcmp(LocalName_p->p, "address") && strcmp(LocalName_p->p, "div") && strcmp(LocalName_p->p, "p")) {
                        break;
                    }
                    else {
                        Node_p = Parser_p->OpenElements.pp[Parser_p->OpenElements.size - 2];
                    }
                }

                if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                }
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_DD :
            case NH_HTML_TAG_DT :
            {
                Parser_p->framesetOk = false;
                nh_webidl_Object *Node_p = nh_html_getCurrentNode(Parser_p);

                while (1)
                {
                    if (!strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Node_p))->p, "dd")) {
                        NH_CORE_CHECK(nh_html_closeElement(Parser_p, "dd"))
                        break;
                    }
                    else if (!strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Node_p))->p, "dt")) {
                        NH_CORE_CHECK(nh_html_closeElement(Parser_p, "dt"))
                        break;
                    }
                    nh_webidl_DOMString *LocalName_p = nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Node_p));
                    if (nh_html_inSpecialCategory(Node_p) && strcmp(LocalName_p->p, "address") && strcmp(LocalName_p->p, "div") && strcmp(LocalName_p->p, "p")) {
                        break;
                    }
                    else {
                        Node_p = Parser_p->OpenElements.pp[Parser_p->OpenElements.size - 2];
                    }
                }

                if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                }
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_PLAINTEXT :
            {
                if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                }
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                Parser_p->Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_PLAINTEXT;
                break;
            }
            case NH_HTML_TAG_BUTTON :
            {
                if (nh_html_hasElementInScope(Parser_p, "button")) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                    NH_CORE_CHECK(nh_html_generateImpliedEndTags(Parser_p, NULL))
                    nh_webidl_Object *Pop_p = nh_html_popCurrentNode(Parser_p);
                    while (Pop_p != NULL && strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Pop_p))->p, "button")) {
                        Pop_p = nh_html_popCurrentNode(Parser_p);
                    }
                }
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                Parser_p->framesetOk = false;
                break;
            }
            case NH_HTML_TAG_A :
            {
                // TODO
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                nh_webidl_Object *Element_p = nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p);
                NH_CORE_CHECK_NULL(Element_p)
                NH_CORE_CHECK(nh_html_pushActiveFormattingElement(Parser_p, Element_p))
                break;
            }
            case NH_HTML_TAG_B      :
            case NH_HTML_TAG_BIG    :
            case NH_HTML_TAG_CODE   :
            case NH_HTML_TAG_EM     :
            case NH_HTML_TAG_FONT   :
            case NH_HTML_TAG_I      :
            case NH_HTML_TAG_S      :
            case NH_HTML_TAG_SMALL  :
            case NH_HTML_TAG_STRIKE :
            case NH_HTML_TAG_STRONG :
            case NH_HTML_TAG_TT     :
            case NH_HTML_TAG_U      :
            {
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                nh_webidl_Object *Element_p = nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p);
                NH_CORE_CHECK_NULL(Element_p)
                NH_CORE_CHECK(nh_html_pushActiveFormattingElement(Parser_p, Element_p))
                break;
            }
            case NH_HTML_TAG_NOBR :
            {
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                if (nh_html_hasElementInScope(Parser_p, "nobr")) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                    // TODO
                }
                nh_webidl_Object *Element_p = nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p);
                NH_CORE_CHECK_NULL(Element_p)
                NH_CORE_CHECK(nh_html_pushActiveFormattingElement(Parser_p, Element_p))
                break;
            }
            case NH_HTML_TAG_APPLET  :
            case NH_HTML_TAG_MARQUEE :
            case NH_HTML_TAG_OBJECT  :
            {
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_CORE_CHECK(nh_html_insertMarker(Parser_p))
                Parser_p->framesetOk = false;
                break;
            }
            case NH_HTML_TAG_TABLE : 
            {
                // TODO
                break;
            }
            case NH_HTML_TAG_AREA   :
            case NH_HTML_TAG_BR     :
            case NH_HTML_TAG_EMBED  :
            case NH_HTML_TAG_IMG    :
            case NH_HTML_TAG_KEYGEN :
            case NH_HTML_TAG_WBR    :
            {
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))

                nh_webidl_Object *Image_p = nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p);
                NH_CORE_CHECK_NULL(Image_p)

                NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                // TODO Acknowledge the token's self-closing flag, if it is set.
                Parser_p->framesetOk = false;

                // By default, images are obtained immediately.
                if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_IMG) {
                    nh_html_updateImageData(NH_WEBIDL_GET_HTML_IMAGE_ELEMENT(Image_p), false);
                }

                break;
            }
            case NH_HTML_TAG_INPUT :
            {
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                // TODO Acknowledge the token's self-closing flag, if it is set.
                // TODO If the token does not have an attribute with the name "type", or if it does, but that attribute's value is not an ASCII case-insensitive match for the string "hidden", then: set the frameset-ok flag to "not ok".
                Parser_p->framesetOk = false;
                break;
            }
            case NH_HTML_TAG_PARAM  :
            case NH_HTML_TAG_SOURCE :
            case NH_HTML_TAG_TRACK  :
            {
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                // TODO Acknowledge the token's self-closing flag, if it is set.
                break;
            }
            case NH_HTML_TAG_HR :
            {
                if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                }
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                // TODO Acknowledge the token's self-closing flag, if it is set.
                Parser_p->framesetOk = false;
                break;
            }
            case NH_HTML_TAG_TEXTAREA :
            {
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                // TODO If the next token is a U+000A LINE FEED (LF) character token, then ignore that token and move on to the next one. (Newlines at the start of textarea elements are ignored as an authoring convenience.)
                Parser_p->Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RCDATA;
                Parser_p->originalInsertionMode = Parser_p->insertionMode;
                Parser_p->framesetOk = false;
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_TEXT;
                break;
            }
            case NH_HTML_TAG_XMP :
            {
                if (nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                }
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                Parser_p->framesetOk = false;
                NH_CORE_CHECK(nh_html_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, true))
                break;
            }
            case NH_HTML_TAG_IFRAME :
            {
                Parser_p->framesetOk = false;
                NH_CORE_CHECK(nh_html_parseRAWTEXTOrRCDATA(Parser_p, Parser_p->Token_p, true))
                break;
            }
//        else 
//        if (!strcmp(Parser_p->Token_p->StartOrEndTag.TagName.p, "noembed")
//        || (!strcmp(Parser_p->Token_p->StartOrEndTag.TagName.p, "noscript") && Parser_p->scripting))
//        {
//            // TODO
//            return NH_API_SUCCESS; 
//        }
            case NH_HTML_TAG_SELECT :
            {
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                Parser_p->framesetOk = false;

                if (Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_TABLE
                ||  Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_CAPTION
                ||  Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_TABLE_BODY
                ||  Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_ROW
                ||  Parser_p->insertionMode == NH_HTML_INSERTION_MODE_IN_CELL) {
                    Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_SELECT_IN_TABLE;
                }
                else {Parser_p->insertionMode = NH_HTML_INSERTION_MODE_IN_SELECT;}

                break;
            }
            case NH_HTML_TAG_OPTGROUP :
            case NH_HTML_TAG_OPTION   :
            {
                if (!strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(nh_html_getCurrentNode(Parser_p)))->p, "option")) {
                    NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                }
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_RB  :
            case NH_HTML_TAG_RTC :
            {
                if (nh_html_hasElementInScope(Parser_p, "ruby")) {
                    NH_CORE_CHECK(nh_html_generateImpliedEndTags(Parser_p, NULL))
                    if (strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(nh_html_getCurrentNode(Parser_p)))->p, "ruby")) {
                        // parse error
                    }
                }
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
                break;
            }
            case NH_HTML_TAG_RP :
            case NH_HTML_TAG_RT :
            {
                // TODO
                return NH_API_SUCCESS; 
            }
            case NH_HTML_TAG_MATH :
            {
                // TODO
                return NH_API_SUCCESS;
            }
            case NH_HTML_TAG_SVG :
            {
                // TODO
                return NH_API_SUCCESS;
            }
            case NH_HTML_TAG_CAPTION  :
            case NH_HTML_TAG_COL      :
            case NH_HTML_TAG_COLGROUP :
            case NH_HTML_TAG_FRAME    :
            case NH_HTML_TAG_HEAD     :
            case NH_HTML_TAG_TBODY    :
            case NH_HTML_TAG_TD       :
            case NH_HTML_TAG_TFOOT    :
            case NH_HTML_TAG_TH       :
            case NH_HTML_TAG_THEAD    :
            case NH_HTML_TAG_TR       :
            {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                break;
            }
            default :
            {
                // catch image
                if (!strcmp(Parser_p->Token_p->StartOrEndTag.TagName.p, "image")) 
                {
                    // parse error
                    nh_webidl_freeDOMString(&Parser_p->Token_p->StartOrEndTag.TagName);
                    Parser_p->Token_p->StartOrEndTag.TagName = nh_webidl_initDOMString(3);
                    nh_webidl_appendToDOMString(&Parser_p->Token_p->StartOrEndTag.TagName, "img", 3);
                    Parser_p->Token_p->StartOrEndTag.tag = NH_HTML_TAG_IMG;
                    return nh_html_reprocessToken(Parser_p, Parser_p->insertionMode);
                }
        
                NH_CORE_CHECK(nh_html_reconstructActiveFormattingElements(Parser_p))
                NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, Parser_p->Token_p))
            }
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG)
    {
        switch (Parser_p->Token_p->StartOrEndTag.tag)
        {
            case NH_HTML_TAG_TEMPLATE :
            {
                NH_CORE_CHECK(nh_html_processInHead(Parser_p))
                break;
            }
            case NH_HTML_TAG_BODY :
            {
                if (!nh_html_hasElementInScope(Parser_p, "body")) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                if (nh_html_hasOtherOpenElements(Parser_p)) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                }
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_AFTER_BODY;
                break;
            }
            case NH_HTML_TAG_HTML :
            {
                if (!nh_html_hasElementInScope(Parser_p, "body")) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                if (nh_html_hasOtherOpenElements(Parser_p)) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                }
                NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_AFTER_BODY))
                break;
            }
            case NH_HTML_TAG_ADDRESS    :
            case NH_HTML_TAG_ARTICLE    :
            case NH_HTML_TAG_ASIDE      :
            case NH_HTML_TAG_BLOCKQUOTE :
            case NH_HTML_TAG_BUTTON     :
            case NH_HTML_TAG_CENTER     :
            case NH_HTML_TAG_DETAILS    :
            case NH_HTML_TAG_DIALOG     :
            case NH_HTML_TAG_DIR        :
            case NH_HTML_TAG_DIV        :
            case NH_HTML_TAG_DL         :
            case NH_HTML_TAG_FIELDSET   :
            case NH_HTML_TAG_FIGCAPTION :
            case NH_HTML_TAG_FIGURE     :
            case NH_HTML_TAG_FOOTER     :
            case NH_HTML_TAG_HEADER     :
            case NH_HTML_TAG_HGROUP     :
            case NH_HTML_TAG_LISTING    :
            case NH_HTML_TAG_MAIN       :
            case NH_HTML_TAG_MENU       :
            case NH_HTML_TAG_NAV        :
            case NH_HTML_TAG_OL         :
            case NH_HTML_TAG_PRE        :
            case NH_HTML_TAG_SECTION    :
            case NH_HTML_TAG_SUMMARY    :
            case NH_HTML_TAG_UL         :
            {
                if (!nh_html_hasElementInScope(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.p)) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }

                NH_CORE_CHECK(nh_html_generateImpliedEndTags(Parser_p, NULL))

                if (strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(nh_html_getCurrentNode(Parser_p)))->p, Parser_p->Token_p->StartOrEndTag.TagName.p)) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                }

                nh_webidl_Object *Pop_p = nh_html_popCurrentNode(Parser_p);
                while (Pop_p != NULL && strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Pop_p))->p, Parser_p->Token_p->StartOrEndTag.TagName.p)) {
                    Pop_p = nh_html_popCurrentNode(Parser_p);
                }

                break; 
            }
            case NH_HTML_TAG_FORM :
            {
                // TODO
                break; 
            }
            case NH_HTML_TAG_P :
            {
                if (!nh_html_hasElementInButtonScope(Parser_p, "p")) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                    NH_CORE_CHECK_NULL(nh_html_insertHTMLElement(Parser_p, nh_html_getEmptyStartTagToken(NH_HTML_TAG_P)))
                }
                NH_CORE_CHECK(nh_html_closePElement(Parser_p))
                break;
            }
            case NH_HTML_TAG_LI :
            {
                if (!nh_html_hasElementInListItemScope(Parser_p, "li")) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                NH_CORE_CHECK(nh_html_closeElement(Parser_p, "li"))
                break;
            }
            case NH_HTML_TAG_DD : 
            case NH_HTML_TAG_DT : 
            {
                if (!nh_html_hasElementInScope(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.p)) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                NH_CORE_CHECK(nh_html_closeElement(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.p))
                break;
            }
            case NH_HTML_TAG_H1 :
            case NH_HTML_TAG_H2 :
            case NH_HTML_TAG_H3 :
            case NH_HTML_TAG_H4 :
            case NH_HTML_TAG_H5 :
            case NH_HTML_TAG_H6 :
            {
                if (!nh_html_hasElementInScope(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.p)) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
                    break;
                }
                NH_CORE_CHECK(nh_html_generateImpliedEndTags(Parser_p, NULL))
                if (strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(nh_html_getCurrentNode(Parser_p)))->p, Parser_p->Token_p->StartOrEndTag.TagName.p)) {
                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                }
                nh_webidl_Object *Pop_p = nh_html_popCurrentNode(Parser_p);
                while (Pop_p != NULL && strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Pop_p))->p, Parser_p->Token_p->StartOrEndTag.TagName.p)) {
                    Pop_p = nh_html_popCurrentNode(Parser_p);
                }
                break;
            }
            case NH_HTML_TAG_A      :
            case NH_HTML_TAG_B      :
            case NH_HTML_TAG_BIG    :
            case NH_HTML_TAG_CODE   :
            case NH_HTML_TAG_EM     :
            case NH_HTML_TAG_FONT   :
            case NH_HTML_TAG_I      :
            case NH_HTML_TAG_NOBR   :
            case NH_HTML_TAG_S      :
            case NH_HTML_TAG_SMALL  :
            case NH_HTML_TAG_STRIKE :
            case NH_HTML_TAG_STRONG :
            case NH_HTML_TAG_TT     :
            case NH_HTML_TAG_U      :
            {
                nh_html_runAdoptionAgencyAlgorithm(Parser_p, Parser_p->Token_p);
                break;
            }
            case NH_HTML_TAG_APPLET  :
            case NH_HTML_TAG_MARQUEE :
            case NH_HTML_TAG_OBJECT  :
            {
                // TODO
                break;
            }
            case NH_HTML_TAG_BR :
            {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
                Parser_p->Token_p->type = NH_HTML_TOKEN_START_TAG;
                NH_CORE_CHECK(nh_html_processInBody(Parser_p))
                break;
            }
            default :
            {
//                nh_webidl_Object *Node_p = nh_html_getCurrentNode(Parser_p); 
//
//                if (!strcmp(nh_dom_getLocalName(NH_WEBIDL_GET_DOM_ELEMENT(Node_p))->p, Parser_p->Token_p->StartOrEndTag.TagName.p)) i
//                {
//                    NH_CORE_CHECK(nh_html_generateImpliedEndTags(Parser_p, Parser_p->Token_p->StartOrEndTag.TagName.p))
//
//                    if (Node_p != nh_html_getCurrentNode(Parser_p)) {
//                        NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
//                    }
//
//                    while (nh_html_popCurrentNode(Parser_p) != Node_p) {}
//                }
//
//                if (nh_html_inSpecialCategory(Node_p)) {
//                    NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
//                    break;
//                }
//                else {
//                    Node_p = Parser_p->OpenElements.pp[Parser_p->OpenElements.size - 2];
//                }
            }
        }
    }
    else if (Parser_p->Token_p->type == NH_HTML_TOKEN_END_OF_FILE)
    {
        if (Parser_p->TemplateInsertionModes.size > 0) {
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_TEMPLATE))
        }
        else {
            if (nh_html_hasOtherOpenElements(Parser_p)) {
                NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            }
            Parser_p->stop = true;
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processText(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            NH_CORE_CHECK(nh_html_insertCharacter(Parser_p, NULL, NULL))
            break;
        }
        case NH_HTML_TOKEN_END_OF_FILE :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, Parser_p->originalInsertionMode))
            break;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_SCRIPT)
            {
                // TODO
            }
            else {
                NH_CORE_CHECK_NULL(nh_html_popCurrentNode(Parser_p))
                Parser_p->insertionMode = Parser_p->originalInsertionMode;
            }
            break;
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processAfterBody(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
            { 
                NH_CORE_CHECK(nh_html_processInBody(Parser_p))
                break;
            }
            goto AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_CORE_CHECK(nh_html_insertComment(Parser_p, Parser_p->OpenElements.pp[0]))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_IGNORED_TOKEN))
            break;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) {
                NH_CORE_CHECK(nh_html_processInBody(Parser_p))
                break;
            }
            goto AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_END_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) {
                // TODO If the parser was created as part of the HTML fragment parsing algorithm, this is a parse error; ignore the token. (fragment case)
                Parser_p->insertionMode = NH_HTML_INSERTION_MODE_AFTER_AFTER_BODY;
                break;
            }
            goto AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_END_OF_FILE :
        {
            Parser_p->stop = true;
            break;
        }
        default : AFTER_BODY_DEFAULT :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_BODY))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processAfterAfterBody(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->Token_p->type)
    {
        case NH_HTML_TOKEN_COMMENT :
        {
            NH_CORE_CHECK(nh_html_insertComment(Parser_p, Parser_p->Document_p))
            break;
        }
        case NH_HTML_TOKEN_DOCTYPE :
        {
            NH_CORE_CHECK(nh_html_processInBody(Parser_p))
            break;
        }
        case NH_HTML_TOKEN_CHARACTER :
        {
            if (Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x09
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0A
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0C
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x0D
            ||  Parser_p->Token_p->CommentOrCharacter.Data.p[0] == 0x20)
            { 
                NH_CORE_CHECK(nh_html_processInBody(Parser_p))
                break;
            }
            goto AFTER_AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_START_TAG :
        {
            if (Parser_p->Token_p->StartOrEndTag.tag == NH_HTML_TAG_HTML) {
                NH_CORE_CHECK(nh_html_processInBody(Parser_p))
                break;
            }
            goto AFTER_AFTER_BODY_DEFAULT;
        }
        case NH_HTML_TOKEN_END_OF_FILE :
        {
            Parser_p->stop = true;
            break;
        }
        default : AFTER_AFTER_BODY_DEFAULT :
        {
            NH_CORE_CHECK(nh_html_newTreeConstructionError(Parser_p, NH_HTML_PARSE_ERROR_PLACEHOLDER))
            NH_CORE_CHECK(nh_html_reprocessToken(Parser_p, NH_HTML_INSERTION_MODE_IN_BODY))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_processToken(
    nh_html_Parser *Parser_p)
{
    switch (Parser_p->insertionMode)
    {
        case NH_HTML_INSERTION_MODE_INITIAL          : return nh_html_processInitial(Parser_p);
        case NH_HTML_INSERTION_MODE_BEFORE_HTML      : return nh_html_processBeforeHTML(Parser_p);
        case NH_HTML_INSERTION_MODE_BEFORE_HEAD      : return nh_html_processBeforeHead(Parser_p);
        case NH_HTML_INSERTION_MODE_IN_HEAD          : return nh_html_processInHead(Parser_p);
        case NH_HTML_INSERTION_MODE_IN_HEAD_NOSCRIPT : return nh_html_processInHeadNoscript(Parser_p); 
        case NH_HTML_INSERTION_MODE_AFTER_HEAD       : return nh_html_processAfterHead(Parser_p);
        case NH_HTML_INSERTION_MODE_IN_BODY          : return nh_html_processInBody(Parser_p);
        case NH_HTML_INSERTION_MODE_TEXT             : return nh_html_processText(Parser_p);
        case NH_HTML_INSERTION_MODE_AFTER_BODY       : return nh_html_processAfterBody(Parser_p);
        case NH_HTML_INSERTION_MODE_AFTER_AFTER_BODY : return nh_html_processAfterAfterBody(Parser_p);
    }

    return NH_API_ERROR_BAD_STATE;
}

static NH_API_RESULT nh_html_reprocessToken(
    nh_html_Parser *Parser_p, NH_HTML_INSERTION_MODE insertionMode)
{
    Parser_p->insertionMode = insertionMode;
    NH_CORE_CHECK(nh_html_processToken(Parser_p))

    return NH_API_SUCCESS;
}

// https://html.spec.whatwg.org/multipage/parsing.html#tree-construction-dispatcher
NH_API_RESULT nh_html_dispatchTreeConstruction(
    nh_html_Parser *Parser_p)
{
    nh_webidl_Object *AdjustedCurrentNode_p = nh_html_getAdjustedCurrentNode(Parser_p);

    if (Parser_p->Token_p->type == NH_HTML_TOKEN_START_TAG || Parser_p->Token_p->type == NH_HTML_TOKEN_END_TAG) {
        Parser_p->Token_p->StartOrEndTag.tag = nh_html_getTagIndex(Parser_p->Token_p->StartOrEndTag.TagName.p);
    }

    if (Parser_p->OpenElements.size == 0
    ||  nh_dom_getNamespaceURI(NH_WEBIDL_GET_DOM_ELEMENT(AdjustedCurrentNode_p)) == &NH_WEBIDL_HTML_NAMESPACE
    ||  Parser_p->Token_p->type == NH_HTML_TOKEN_END_OF_FILE) {
        NH_CORE_CHECK(nh_html_processToken(Parser_p))
    }
    else {
        exit(0);
    }

    return NH_API_SUCCESS;
}

