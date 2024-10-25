// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Tokenizer.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/Array.h"

#include "../../nh-encoding/Encodings/UTF32.h"

#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// TOKEN NAMES =====================================================================================

const char *nh_css_getTokenName(
    NH_CSS_TOKEN token)
{
    static const char *tokenNames_pp[] =
    {
        "<ident-token>", 
        "<function-token>", 
        "<at-keyword-token>", 
        "<hash-token>", 
        "<string-token>", 
        "<bad-string-token>", 
        "<url-token>", 
        "<bad-url-token>", 
        "<delim-token>", 
        "<number-token>", 
        "<percentage-token>", 
        "<dimension-token>", 
        "<whitespace-token>", 
        "<CDO-token>", 
        "<CDC-token>", 
        "<colon-token>", 
        "<semicolon-token>", 
        "<comma-token>", 
        "<[-token>", 
        "<]-token>", 
        "<(-token>", 
        "<)-token>", 
        "<{-token>",
        "<}-token>",
        "<EOF-token>",
    };
    
    return tokenNames_pp[token];
}

// TOKENIZER =======================================================================================

typedef struct nh_css_Tokenizer {
    nh_core_Array Tokens;
    NH_ENCODING_UTF32 *codepoints_p;
    unsigned long long length;
} nh_css_Tokenizer;

static nh_css_Tokenizer nh_css_initTokenizer(
    nh_encoding_UTF32String String)
{
    nh_css_Tokenizer Tokenizer;
    Tokenizer.Tokens       = nh_core_initArray(sizeof(nh_css_Token), 255);
    Tokenizer.codepoints_p = String.p;
    Tokenizer.length       = String.length;

    return Tokenizer;
}

// HELPER ==========================================================================================

static void nh_css_advanceTokenizer(
    nh_css_Tokenizer *Tokenizer_p, unsigned long long advance)
{
    Tokenizer_p->codepoints_p = Tokenizer_p->codepoints_p + advance;
    Tokenizer_p->length -= advance; 

    return;
}

static bool nh_css_isWhitespace(
    NH_ENCODING_UTF32 codepoint)
{
    if (codepoint == 0x0A || codepoint == 0x09 || codepoint == 0x20) {
        return true;
    }

    return false;
}

static bool nh_css_isNameStart(
    NH_ENCODING_UTF32 codepoint)
{
    if (nh_encoding_isASCIIAlpha(codepoint) || codepoint >= 0x80 || codepoint == 0x5F) {
        return true;
    }

    return false;
}

static bool nh_css_isName(
    NH_ENCODING_UTF32 codepoint)
{
    if (nh_css_isNameStart(codepoint) || nh_encoding_isASCIIDigit(codepoint) || codepoint == 0x2D) {
        return true;
    }

    return false;
}

static bool nh_css_isValidEscape(
    NH_ENCODING_UTF32 *codepoints_p)
{
    if (codepoints_p[0] != 0x5C) {return false;}
    if (codepoints_p[1] == 0x0A) {return false;}

    return true;
}

static bool nh_css_startsWithIdentifier(
    NH_ENCODING_UTF32 *codepoints_p)
{
    if (codepoints_p[0] == 0x2D) {
        if (nh_css_isNameStart(codepoints_p[1]) || codepoints_p[0] == 0x2D || nh_css_isValidEscape(&codepoints_p[1])) {
            return true;
        }
    }
    else if (nh_css_isNameStart(codepoints_p[0])) {
        return true;
    }
    else if (codepoints_p[0] == 0x5C) {
        if (nh_css_isValidEscape(codepoints_p)) {return true;}
    }

    return false;
}

static bool nh_css_startsWithNumber(
    NH_ENCODING_UTF32 *codepoints_p)
{
    if (codepoints_p[0] == 0x2B || codepoints_p[0] == 0x2D) {
        if (nh_encoding_isASCIIDigit(codepoints_p[1])) {
            return true;
        }
        else if (codepoints_p[1] == 0x2E && nh_encoding_isASCIIDigit(codepoints_p[2])) {
            return true;
        }
    }
    else if (codepoints_p[0] == 0x2E && nh_encoding_isASCIIDigit(codepoints_p[1])) {
        return true;
    }
    else if (nh_encoding_isASCIIDigit(codepoints_p[0])) {
        return true;
    }

    return false;
}

static NH_WEBIDL_DOUBLE nh_css_convertToNumber(
    nh_encoding_UTF32String *String_p)
{
    int index = 0;

    NH_WEBIDL_DOUBLE s = 1;
    if (String_p->p[index] == 0x2D) {s = -1; index++;}
    else if (String_p->p[index] == 0x2B) {index++;}

    NH_WEBIDL_DOUBLE i = 0;
    char num_p[255] = {0};
    memset(num_p, 0, 255);
    int tmp = 0;
    while (index < String_p->length && nh_encoding_isASCIIDigit(String_p->p[index])) {
        num_p[tmp++] = String_p->p[index++];
    }
    if (tmp > 0) {i = atoi(num_p);}

    if (String_p->p[index] == 0x2E) {index++;}

    NH_WEBIDL_DOUBLE f = 0, d = 0;
    memset(num_p, 0, 255);
    tmp = 0;
    while (index < String_p->length && nh_encoding_isASCIIDigit(String_p->p[index])) {
        num_p[tmp++] = String_p->p[index++];
    }
    d = tmp;
    if (d > 0) {f = atoi(num_p);}

    if (String_p->p[index] == 0x45 || String_p->p[index] == 0x65) {index++;}

    NH_WEBIDL_DOUBLE t = 1;
    if (String_p->p[index] == 0x2D) {t = -1; index++;}
    else if (String_p->p[index] == 0x2B) {index++;}

    NH_WEBIDL_DOUBLE e = 0;
    memset(num_p, 0, 255);
    tmp = 0;
    while (index < String_p->length && nh_encoding_isASCIIDigit(String_p->p[index])) {
        num_p[tmp++] = String_p->p[index++];
    }
    if (tmp > 0) {e = atoi(num_p);}

    return s * (i + f * pow(10, -d)) * pow(10, t * e);
}

// CONSUME =========================================================================================

static nh_css_Token *nh_css_reconsumeToken(
    nh_css_Tokenizer *Tokenizer_p, nh_css_Token *Token_p
); 

static NH_API_RESULT nh_css_consumeToken(
    nh_css_Tokenizer *Tokenizer_p
); 

static NH_API_RESULT nh_css_consumeComments(
    nh_css_Tokenizer *Tokenizer_p)
{
    if (Tokenizer_p->length > 1 && Tokenizer_p->codepoints_p[0] == 0x2F && Tokenizer_p->codepoints_p[1] == 0x2A) 
    {
        nh_css_advanceTokenizer(Tokenizer_p, 2);

        while (Tokenizer_p->length > 1 && (Tokenizer_p->codepoints_p[0] != 0x2A || Tokenizer_p->codepoints_p[1] != 0x2F)) {
            nh_css_advanceTokenizer(Tokenizer_p, 1);
        }

        nh_css_advanceTokenizer(Tokenizer_p, 2);
    }

    return NH_API_SUCCESS;
}

static nh_css_Token *nh_css_consumeString(
    nh_css_Tokenizer *Tokenizer_p, nh_css_Token *Token_p)
{
    NH_ENCODING_UTF32 endingCodepoint = Tokenizer_p->codepoints_p[0];

    Token_p->type = NH_CSS_TOKEN_STRING;
    Token_p->Other.Value = nh_encoding_initUTF32(16);

    nh_css_advanceTokenizer(Tokenizer_p, 1);

    while (Tokenizer_p->length && Tokenizer_p->codepoints_p[0] != endingCodepoint)
    {
        switch (Tokenizer_p->codepoints_p[0])
        {
            case 0x0A :
                break;
            case 0x5C :
                break;
            default :
                nh_encoding_appendUTF32(&Token_p->Other.Value, Tokenizer_p->codepoints_p, 1);
        }

        nh_css_advanceTokenizer(Tokenizer_p, 1);
    }

    nh_css_advanceTokenizer(Tokenizer_p, 1);

    return Token_p;
}

static NH_ENCODING_UTF32 nh_css_consumeEscaped(
    nh_css_Tokenizer *Tokenizer_p)
{
    NH_ENCODING_UTF32 codepoint = 0xFFFD;

    if (Tokenizer_p->length <= 0) {
        // TODO parse error
    }
    else if (nh_encoding_isASCIIHexDigit(Tokenizer_p->codepoints_p[0])) {
        char hexDigits_p[7];
        memset(hexDigits_p, 0, 7);
        int i;
        for (i = 0; i < 6 && nh_encoding_isASCIIHexDigit(Tokenizer_p->codepoints_p[0]); ++i) {
            hexDigits_p[i] = Tokenizer_p->codepoints_p[0];
            nh_css_advanceTokenizer(Tokenizer_p, 1);
        }
        char *end_p;
        codepoint = strtol(hexDigits_p, &end_p, 16);
        if (codepoint == 0 || nh_encoding_isSurrogate(codepoint) || codepoint > 0x10FFFF) {
            codepoint = 0xFFFD;
        }
        if (Tokenizer_p->length > 0 && nh_css_isWhitespace(Tokenizer_p->codepoints_p[0])) {
            nh_css_advanceTokenizer(Tokenizer_p, 1);
        }
    }
    else {
        codepoint = Tokenizer_p->codepoints_p[0];
    }

    return codepoint;
}

static nh_encoding_UTF32String nh_css_consumeName(
    nh_css_Tokenizer *Tokenizer_p)
{
    nh_encoding_UTF32String String = nh_encoding_initUTF32(16);

    while (Tokenizer_p->length)
    {
        if (nh_css_isName(Tokenizer_p->codepoints_p[0])) {
            nh_encoding_appendUTF32(&String, &Tokenizer_p->codepoints_p[0], 1);
            nh_css_advanceTokenizer(Tokenizer_p, 1);
        }
        else if (nh_css_isValidEscape(Tokenizer_p->codepoints_p)) {
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            NH_ENCODING_UTF32 codepoint = nh_css_consumeEscaped(Tokenizer_p);
            nh_encoding_appendUTF32(&String, &codepoint, 1);
        }
        else {break;}
    }

    return String;
}

static NH_WEBIDL_DOUBLE nh_css_consumeNumber(
    nh_css_Tokenizer *Tokenizer_p, bool *isInteger_p)
{
    *isInteger_p = true;
    nh_encoding_UTF32String Representation = nh_encoding_initUTF32(8);

    if (Tokenizer_p->codepoints_p[0] == 0x2B || Tokenizer_p->codepoints_p[0] == 0x2D) {
        nh_encoding_appendUTF32(&Representation, Tokenizer_p->codepoints_p, 1);
        nh_css_advanceTokenizer(Tokenizer_p, 1);
    }

    while (nh_encoding_isASCIIDigit(Tokenizer_p->codepoints_p[0])) {
        nh_encoding_appendUTF32(&Representation, Tokenizer_p->codepoints_p, 1);
        nh_css_advanceTokenizer(Tokenizer_p, 1);
    }

    if (Tokenizer_p->codepoints_p[0] == 0x2E && nh_encoding_isASCIIDigit(Tokenizer_p->codepoints_p[1])) 
    {
        nh_encoding_appendUTF32(&Representation, Tokenizer_p->codepoints_p, 2);
        nh_css_advanceTokenizer(Tokenizer_p, 2);
        *isInteger_p = false;

        while (nh_encoding_isASCIIDigit(Tokenizer_p->codepoints_p[0])) {
            nh_encoding_appendUTF32(&Representation, Tokenizer_p->codepoints_p, 1);
            nh_css_advanceTokenizer(Tokenizer_p, 1);
        }
    }

    if (Tokenizer_p->codepoints_p[0] == 0x45 || Tokenizer_p->codepoints_p[0] == 0x65)
    {
        bool match = false;
        if ((Tokenizer_p->codepoints_p[1] == 0x2D || Tokenizer_p->codepoints_p[1] == 0x2B) && nh_encoding_isASCIIDigit(Tokenizer_p->codepoints_p[2])) {
            nh_encoding_appendUTF32(&Representation, Tokenizer_p->codepoints_p, 3);
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            match = true;
        }
        else if (nh_encoding_isASCIIDigit(Tokenizer_p->codepoints_p[1])) {
            nh_encoding_appendUTF32(&Representation, Tokenizer_p->codepoints_p, 2);
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            match = true;
        }
        if (match)
        {
            *isInteger_p = false;
            while (nh_encoding_isASCIIDigit(Tokenizer_p->codepoints_p[0])) {
                nh_encoding_appendUTF32(&Representation, Tokenizer_p->codepoints_p, 1);
                nh_css_advanceTokenizer(Tokenizer_p, 1);
            }
        }
    }

    NH_WEBIDL_DOUBLE number = nh_css_convertToNumber(&Representation);

    nh_encoding_freeUTF32(&Representation);

    return number;
}

static nh_css_Token *nh_css_consumeNumericToken(
    nh_css_Tokenizer *Tokenizer_p, nh_css_Token *Token_p)
{
    bool isInteger;
    NH_WEBIDL_DOUBLE number = nh_css_consumeNumber(Tokenizer_p, &isInteger);

    if (nh_css_startsWithIdentifier(Tokenizer_p->codepoints_p)) {
        Token_p->type = NH_CSS_TOKEN_DIMENSION;
        Token_p->Dimension.value = number;
        Token_p->Dimension.isInteger = isInteger;
        Token_p->Dimension.Unit = nh_css_consumeName(Tokenizer_p);
    }
    else if (Tokenizer_p->codepoints_p[0] == 0x25) {
        nh_css_advanceTokenizer(Tokenizer_p, 1);
        Token_p->type = NH_CSS_TOKEN_PERCENTAGE;
        Token_p->Percentage.value = number;
    }
    else {
        Token_p->type = NH_CSS_TOKEN_NUMBER;
        Token_p->Number.value = number;
        Token_p->Number.isInteger = isInteger; 
    }

    return Token_p;
}

static nh_css_Token *nh_css_consumeURLToken(
    nh_css_Tokenizer *Tokenizer_p, nh_css_Token *Token_p)
{
    Token_p->type = NH_CSS_TOKEN_URL;
    Token_p->Other.Value = nh_encoding_initUTF32(16);

    while(nh_css_isWhitespace(Tokenizer_p->codepoints_p[0])) {
        nh_css_advanceTokenizer(Tokenizer_p, 1);
    }

    while (Tokenizer_p->length)
    {
        while(nh_css_isWhitespace(Tokenizer_p->codepoints_p[0])) {
            nh_css_advanceTokenizer(Tokenizer_p, 1);
        }

        switch (Tokenizer_p->codepoints_p[0])
        {
            case 0x29 :
                nh_css_advanceTokenizer(Tokenizer_p, 1);
                return Token_p;
            case 0 :
                // parse error 
                return Token_p;
            case 0x22 :
            case 0x27 :
            case 0x28 :
                // parse error
                exit(0);
            default : 
                nh_encoding_appendUTF32(&Token_p->Other.Value, Tokenizer_p->codepoints_p, 1);
        }

        nh_css_advanceTokenizer(Tokenizer_p, 1);
    }

    return Token_p;
}

static nh_css_Token *nh_css_consumeIdentLikeToken(
    nh_css_Tokenizer *Tokenizer_p, nh_css_Token *Token_p)
{
    nh_encoding_UTF32String String = nh_css_consumeName(Tokenizer_p);

    if (String.length > 2 
    && (String.p[0] == 'u' || String.p[0] == 'U') 
    && (String.p[1] == 'r' || String.p[1] == 'R') 
    && (String.p[2] == 'l' || String.p[2] == 'L') && Tokenizer_p->codepoints_p[0] == 0x28) {
        nh_css_advanceTokenizer(Tokenizer_p, 1);
        for (int i = 0; i < 2 && nh_css_isWhitespace(Tokenizer_p->codepoints_p[0]); ++i) {
            nh_css_advanceTokenizer(Tokenizer_p, 1);
        }
        if((Tokenizer_p->codepoints_p[0] == 0x22 || Tokenizer_p->codepoints_p[0] == 0x27 || nh_css_isWhitespace(Tokenizer_p->codepoints_p[0]))
        && (Tokenizer_p->codepoints_p[1] == 0x22 || Tokenizer_p->codepoints_p[1] == 0x27)) {
            Token_p->type = NH_CSS_TOKEN_FUNCTION;
            Token_p->Other.Value = String;
        }
        else {
            nh_encoding_freeUTF32(&String);
            return nh_css_consumeURLToken(Tokenizer_p, Token_p);
        } 
    }
    else if (Tokenizer_p->codepoints_p[0] == 0x28) {
        nh_css_advanceTokenizer(Tokenizer_p, 1);
        Token_p->type = NH_CSS_TOKEN_FUNCTION;
        Token_p->Other.Value = String;
    }
    else {
        Token_p->type = NH_CSS_TOKEN_IDENT;
        Token_p->Other.Value = String;
    }

    return Token_p;
}

static nh_css_Token *nh_css_getToken(
    nh_css_Tokenizer *Tokenizer_p, nh_css_Token *Token_p)
{
    nh_css_consumeComments(Tokenizer_p);

    if (nh_css_isWhitespace(Tokenizer_p->codepoints_p[0])) {
        while (nh_css_isWhitespace(Tokenizer_p->codepoints_p[0])) {nh_css_advanceTokenizer(Tokenizer_p, 1);}
        Token_p->type = NH_CSS_TOKEN_WHITESPACE;
        return Token_p;
    }

    if (nh_encoding_isASCIIDigit(Tokenizer_p->codepoints_p[0])) {
        return nh_css_consumeNumericToken(Tokenizer_p, Token_p);
    }

    if (nh_css_isNameStart(Tokenizer_p->codepoints_p[0])) {
        return nh_css_consumeIdentLikeToken(Tokenizer_p, Token_p);
    }

    switch (Tokenizer_p->codepoints_p[0])
    {
        case 0x00 :
            Token_p->type = NH_CSS_TOKEN_EOF;
            break;
        case 0x22 :
            return nh_css_consumeString(Tokenizer_p, Token_p);
            break;
        case 0x23 :
            if (nh_css_isName(Tokenizer_p->codepoints_p[1]) || (Tokenizer_p->length > 2 && nh_css_isValidEscape(&Tokenizer_p->codepoints_p[1]))) {
                Token_p->type = NH_CSS_TOKEN_HASH;
                Token_p->Hash.unrestricted = true;
                if (nh_css_startsWithIdentifier(&Tokenizer_p->codepoints_p[1])) {
                    Token_p->Hash.unrestricted = false;
                }
                nh_css_advanceTokenizer(Tokenizer_p, 1);
                Token_p->Hash.Value = nh_css_consumeName(Tokenizer_p);
            }
            else {
                Token_p->type = NH_CSS_TOKEN_DELIM;
                Token_p->Delim.value = Tokenizer_p->codepoints_p[0];
                nh_css_advanceTokenizer(Tokenizer_p, 1);
            }
            break;
        case 0x27 :
            return nh_css_consumeString(Tokenizer_p, Token_p);
            break;
        case 0x28 :
            Token_p->type = NH_CSS_TOKEN_LEFT_PARENTHESIS;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        case 0x29 :
            Token_p->type = NH_CSS_TOKEN_RIGHT_PARENTHESIS;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        case 0x2B :
            if (nh_css_startsWithNumber(Tokenizer_p->codepoints_p)) {
                return nh_css_consumeNumericToken(Tokenizer_p, Token_p);
            }
            else {
                Token_p->type = NH_CSS_TOKEN_DELIM;
                Token_p->Delim.value = Tokenizer_p->codepoints_p[0];
                nh_css_advanceTokenizer(Tokenizer_p, 1);
            }
            break;
        case 0x2C :
            Token_p->type = NH_CSS_TOKEN_COMMA;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        case 0x2D :
            if (nh_css_startsWithNumber(Tokenizer_p->codepoints_p)) {
                return nh_css_consumeNumericToken(Tokenizer_p, Token_p);
            }
            else if (Tokenizer_p->length > 1 && Tokenizer_p->codepoints_p[0] == 0x2D && Tokenizer_p->codepoints_p[1] == 0x3E) {
                Token_p->type = NH_CSS_TOKEN_CDC;
                nh_css_advanceTokenizer(Tokenizer_p, 2);
            }
            else if (nh_css_startsWithIdentifier(Tokenizer_p->codepoints_p)) {
                return nh_css_consumeIdentLikeToken(Tokenizer_p, Token_p);
            }
            else {
                Token_p->type = NH_CSS_TOKEN_DELIM;
                Token_p->Delim.value = Tokenizer_p->codepoints_p[0];
                nh_css_advanceTokenizer(Tokenizer_p, 1);
            }
            break;
        case 0x2E :
            if (nh_css_startsWithNumber(Tokenizer_p->codepoints_p)) {
                return nh_css_consumeNumericToken(Tokenizer_p, Token_p);
            }
            else {
                Token_p->type = NH_CSS_TOKEN_DELIM;
                Token_p->Delim.value = Tokenizer_p->codepoints_p[0];
                nh_css_advanceTokenizer(Tokenizer_p, 1);
            }
            break;
        case 0x3A :
            Token_p->type = NH_CSS_TOKEN_COLON;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        case 0x3B :
            Token_p->type = NH_CSS_TOKEN_SEMICOLON;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        case 0x3C :
            if (Tokenizer_p->codepoints_p[1] == 0x21 && Tokenizer_p->codepoints_p[2] == 0x2D && Tokenizer_p->codepoints_p[3] == 0x2D) {
exit(0);
//                nh_css_advanceTokenizer(Tokenizer_p, 1);
//                NH_CORE_CHECK(NULL, nh_css_consumeToken(Tokenizer_p))
//                NH_CORE_CHECK(NULL, nh_css_consumeToken(Tokenizer_p))
//                NH_CORE_CHECK(NULL, nh_css_consumeToken(Tokenizer_p))
//                Token_p->type == NH_CSS_TOKEN_CDO;
            }
            else {
                Token_p->type = NH_CSS_TOKEN_DELIM;
                Token_p->Delim.value = Tokenizer_p->codepoints_p[0];
                nh_css_advanceTokenizer(Tokenizer_p, 1);
            }
            break;
        case 0x40 :
            if (nh_css_startsWithIdentifier(&Tokenizer_p->codepoints_p[1])) {
                nh_css_advanceTokenizer(Tokenizer_p, 1);
                nh_encoding_UTF32String String = nh_css_consumeName(Tokenizer_p);
                Token_p->type = NH_CSS_TOKEN_AT_KEYWORD;
                Token_p->Other.Value = String;
            }
            else {
                Token_p->type = NH_CSS_TOKEN_DELIM;
                Token_p->Delim.value = Tokenizer_p->codepoints_p[0];
                nh_css_advanceTokenizer(Tokenizer_p, 1);
            }
            break;
        case 0x5B :
            Token_p->type = NH_CSS_TOKEN_LEFT_SQUARE_BRACKET;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        case 0x5C :
            if (nh_css_isValidEscape(Tokenizer_p->codepoints_p)) {
                return nh_css_consumeIdentLikeToken(Tokenizer_p, Token_p);
            }
            else {
                // parse error
                Token_p->type = NH_CSS_TOKEN_DELIM;
                Token_p->Delim.value = Tokenizer_p->codepoints_p[0];
                nh_css_advanceTokenizer(Tokenizer_p, 1);
            }
            break;
        case 0x5D :
            Token_p->type = NH_CSS_TOKEN_RIGHT_SQUARE_BRACKET;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        case 0x7B :
            Token_p->type = NH_CSS_TOKEN_LEFT_CURLY_BRACKET;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        case 0x7D :
            Token_p->type = NH_CSS_TOKEN_RIGHT_CURLY_BRACKET;
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
        default :
            Token_p->type = NH_CSS_TOKEN_DELIM;
            Token_p->Delim.value = Tokenizer_p->codepoints_p[0];
            nh_css_advanceTokenizer(Tokenizer_p, 1);
            break;
    }

    return Token_p;
}

static NH_API_RESULT nh_css_consumeToken(
    nh_css_Tokenizer *Tokenizer_p) 
{
    nh_css_Token *Token_p = nh_core_incrementArray(&Tokenizer_p->Tokens);
    NH_CORE_CHECK_MEM(Token_p)

    NH_CORE_CHECK_NULL(nh_css_getToken(Tokenizer_p, Token_p))

    return NH_API_SUCCESS;
}

static nh_css_Token *nh_css_reconsumeToken(
    nh_css_Tokenizer *Tokenizer_p, nh_css_Token *Token_p) 
{
    NH_CORE_CHECK_NULL_2(NULL, nh_css_getToken(Tokenizer_p, Token_p))
    return nh_core_incrementArray(&Tokenizer_p->Tokens);
}

// TOKENIZE ========================================================================================

static nh_encoding_UTF32String nh_css_preprocessInputStream(
    NH_ENCODING_UTF32 *codepoints_p, unsigned long long length)
{
    nh_encoding_UTF32String String = nh_encoding_initUTF32(length + 1);

    NH_ENCODING_UTF32 linefeed = 0x0A;
    NH_ENCODING_UTF32 replace = 0xFFFD;

    for (int i = 0; i < length; ++i)
    {
        if (codepoints_p[i] == 0x0D) 
        {
            if (i + 2 < length && codepoints_p[i + 1] == 0x0D && codepoints_p[i + 2] == 0x0A) 
            {
                nh_encoding_appendUTF32(&String, &linefeed, 1);
                i += 2;
            }
            else {nh_encoding_appendUTF32(&String, &linefeed, 1);}
        }
        else if (codepoints_p[i] == 0x0C) {
            nh_encoding_appendUTF32(&String, &linefeed, 1);
        }
        else if (codepoints_p[i] == 0 || nh_encoding_isSurrogate(codepoints_p[i])) {
            nh_encoding_appendUTF32(&String, &replace, 1);
        }
        else {
            nh_encoding_appendUTF32(&String, &codepoints_p[i], 1);
        }
    }

    return String;
}

nh_core_Array nh_css_tokenize(
    nh_encoding_UTF32String *String_p)
{
    nh_encoding_UTF32String String = nh_css_preprocessInputStream(String_p->p, String_p->length);
    nh_css_Tokenizer Tokenizer = nh_css_initTokenizer(String);

    while (nh_css_consumeToken(&Tokenizer) == NH_API_SUCCESS) {
        if (((nh_css_Token*)Tokenizer.Tokens.p)[Tokenizer.Tokens.length - 1].type == NH_CSS_TOKEN_EOF) {
            break;
        }
    }

    nh_encoding_freeUTF32(&String);

    return Tokenizer.Tokens;
}
