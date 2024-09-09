#ifndef NH_HTML_TREE_CONSTRUCTION_HELPER_H
#define NH_HTML_TREE_CONSTRUCTION_HELPER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Elements.h"
#include "Parser.h"

#endif

/** @addtogroup lib_nh-html_functions
 *  @{
 */

    nh_webidl_Object *nh_html_createElementForToken(
        nh_html_Token *Token_p, nh_webidl_DOMString *Namespace_p, nh_webidl_Object *IntendedParent_p
    );
    
    NH_WEBIDL_UNSIGNED_LONG nh_html_getAppropriatePlaceForInsertingNode(
        nh_html_Parser *Parser_p, nh_webidl_Object **Target_pp
    );
    
    NH_HTML_RESULT nh_html_insertCommentAtPosition(
        nh_html_Parser *Parser_p, nh_webidl_Object *Target_p, NH_WEBIDL_UNSIGNED_LONG position
    );
    
    NH_HTML_RESULT nh_html_insertComment(
        nh_html_Parser *Parser_p, nh_webidl_Object *Node_p
    );
    
    nh_webidl_Object *nh_html_insertForeignElement(
        nh_html_Parser *Parser_p, nh_html_Token *Token_p, nh_webidl_DOMString *Namespace_p
    );
    
    nh_webidl_Object *nh_html_insertHTMLElement(
        nh_html_Parser *Parser_p, nh_html_Token *Token_p
    );
    
    NH_HTML_RESULT nh_html_insertCharacter(
        nh_html_Parser *Parser_p, nh_webidl_Object *Target_p, nh_webidl_DOMString *Data_p
    );
    
    NH_HTML_RESULT nh_html_parseRAWTEXTOrRCDATA(
        nh_html_Parser *Parser_p, nh_html_Token *Token_p, NH_BOOL RAWTEXT
    );
    
    nh_html_Token *nh_html_getEmptyStartTagToken(
        NH_HTML_TAG tag
    );

// ACTIVE FORMATTING ELEMENTS

    NH_HTML_RESULT nh_html_pushActiveFormattingElement(
        nh_html_Parser *Parser_p, nh_webidl_Object *Element_p
    );

    NH_HTML_RESULT nh_html_insertMarker(
        nh_html_Parser *Parser_p
    );

    NH_HTML_RESULT nh_html_reconstructActiveFormattingElements(
        nh_html_Parser *Parser_p
    );

// IMPLIED END TAGS

    NH_HTML_RESULT nh_html_generateImpliedEndTags(
        nh_html_Parser *Parser_p, NH_BYTE *exclude_p
    );
    
    NH_HTML_RESULT nh_html_generateAllImpliedEndTags(
        nh_html_Parser *Parser_p
    );

// STACK OF OPEN ELEMENTS

    nh_webidl_Object *nh_html_getCurrentNode(
        nh_html_Parser *Parser_p
    );

    nh_webidl_Object *nh_html_getAdjustedCurrentNode(
        nh_html_Parser *Parser_p
    );

    NH_HTML_RESULT nh_html_pushOpenElement(
        nh_html_Parser *Parser_p, nh_webidl_Object *Object_p
    );

    nh_webidl_Object *nh_html_popCurrentNode(
        nh_html_Parser *Parser_p
    );

    NH_BOOL nh_html_inSpecialCategory(
        nh_webidl_Object *Node_p
    );

    NH_BOOL nh_html_hasElementInScope(
        nh_html_Parser *Parser_p, NH_BYTE *target_p
    );
    
    NH_BOOL nh_html_hasElementInListItemScope(
        nh_html_Parser *Parser_p, NH_BYTE *target_p
    );
    
    NH_BOOL nh_html_hasElementInButtonScope(
        nh_html_Parser *Parser_p, NH_BYTE *target_p
    );
    
    NH_BOOL nh_html_hasElementInTableScope(
        nh_html_Parser *Parser_p, NH_BYTE *target_p
    );
    
    NH_BOOL nh_html_hasElementInSelectScope(
        nh_html_Parser *Parser_p, NH_BYTE *target_p
    );

/** @} */

#endif
