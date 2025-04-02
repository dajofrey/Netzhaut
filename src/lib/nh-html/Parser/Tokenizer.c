// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Tokenizer.h"
#include "Parser.h"
#include "TreeConstruction.h"
#include "NamedCharacterReferences.h"

#include "../../nh-core/Util/ArrayList.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/Array.h"

#include "../../nh-encoding/Encodings/UTF32.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DECLARE =========================================================================================

static NH_API_RESULT nh_html_consume(
    nh_html_Tokenizer *Tokenizer_p
);

static NH_API_RESULT nh_html_reconsume(
    nh_html_Tokenizer *Tokenizer_p, NH_HTML_TOKENIZATION_STATE newState
);

static void nh_html_incrementTokenizer(
    nh_html_Tokenizer *Tokenizer_p
);

static void nh_html_decrementTokenizer(
    nh_html_Tokenizer *Tokenizer_p
);

// INIT ============================================================================================

nh_html_Tokenizer nh_html_initTokenizer(
    void *Parser_p, nh_webidl_USVString InputStream)
{
    nh_html_Tokenizer Tokenizer;
    Tokenizer.state           = NH_HTML_TOKENIZATION_STATE_DATA;
    Tokenizer.returnState     = Tokenizer.state;
    Tokenizer.Tokens          = nh_core_initArrayList(sizeof(nh_html_Token), 1024);
    Tokenizer.Emits           = nh_core_initList(1024);
    Tokenizer.index           = -1;
    Tokenizer.InputStream     = InputStream;
    Tokenizer.codepoint       = 0;
    Tokenizer.Token_p         = NULL;
    Tokenizer.TemporaryBuffer = nh_webidl_initUSVString(64);
    Tokenizer.Parser_p        = Parser_p;

    return Tokenizer;
}

// HELPER ==========================================================================================

static bool nh_html_isAppropriateEndTagToken(
    nh_html_Tokenizer *Tokenizer_p)
{
    for (int i = Tokenizer_p->Emits.size - 1; i >= 0; --i)
    {
        nh_html_Token *Emit_p = Tokenizer_p->Emits.pp[i];
        if (Emit_p->type == NH_HTML_TOKEN_START_TAG) {
            if (!strcmp(Tokenizer_p->Token_p->StartOrEndTag.TagName.p, Emit_p->StartOrEndTag.TagName.p)) {
                return true;
            }
            break;
        }
    } 

    return false;
}

static NH_API_RESULT nh_html_resetTemporaryBuffer(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_webidl_freeUSVString(&Tokenizer_p->TemporaryBuffer);
    Tokenizer_p->TemporaryBuffer = nh_webidl_initUSVString(64);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_newTokenizerError(
    nh_html_Tokenizer *Tokenizer_p, NH_HTML_PARSE_ERROR type)
{
    return nh_html_newParseError(Tokenizer_p->Parser_p, Tokenizer_p->index, type);
}

static nh_html_Attribute *nh_html_newAttribute(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_html_Attribute *Attribute_p = nh_core_incrementArray(&Tokenizer_p->Token_p->StartOrEndTag.Attributes);
    NH_CORE_CHECK_MEM_2(NULL, Attribute_p)

    Attribute_p->Name = nh_webidl_initDOMString(16);
    Attribute_p->Value = nh_webidl_initDOMString(64);

    return Attribute_p;
}

static nh_html_Attribute *nh_html_getCurrentAttribute(
    nh_html_Tokenizer *Tokenizer_p)
{
    return &((nh_html_Attribute*)Tokenizer_p->Token_p->StartOrEndTag.Attributes.p)[Tokenizer_p->Token_p->StartOrEndTag.Attributes.length - 1];
}

static NH_API_RESULT nh_html_emit(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_core_appendToList(&Tokenizer_p->Emits, Tokenizer_p->Token_p);
    ((nh_html_Parser*)Tokenizer_p->Parser_p)->Token_p = Tokenizer_p->Token_p;

    return nh_html_dispatchTreeConstruction(Tokenizer_p->Parser_p);
}

static nh_html_Token *nh_html_newToken(
    nh_html_Tokenizer *Tokenizer_p, NH_HTML_TOKEN type)
{
    nh_html_Token *Token_p = nh_core_incrementArrayList(&Tokenizer_p->Tokens);
    NH_CORE_CHECK_MEM_2(NULL, Token_p)

    Token_p->type = type;

    switch (type)
    {
        case NH_HTML_TOKEN_DOCTYPE :
            Token_p->DOCTYPE.Name_p = NULL;
            Token_p->DOCTYPE.PublicIdentifier_p = NULL;
            Token_p->DOCTYPE.SystemIdentifier_p = NULL;
            break;

        case NH_HTML_TOKEN_START_TAG :
        case NH_HTML_TOKEN_END_TAG :
            Token_p->StartOrEndTag.TagName = nh_webidl_initDOMString(8);
            Token_p->StartOrEndTag.Attributes = nh_core_initArray(sizeof(nh_html_Attribute), 8);
            break;

        case NH_HTML_TOKEN_COMMENT :
            Token_p->CommentOrCharacter.Data = nh_webidl_initDOMString(64);
            break;

        case NH_HTML_TOKEN_CHARACTER :
            Token_p->CommentOrCharacter.Data = nh_webidl_initDOMString(2);
            break;
    }

    Tokenizer_p->Token_p = Token_p;

    return Token_p;
}

static NH_API_RESULT nh_html_newDOCTYPEToken(
    nh_html_Tokenizer *Tokenizer_p, bool forceQuirks)
{
    nh_html_Token *Token_p = nh_html_newToken(Tokenizer_p, NH_HTML_TOKEN_DOCTYPE);
    NH_CORE_CHECK_MEM(Token_p)
    Token_p->DOCTYPE.forceQuirks = forceQuirks;

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_newCommentToken(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_html_Token *Token_p = nh_html_newToken(Tokenizer_p, NH_HTML_TOKEN_COMMENT);
    NH_CORE_CHECK_MEM(Token_p)

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_newStartTagToken(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_html_Token *Token_p = nh_html_newToken(Tokenizer_p, NH_HTML_TOKEN_START_TAG);
    NH_CORE_CHECK_MEM(Token_p)

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_newEndTagToken(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_html_Token *Token_p = nh_html_newToken(Tokenizer_p, NH_HTML_TOKEN_END_TAG);
    NH_CORE_CHECK_MEM(Token_p)

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_emitCharacterToken(
    nh_html_Tokenizer *Tokenizer_p, NH_ENCODING_UTF32 codepoint)
{
    nh_html_Token *Token_p = nh_html_newToken(Tokenizer_p, NH_HTML_TOKEN_CHARACTER);
    NH_CORE_CHECK_MEM(Token_p)
    nh_webidl_appendUnicodeToDOMString(&Token_p->CommentOrCharacter.Data, &codepoint, 1);
    return nh_html_emit(Tokenizer_p);
}

static NH_API_RESULT nh_html_emitEOFToken(
    nh_html_Tokenizer *Tokenizer_p)
{
    NH_CORE_CHECK_MEM(nh_html_newToken(Tokenizer_p, NH_HTML_TOKEN_END_OF_FILE))

    return nh_html_emit(Tokenizer_p);
}

static NH_API_RESULT nh_html_allocateDOCTYPEName(
    nh_html_Tokenizer *Tokenizer_p)
{
    Tokenizer_p->Token_p->DOCTYPE.Name_p = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_CORE_CHECK_MEM(Tokenizer_p->Token_p->DOCTYPE.Name_p)
    *Tokenizer_p->Token_p->DOCTYPE.Name_p = nh_webidl_initDOMString(16);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_allocateDOCTYPEPublicIdentifier(
    nh_html_Tokenizer *Tokenizer_p)
{
    Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier_p = nh_core_allocate(sizeof(nh_webidl_DOMString));
    NH_CORE_CHECK_MEM(Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier_p)
    *Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier_p = nh_webidl_initDOMString(16);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_allocateDOCTYPESystemIdentifier(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (!Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier_p) 
    {
        Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier_p = nh_core_allocate(sizeof(nh_webidl_DOMString));
        NH_CORE_CHECK_MEM(Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier_p)
        *Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier_p = nh_webidl_initDOMString(16);
    }

    return NH_API_SUCCESS;
}

// CONSUME =========================================================================================

static NH_API_RESULT nh_html_consumeData(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_DATA;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x3c :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_TAG_OPEN;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
        default :
            return nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeRCDATA(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_RCDATA;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x3c :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RCDATA_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            return nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD);
        default :
            return nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeRAWTEXT(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RAWTEXT_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            return nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD);
        default :
            return nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptData(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            return nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD);
        default :
            return nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumePLAINTEXT(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            return nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD);
        default :
            return nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeTagOpen(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIAlpha(Tokenizer_p->codepoint)) {
        NH_CORE_CHECK(nh_html_newStartTagToken(Tokenizer_p))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_TAG_NAME);
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x21 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_MARKUP_DECLARATION_OPEN;
            break;
        case 0x2f :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_END_TAG_OPEN;
            break;
        case 0x3f :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_QUESTION_MARK_INSTEAD_OF_TAG_NAME))
            NH_CORE_CHECK(nh_html_newCommentToken(Tokenizer_p))
            return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT);
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_INVALID_FIRST_CHARACTER_OF_TAG_NAME))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_DATA);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeEndTagOpen(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIAlpha(Tokenizer_p->codepoint)) {
        NH_CORE_CHECK(nh_html_newEndTagToken(Tokenizer_p))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_TAG_NAME);
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_END_TAG_NAME))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_INVALID_FIRST_CHARACTER_OF_TAG_NAME))
            NH_CORE_CHECK(nh_html_newCommentToken(Tokenizer_p))
            return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeTagName(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
            break;
        case 0x2F :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
            break;
        case 0x3E:
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeRCDATALessThanSign(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (Tokenizer_p->codepoint == 0x2F) {
        nh_html_resetTemporaryBuffer(Tokenizer_p);
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_OPEN;
    }
    else {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeRCDATAEndTagOpen(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIAlpha(Tokenizer_p->codepoint)) 
    {
        NH_CORE_CHECK(nh_html_newEndTagToken(Tokenizer_p))
        NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_NAME))
    } 
    else {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeRCDATAEndTagName(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        return NH_API_SUCCESS;
    }

    if (nh_encoding_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
                return NH_API_SUCCESS;
            }
            break;
        case 0x2F :
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
                return NH_API_SUCCESS;
            }
            break;
        case 0x3E:
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
                return nh_html_emit(Tokenizer_p);
            }
            break;
    }

    NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
    NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))

    for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))    
    }

    return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA);
}

static NH_API_RESULT nh_html_consumeRAWTEXTLessThanSign(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (Tokenizer_p->codepoint == 0x2F) {
        nh_html_resetTemporaryBuffer(Tokenizer_p);
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_OPEN;
    } 
    else {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeRAWTEXTEndTagOpen(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIAlpha(Tokenizer_p->codepoint)) {
        NH_CORE_CHECK(nh_html_newEndTagToken(Tokenizer_p))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_NAME);
    } 
    else {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeRAWTEXTEndTagName(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        return NH_API_SUCCESS;
    }

    if (nh_encoding_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
                return NH_API_SUCCESS;
            }
            break;
        case 0x2F :
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
                return NH_API_SUCCESS;
            }
            break;
        case 0x3E:
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
                return nh_html_emit(Tokenizer_p);
            }
            break;
    }

    NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
    NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))

    for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))    
    }

    return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT);
}

static NH_API_RESULT nh_html_consumeScriptDataLessThanSign(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2F :
            nh_html_resetTemporaryBuffer(Tokenizer_p);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_OPEN;
            break;
        case 0x21 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x21))
            break;
        default :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEndTagOpen(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIAlpha(Tokenizer_p->codepoint)) 
    {
        NH_CORE_CHECK(nh_html_newEndTagToken(Tokenizer_p))
        NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_NAME))
    } 
    else {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEndTagName(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        return NH_API_SUCCESS;
    }

    if (nh_encoding_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
                return NH_API_SUCCESS;
            }
            break;
        case 0x2F :
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
                return NH_API_SUCCESS;
            }
            break;
        case 0x3E:
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
                return nh_html_emit(Tokenizer_p);
            }
            break;
    }

    NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
    NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))

    for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))    
    }

    return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA);
}

static NH_API_RESULT nh_html_consumeScriptDataEscapeStart(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (Tokenizer_p->codepoint == 0x2D) {
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START_DASH;
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2D))
    } 
    else {
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEscapeStartDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (Tokenizer_p->codepoint == 0x2D) {
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH_DASH;
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2D))
    } 
    else {
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEscaped(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEscapedDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH_DASH;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEscapedDashDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3E))
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEscapedLessThanSign(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIAlpha(Tokenizer_p->codepoint)) {
        nh_html_resetTemporaryBuffer(Tokenizer_p);
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_START);
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x2F :
            nh_html_resetTemporaryBuffer(Tokenizer_p);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_OPEN;
            break;
        default :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEscapedEndTagOpen(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIAlpha(Tokenizer_p->codepoint)) {
        NH_CORE_CHECK(nh_html_newEndTagToken(Tokenizer_p))
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_NAME);
    }
    else {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
        NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataEscapedEndTagName(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &lower, 1);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        return NH_API_SUCCESS;
    }

    if (nh_encoding_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->StartOrEndTag.TagName, &Tokenizer_p->codepoint, 1);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
                return NH_API_SUCCESS;
            }
            break;
        case 0x2F :
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
                return NH_API_SUCCESS;
            }
            break;
        case 0x3E:
            if (nh_html_isAppropriateEndTagToken(Tokenizer_p)) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
                return nh_html_emit(Tokenizer_p);
            }
            break;
    }

    NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
    NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))

    for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))    
    }

    NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataDoubleEscapeStart(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &lower, 1);
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
        return NH_API_SUCCESS;
    }

    if (nh_encoding_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
        case 0x2F :
        case 0x3E :
        {
            nh_webidl_DOMString TemporaryBuffer = nh_webidl_encodeTextToDOMString(Tokenizer_p->TemporaryBuffer.p, Tokenizer_p->TemporaryBuffer.length);
            if (TemporaryBuffer.length > 0 && !strcmp(TemporaryBuffer.p, "script")) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;
            }
            else {Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;}
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            nh_webidl_freeDOMString(&TemporaryBuffer);
            break;
        }
        default :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataDoubleEscaped(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataDoubleEscapedDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataDoubleEscapedDashDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2D))
            break;
        case 0x3C :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3E))
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0xFFFD))
            break;
        default :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataDoubleEscapedLessThanSign(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2F :
            nh_html_resetTemporaryBuffer(Tokenizer_p);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_END;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            break;
        default :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeScriptDataDoubleEscapeEnd(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &lower, 1);
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
        return NH_API_SUCCESS;
    }

    if (nh_encoding_isASCIILowerAlpha(Tokenizer_p->codepoint)) {
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1);
        NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
        case 0x2F :
        case 0x3E :
        {
            nh_webidl_DOMString TemporaryBuffer = nh_webidl_encodeTextToDOMString(Tokenizer_p->TemporaryBuffer.p, Tokenizer_p->TemporaryBuffer.length);
            if (TemporaryBuffer.length > 0 && !strcmp(TemporaryBuffer.p, "script")) {
                Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED;
            }
            else {Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED;}
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            nh_webidl_freeDOMString(&TemporaryBuffer);
            break;
        }
        default :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeBeforeAttributeName(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore character
        case 0x2F :
        case 0x3E :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME))
            break;
        case 0x3D :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_EQUALS_SIGN_BEFORE_ATTRIBUTE_NAME))
            nh_html_Attribute *Attribute_p = nh_html_newAttribute(Tokenizer_p);
            NH_CORE_CHECK_MEM(Attribute_p)
            nh_webidl_appendUnicodeToDOMString(&Attribute_p->Name, &Tokenizer_p->codepoint, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME;
            break;
        default :
            NH_CORE_CHECK_MEM(nh_html_newAttribute(Tokenizer_p))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME))
            break;
    }

    return NH_API_SUCCESS;
}

// TODO 13.2.5.33 When the user agent leaves the attribute name state (and before emitting the tag token, if appropriate), the complete attribute's name must be compared to the other attributes on the same token; if there is already an attribute on the token with the exact same name, then this is a duplicate-attribute parse error and the new attribute must be removed from the token.
static NH_API_RESULT nh_html_consumeAttributeName(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Name, &lower, 1);
        return NH_API_SUCCESS;
    }
 
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
        case 0x2F :
        case 0x3E :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME))
            break;
        case 0x3D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_VALUE;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Name, &replace, 1);
            break;
        case 0x22 :
        case 0x27 :
        case 0x3C :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_IN_ATTRIBUTE_NAME))
        default :
            nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Name, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAfterAttributeName(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore character
        case 0x2F :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG))
            break;
        case 0x3D :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_VALUE))
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
        default :
            NH_CORE_CHECK_MEM(nh_html_newAttribute(Tokenizer_p))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeBeforeAttributeValue(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore character
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED;
            break;
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_ATTRIBUTE_VALUE))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
        default :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAttributeValueDoubleQuoted(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_VALUE_QUOTED;
            break;
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Value, &replace, 1);
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Value, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAttributeValueSingleQuoted(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_VALUE_QUOTED;
            break;
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Value, &replace, 1);
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Value, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAttributeValueUnquoted(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
            break;
        case 0x26 :
            Tokenizer_p->returnState = NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Value, &replace, 1);
            break;
        case 0x22 :
        case 0x27 :
        case 0x3C :
        case 0x3D :
        case 0x60 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_IN_UNQUOTED_ATTRIBUTE_VALUE))
            // fallthrough
        default :
            nh_webidl_appendUnicodeToDOMString(&nh_html_getCurrentAttribute(Tokenizer_p)->Value, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAfterAttributeValueQuoted(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME;
            break;
        case 0x2F :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_BETWEEN_ATTRIBUTES))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeSelfClosingStartTag(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            Tokenizer_p->Token_p->StartOrEndTag.selfClosing = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_SOLIDUS_IN_TAG))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeBogusComment(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &replace, 1);
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeMarkupDeclarationOpen(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (Tokenizer_p->InputStream.length - (Tokenizer_p->index + 1) > 1) 
    {
        if (Tokenizer_p->InputStream.p[Tokenizer_p->index + 0] == 0x2D
        &&  Tokenizer_p->InputStream.p[Tokenizer_p->index + 1] == 0x2D) 
        {
            Tokenizer_p->index += 2;
            NH_CORE_CHECK(nh_html_newCommentToken(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_START;
            return NH_API_SUCCESS;
        }
    }

    if (Tokenizer_p->InputStream.length - (Tokenizer_p->index + 1) > 6) 
    {
        if (Tokenizer_p->InputStream.p[Tokenizer_p->index + 0] == '['
        &&  Tokenizer_p->InputStream.p[Tokenizer_p->index + 1] == 'C'
        &&  Tokenizer_p->InputStream.p[Tokenizer_p->index + 2] == 'D'
        &&  Tokenizer_p->InputStream.p[Tokenizer_p->index + 3] == 'A'
        &&  Tokenizer_p->InputStream.p[Tokenizer_p->index + 4] == 'T'
        &&  Tokenizer_p->InputStream.p[Tokenizer_p->index + 5] == 'A'
        &&  Tokenizer_p->InputStream.p[Tokenizer_p->index + 6] == '[') 
        {
            Tokenizer_p->index += 7;
            printf("TODO HTML PARSER 13.2.5.42 Markup declaration open state\n");
            exit(0);
        }

        nh_webidl_DOMString Temporary = nh_webidl_initDOMString(7);
        nh_webidl_appendUnicodeToDOMString(&Temporary, &Tokenizer_p->InputStream.p[Tokenizer_p->index], 7);

        if (nh_encoding_isASCIICaseInsensitiveMatch(Temporary.p, "DOCTYPE")) {
            Tokenizer_p->index += 7;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE;
            nh_webidl_freeDOMString(&Temporary);
            return NH_API_SUCCESS;
        }

        nh_webidl_freeDOMString(&Temporary);
    }

    NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_INCORRECTLY_OPENED_COMMENT))
    NH_CORE_CHECK(nh_html_newCommentToken(Tokenizer_p))
    Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT;

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentStart(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_START_DASH;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_ABRUPT_CLOSING_OF_EMPTY_COMMENT))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentStartDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_ABRUPT_CLOSING_OF_EMPTY_COMMENT))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
        {
            NH_ENCODING_UTF32 append = 0x2D;
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &append, 1);
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeComment(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x3C :
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN;
            break;
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &replace, 1);
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentLessThanSign(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x21 :
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG;
            break;
        case 0x3C :
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            break;
        default :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentLessThanSignBang(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH;
            break;
        default :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentLessThanSignBangDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH;
            break;
        default :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentLessThanSignBangDashDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_NESTED_COMMENT))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentEndDash(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END;
            break;
        default :
        {
            NH_ENCODING_UTF32 append = 0x2D;
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &append, 1);
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentEnd(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        case 0x21 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END_BANG;
            break;
        case 0x2D :
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            break;
        default :
        {
            NH_ENCODING_UTF32 append = 0x2D;
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &append, 1);
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCommentEndBang(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x2D :
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &Tokenizer_p->codepoint, 1);
            NH_ENCODING_UTF32 exclamation = 0x21;
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &exclamation, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_INCORRECTLY_CLOSED_COMMENT))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
        {
            NH_ENCODING_UTF32 minus = 0x2D, exclamation = 0x21;
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &minus, 1);
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &minus, 1);
            nh_webidl_appendUnicodeToDOMString(&Tokenizer_p->Token_p->CommentOrCharacter.Data, &exclamation, 1);
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeDOCTYPE(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_BEFORE_DOCTYPE_NAME))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeBeforeDOCTYPEName(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) 
    {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        NH_CORE_CHECK(nh_html_newDOCTYPEToken(Tokenizer_p, false))
        NH_CORE_CHECK(nh_html_allocateDOCTYPEName(Tokenizer_p))
        nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.Name_p, &lower, 1);
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME;
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore character
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_CORE_CHECK(nh_html_newDOCTYPEToken(Tokenizer_p, false))
            NH_CORE_CHECK(nh_html_allocateDOCTYPEName(Tokenizer_p))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.Name_p, &replace, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_NAME))
            NH_CORE_CHECK(nh_html_newDOCTYPEToken(Tokenizer_p, true))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_newDOCTYPEToken(Tokenizer_p, false))
            NH_CORE_CHECK(nh_html_allocateDOCTYPEName(Tokenizer_p))
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.Name_p, &Tokenizer_p->codepoint, 1);
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME;
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeDOCTYPEName(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (nh_encoding_isASCIIUpperAlpha(Tokenizer_p->codepoint)) {
        NH_ENCODING_UTF32 lower = Tokenizer_p->codepoint + 0x20;
        nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.Name_p, &lower, 1);
        return NH_API_SUCCESS;
    }

    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_NAME;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.Name_p, &replace, 1);
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.Name_p, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAfterDOCTYPEName(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            if (Tokenizer_p->InputStream.length - (Tokenizer_p->index + 1) > 5) 
            {
                nh_webidl_DOMString Temporary = nh_webidl_initDOMString(6);
                nh_webidl_appendUnicodeToDOMString(&Temporary, &Tokenizer_p->InputStream.p[Tokenizer_p->index], 6);

                bool isPublic = nh_encoding_isASCIICaseInsensitiveMatch(Temporary.p, "PUBLIC");
                bool isSystem = nh_encoding_isASCIICaseInsensitiveMatch(Temporary.p, "SYSTEM");

                nh_webidl_freeDOMString(&Temporary);
 
                if (isPublic) {
                    Tokenizer_p->index += 6;
                    Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_KEYWORD;
                    return NH_API_SUCCESS;
                }
                if (isSystem) {
                    Tokenizer_p->index += 6;
                    Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_KEYWORD;
                    return NH_API_SUCCESS;
                }
            }
 
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_INVALID_CHARACTER_SEQUENCE_AFTER_DOCTYPE_NAME))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))

            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAfterDOCTYPEPublicKeyword(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER;
            break;
        case 0x22 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_AFTER_DOCTYPE_PUBLIC_KEYWORD))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeBeforeDOCTYPEPublicIdentifier(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x22 :
            NH_CORE_CHECK(nh_html_allocateDOCTYPEPublicIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_CORE_CHECK(nh_html_allocateDOCTYPEPublicIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeDOCTYPEPublicIdentifierDoubleQuoted(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier_p, &replace, 1);
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier_p, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeDOCTYPEPublicIdentifierSingleQuoted(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_IDENTIFIER;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier_p, &replace, 1);
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_ABRUPT_DOCTYPE_PUBLIC_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.PublicIdentifier_p, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAfterDOCTYPEPublicIdentifier(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS;
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        case 0x22 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS))
            NH_CORE_CHECK(nh_html_allocateDOCTYPESystemIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS))
            NH_CORE_CHECK(nh_html_allocateDOCTYPESystemIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeBetweenDOCTYPEPublicAndSystemIdentifiers(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        case 0x22 :
            NH_CORE_CHECK(nh_html_allocateDOCTYPESystemIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_CORE_CHECK(nh_html_allocateDOCTYPESystemIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAfterDOCTYPESystemKeyword(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER;
            break;
        case 0x22 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD))
            NH_CORE_CHECK(nh_html_allocateDOCTYPESystemIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_WHITESPACE_AFTER_DOCTYPE_SYSTEM_KEYWORD))
            NH_CORE_CHECK(nh_html_allocateDOCTYPESystemIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeBeforeDOCTYPESystemIdentifier(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x22 :
            NH_CORE_CHECK(nh_html_allocateDOCTYPESystemIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED;
            break;
        case 0x27 :
            NH_CORE_CHECK(nh_html_allocateDOCTYPESystemIdentifier(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED;
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_MISSING_QUOTE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeDOCTYPESystemIdentifierDoubleQuoted(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x22 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier_p, &replace, 1);
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier_p, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeDOCTYPESystemIdentifierSingleQuoted(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x27 :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_IDENTIFIER;
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            NH_ENCODING_UTF32 replace = 0xFFFD;
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier_p, &replace, 1);
            break;
        case 0x3E :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_ABRUPT_DOCTYPE_SYSTEM_IDENTIFIER))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            nh_webidl_appendUnicodeToDOMString(Tokenizer_p->Token_p->DOCTYPE.SystemIdentifier_p, &Tokenizer_p->codepoint, 1);
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeAfterDOCTYPESystemIdentifier(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x09 :
        case 0x0A :
        case 0x0C :
        case 0x20 :
            break; // ignore
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        default :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_CHARACTER_AFTER_DOCTYPE_SYSTEM_IDENTIFIER))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeBogusDOCTYPE(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            break;
        case 0x00 :
            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_UNEXPECTED_NULL_CHARACTER))
            break;
        default :
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCDATASection(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x5D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_BRACKET;
            break;
        default :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->codepoint))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCDATASectionBracket(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x5D :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_END;
            break;
        default :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_CDATA_SECTION))
            break;
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCDATASectionEnd(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->codepoint)
    {
        case 0x5D :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x5D))
            break;
        case 0x3E :
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_DATA;
            break;
        default :
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_CDATA_SECTION))
            break;
    }

    return NH_API_SUCCESS;
}

// https://html.spec.whatwg.org/multipage/parsing.html#flush-code-points-consumed-as-a-character-reference
static NH_API_RESULT nh_html_flushCharacterReference(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (Tokenizer_p->returnState == NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED
    ||  Tokenizer_p->returnState == NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED
    ||  Tokenizer_p->returnState == NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED) 
    {
        nh_webidl_appendUnicodeToDOMString(&((nh_html_Attribute*)Tokenizer_p->Token_p->StartOrEndTag.Attributes.p)[Tokenizer_p->Token_p->StartOrEndTag.Attributes.length - 1].Value, Tokenizer_p->TemporaryBuffer.p, Tokenizer_p->TemporaryBuffer.length);
    }
    else {
        for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))
        }
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_html_consumeCharacterReference(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_html_resetTemporaryBuffer(Tokenizer_p);
    NH_ENCODING_UTF32 ambersand = 0x26;
    nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &ambersand, 1); 

    if (nh_encoding_isASCIIAlphaNumeric(Tokenizer_p->codepoint)) {
        return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_NAMED_CHARACTER_REFERENCE);
    }

    if (Tokenizer_p->codepoint == 0x23) {
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &Tokenizer_p->codepoint, 1); 
        Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_NUMERIC_CHARACTER_REFERENCE;
        return NH_API_SUCCESS;
    }

    NH_CORE_CHECK(nh_html_flushCharacterReference(Tokenizer_p))
    NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, Tokenizer_p->returnState))

    return NH_API_SUCCESS;
}

// https://html.spec.whatwg.org/multipage/parsing.html#named-character-reference-state
static NH_API_RESULT nh_html_consumeNamedCharacterReference(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_html_resetTemporaryBuffer(Tokenizer_p);
    NH_ENCODING_UTF32 bestMatch = 0;
    int bestLength = 0;

    while (Tokenizer_p->index < Tokenizer_p->InputStream.length) {
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, Tokenizer_p->InputStream.p+Tokenizer_p->index, 1); 
        nh_html_incrementTokenizer(Tokenizer_p);
        int length = 0;
        nh_encoding_UTF8String UTF8 = nh_encoding_encodeUTF8(Tokenizer_p->TemporaryBuffer.p, Tokenizer_p->TemporaryBuffer.length);
        NH_ENCODING_UTF32 match = nh_html_matchCharacterReferencesEntity(UTF8.p, &length); 
        nh_encoding_freeUTF8(&UTF8);
        if (length == 0 || length <= bestLength) {
            nh_html_decrementTokenizer(Tokenizer_p);
            break;
        }
        bestMatch = match;
        bestLength = length;
    }

    if (bestMatch) {
        nh_html_resetTemporaryBuffer(Tokenizer_p);
        nh_webidl_appendToUSVString(&Tokenizer_p->TemporaryBuffer, &bestMatch, 1); 
        NH_CORE_CHECK(nh_html_flushCharacterReference(Tokenizer_p))
        return nh_html_reconsume(Tokenizer_p, Tokenizer_p->returnState);
    }

    NH_CORE_CHECK(nh_html_flushCharacterReference(Tokenizer_p))
    return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_AMBIGUOUS_AMPERSAND);
}

// 13.2.5.74

// HANDLE EOF ======================================================================================

static NH_API_RESULT nh_html_handleEOF(
    nh_html_Tokenizer *Tokenizer_p)
{
    switch (Tokenizer_p->state)
    {
        case NH_HTML_TOKENIZATION_STATE_DATA        :
        case NH_HTML_TOKENIZATION_STATE_RCDATA      :
        case NH_HTML_TOKENIZATION_STATE_RAWTEXT     :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA :
        case NH_HTML_TOKENIZATION_STATE_PLAINTEXT   :

            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_TAG_OPEN :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_BEFORE_TAG_NAME))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_END_TAG_OPEN :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_BEFORE_TAG_NAME))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_TAG_NAME :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_TAG))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_RCDATA_LESS_THAN_SIGN :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_OPEN :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_NAME :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))    
            }
            return nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RCDATA);
            break;

        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_LESS_THAN_SIGN :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
            break;

        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_OPEN :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
            break;

        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_NAME :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))    
            }
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_RAWTEXT))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_LESS_THAN_SIGN :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_OPEN :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_NAME :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))    
            }
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START_DASH :

            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH_DASH :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_OPEN :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_NAME :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x3C))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x2F))
            for (int i = 0; i < Tokenizer_p->TemporaryBuffer.length; ++i) {
                NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, Tokenizer_p->TemporaryBuffer.p[i]))    
            }
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_START :

            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_SCRIPT_HTML_COMMENT_LIKE_TEXT))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN :
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_END :

            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED))
            break;

        case NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME :
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME :

            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME))
            break;

        case NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_TAG))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_VALUE :
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED))
            break;

        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED :
        case NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_VALUE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_TAG))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT :

            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_MARKUP_DECLARATION_OPEN :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_INCORRECTLY_OPENED_COMMENT))
            NH_CORE_CHECK(nh_html_newCommentToken(Tokenizer_p))
            Tokenizer_p->state = NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT;
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_START :

            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_START_DASH :
        case NH_HTML_TOKENIZATION_STATE_COMMENT :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_COMMENT))
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN :
        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG :

            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH :

            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH :

            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_COMMENT_END))
            break;

        case NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH :
        case NH_HTML_TOKENIZATION_STATE_COMMENT_END :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_COMMENT))
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_DOCTYPE :
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_DOCTYPE))
            NH_CORE_CHECK(nh_html_newDOCTYPEToken(Tokenizer_p, true))
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_NAME :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_KEYWORD :
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER :
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_IDENTIFIER :
        case NH_HTML_TOKENIZATION_STATE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_KEYWORD :
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER :
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED :
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_IDENTIFIER :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_DOCTYPE))
            Tokenizer_p->Token_p->DOCTYPE.forceQuirks = true;
            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE :

            NH_CORE_CHECK(nh_html_emit(Tokenizer_p))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION :

            NH_CORE_CHECK(nh_html_newTokenizerError(Tokenizer_p, NH_HTML_PARSE_ERROR_EOF_IN_CDATA))
            NH_CORE_CHECK(nh_html_emitEOFToken(Tokenizer_p))
            break;

        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_BRACKET :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_CDATA_SECTION))
            break;

        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_END :

            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_CORE_CHECK(nh_html_emitCharacterToken(Tokenizer_p, 0x5D))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, NH_HTML_TOKENIZATION_STATE_CDATA_SECTION))
            break;

        case NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE :
        
            NH_CORE_CHECK(nh_html_flushCharacterReference(Tokenizer_p))
            NH_CORE_CHECK(nh_html_reconsume(Tokenizer_p, Tokenizer_p->returnState))
            break;
    }

    return NH_API_SUCCESS;
}

// CONSUME NEXT ====================================================================================

static NH_API_RESULT nh_html_reconsume(
    nh_html_Tokenizer *Tokenizer_p, NH_HTML_TOKENIZATION_STATE newState)
{
    Tokenizer_p->state = newState;

    return nh_html_consume(Tokenizer_p);
}

static NH_API_RESULT nh_html_consume(
    nh_html_Tokenizer *Tokenizer_p)
{
    if (Tokenizer_p->index == Tokenizer_p->InputStream.length) {
        return nh_html_handleEOF(Tokenizer_p);
    }

    switch (Tokenizer_p->state)
    {
        case NH_HTML_TOKENIZATION_STATE_DATA                                          : return nh_html_consumeData(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_RCDATA                                        : return nh_html_consumeRCDATA(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_RAWTEXT                                       : return nh_html_consumeRAWTEXT(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA                                   : return nh_html_consumeScriptData(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_PLAINTEXT                                     : return nh_html_consumePLAINTEXT(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_TAG_OPEN                                      : return nh_html_consumeTagOpen(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_END_TAG_OPEN                                  : return nh_html_consumeEndTagOpen(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_TAG_NAME                                      : return nh_html_consumeTagName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_RCDATA_LESS_THAN_SIGN                         : return nh_html_consumeRCDATALessThanSign(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_OPEN                           : return nh_html_consumeRCDATAEndTagOpen(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_RCDATA_END_TAG_NAME                           : return nh_html_consumeRCDATAEndTagName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_LESS_THAN_SIGN                        : return nh_html_consumeRAWTEXTLessThanSign(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_OPEN                          : return nh_html_consumeRAWTEXTEndTagOpen(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_RAWTEXT_END_TAG_NAME                          : return nh_html_consumeRAWTEXTEndTagName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_LESS_THAN_SIGN                    : return nh_html_consumeScriptDataLessThanSign(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_OPEN                      : return nh_html_consumeScriptDataEndTagOpen(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_END_TAG_NAME                      : return nh_html_consumeScriptDataEndTagName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START                      : return nh_html_consumeScriptDataEscapeStart(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPE_START_DASH                 : return nh_html_consumeScriptDataEscapeStartDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED                           : return nh_html_consumeScriptDataEscaped(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH                      : return nh_html_consumeScriptDataEscapedDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_DASH_DASH                 : return nh_html_consumeScriptDataEscapedDashDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_LESS_THAN_SIGN            : return nh_html_consumeScriptDataEscapedLessThanSign(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_OPEN              : return nh_html_consumeScriptDataEscapedEndTagOpen(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_ESCAPED_END_TAG_NAME              : return nh_html_consumeScriptDataEscapedEndTagName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_START               : return nh_html_consumeScriptDataDoubleEscapeStart(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED                    : return nh_html_consumeScriptDataDoubleEscaped(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH               : return nh_html_consumeScriptDataDoubleEscapedDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_DASH_DASH          : return nh_html_consumeScriptDataDoubleEscapedDashDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPED_LESS_THAN_SIGN     : return nh_html_consumeScriptDataDoubleEscapedLessThanSign(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SCRIPT_DATA_DOUBLE_ESCAPE_END                 : return nh_html_consumeScriptDataDoubleEscapeEnd(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_NAME                         : return nh_html_consumeBeforeAttributeName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_NAME                                : return nh_html_consumeAttributeName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_NAME                          : return nh_html_consumeAfterAttributeName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_BEFORE_ATTRIBUTE_VALUE                        : return nh_html_consumeBeforeAttributeValue(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_DOUBLE_QUOTED                 : return nh_html_consumeAttributeValueDoubleQuoted(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_SINGLE_QUOTED                 : return nh_html_consumeAttributeValueSingleQuoted(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_ATTRIBUTE_VALUE_UNQUOTED                      : return nh_html_consumeAttributeValueUnquoted(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_AFTER_ATTRIBUTE_VALUE_QUOTED                  : return nh_html_consumeAfterAttributeValueQuoted(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_SELF_CLOSING_START_TAG                        : return nh_html_consumeSelfClosingStartTag(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_BOGUS_COMMENT                                 : return nh_html_consumeBogusComment(Tokenizer_p); 
        case NH_HTML_TOKENIZATION_STATE_MARKUP_DECLARATION_OPEN                       : return nh_html_consumeMarkupDeclarationOpen(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_START                                 : return nh_html_consumeCommentStart(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_START_DASH                            : return nh_html_consumeCommentStartDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT                                       : return nh_html_consumeComment(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN                        : return nh_html_consumeCommentLessThanSign(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG                   : return nh_html_consumeCommentLessThanSignBang(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH              : return nh_html_consumeCommentLessThanSignBangDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_LESS_THAN_SIGN_BANG_DASH_DASH         : return nh_html_consumeCommentLessThanSignBangDashDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_END_DASH                              : return nh_html_consumeCommentEndDash(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_END                                   : return nh_html_consumeCommentEnd(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_COMMENT_END_BANG                              : return nh_html_consumeCommentEndBang(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE                                       : return nh_html_consumeDOCTYPE(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_NAME                           : return nh_html_consumeBeforeDOCTYPEName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_NAME                                  : return nh_html_consumeDOCTYPEName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_NAME                            : return nh_html_consumeAfterDOCTYPEName(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_KEYWORD                  : return nh_html_consumeAfterDOCTYPEPublicKeyword(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_PUBLIC_IDENTIFIER              : return nh_html_consumeBeforeDOCTYPEPublicIdentifier(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_DOUBLE_QUOTED       : return nh_html_consumeDOCTYPEPublicIdentifierDoubleQuoted(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_PUBLIC_IDENTIFIER_SINGLE_QUOTED       : return nh_html_consumeDOCTYPEPublicIdentifierSingleQuoted(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_PUBLIC_IDENTIFIER               : return nh_html_consumeAfterDOCTYPEPublicIdentifier(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_BETWEEN_DOCTYPE_PUBLIC_AND_SYSTEM_IDENTIFIERS : return nh_html_consumeBetweenDOCTYPEPublicAndSystemIdentifiers(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_KEYWORD                  : return nh_html_consumeAfterDOCTYPESystemKeyword(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_BEFORE_DOCTYPE_SYSTEM_IDENTIFIER              : return nh_html_consumeBeforeDOCTYPESystemIdentifier(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_DOUBLE_QUOTED       : return nh_html_consumeDOCTYPESystemIdentifierDoubleQuoted(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_DOCTYPE_SYSTEM_IDENTIFIER_SINGLE_QUOTED       : return nh_html_consumeDOCTYPESystemIdentifierSingleQuoted(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_AFTER_DOCTYPE_SYSTEM_IDENTIFIER               : return nh_html_consumeAfterDOCTYPESystemIdentifier(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_BOGUS_DOCTYPE                                 : return nh_html_consumeBogusDOCTYPE(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION                                 : return nh_html_consumeCDATASection(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_BRACKET                         : return nh_html_consumeCDATASectionBracket(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_CDATA_SECTION_END                             : return nh_html_consumeCDATASectionEnd(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_CHARACTER_REFERENCE                           : return nh_html_consumeCharacterReference(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_NAMED_CHARACTER_REFERENCE                     : return nh_html_consumeNamedCharacterReference(Tokenizer_p);
        case NH_HTML_TOKENIZATION_STATE_AMBIGUOUS_AMPERSAND                           : puts("bafsdssssssss"); exit(0); break; 
        case NH_HTML_TOKENIZATION_STATE_NUMERIC_CHARACTER_REFERENCE                   : puts("bafsdssssssss"); exit(0); break; 
        case NH_HTML_TOKENIZATION_STATE_HEXADECIMAL_CHARACTER_REFERENCE_START         : puts("bafsdssssssss"); exit(0); break; 
        case NH_HTML_TOKENIZATION_STATE_DECIMAL_CHARACTER_REFERENCE_START             : puts("bafsdssssssss"); exit(0); break; 
        case NH_HTML_TOKENIZATION_STATE_HEXADECIMAL_CHARACTER_REFERENCE               : puts("bafsdssssssss"); exit(0); break; 
        case NH_HTML_TOKENIZATION_STATE_DECIMAL_CHARACTER_REFERENCE                   : puts("bafsdssssssss"); exit(0); break; 
        case NH_HTML_TOKENIZATION_STATE_NUMERIC_CHARACTER_REFERENCE_END               : puts("bafsdssssssss"); exit(0); break; 

    }

    return NH_API_ERROR_BAD_STATE;
}

static void nh_html_incrementTokenizer(
    nh_html_Tokenizer *Tokenizer_p)
{
    Tokenizer_p->index++;
    if (Tokenizer_p->index < Tokenizer_p->InputStream.length) {
        Tokenizer_p->codepoint = Tokenizer_p->InputStream.p[Tokenizer_p->index];
    }
}

static void nh_html_decrementTokenizer(
    nh_html_Tokenizer *Tokenizer_p)
{
    Tokenizer_p->index--;
    if (Tokenizer_p->index > 0) {
        Tokenizer_p->codepoint = Tokenizer_p->InputStream.p[Tokenizer_p->index];
    }
}

NH_API_RESULT nh_html_consumeNext(
    nh_html_Tokenizer *Tokenizer_p)
{
    nh_html_incrementTokenizer(Tokenizer_p);
    return nh_html_consume(Tokenizer_p);
}
