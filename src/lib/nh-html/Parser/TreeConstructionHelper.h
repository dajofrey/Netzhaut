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

    nh_ecmascript_Object *nh_html_createElementForToken(
        nh_html_Token *Token_p, nh_encoding_UTF8String *Namespace_p, nh_ecmascript_Object *IntendedParent_p
    );
    
    NH_WEBIDL_UNSIGNED_LONG nh_html_getAppropriatePlaceForInsertingNode(
        nh_html_Parser *Parser_p, nh_ecmascript_Object **Target_pp
    );
    
    NH_API_RESULT nh_html_insertCommentAtPosition(
        nh_html_Parser *Parser_p, nh_ecmascript_Object *Target_p, NH_WEBIDL_UNSIGNED_LONG position
    );
    
    NH_API_RESULT nh_html_insertComment(
        nh_html_Parser *Parser_p, nh_ecmascript_Object *Node_p
    );
    
    nh_ecmascript_Object *nh_html_insertForeignElement(
        nh_html_Parser *Parser_p, nh_html_Token *Token_p, nh_encoding_UTF8String *Namespace_p
    );
    
    nh_ecmascript_Object *nh_html_insertHTMLElement(
        nh_html_Parser *Parser_p, nh_html_Token *Token_p
    );
    
    NH_API_RESULT nh_html_insertCharacter(
        nh_html_Parser *Parser_p, nh_ecmascript_Object *Target_p, nh_encoding_UTF8String *Data_p
    );
    
    NH_API_RESULT nh_html_parseRAWTEXTOrRCDATA(
        nh_html_Parser *Parser_p, nh_html_Token *Token_p, bool RAWTEXT
    );
    
    nh_html_Token *nh_html_getEmptyStartTagToken(
        NH_HTML_TAG tag
    );

// ACTIVE FORMATTING ELEMENTS

    NH_API_RESULT nh_html_pushActiveFormattingElement(
        nh_html_Parser *Parser_p, nh_ecmascript_Object *Element_p
    );

    NH_API_RESULT nh_html_insertMarker(
        nh_html_Parser *Parser_p
    );

    NH_API_RESULT nh_html_reconstructActiveFormattingElements(
        nh_html_Parser *Parser_p
    );

// IMPLIED END TAGS

    NH_API_RESULT nh_html_generateImpliedEndTags(
        nh_html_Parser *Parser_p, char *exclude_p
    );
    
    NH_API_RESULT nh_html_generateAllImpliedEndTags(
        nh_html_Parser *Parser_p
    );

// STACK OF OPEN ELEMENTS

    nh_ecmascript_Object *nh_html_getCurrentNode(
        nh_html_Parser *Parser_p
    );

    nh_ecmascript_Object *nh_html_getAdjustedCurrentNode(
        nh_html_Parser *Parser_p
    );

    NH_API_RESULT nh_html_pushOpenElement(
        nh_html_Parser *Parser_p, nh_ecmascript_Object *Object_p
    );

    nh_ecmascript_Object *nh_html_popCurrentNode(
        nh_html_Parser *Parser_p
    );

    bool nh_html_inSpecialCategory(
        nh_ecmascript_Object *Node_p
    );

    bool nh_html_hasElementInScope(
        nh_html_Parser *Parser_p, char *target_p
    );
    
    bool nh_html_hasElementInListItemScope(
        nh_html_Parser *Parser_p, char *target_p
    );
    
    bool nh_html_hasElementInButtonScope(
        nh_html_Parser *Parser_p, char *target_p
    );
    
    bool nh_html_hasElementInTableScope(
        nh_html_Parser *Parser_p, char *target_p
    );
    
    bool nh_html_hasElementInSelectScope(
        nh_html_Parser *Parser_p, char *target_p
    );

/** @} */

#endif
