// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Parser.h"

#include "../Common/IndexMap.h"
#include "../Common/Log.h"
#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// DATA ============================================================================================

const NH_BYTE *NH_WEBIDL_PARSE_NODE_NAMES_PP[] = {
    "Definitions",
    "Definition",
    "ArgumentNameKeyword",
    "CallbackOrInterfaceOrMixin",
    "InterfaceOrMixin",
    "InterfaceRest",
    "Partial",
    "PartialDefinition",
    "PartialInterfaceOrPartialMixin",
    "PartialInterfaceRest",
    "InterfaceMembers",
    "InterfaceMember",
    "PartialInterfaceMembers",
    "PartialInterfaceMember",
    "Inheritance",
    "MixinRest",
    "MixinMembers",
    "MixinMember",
    "IncludesStatement",
    "CallbackRestOrInterface",
    "CallbackInterfaceMembers",
    "CallbackInterfaceMember",
    "Const",
    "ConstValue",
    "BooleanLiteral",
    "FloatLiteral",
    "ConstType",
    "ReadOnlyMember",
    "ReadOnlyMemberRest",
    "ReadWriteAttribute",
    "InheritAttribute",
    "AttributeRest",
    "AttributeName",
    "AttributeNameKeyword",
    "OptionalReadOnly",
    "DefaultValue",
    "Operation",
    "RegularOperation",
    "SpecialOperation",
    "Special",
    "OperationRest",
    "OptionalOperationName",
    "OperationName",
    "OperationNameKeyword",
    "ArgumentList",
    "Arguments",
    "Argument",
    "ArgumentRest",
    "ArgumentName",
    "Ellipsis",
    "Constructor",
    "Stringifier",
    "StringifierRest",
    "StaticMember",
    "StaticMemberRest",
    "Iterable",
    "OptionalType",
    "AsyncIterable",
    "OptionalArgumentList",
    "ReadWriteMaplike",
    "MaplikeRest",
    "ReadWriteSetlike",
    "SetlikeRest",
    "Namespace",
    "NamespaceMembers",
    "NamespaceMember",
    "Dictionary",
    "DictionaryMembers",
    "DictionaryMember",
    "DictionaryMemberRest",
    "PartialDictionary",
    "Default",
    "Enum",
    "EnumValueList",
    "EnumValueListComma",
    "EnumValueListString",
    "CallbackRest",
    "Typedef",
    "Type",
    "TypeWithExtendedAttributes",
    "SingleType",
    "UnionType",
    "UnionMemberType",
    "UnionMemberTypes",
    "DistinguishableType",
    "PrimitiveType",
    "UnrestrictedFloatType",
    "FloatType",
    "UnsignedIntegerType",
    "IntegerType",
    "OptionalLong",
    "StringType",
    "PromiseType",
    "RecordType",
    "Null",
    "BufferRelatedType",
    "ExtendedAttributeList",
    "ExtendedAttributes",
    "ExtendedAttribute",
    "ExtendedAttributeRest",
    "ExtendedAttributeInner",
    "Other",
    "OtherOrComma",
    "IdentifierList",
    "Identifiers",
    "ExtendedAttributeNoArgs",
    "ExtendedAttributeArgList",
    "ExtendedAttributeIdent",
    "ExtendedAttributeIdentList",
    "ExtendedAttributeNamedArgList",
    "NON_STANDARD_Specifier",
};

size_t NH_WEBIDL_PARSE_NODE_NAMES_PP_COUNT = sizeof(NH_WEBIDL_PARSE_NODE_NAMES_PP) / sizeof(NH_WEBIDL_PARSE_NODE_NAMES_PP[0]);

// DECLARE =========================================================================================

typedef struct nh_webidl_ParseResult {
    nh_webidl_ParseNode *Node_p;
    nh_List SyntaxErrors;
} nh_webidl_ParseResult;

typedef struct nh_webidl_Parser {
    nh_webidl_Token *Tokens_p;
    int unparsed;
} nh_webidl_Parser;

static nh_webidl_ParseResult nh_webidl_parseExtendedAttribute(
    nh_webidl_Parser *Parser_p
);
static nh_webidl_ParseResult nh_webidl_parseType(
    nh_webidl_Parser *Parser_p
);
static nh_webidl_ParseResult nh_webidl_parseTypeWithExtendedAttributes(
    nh_webidl_Parser *Parser_p
);

// HELPER ==========================================================================================

static nh_webidl_ParseResult nh_webidl_initParseResult(
    nh_webidl_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult Result;
    Result.Node_p = Node_p;
    Result.SyntaxErrors = nh_core_initList(8);

NH_WEBIDL_END(Result)
}

static nh_webidl_ParseNode *nh_webidl_allocateNonTerminalParseNode(
    const NH_BYTE *name_p, int chunkSize)
{
NH_WEBIDL_BEGIN()

    unsigned int *index_p = nh_core_getFromHashMap(&NH_WEBIDL_INDEXMAP.ParseNodeNames, (NH_BYTE*)name_p);
    if (index_p == NULL) {NH_WEBIDL_END(NULL)}

    nh_webidl_ParseNode *Node_p = nh_core_allocate(sizeof(nh_webidl_ParseNode));
    Node_p->Children = nh_core_initList(chunkSize);
    Node_p->Token_p  = NULL;
    Node_p->type     = *index_p;

NH_WEBIDL_END(Node_p)
}

static nh_webidl_ParseNode *nh_webidl_allocateTerminalParseNode(
    nh_webidl_Token *Token_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *Node_p = nh_core_allocate(sizeof(nh_webidl_ParseNode));
    Node_p->Children = nh_core_initList(1);
    Node_p->Token_p  = Token_p;
    Node_p->type     = -1;

NH_WEBIDL_END(Node_p)
}

static void nh_webidl_freeParseNode(
    nh_webidl_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    // TODO

NH_WEBIDL_SILENT_END()
}

static nh_webidl_Parser nh_webidl_advanceParser(
    nh_webidl_Parser Parser, int steps)
{
NH_WEBIDL_BEGIN()

    Parser.unparsed -= steps;
    Parser.Tokens_p  = Parser.Tokens_p + steps;
 
NH_WEBIDL_END(Parser)
}

// PARSER ==========================================================================================

static nh_webidl_ParseResult nh_webidl_parseOther(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0 
    ||  Parser_p->Tokens_p[0].String.p[0] == ','
    ||  Parser_p->Tokens_p[0].String.p[0] == '('
    ||  Parser_p->Tokens_p[0].String.p[0] == '['
    ||  Parser_p->Tokens_p[0].String.p[0] == '{'
    ||  Parser_p->Tokens_p[0].String.p[0] == ')'
    ||  Parser_p->Tokens_p[0].String.p[0] == ']'
    ||  Parser_p->Tokens_p[0].String.p[0] == '}') 
    {
        NH_WEBIDL_END(nh_webidl_initParseResult(NULL))   
    }

    nh_webidl_ParseNode *Other_p = nh_webidl_allocateNonTerminalParseNode("Other", 1);
    nh_webidl_ParseNode *Child_p = nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
    nh_core_appendToList(&Other_p->Children, Child_p);
    *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);

NH_WEBIDL_END(nh_webidl_initParseResult(Other_p))
}

static nh_webidl_ParseResult nh_webidl_parseOtherOrComma(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult Result = nh_webidl_parseOther(Parser_p);

    if (Result.Node_p != NULL) {
        nh_webidl_ParseNode *OtherOrComma_p = nh_webidl_allocateNonTerminalParseNode("OtherOrComma", 1);
        nh_core_appendToList(&OtherOrComma_p->Children, Result.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(OtherOrComma_p))
    } 
    else if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == ',') 
    {
        nh_webidl_ParseNode *OtherOrComma_p = nh_webidl_allocateNonTerminalParseNode("OtherOrComma", 1);
        nh_webidl_ParseNode *Child_p = nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_core_appendToList(&OtherOrComma_p->Children, Child_p);
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(OtherOrComma_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseExtendedAttributeInner(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    NH_BYTE openBrackets_p[3] = "([{";
    NH_BYTE closeBrackets_p[3] = ")]}";

    for (int i = 0; i < 3; ++i) 
    {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == openBrackets_p[i]) 
        {
            nh_webidl_Parser LocalParser = *Parser_p;
            nh_webidl_ParseResult Result1 = nh_webidl_parseExtendedAttributeInner(&LocalParser);
    
            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == closeBrackets_p[i]) 
            {
                nh_webidl_ParseResult Result2 = nh_webidl_parseExtendedAttributeInner(&LocalParser);
                nh_webidl_ParseNode *ExtendedAttributeInner_p = nh_webidl_allocateNonTerminalParseNode("ExtendedAttributeInner", 4);
                nh_core_appendToList(&ExtendedAttributeInner_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&ExtendedAttributeInner_p->Children, Result1.Node_p);
                nh_core_appendToList(&ExtendedAttributeInner_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                nh_core_appendToList(&ExtendedAttributeInner_p->Children, Result2.Node_p);
                *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
                NH_WEBIDL_END(nh_webidl_initParseResult(ExtendedAttributeInner_p))
            }
            else {nh_webidl_freeParseNode(Result1.Node_p);}
        }
    }

    nh_webidl_ParseNode *ExtendedAttributeInner_p = nh_webidl_allocateNonTerminalParseNode("ExtendedAttributeInner", 1);
    nh_webidl_ParseResult Result = nh_webidl_parseOtherOrComma(Parser_p);

    if (Result.Node_p != NULL) {
        nh_core_appendToList(&ExtendedAttributeInner_p->Children, Result.Node_p);
        nh_webidl_ParseResult Result = nh_webidl_parseExtendedAttributeInner(Parser_p);
        nh_core_appendToList(&ExtendedAttributeInner_p->Children, Result.Node_p);
    } 

NH_WEBIDL_END(nh_webidl_initParseResult(ExtendedAttributeInner_p))
}

static nh_webidl_ParseResult nh_webidl_parseExtendedAttributeRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult Result = nh_webidl_parseExtendedAttribute(Parser_p);
    nh_webidl_ParseNode *ExtendedAttributeRest_p = nh_webidl_allocateNonTerminalParseNode("ExtendedAttributeRest", 1);

    if (Result.Node_p != NULL) {
        nh_core_appendToList(&ExtendedAttributeRest_p->Children, Result.Node_p);
    } 

NH_WEBIDL_END(nh_webidl_initParseResult(ExtendedAttributeRest_p))
}

static nh_webidl_ParseResult nh_webidl_parseExtendedAttribute(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    NH_BYTE openBrackets_p[3] = "([{";
    NH_BYTE closeBrackets_p[3] = ")]}";

    for (int i = 0; i < 3; ++i) 
    {
        if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == openBrackets_p[i]) 
        {
            nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
            nh_webidl_ParseResult ExtendedAttributeInner = nh_webidl_parseExtendedAttributeInner(&LocalParser);
    
            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == closeBrackets_p[i]) 
            {
                nh_webidl_ParseResult ExtendedAttributeRest = nh_webidl_parseExtendedAttributeRest(&LocalParser);
                nh_webidl_ParseNode *ExtendedAttribute_p = nh_webidl_allocateNonTerminalParseNode("ExtendedAttribute", 4);

                nh_core_appendToList(&ExtendedAttribute_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeInner.Node_p);
                nh_core_appendToList(&ExtendedAttribute_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                nh_core_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeRest.Node_p);

                *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
                NH_WEBIDL_END(nh_webidl_initParseResult(ExtendedAttribute_p))
            }
            else {nh_webidl_freeParseNode(ExtendedAttributeInner.Node_p);}
        }
    }

    nh_webidl_ParseResult Other = nh_webidl_parseOther(Parser_p);

    if (Other.Node_p != NULL) 
    {
        nh_webidl_ParseResult ExtendedAttributeRest = nh_webidl_parseExtendedAttributeRest(Parser_p);
        nh_webidl_ParseNode *ExtendedAttribute_p = nh_webidl_allocateNonTerminalParseNode("ExtendedAttribute", 2);
        nh_core_appendToList(&ExtendedAttribute_p->Children, Other.Node_p);
        nh_core_appendToList(&ExtendedAttribute_p->Children, ExtendedAttributeRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ExtendedAttribute_p))
    } 

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseExtendedAttributes(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *ExtendedAttributes_p = nh_webidl_allocateNonTerminalParseNode("ExtendedAttributes", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == ',') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult ExtendedAttribute = nh_webidl_parseExtendedAttribute(&LocalParser);

        if (ExtendedAttribute.Node_p != NULL) 
        {
            nh_webidl_ParseResult ExtendedAttributes = nh_webidl_parseExtendedAttributes(&LocalParser);
            nh_core_appendToList(&ExtendedAttributes_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&ExtendedAttributes_p->Children, ExtendedAttribute.Node_p);
            nh_core_appendToList(&ExtendedAttributes_p->Children, ExtendedAttributes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(ExtendedAttributes_p))
}

static nh_webidl_ParseResult nh_webidl_parseExtendedAttributeList(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *ExtendedAttributeList_p = nh_webidl_allocateNonTerminalParseNode("ExtendedAttributeList", 1);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == '[') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult ExtendedAttribute = nh_webidl_parseExtendedAttribute(&LocalParser);

        if (ExtendedAttribute.Node_p != NULL) 
        {
            nh_webidl_ParseResult ExtendedAttributes = nh_webidl_parseExtendedAttributes(&LocalParser);

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ']') 
            {
                nh_core_appendToList(&ExtendedAttributeList_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&ExtendedAttributeList_p->Children, ExtendedAttribute.Node_p);
                nh_core_appendToList(&ExtendedAttributeList_p->Children, ExtendedAttributes.Node_p);
                nh_core_appendToList(&ExtendedAttributeList_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
            }
            else {
                nh_webidl_freeParseNode(ExtendedAttributes.Node_p);
                nh_webidl_freeParseNode(ExtendedAttribute.Node_p);
            }
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(ExtendedAttributeList_p))
}

static nh_webidl_ParseResult nh_webidl_parseOptionalLong(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *OptionalLong_p = nh_webidl_allocateNonTerminalParseNode("OptionalLong", 1);

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "long")) 
    {
        nh_core_appendToList(&OptionalLong_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(OptionalLong_p))
}

static nh_webidl_ParseResult nh_webidl_parseIntegerType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "short"))
    {
        nh_webidl_ParseNode *IntegerType_p = nh_webidl_allocateNonTerminalParseNode("IntegerType", 1);
        nh_core_appendToList(&IntegerType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(IntegerType_p))
    }

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "long")) 
    {
        nh_webidl_ParseNode *IntegerType_p = nh_webidl_allocateNonTerminalParseNode("IntegerType", 1);
        nh_core_appendToList(&IntegerType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult OptionalLong = nh_webidl_parseOptionalLong(Parser_p);
        nh_core_appendToList(&IntegerType_p->Children, OptionalLong.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(IntegerType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseUnsignedIntegerType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "unsigned"))
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult IntegerType = nh_webidl_parseIntegerType(&LocalParser);

        if (IntegerType.Node_p != NULL) 
        {
            nh_webidl_ParseNode *UnsignedIntegerType_p = nh_webidl_allocateNonTerminalParseNode("UnsignedIntegerType", 1);
            nh_webidl_ParseNode *Unsigned_p = nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
            nh_core_appendToList(&UnsignedIntegerType_p->Children, Unsigned_p); 
            nh_core_appendToList(&UnsignedIntegerType_p->Children, IntegerType.Node_p); 
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(UnsignedIntegerType_p))
        }
    }

    nh_webidl_ParseResult IntegerType = nh_webidl_parseIntegerType(Parser_p);
    if (IntegerType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *UnsignedIntegerType_p = nh_webidl_allocateNonTerminalParseNode("UnsignedIntegerType", 1);
        nh_core_appendToList(&UnsignedIntegerType_p->Children, IntegerType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(UnsignedIntegerType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseFloatType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "float")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "double"))
    {
        nh_webidl_ParseNode *FloatType_p = nh_webidl_allocateNonTerminalParseNode("FloatType", 1);
        nh_webidl_ParseNode *Terminal_p = nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_core_appendToList(&FloatType_p->Children, Terminal_p);
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(FloatType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseUnrestrictedFloatType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "unrestricted"))
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult FloatType = nh_webidl_parseFloatType(&LocalParser);

        if (FloatType.Node_p != NULL) 
        {
            nh_webidl_ParseNode *UnrestrictedFloatType_p = nh_webidl_allocateNonTerminalParseNode("UnrestrictedFloatType", 1);
            nh_webidl_ParseNode *Unrestricted_p = nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
            nh_core_appendToList(&UnrestrictedFloatType_p->Children, Unrestricted_p); 
            nh_core_appendToList(&UnrestrictedFloatType_p->Children, FloatType.Node_p); 
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(UnrestrictedFloatType_p))
        }
    }

    nh_webidl_ParseResult FloatType = nh_webidl_parseFloatType(Parser_p);
    if (FloatType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *UnrestrictedFloatType_p = nh_webidl_allocateNonTerminalParseNode("UnrestrictedFloatType", 1);
        nh_core_appendToList(&UnrestrictedFloatType_p->Children, FloatType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(UnrestrictedFloatType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parsePrimitiveType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "undefined")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "boolean")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "byte")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "octet")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "bigint")) 
    {
        nh_webidl_ParseNode *PrimitiveType_p = nh_webidl_allocateNonTerminalParseNode("PrimitiveType", 1);
        nh_core_appendToList(&PrimitiveType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1); 
        NH_WEBIDL_END(nh_webidl_initParseResult(PrimitiveType_p))
    }

    nh_webidl_ParseResult UnsignedIntegerType = nh_webidl_parseUnsignedIntegerType(Parser_p);
    if (UnsignedIntegerType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PrimitiveType_p = nh_webidl_allocateNonTerminalParseNode("PrimitiveType", 1);
        nh_core_appendToList(&PrimitiveType_p->Children, UnsignedIntegerType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PrimitiveType_p))
    }

    nh_webidl_ParseResult UnrestrictedFloatType = nh_webidl_parseUnrestrictedFloatType(Parser_p);
    if (UnrestrictedFloatType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PrimitiveType_p = nh_webidl_allocateNonTerminalParseNode("PrimitiveType", 1);
        nh_core_appendToList(&PrimitiveType_p->Children, UnrestrictedFloatType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PrimitiveType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseNull(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *Null_p = nh_webidl_allocateNonTerminalParseNode("Null", 1);

    if (Parser_p->Tokens_p[0].String.p[0] == '?') {
        nh_core_appendToList(&Null_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(Null_p))
}

static nh_webidl_ParseResult nh_webidl_parseStringType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "ByteString") 
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "DOMString") 
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "USVString")) 
    {
        nh_webidl_ParseNode *StringType_p = nh_webidl_allocateNonTerminalParseNode("StringType", 1);
        nh_core_appendToList(&StringType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(StringType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseBufferRelatedType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}
    
    if (!strcmp(Parser_p->Tokens_p[0].String.p, "ArrayBuffer")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "DataView")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Int8Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Int16Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Int32Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Uint8Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Uint16Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Uint32Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Uint8ClampedArray")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Float32Array")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Float64Array"))
    {
        nh_webidl_ParseNode *BufferRelatedType_p = nh_webidl_allocateNonTerminalParseNode("BufferRelatedType", 1);
        nh_core_appendToList(&BufferRelatedType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(BufferRelatedType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseRecordType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "record") && Parser_p->Tokens_p[1].String.p[0] == '<')
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
        nh_webidl_ParseResult StringType = nh_webidl_parseStringType(&LocalParser);

        if (StringType.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.p[0] == ',')
        {
            nh_webidl_Token *Comma_p = &LocalParser.Tokens_p[0];
            LocalParser = nh_webidl_advanceParser(LocalParser, 1);
            nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

            if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed >= 1 && LocalParser.Tokens_p[0].String.p[0] == '>')
            {
                nh_webidl_ParseNode *RecordType_p = nh_webidl_allocateNonTerminalParseNode("RecordType", 6);
                nh_core_appendToList(&RecordType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&RecordType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                nh_core_appendToList(&RecordType_p->Children, StringType.Node_p);
                nh_core_appendToList(&RecordType_p->Children, nh_webidl_allocateTerminalParseNode(Comma_p));
                nh_core_appendToList(&RecordType_p->Children, TypeWithExtendedAttributes.Node_p);
                nh_core_appendToList(&RecordType_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
                NH_WEBIDL_END(nh_webidl_initParseResult(RecordType_p))
            }
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDistinguishableType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult PrimitiveType = nh_webidl_parsePrimitiveType(Parser_p);
    if (PrimitiveType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *DistinguishableType_p = nh_webidl_allocateNonTerminalParseNode("DistinguishableType", 1);
        nh_core_appendToList(&DistinguishableType_p->Children, PrimitiveType.Node_p);
        nh_webidl_ParseResult Null = nh_webidl_parseNull(Parser_p);
        nh_core_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(DistinguishableType_p))
    } 
    
    nh_webidl_ParseResult StringType = nh_webidl_parseStringType(Parser_p);
    if (StringType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *DistinguishableType_p = nh_webidl_allocateNonTerminalParseNode("DistinguishableType", 1);
        nh_core_appendToList(&DistinguishableType_p->Children, StringType.Node_p);
        nh_webidl_ParseResult Null = nh_webidl_parseNull(Parser_p);
        nh_core_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(DistinguishableType_p))
    } 

    nh_webidl_ParseResult BufferRelatedType = nh_webidl_parseStringType(Parser_p);
    if (BufferRelatedType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *DistinguishableType_p = nh_webidl_allocateNonTerminalParseNode("DistinguishableType", 1);
        nh_core_appendToList(&DistinguishableType_p->Children, BufferRelatedType.Node_p);
        nh_webidl_ParseResult Null = nh_webidl_parseNull(Parser_p);
        nh_core_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(DistinguishableType_p))
    } 

    nh_webidl_ParseResult RecordType = nh_webidl_parseRecordType(Parser_p);
    if (RecordType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *DistinguishableType_p = nh_webidl_allocateNonTerminalParseNode("DistinguishableType", 1);
        nh_core_appendToList(&DistinguishableType_p->Children, RecordType.Node_p);
        nh_webidl_ParseResult Null = nh_webidl_parseNull(Parser_p);
        nh_core_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(DistinguishableType_p))
    } 

    if (Parser_p->unparsed > 3)
    {
        if (!strcmp(Parser_p->Tokens_p[0].String.p, "sequence") && Parser_p->Tokens_p[1].String.p[0] == '<'
        ||  !strcmp(Parser_p->Tokens_p[0].String.p, "FrozenArray") && Parser_p->Tokens_p[1].String.p[0] == '<'
        ||  !strcmp(Parser_p->Tokens_p[0].String.p, "ObservableArray") && Parser_p->Tokens_p[1].String.p[0] == '<')
        {
            nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
            nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

            if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == '>')
            {
                nh_webidl_ParseNode *DistinguishableType_p = nh_webidl_allocateNonTerminalParseNode("DistinguishableType", 5);
                nh_core_appendToList(&DistinguishableType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&DistinguishableType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                nh_core_appendToList(&DistinguishableType_p->Children, TypeWithExtendedAttributes.Node_p);
                nh_core_appendToList(&DistinguishableType_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
                nh_webidl_ParseResult Null = nh_webidl_parseNull(Parser_p);
                nh_core_appendToList(&DistinguishableType_p->Children, Null.Node_p);
                NH_WEBIDL_END(nh_webidl_initParseResult(DistinguishableType_p))
            }
        }
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        nh_webidl_ParseNode *DistinguishableType_p = nh_webidl_allocateNonTerminalParseNode("DistinguishableType", 1);
        nh_core_appendToList(&DistinguishableType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult Null = nh_webidl_parseNull(Parser_p);
        nh_core_appendToList(&DistinguishableType_p->Children, Null.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(DistinguishableType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parsePromiseType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 3 && !strcmp(Parser_p->Tokens_p[0].String.p, "Promise") && Parser_p->Tokens_p[1].String.p[0] == '<') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
        nh_webidl_ParseResult Type = nh_webidl_parseType(&LocalParser);

        if (Type.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == '>')
        {
            nh_webidl_ParseNode *PromiseType_p = nh_webidl_allocateNonTerminalParseNode("PromiseType", 4);
            nh_core_appendToList(&PromiseType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&PromiseType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&PromiseType_p->Children, Type.Node_p);
            nh_core_appendToList(&PromiseType_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
            NH_WEBIDL_END(nh_webidl_initParseResult(PromiseType_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseSingleType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult PromiseType = nh_webidl_parsePromiseType(Parser_p);
    if (PromiseType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *SingleType_p = nh_webidl_allocateNonTerminalParseNode("SingleType", 1);
        nh_core_appendToList(&SingleType_p->Children, PromiseType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(SingleType_p))
    } 

    nh_webidl_ParseResult DistinguishableType = nh_webidl_parseDistinguishableType(Parser_p);
    if (DistinguishableType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *SingleType_p = nh_webidl_allocateNonTerminalParseNode("SingleType", 1);
        nh_core_appendToList(&SingleType_p->Children, DistinguishableType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(SingleType_p))
    }
 
    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "any"))
    {
        nh_webidl_ParseNode *SingleType_p = nh_webidl_allocateNonTerminalParseNode("SingleType", 1);
        nh_core_appendToList(&SingleType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(SingleType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseUnionType(
    nh_webidl_Parser *Parser_p
);

static nh_webidl_ParseResult nh_webidl_parseUnionMemberType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(Parser_p);
    nh_webidl_ParseResult DistinguishableType = nh_webidl_parseDistinguishableType(Parser_p);

    if (DistinguishableType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *UnionMemberType_p = nh_webidl_allocateNonTerminalParseNode("UnionMemberType", 2);
        nh_core_appendToList(&UnionMemberType_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&UnionMemberType_p->Children, DistinguishableType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(UnionMemberType_p))
    }
    else {nh_webidl_freeParseNode(DistinguishableType.Node_p);}

    nh_webidl_ParseResult UnionType = nh_webidl_parseUnionType(Parser_p);

    if (UnionType.Node_p != NULL)
    {
        nh_webidl_ParseResult Null = nh_webidl_parseNull(Parser_p);
        nh_webidl_ParseNode *UnionMemberType_p = nh_webidl_allocateNonTerminalParseNode("UnionMemberType", 2);
        nh_core_appendToList(&UnionMemberType_p->Children, UnionType.Node_p);
        nh_core_appendToList(&UnionMemberType_p->Children, Null.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(UnionMemberType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseUnionMemberTypes(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *UnionMemberTypes_p = nh_webidl_allocateNonTerminalParseNode("UnionMemberTypes", 3);

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "or")) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult UnionMemberType = nh_webidl_parseUnionMemberType(&LocalParser);

        if (UnionMemberType.Node_p != NULL) 
        {
            nh_webidl_ParseResult UnionMemberTypes = nh_webidl_parseUnionMemberTypes(&LocalParser);
            nh_core_appendToList(&UnionMemberTypes_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&UnionMemberTypes_p->Children, UnionMemberType.Node_p);
            nh_core_appendToList(&UnionMemberTypes_p->Children, UnionMemberTypes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(UnionMemberTypes_p))
}

static nh_webidl_ParseResult nh_webidl_parseUnionType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == '(') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult UnionMemberType1 = nh_webidl_parseUnionMemberType(&LocalParser);

        if (UnionMemberType1.Node_p != NULL && LocalParser.unparsed > 0 && !strcmp(LocalParser.Tokens_p[0].String.p, "or")) 
        {
            nh_webidl_Token *OrToken_p = &LocalParser.Tokens_p[0];
            LocalParser = nh_webidl_advanceParser(LocalParser, 1);
            nh_webidl_ParseResult UnionMemberType2 = nh_webidl_parseUnionMemberType(&LocalParser);

            if (UnionMemberType2.Node_p != NULL) 
            {
                nh_webidl_ParseResult UnionMemberTypes = nh_webidl_parseUnionMemberTypes(&LocalParser);
                if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ')') 
                {
                    nh_webidl_ParseNode *UnionType_p = nh_webidl_allocateNonTerminalParseNode("UnionType", 6);
                    nh_core_appendToList(&UnionType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    nh_core_appendToList(&UnionType_p->Children, UnionMemberType1.Node_p);
                    nh_core_appendToList(&UnionType_p->Children, nh_webidl_allocateTerminalParseNode(OrToken_p));
                    nh_core_appendToList(&UnionType_p->Children, UnionMemberType2.Node_p);
                    nh_core_appendToList(&UnionType_p->Children, UnionMemberTypes.Node_p);
                    nh_core_appendToList(&UnionType_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
                    NH_WEBIDL_END(nh_webidl_initParseResult(UnionType_p))
                }
            }
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult SingleType = nh_webidl_parseSingleType(Parser_p);
    if (SingleType.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Type_p = nh_webidl_allocateNonTerminalParseNode("Type", 1);
        nh_core_appendToList(&Type_p->Children, SingleType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Type_p))
    } 
   
    nh_webidl_ParseResult UnionType = nh_webidl_parseUnionType(Parser_p);
    if (UnionType.Node_p != NULL) 
    {
        nh_webidl_ParseResult Null = nh_webidl_parseNull(Parser_p);
        nh_webidl_ParseNode *Type_p = nh_webidl_allocateNonTerminalParseNode("Type", 1);
        nh_core_appendToList(&Type_p->Children, UnionType.Node_p);
        nh_core_appendToList(&Type_p->Children, Null.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Type_p))
    } 

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseTypeWithExtendedAttributes(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Parser LocalParser = *Parser_p;

    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(&LocalParser);
    nh_webidl_ParseResult Type = nh_webidl_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        nh_webidl_ParseNode *TypeWithExtendedAttributes_p = nh_webidl_allocateNonTerminalParseNode("TypeWithExtendedAttributes", 2);
        nh_core_appendToList(&TypeWithExtendedAttributes_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&TypeWithExtendedAttributes_p->Children, Type.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(nh_webidl_initParseResult(TypeWithExtendedAttributes_p))
    }

    nh_webidl_freeParseNode(ExtendedAttributeList.Node_p);

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseArgumentNameKeyword(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "async")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "attribute")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "callback")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "const")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "constructor")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "deleter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "dictionary")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "enum")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "getter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "includes")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "inherit")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "interface")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "iterable")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "maplike")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "mixin")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "namespace")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "partial")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "readonly")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "required")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "setlike")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "setter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "static")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "stringifier")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "typedef")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "unrestricted"))
    {
        nh_webidl_ParseNode *ArgumentNameKeyword_p = nh_webidl_allocateNonTerminalParseNode("ArgumentNameKeyword", 1);
        nh_core_appendToList(&ArgumentNameKeyword_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(ArgumentNameKeyword_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseArgumentName(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult ArgumentNameKeyword = nh_webidl_parseArgumentNameKeyword(Parser_p);

    if (ArgumentNameKeyword.Node_p != NULL) 
    {
        nh_webidl_ParseNode *ArgumentName_p = nh_webidl_allocateNonTerminalParseNode("ArgumentName", 1);
        nh_core_appendToList(&ArgumentName_p->Children, ArgumentNameKeyword.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ArgumentName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        nh_webidl_ParseNode *ArgumentName_p = nh_webidl_allocateNonTerminalParseNode("ArgumentName", 1);
        nh_core_appendToList(&ArgumentName_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(ArgumentName_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseBooleanLiteral(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "true")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "false"))
    {
        nh_webidl_ParseNode *BooleanLiteral_p = nh_webidl_allocateNonTerminalParseNode("BooleanLiteral", 1);
        nh_core_appendToList(&BooleanLiteral_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(BooleanLiteral_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseFloatLiteral(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "-Infinity")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "Infinity")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "NaN")
    ||  nh_webidl_isNumericToken(&Parser_p->Tokens_p[0]))
    {
        nh_webidl_ParseNode *FloatLiteral_p = nh_webidl_allocateNonTerminalParseNode("FloatLiteral", 1);
        nh_core_appendToList(&FloatLiteral_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(FloatLiteral_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseConstValue(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    nh_webidl_ParseResult BooleanLiteral = nh_webidl_parseBooleanLiteral(Parser_p);
    if (BooleanLiteral.Node_p != NULL) {
        nh_webidl_ParseNode *ConstValue_p = nh_webidl_allocateNonTerminalParseNode("ConstValue", 1);
        nh_core_appendToList(&ConstValue_p->Children, BooleanLiteral.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ConstValue_p))
    }

    nh_webidl_ParseResult FloatLiteral = nh_webidl_parseFloatLiteral(Parser_p);
    if (FloatLiteral.Node_p != NULL) {
        nh_webidl_ParseNode *ConstValue_p = nh_webidl_allocateNonTerminalParseNode("ConstValue", 1);
        nh_core_appendToList(&ConstValue_p->Children, FloatLiteral.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ConstValue_p))
    }

    if (nh_webidl_isNumericToken(&Parser_p->Tokens_p[0])) 
    {
        nh_webidl_ParseNode *ConstValue_p = nh_webidl_allocateNonTerminalParseNode("ConstValue", 1);
        nh_core_appendToList(&ConstValue_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(ConstValue_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDefaultValue(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    nh_webidl_ParseResult ConstValue = nh_webidl_parseConstValue(Parser_p);
    if (ConstValue.Node_p != NULL) 
    {
        nh_webidl_ParseNode *DefaultValue_p = nh_webidl_allocateNonTerminalParseNode("DefaultValue", 1);
        nh_core_appendToList(&DefaultValue_p->Children, ConstValue.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(DefaultValue_p))
    }

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "null")) 
    {
        nh_webidl_ParseNode *DefaultValue_p = nh_webidl_allocateNonTerminalParseNode("DefaultValue", 1);
        nh_core_appendToList(&DefaultValue_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(DefaultValue_p))
    }

    if (Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_STRING) 
    {
        nh_webidl_ParseNode *DefaultValue_p = nh_webidl_allocateNonTerminalParseNode("DefaultValue", 1);
        nh_core_appendToList(&DefaultValue_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(DefaultValue_p))
    }

    if (Parser_p->unparsed <= 1) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if ((Parser_p->Tokens_p[0].String.p[0] == '[' && Parser_p->Tokens_p[1].String.p[0] == ']')
    ||  (Parser_p->Tokens_p[0].String.p[0] == '{' && Parser_p->Tokens_p[1].String.p[0] == '}'))
    {
        nh_webidl_ParseNode *DefaultValue_p = nh_webidl_allocateNonTerminalParseNode("DefaultValue", 1);
        nh_core_appendToList(&DefaultValue_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        nh_core_appendToList(&DefaultValue_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 2);
        NH_WEBIDL_END(nh_webidl_initParseResult(DefaultValue_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDefault(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *Default_p = nh_webidl_allocateNonTerminalParseNode("Default", 1);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == '=') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult DefaultValue = nh_webidl_parseDefaultValue(&LocalParser);

        if (DefaultValue.Node_p != NULL) 
        {
            nh_core_appendToList(&Default_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&Default_p->Children, DefaultValue.Node_p);
            *Parser_p = LocalParser;    
        }
    } 

NH_WEBIDL_END(nh_webidl_initParseResult(Default_p))
}

static nh_webidl_ParseResult nh_webidl_parseEllipsis(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *Ellipsis_p = nh_webidl_allocateNonTerminalParseNode("Ellipsis", 1);

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "...")) 
    {
        nh_webidl_ParseNode *Terminal_p = nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]);
        nh_core_appendToList(&Ellipsis_p->Children, Terminal_p);
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(Ellipsis_p))
}

static nh_webidl_ParseResult nh_webidl_parseArgumentRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp("optional", Parser_p->Tokens_p[0].String.p)) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1); 
        nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL) 
        {
            nh_webidl_ParseResult ArgumentName = nh_webidl_parseArgumentName(&LocalParser);

            if (ArgumentName.Node_p != NULL) 
            {
                nh_webidl_ParseResult Default = nh_webidl_parseDefault(&LocalParser);
                nh_webidl_ParseNode *ArgumentRest_p = nh_webidl_allocateNonTerminalParseNode("ArgumentRest", 4);
                nh_core_appendToList(&ArgumentRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&ArgumentRest_p->Children, TypeWithExtendedAttributes.Node_p);
                nh_core_appendToList(&ArgumentRest_p->Children, ArgumentName.Node_p);
                nh_core_appendToList(&ArgumentRest_p->Children, Default.Node_p);
                *Parser_p = LocalParser;
                NH_WEBIDL_END(nh_webidl_initParseResult(ArgumentRest_p))
            }
            nh_webidl_freeParseNode(TypeWithExtendedAttributes.Node_p);
        }
        NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
    }

    nh_webidl_Parser LocalParser = *Parser_p;
    nh_webidl_ParseResult Type = nh_webidl_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        nh_webidl_ParseResult Ellipsis = nh_webidl_parseEllipsis(&LocalParser);
        nh_webidl_ParseResult ArgumentName = nh_webidl_parseArgumentName(&LocalParser);

        if (ArgumentName.Node_p != NULL) 
        {
            nh_webidl_ParseNode *ArgumentRest_p = nh_webidl_allocateNonTerminalParseNode("ArgumentRest", 3);
            nh_core_appendToList(&ArgumentRest_p->Children, Type.Node_p);
            nh_core_appendToList(&ArgumentRest_p->Children, Ellipsis.Node_p);
            nh_core_appendToList(&ArgumentRest_p->Children, ArgumentName.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(ArgumentRest_p))
        }

        nh_webidl_freeParseNode(Type.Node_p);
        nh_webidl_freeParseNode(Ellipsis.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseArgument(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Parser LocalParser = *Parser_p;

    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(&LocalParser);
    nh_webidl_ParseResult ArgumentRest = nh_webidl_parseArgumentRest(&LocalParser);

    if (ArgumentRest.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Argument_p = nh_webidl_allocateNonTerminalParseNode("Argument", 2);
        nh_core_appendToList(&Argument_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&Argument_p->Children, ArgumentRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(nh_webidl_initParseResult(Argument_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseArguments(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *Arguments_p = nh_webidl_allocateNonTerminalParseNode("Arguments", 3);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == ',') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult Argument = nh_webidl_parseArgument(&LocalParser);

        if (Argument.Node_p != NULL) 
        {
            nh_webidl_ParseResult Arguments = nh_webidl_parseArguments(&LocalParser);
            nh_core_appendToList(&Arguments_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&Arguments_p->Children, Argument.Node_p);
            nh_core_appendToList(&Arguments_p->Children, Arguments.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(Arguments_p))
}

static nh_webidl_ParseResult nh_webidl_parseArgumentList(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *ArgumentList_p = nh_webidl_allocateNonTerminalParseNode("ArgumentList", 2);

    nh_webidl_ParseResult Argument = nh_webidl_parseArgument(Parser_p);
    if (Argument.Node_p != NULL) 
    {
        nh_webidl_ParseResult Arguments = nh_webidl_parseArguments(Parser_p);
        nh_core_appendToList(&ArgumentList_p->Children, Argument.Node_p);
        nh_core_appendToList(&ArgumentList_p->Children, Arguments.Node_p);
    } 
   
NH_WEBIDL_END(nh_webidl_initParseResult(ArgumentList_p))
}

static nh_webidl_ParseResult nh_webidl_parseCallbackRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == '=') 
        {
            nh_webidl_Token *Equal_p = &LocalParser.Tokens_p[0];
            LocalParser = nh_webidl_advanceParser(LocalParser, 1);
            nh_webidl_ParseResult Type = nh_webidl_parseType(&LocalParser);

            if (Type.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == '(') 
            {
                nh_webidl_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
                LocalParser = nh_webidl_advanceParser(LocalParser, 1);
                nh_webidl_ParseResult ArgumentList = nh_webidl_parseArgumentList(&LocalParser);

                if (LocalParser.unparsed > 1 
                &&  LocalParser.Tokens_p[0].String.p[0] == ')'
                &&  LocalParser.Tokens_p[1].String.p[0] == ';')
                {
                    nh_webidl_ParseNode *CallbackRest_p = nh_webidl_allocateNonTerminalParseNode("CallbackRest", 1);
                    nh_core_appendToList(&CallbackRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
                    nh_core_appendToList(&CallbackRest_p->Children, nh_webidl_allocateTerminalParseNode(Equal_p)); 
                    nh_core_appendToList(&CallbackRest_p->Children, Type.Node_p); 
                    nh_core_appendToList(&CallbackRest_p->Children, nh_webidl_allocateTerminalParseNode(LeftBracket_p)); 
                    nh_core_appendToList(&CallbackRest_p->Children, ArgumentList.Node_p); 
                    nh_core_appendToList(&CallbackRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0])); 
                    nh_core_appendToList(&CallbackRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1])); 
                    *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
                    NH_WEBIDL_END(nh_webidl_initParseResult(CallbackRest_p))
                }

                nh_webidl_freeParseNode(ArgumentList.Node_p);
            }

            nh_webidl_freeParseNode(Type.Node_p);
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseConstType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult PrimitiveType = nh_webidl_parsePrimitiveType(Parser_p);
 
    if (PrimitiveType.Node_p != NULL) {
        nh_webidl_ParseNode *ConstType_p = nh_webidl_allocateNonTerminalParseNode("ConstType", 1);
        nh_core_appendToList(&ConstType_p->Children, PrimitiveType.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ConstType_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) {
        nh_webidl_ParseNode *ConstType_p = nh_webidl_allocateNonTerminalParseNode("ConstType", 1);
        nh_core_appendToList(&ConstType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        NH_WEBIDL_END(nh_webidl_initParseResult(ConstType_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseConst(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 0 || strcmp(Parser_p->Tokens_p[0].String.p, "const")) {
        NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
    }

    nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
    nh_webidl_ParseResult ConstType = nh_webidl_parseConstType(&LocalParser);
 
    if (ConstType.Node_p != NULL) 
    {
        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
        {
            nh_webidl_Token *IdentifierToken_p = &LocalParser.Tokens_p[0];
            LocalParser = nh_webidl_advanceParser(LocalParser, 1);   

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == '=') 
            {
                nh_webidl_Token *EqualToken_p = &LocalParser.Tokens_p[0];
                LocalParser = nh_webidl_advanceParser(LocalParser, 1);   

                nh_webidl_ParseResult ConstValue = nh_webidl_parseConstValue(&LocalParser);

                if (ConstValue.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ';') 
                {
                    nh_webidl_ParseNode *Const_p = nh_webidl_allocateNonTerminalParseNode("Const", 6);
                    nh_core_appendToList(&Const_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    nh_core_appendToList(&Const_p->Children, ConstType.Node_p);
                    nh_core_appendToList(&Const_p->Children, nh_webidl_allocateTerminalParseNode(IdentifierToken_p));
                    nh_core_appendToList(&Const_p->Children, nh_webidl_allocateTerminalParseNode(EqualToken_p));
                    nh_core_appendToList(&Const_p->Children, ConstValue.Node_p);
                    nh_core_appendToList(&Const_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
                    NH_WEBIDL_END(nh_webidl_initParseResult(Const_p))
                }

                nh_webidl_freeParseNode(ConstValue.Node_p);
            }
        }

        nh_webidl_freeParseNode(ConstType.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseOperationNameKeyword(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "includes")) 
    {
        nh_webidl_ParseNode *OperationNameKeyword_p = nh_webidl_allocateNonTerminalParseNode("OperationNameKeyword", 1);
        nh_core_appendToList(&OperationNameKeyword_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(OperationNameKeyword_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseOperationName(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult OperationNameKeyword = nh_webidl_parseOperationNameKeyword(Parser_p);
    if (OperationNameKeyword.Node_p != NULL) {
        nh_webidl_ParseNode *OperationName_p = nh_webidl_allocateNonTerminalParseNode("OperationName", 1);
        nh_core_appendToList(&OperationName_p->Children, OperationNameKeyword.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(OperationName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) {
        nh_webidl_ParseNode *OperationName_p = nh_webidl_allocateNonTerminalParseNode("OperationName", 1);
        nh_core_appendToList(&OperationName_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0])); 
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(OperationName_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseOptionalOperationName(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *OptionalOperationName_p = nh_webidl_allocateNonTerminalParseNode("OptionalOperationName", 1);

    nh_webidl_ParseResult OperationName = nh_webidl_parseOperationName(Parser_p);
    if (OperationName.Node_p != NULL) {
        nh_core_appendToList(&OptionalOperationName_p->Children, OperationName.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(OptionalOperationName_p))
}

static nh_webidl_ParseResult nh_webidl_parseOperationRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Parser LocalParser = *Parser_p;
    nh_webidl_ParseResult OptionalOperationName = nh_webidl_parseOptionalOperationName(&LocalParser);

    if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == '(') 
    {
        nh_webidl_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
        LocalParser = nh_webidl_advanceParser(LocalParser, 1);
        nh_webidl_ParseResult ArgumentList = nh_webidl_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 1 
        &&  LocalParser.Tokens_p[0].String.p[0] == ')'
        &&  LocalParser.Tokens_p[1].String.p[0] == ';')
        {
            nh_webidl_ParseNode *OperationRest_p = nh_webidl_allocateNonTerminalParseNode("OperationRest", 5);
            nh_core_appendToList(&OperationRest_p->Children, OptionalOperationName.Node_p); 
            nh_core_appendToList(&OperationRest_p->Children, nh_webidl_allocateTerminalParseNode(LeftBracket_p));
            nh_core_appendToList(&OperationRest_p->Children, ArgumentList.Node_p);
            nh_core_appendToList(&OperationRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&OperationRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(OperationRest_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseRegularOperation(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Parser LocalParser = *Parser_p;
    nh_webidl_ParseResult Type = nh_webidl_parseType(&LocalParser);

    if (Type.Node_p != NULL) 
    {
        nh_webidl_ParseResult OperationRest = nh_webidl_parseOperationRest(&LocalParser);

        if (OperationRest.Node_p != NULL) 
        {
            nh_webidl_ParseNode *RegularOperation_p = nh_webidl_allocateNonTerminalParseNode("RegularOperation", 2);
            nh_core_appendToList(&RegularOperation_p->Children, Type.Node_p);
            nh_core_appendToList(&RegularOperation_p->Children, OperationRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(RegularOperation_p))
        }

        nh_webidl_freeParseNode(Type.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseCallbackInterfaceMember(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult Const = nh_webidl_parseConst(Parser_p);
    if (Const.Node_p != NULL) 
    {
        nh_webidl_ParseNode *CallbackInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("CallbackInterfaceMember", 1);
        nh_core_appendToList(&CallbackInterfaceMember_p->Children, Const.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(CallbackInterfaceMember_p))
    }

    nh_webidl_ParseResult RegularOperation = nh_webidl_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL) 
    {
        nh_webidl_ParseNode *CallbackInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("CallbackInterfaceMember", 1);
        nh_core_appendToList(&CallbackInterfaceMember_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(CallbackInterfaceMember_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseCallbackInterfaceMembers(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *CallbackInterfaceMembers_p = nh_webidl_allocateNonTerminalParseNode("CallbackInterfaceMembers", 1);

    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(Parser_p);
    nh_webidl_ParseResult CallbackInterfaceMember = nh_webidl_parseCallbackInterfaceMember(Parser_p);

    if (CallbackInterfaceMember.Node_p != NULL) {
        nh_webidl_ParseResult CallbackInterfaceMembers = nh_webidl_parseCallbackInterfaceMembers(Parser_p);
        nh_core_appendToList(&CallbackInterfaceMembers_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&CallbackInterfaceMembers_p->Children, CallbackInterfaceMember.Node_p);
        nh_core_appendToList(&CallbackInterfaceMembers_p->Children, CallbackInterfaceMembers.Node_p);
    }
    else {nh_webidl_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(CallbackInterfaceMembers_p))
}

static nh_webidl_ParseResult nh_webidl_parseCallbackRestOrInterface(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult CallbackRest = nh_webidl_parseCallbackRest(Parser_p);

    if (CallbackRest.Node_p != NULL)
    {
        nh_webidl_ParseNode *CallbackRestOrInterface_p = nh_webidl_allocateNonTerminalParseNode("CallbackRestOrInterface", 1);
        nh_core_appendToList(&CallbackRestOrInterface_p->Children, CallbackRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(CallbackRestOrInterface_p))
    }
    
    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "interface")) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
        {
            nh_webidl_Token *Identifier_p = &LocalParser.Tokens_p[0];
            LocalParser = nh_webidl_advanceParser(LocalParser, 1);

            if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == '{') 
            {
                nh_webidl_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
                LocalParser = nh_webidl_advanceParser(LocalParser, 1);

                nh_webidl_ParseResult CallbackInterfaceMembers = nh_webidl_parseCallbackInterfaceMembers(&LocalParser);

                if (LocalParser.unparsed > 1 
                &&  LocalParser.Tokens_p[0].String.p[0] == '}'
                &&  LocalParser.Tokens_p[1].String.p[0] == ';') 
                {
                    nh_webidl_ParseNode *CallbackRestOrInterface_p = nh_webidl_allocateNonTerminalParseNode("CallbackRestOrInterface", 6);
                    nh_core_appendToList(&CallbackRestOrInterface_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    nh_core_appendToList(&CallbackRestOrInterface_p->Children, nh_webidl_allocateTerminalParseNode(Identifier_p));
                    nh_core_appendToList(&CallbackRestOrInterface_p->Children, nh_webidl_allocateTerminalParseNode(LeftBracket_p));
                    nh_core_appendToList(&CallbackRestOrInterface_p->Children, CallbackInterfaceMembers.Node_p);
                    nh_core_appendToList(&CallbackRestOrInterface_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    nh_core_appendToList(&CallbackRestOrInterface_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                    *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
                    NH_WEBIDL_END(nh_webidl_initParseResult(CallbackRestOrInterface_p))
                }
                else {nh_webidl_freeParseNode(CallbackInterfaceMembers.Node_p);} 
            }
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseNON_STANDARD_Specifier(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *Specifier_p = nh_webidl_allocateNonTerminalParseNode("NON_STANDARD_Specifier", 2);

    if (Parser_p->unparsed > 1 && Parser_p->Tokens_p[0].String.p[0] == '@' && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER)
    {
        nh_core_appendToList(&Specifier_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        nh_core_appendToList(&Specifier_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));

        *Parser_p = nh_webidl_advanceParser(*Parser_p, 2);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(Specifier_p))
}

static nh_webidl_ParseResult nh_webidl_parseInheritance(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *Inheritance_p = nh_webidl_allocateNonTerminalParseNode("Inheritance", 3);

    if (Parser_p->unparsed > 1 && Parser_p->Tokens_p[0].String.p[0] == ':' && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER)
    {
        nh_core_appendToList(&Inheritance_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        nh_core_appendToList(&Inheritance_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));

        *Parser_p = nh_webidl_advanceParser(*Parser_p, 2);

        nh_webidl_ParseResult Specifier = nh_webidl_parseNON_STANDARD_Specifier(Parser_p);
        nh_core_appendToList(&Inheritance_p->Children, Specifier.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(Inheritance_p))
}

static nh_webidl_ParseResult nh_webidl_parseSpecial(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "getter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "setter")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "deleter"))
    {
        nh_webidl_ParseNode *Special_p = nh_webidl_allocateNonTerminalParseNode("Special", 1);
        nh_core_appendToList(&Special_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(Special_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseSpecialOperation(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult Special = nh_webidl_parseSpecial(Parser_p);
    if (Special.Node_p != NULL) 
    {
        nh_webidl_ParseResult RegularOperation = nh_webidl_parseRegularOperation(Parser_p);
        if (RegularOperation.Node_p != NULL)
        {
            nh_webidl_ParseNode *SpecialOperation_p = nh_webidl_allocateNonTerminalParseNode("SpecialOperation", 2);
            nh_core_appendToList(&SpecialOperation_p->Children, Special.Node_p);
            nh_core_appendToList(&SpecialOperation_p->Children, RegularOperation.Node_p);
            NH_WEBIDL_END(nh_webidl_initParseResult(SpecialOperation_p))
        }
        nh_webidl_freeParseNode(Special.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseOperation(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult RegularOperation = nh_webidl_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Operation_p = nh_webidl_allocateNonTerminalParseNode("Operation", 1);
        nh_core_appendToList(&Operation_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Operation_p))
    }

    nh_webidl_ParseResult SpecialOperation = nh_webidl_parseSpecialOperation(Parser_p);
    if (SpecialOperation.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Operation_p = nh_webidl_allocateNonTerminalParseNode("Operation", 1);
        nh_core_appendToList(&Operation_p->Children, SpecialOperation.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Operation_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseOptionalReadOnly(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *OptionalReadOnly_p = nh_webidl_allocateNonTerminalParseNode("OptionalReadOnly", 1);

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "readonly"))
    {
        nh_core_appendToList(&OptionalReadOnly_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(OptionalReadOnly_p))
}

static nh_webidl_ParseResult nh_webidl_parseAttributeNameKeyword(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "async")
    ||  !strcmp(Parser_p->Tokens_p[0].String.p, "required"))
    {
        nh_webidl_ParseNode *AttributeNameKeyword_p = nh_webidl_allocateNonTerminalParseNode("AttributeNameKeyword", 1);
        nh_core_appendToList(&AttributeNameKeyword_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(AttributeNameKeyword_p))
    }
    
NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseAttributeName(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult AttributeNameKeyword = nh_webidl_parseAttributeNameKeyword(Parser_p);
    if (AttributeNameKeyword.Node_p != NULL) 
    {
        nh_webidl_ParseNode *AttributeName_p = nh_webidl_allocateNonTerminalParseNode("AttributeName", 1);
        nh_core_appendToList(&AttributeName_p->Children, AttributeNameKeyword.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(AttributeName_p))
    }

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER)
    {
        nh_webidl_ParseNode *AttributeName_p = nh_webidl_allocateNonTerminalParseNode("AttributeName", 1);
        nh_core_appendToList(&AttributeName_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(AttributeName_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseAttributeRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "attribute"))
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            nh_webidl_ParseResult AttributeName = nh_webidl_parseAttributeName(&LocalParser);
            if (AttributeName.Node_p != NULL && LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ';')
            {
                nh_webidl_ParseNode *AttributeRest_p = nh_webidl_allocateNonTerminalParseNode("AttributeRest", 4);
                nh_core_appendToList(&AttributeRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&AttributeRest_p->Children, TypeWithExtendedAttributes.Node_p);
                nh_core_appendToList(&AttributeRest_p->Children, AttributeName.Node_p);
                nh_core_appendToList(&AttributeRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
                NH_WEBIDL_END(nh_webidl_initParseResult(AttributeRest_p))
            }
        }

        nh_webidl_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseStringifierRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->Tokens_p[0].String.p[0] == ';')
    {
        nh_webidl_ParseNode *StringifierRest_p = nh_webidl_allocateNonTerminalParseNode("StringifierRest", 1);
        nh_core_appendToList(&StringifierRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 1);
        NH_WEBIDL_END(nh_webidl_initParseResult(StringifierRest_p))
    }

    nh_webidl_ParseResult RegularOperation = nh_webidl_parseRegularOperation(Parser_p);

    if (RegularOperation.Node_p != NULL) 
    {
        nh_webidl_ParseNode *StringifierRest_p = nh_webidl_allocateNonTerminalParseNode("StringifierRest", 1);
        nh_core_appendToList(&StringifierRest_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(StringifierRest_p))
    }

    nh_webidl_Parser LocalParser = *Parser_p;
    nh_webidl_ParseResult OptionalReadOnly = nh_webidl_parseOptionalReadOnly(&LocalParser);
    nh_webidl_ParseResult AttributeRest = nh_webidl_parseAttributeRest(&LocalParser);

    if (AttributeRest.Node_p != NULL) 
    {
        nh_webidl_ParseNode *StringifierRest_p = nh_webidl_allocateNonTerminalParseNode("StringifierRest", 2);
        nh_core_appendToList(&StringifierRest_p->Children, OptionalReadOnly.Node_p);
        nh_core_appendToList(&StringifierRest_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(nh_webidl_initParseResult(StringifierRest_p))
    }
    else {nh_webidl_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseStringifier(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "stringifier"))
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult StringifierRest = nh_webidl_parseStringifierRest(&LocalParser);

        if (StringifierRest.Node_p != NULL)
        {
            nh_webidl_ParseNode *Stringifier_p = nh_webidl_allocateNonTerminalParseNode("Stringifier", 2);
            nh_core_appendToList(&Stringifier_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&Stringifier_p->Children, StringifierRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(Stringifier_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseStaticMemberRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult RegularOperation = nh_webidl_parseRegularOperation(Parser_p);

    if (RegularOperation.Node_p != NULL) 
    {
        nh_webidl_ParseNode *StaticMemberRest_p = nh_webidl_allocateNonTerminalParseNode("StaticMemberRest", 1);
        nh_core_appendToList(&StaticMemberRest_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(StaticMemberRest_p))
    }

    nh_webidl_Parser LocalParser = *Parser_p;
    nh_webidl_ParseResult OptionalReadOnly = nh_webidl_parseOptionalReadOnly(&LocalParser);
    nh_webidl_ParseResult AttributeRest = nh_webidl_parseAttributeRest(&LocalParser);

    if (AttributeRest.Node_p != NULL) 
    {
        nh_webidl_ParseNode *StaticMemberRest_p = nh_webidl_allocateNonTerminalParseNode("StaticMemberRest", 2);
        nh_core_appendToList(&StaticMemberRest_p->Children, OptionalReadOnly.Node_p);
        nh_core_appendToList(&StaticMemberRest_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(nh_webidl_initParseResult(StaticMemberRest_p))
    }
    else {nh_webidl_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseStaticMember(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed == 0) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "static"))
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult StaticMemberRest = nh_webidl_parseStaticMemberRest(&LocalParser);

        if (StaticMemberRest.Node_p != NULL)
        {
            nh_webidl_ParseNode *StaticMember_p = nh_webidl_allocateNonTerminalParseNode("StaticMember", 2);
            nh_core_appendToList(&StaticMember_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&StaticMember_p->Children, StaticMemberRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(StaticMember_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseOptionalType(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *OptionalType_p = nh_webidl_allocateNonTerminalParseNode("OptionalType", 2);

    if (Parser_p->Tokens_p[0].String.p[0] == ',')
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            nh_core_appendToList(&OptionalType_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&OptionalType_p->Children, TypeWithExtendedAttributes.Node_p);
            *Parser_p = LocalParser;
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(OptionalType_p))
}

static nh_webidl_ParseResult nh_webidl_parseIterable(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "iterable") && Parser_p->Tokens_p[1].String.p[0] == '<')
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
        nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            nh_webidl_ParseResult OptionalType = nh_webidl_parseOptionalType(&LocalParser);

            if (LocalParser.unparsed >= 2 && LocalParser.Tokens_p[0].String.p[0] == '>' && LocalParser.Tokens_p[1].String.p[0] == ';')
            {
                nh_webidl_ParseNode *Iterable_p = nh_webidl_allocateNonTerminalParseNode("Iterable", 6);
                nh_core_appendToList(&Iterable_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&Iterable_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                nh_core_appendToList(&Iterable_p->Children, TypeWithExtendedAttributes.Node_p);
                nh_core_appendToList(&Iterable_p->Children, OptionalType.Node_p);
                nh_core_appendToList(&Iterable_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                nh_core_appendToList(&Iterable_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
                NH_WEBIDL_END(nh_webidl_initParseResult(Iterable_p))
            }
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseOptionalArgumentList(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *OptionalArgumentList_p = nh_webidl_allocateNonTerminalParseNode("OptionalArgumentList", 3);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == '(')
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult ArgumentList = nh_webidl_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ')')
        {
            nh_core_appendToList(&OptionalArgumentList_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&OptionalArgumentList_p->Children, ArgumentList.Node_p);
            nh_core_appendToList(&OptionalArgumentList_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(OptionalArgumentList_p))
}

static nh_webidl_ParseResult nh_webidl_parseAsyncIterable(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 5) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "async") && !strcmp(Parser_p->Tokens_p[1].String.p, "iterable") && Parser_p->Tokens_p[2].String.p[0] == '<')
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 3);
        nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL)
        {
            nh_webidl_ParseResult OptionalType = nh_webidl_parseOptionalType(&LocalParser);

            if (LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.p[0] == '>')
            {
                nh_webidl_Token *Right_p = &LocalParser.Tokens_p[0];
                nh_webidl_ParseResult OptionalArgumentList = nh_webidl_parseOptionalArgumentList(&LocalParser);

                if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ';')
                {
                    nh_webidl_ParseNode *AsyncIterable_p = nh_webidl_allocateNonTerminalParseNode("AsyncIterable", 8);
                    nh_core_appendToList(&AsyncIterable_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                    nh_core_appendToList(&AsyncIterable_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                    nh_core_appendToList(&AsyncIterable_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
                    nh_core_appendToList(&AsyncIterable_p->Children, TypeWithExtendedAttributes.Node_p);
                    nh_core_appendToList(&AsyncIterable_p->Children, OptionalType.Node_p);
                    nh_core_appendToList(&AsyncIterable_p->Children, nh_webidl_allocateTerminalParseNode(Right_p));
                    nh_core_appendToList(&AsyncIterable_p->Children, OptionalArgumentList.Node_p);
                    nh_core_appendToList(&AsyncIterable_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                    *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
                    NH_WEBIDL_END(nh_webidl_initParseResult(AsyncIterable_p))
                }
                nh_webidl_freeParseNode(OptionalArgumentList.Node_p);
            }
            nh_webidl_freeParseNode(OptionalType.Node_p);
        }
        nh_webidl_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseMaplikeRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "maplike") && Parser_p->Tokens_p[1].String.p[0] == '<')
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
        nh_webidl_ParseResult TypeWithExtendedAttributes1 = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes1.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.p[0] == ',')
        {
            nh_webidl_Token *Comma_p = &LocalParser.Tokens_p[0];
            LocalParser = nh_webidl_advanceParser(LocalParser, 1);

            nh_webidl_ParseResult TypeWithExtendedAttributes2 = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);
    
            if (TypeWithExtendedAttributes2.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.p[0] == '>' && LocalParser.Tokens_p[1].String.p[0] == ';')
            {
                nh_webidl_ParseNode *MaplikeRest_p = nh_webidl_allocateNonTerminalParseNode("MaplikeRest", 7);
                nh_core_appendToList(&MaplikeRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&MaplikeRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                nh_core_appendToList(&MaplikeRest_p->Children, TypeWithExtendedAttributes1.Node_p);
                nh_core_appendToList(&MaplikeRest_p->Children, nh_webidl_allocateTerminalParseNode(Comma_p));
                nh_core_appendToList(&MaplikeRest_p->Children, TypeWithExtendedAttributes2.Node_p);
                nh_core_appendToList(&MaplikeRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                nh_core_appendToList(&MaplikeRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
                NH_WEBIDL_END(nh_webidl_initParseResult(MaplikeRest_p))
            }
            nh_webidl_freeParseNode(TypeWithExtendedAttributes2.Node_p);
        }
        nh_webidl_freeParseNode(TypeWithExtendedAttributes1.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseSetlikeRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 4) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "setlike") && Parser_p->Tokens_p[1].String.p[0] == '<')
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
        nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser);

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.p[0] == '>' && LocalParser.Tokens_p[0].String.p[0] == ';')
        {
            nh_webidl_ParseNode *SetlikeRest_p = nh_webidl_allocateNonTerminalParseNode("SetlikeRest", 5);
            nh_core_appendToList(&SetlikeRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&SetlikeRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&SetlikeRest_p->Children, TypeWithExtendedAttributes.Node_p);
            nh_core_appendToList(&SetlikeRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&SetlikeRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(SetlikeRest_p))
        }

        nh_webidl_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseReadOnlyMemberRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult AttributeRest = nh_webidl_parseAttributeRest(Parser_p);
    if (AttributeRest.Node_p != NULL)
    {
        nh_webidl_ParseNode *ReadOnlyMemberRest_p = nh_webidl_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        nh_core_appendToList(&ReadOnlyMemberRest_p->Children, AttributeRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ReadOnlyMemberRest_p))
    }

    nh_webidl_ParseResult MaplikeRest = nh_webidl_parseMaplikeRest(Parser_p);
    if (MaplikeRest.Node_p != NULL)
    {
        nh_webidl_ParseNode *ReadOnlyMemberRest_p = nh_webidl_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        nh_core_appendToList(&ReadOnlyMemberRest_p->Children, MaplikeRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ReadOnlyMemberRest_p))
    }

    nh_webidl_ParseResult SetlikeRest = nh_webidl_parseSetlikeRest(Parser_p);
    if (SetlikeRest.Node_p != NULL)
    {
        nh_webidl_ParseNode *ReadOnlyMemberRest_p = nh_webidl_allocateNonTerminalParseNode("ReadOnlyMemberRest", 1);
        nh_core_appendToList(&ReadOnlyMemberRest_p->Children, SetlikeRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ReadOnlyMemberRest_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseReadOnlyMember(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 2) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "readonly"))
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult ReadOnlyMemberRest = nh_webidl_parseReadOnlyMemberRest(&LocalParser);

        if (ReadOnlyMemberRest.Node_p != NULL)
        {
            nh_webidl_ParseNode *ReadOnlyMember_p = nh_webidl_allocateNonTerminalParseNode("ReadOnlyMember", 2);
            nh_core_appendToList(&ReadOnlyMember_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&ReadOnlyMember_p->Children, ReadOnlyMemberRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(ReadOnlyMember_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseInheritAttribute(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 2) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "inherit"))
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult AttributeRest = nh_webidl_parseAttributeRest(&LocalParser);

        if (AttributeRest.Node_p != NULL)
        {
            nh_webidl_ParseNode *InheritAttribute_p = nh_webidl_allocateNonTerminalParseNode("InheritAttribute", 2);
            nh_core_appendToList(&InheritAttribute_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&InheritAttribute_p->Children, AttributeRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(InheritAttribute_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseReadWriteAttribute(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult AttributeRest = nh_webidl_parseAttributeRest(Parser_p);

    if (AttributeRest.Node_p != NULL)
    {
        nh_webidl_ParseNode *ReadWriteAttribute_p = nh_webidl_allocateNonTerminalParseNode("ReadWriteAttribute", 1);
        nh_core_appendToList(&ReadWriteAttribute_p->Children, AttributeRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ReadWriteAttribute_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseReadWriteSetlike(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult SetlikeRest = nh_webidl_parseSetlikeRest(Parser_p);

    if (SetlikeRest.Node_p != NULL)
    {
        nh_webidl_ParseNode *ReadWriteSetlike_p = nh_webidl_allocateNonTerminalParseNode("ReadWriteSetlike", 1);
        nh_core_appendToList(&ReadWriteSetlike_p->Children, SetlikeRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ReadWriteSetlike_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseReadWriteMaplike(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult MaplikeRest = nh_webidl_parseMaplikeRest(Parser_p);

    if (MaplikeRest.Node_p != NULL)
    {
        nh_webidl_ParseNode *ReadWriteMaplike_p = nh_webidl_allocateNonTerminalParseNode("ReadWriteMaplike", 1);
        nh_core_appendToList(&ReadWriteMaplike_p->Children, MaplikeRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(ReadWriteMaplike_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parsePartialInterfaceMember(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult Const = nh_webidl_parseConst(Parser_p);
    if (Const.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, Const.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult Operation = nh_webidl_parseOperation(Parser_p);
    if (Operation.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, Operation.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult Stringifier = nh_webidl_parseStringifier(Parser_p);
    if (Stringifier.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, Stringifier.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult StaticMember = nh_webidl_parseStaticMember(Parser_p);
    if (Stringifier.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, StaticMember.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult Iterable = nh_webidl_parseIterable(Parser_p);
    if (Iterable.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, Iterable.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult AsyncIterable = nh_webidl_parseAsyncIterable(Parser_p);
    if (AsyncIterable.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, AsyncIterable.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult ReadOnlyMember = nh_webidl_parseReadOnlyMember(Parser_p);
    if (ReadOnlyMember.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, ReadOnlyMember.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult ReadWriteAttribute = nh_webidl_parseReadWriteAttribute(Parser_p);
    if (ReadWriteAttribute.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, ReadWriteAttribute.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult ReadWriteMaplike = nh_webidl_parseReadWriteMaplike(Parser_p);
    if (ReadWriteMaplike.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, ReadWriteMaplike.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult ReadWriteSetlike = nh_webidl_parseReadWriteSetlike(Parser_p);
    if (ReadWriteSetlike.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, ReadWriteSetlike.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

    nh_webidl_ParseResult InheritAttribute = nh_webidl_parseInheritAttribute(Parser_p);
    if (InheritAttribute.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMember", 1);
        nh_core_appendToList(&PartialInterfaceMember_p->Children, InheritAttribute.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMember_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseConstructor(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed <= 3) {NH_WEBIDL_END(nh_webidl_initParseResult(NULL))}

    if (!strcmp(Parser_p->Tokens_p[0].String.p, "constructor") && Parser_p->Tokens_p[1].String.p[0] == '(')
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
        nh_webidl_ParseResult ArgumentList = nh_webidl_parseArgumentList(&LocalParser);

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.p[0] == ')' && LocalParser.Tokens_p[1].String.p[0] == ';')
        {
            nh_webidl_ParseNode *Constructor_p = nh_webidl_allocateNonTerminalParseNode("Constructor", 5);
            nh_core_appendToList(&Constructor_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&Constructor_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&Constructor_p->Children, ArgumentList.Node_p);
            nh_core_appendToList(&Constructor_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&Constructor_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(Constructor_p))
        }

        nh_webidl_freeParseNode(ArgumentList.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseInterfaceMember(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult PartialInterfaceMember = nh_webidl_parsePartialInterfaceMember(Parser_p);
    if (PartialInterfaceMember.Node_p != NULL)  
    {
        nh_webidl_ParseNode *InterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("InterfaceMember", 1);
        nh_core_appendToList(&InterfaceMember_p->Children, PartialInterfaceMember.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(InterfaceMember_p))
    }

    nh_webidl_ParseResult Constructor = nh_webidl_parseConstructor(Parser_p);
    if (Constructor.Node_p != NULL)  
    {
        nh_webidl_ParseNode *InterfaceMember_p = nh_webidl_allocateNonTerminalParseNode("InterfaceMember", 1);
        nh_core_appendToList(&InterfaceMember_p->Children, Constructor.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(InterfaceMember_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseInterfaceMembers(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *InterfaceMembers_p = nh_webidl_allocateNonTerminalParseNode("InterfaceMembers", 3);
    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(Parser_p);
    nh_webidl_ParseResult InterfaceMember = nh_webidl_parseInterfaceMember(Parser_p);

    if (InterfaceMember.Node_p != NULL) {
        nh_webidl_ParseResult InterfaceMembers = nh_webidl_parseInterfaceMembers(Parser_p);
        nh_core_appendToList(&InterfaceMembers_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&InterfaceMembers_p->Children, InterfaceMember.Node_p);
        nh_core_appendToList(&InterfaceMembers_p->Children, InterfaceMembers.Node_p);
    }
    else {nh_webidl_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(InterfaceMembers_p))
}

static nh_webidl_ParseResult nh_webidl_parseInterfaceRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult Inheritance = nh_webidl_parseInheritance(&LocalParser); 

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == '{') 
        {
            nh_webidl_Token *LeftBracket_p = &LocalParser.Tokens_p[0];
            LocalParser = nh_webidl_advanceParser(LocalParser, 1);

            nh_webidl_ParseResult InterfaceMembers = nh_webidl_parseInterfaceMembers(&LocalParser);

            if (LocalParser.unparsed > 1 
            &&  LocalParser.Tokens_p[0].String.p[0] == '}'
            &&  LocalParser.Tokens_p[1].String.p[0] == ';') 
            {
                nh_webidl_ParseNode *InterfaceRest_p = nh_webidl_allocateNonTerminalParseNode("InterfaceRest", 6);
                nh_core_appendToList(&InterfaceRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&InterfaceRest_p->Children, Inheritance.Node_p);
                nh_core_appendToList(&InterfaceRest_p->Children, nh_webidl_allocateTerminalParseNode(LeftBracket_p));
                nh_core_appendToList(&InterfaceRest_p->Children, InterfaceMembers.Node_p);
                nh_core_appendToList(&InterfaceRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                nh_core_appendToList(&InterfaceRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
                NH_WEBIDL_END(nh_webidl_initParseResult(InterfaceRest_p))
            }
            else {nh_webidl_freeParseNode(InterfaceMembers.Node_p);} 
        }
        nh_webidl_freeParseNode(Inheritance.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseMixinMember(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult Const = nh_webidl_parseConst(Parser_p);
    if (Const.Node_p != NULL)  
    {
        nh_webidl_ParseNode *MixinMember_p = nh_webidl_allocateNonTerminalParseNode("MixinMember", 1);
        nh_core_appendToList(&MixinMember_p->Children, Const.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(MixinMember_p))
    }

    nh_webidl_ParseResult RegularOperation = nh_webidl_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL)  
    {
        nh_webidl_ParseNode *MixinMember_p = nh_webidl_allocateNonTerminalParseNode("MixinMember", 1);
        nh_core_appendToList(&MixinMember_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(MixinMember_p))
    }

    nh_webidl_ParseResult Stringifier = nh_webidl_parseStringifier(Parser_p);
    if (Stringifier.Node_p != NULL)  
    {
        nh_webidl_ParseNode *MixinMember_p = nh_webidl_allocateNonTerminalParseNode("MixinMember", 1);
        nh_core_appendToList(&MixinMember_p->Children, Stringifier.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(MixinMember_p))
    }

    nh_webidl_Parser LocalParser = *Parser_p;
    nh_webidl_ParseResult OptionalReadOnly = nh_webidl_parseOptionalReadOnly(&LocalParser);
    nh_webidl_ParseResult AttributeRest = nh_webidl_parseAttributeRest(&LocalParser);
    if (AttributeRest.Node_p != NULL)  
    {
        nh_webidl_ParseNode *MixinMember_p = nh_webidl_allocateNonTerminalParseNode("MixinMember", 2);
        nh_core_appendToList(&MixinMember_p->Children, OptionalReadOnly.Node_p);
        nh_core_appendToList(&MixinMember_p->Children, AttributeRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(nh_webidl_initParseResult(MixinMember_p))
    }
    else {nh_webidl_freeParseNode(OptionalReadOnly.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseMixinMembers(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *MixinMembers_p = nh_webidl_allocateNonTerminalParseNode("MixinMembers", 3);
    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(Parser_p);
    nh_webidl_ParseResult MixinMember = nh_webidl_parseMixinMember(Parser_p);

    if (MixinMember.Node_p != NULL) {
        nh_webidl_ParseResult MixinMembers = nh_webidl_parseMixinMembers(Parser_p);
        nh_core_appendToList(&MixinMembers_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&MixinMembers_p->Children, MixinMember.Node_p);
        nh_core_appendToList(&MixinMembers_p->Children, MixinMembers.Node_p);
    }
    else {nh_webidl_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(MixinMembers_p))
}

static nh_webidl_ParseResult nh_webidl_parseMixinRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.p, "mixin") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.p[0] == '{') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 3);
        nh_webidl_ParseResult MixinMembers = nh_webidl_parseMixinMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.p[0] == '}' && LocalParser.Tokens_p[1].String.p[0] == ';') 
        {
            nh_webidl_ParseNode *MixinRest_p = nh_webidl_allocateNonTerminalParseNode("MixinRest", 6);
            nh_core_appendToList(&MixinRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&MixinRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&MixinRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            nh_core_appendToList(&MixinRest_p->Children, MixinMembers.Node_p);
            nh_core_appendToList(&MixinRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&MixinRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(MixinRest_p))
        }

        nh_webidl_freeParseNode(MixinMembers.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseInterfaceOrMixin(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult InterfaceRest = nh_webidl_parseInterfaceRest(Parser_p);

    if (InterfaceRest.Node_p != NULL) 
    {
        nh_webidl_ParseNode *InterfaceOrMixin_p = nh_webidl_allocateNonTerminalParseNode("InterfaceOrMixin", 1);
        nh_core_appendToList(&InterfaceOrMixin_p->Children, InterfaceRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(InterfaceOrMixin_p))
    }
    
    nh_webidl_ParseResult MixinRest = nh_webidl_parseMixinRest(Parser_p);

    if (MixinRest.Node_p != NULL) 
    {
        nh_webidl_ParseNode *InterfaceOrMixin_p = nh_webidl_allocateNonTerminalParseNode("InterfaceOrMixin", 1);
        nh_core_appendToList(&InterfaceOrMixin_p->Children, MixinRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(InterfaceOrMixin_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseCallbackOrInterfaceOrMixin(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp("callback", Parser_p->Tokens_p[0].String.p)) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult CallbackRestOrInterface = nh_webidl_parseCallbackRestOrInterface(&LocalParser);

        if (CallbackRestOrInterface.Node_p != NULL) 
        {
            nh_webidl_ParseNode *CallbackOrInterfaceOrMixin_p = nh_webidl_allocateNonTerminalParseNode("CallbackOrInterfaceOrMixin", 2);
            nh_core_appendToList(&CallbackOrInterfaceOrMixin_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&CallbackOrInterfaceOrMixin_p->Children, CallbackRestOrInterface.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(CallbackOrInterfaceOrMixin_p))
        }
    }

    if (Parser_p->unparsed > 0 && !strcmp("interface", Parser_p->Tokens_p[0].String.p)) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult InterfaceOrMixin = nh_webidl_parseInterfaceOrMixin(&LocalParser);

        if (InterfaceOrMixin.Node_p != NULL) 
        {
            nh_webidl_ParseNode *CallbackOrInterfaceOrMixin_p = nh_webidl_allocateNonTerminalParseNode("CallbackOrInterfaceOrMixin", 2);
            nh_core_appendToList(&CallbackOrInterfaceOrMixin_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&CallbackOrInterfaceOrMixin_p->Children, InterfaceOrMixin.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(CallbackOrInterfaceOrMixin_p))
        }
    }
    
NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseNamespaceMember(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult RegularOperation = nh_webidl_parseRegularOperation(Parser_p);
    if (RegularOperation.Node_p != NULL)  
    {
        nh_webidl_ParseNode *NamespaceMember_p = nh_webidl_allocateNonTerminalParseNode("NamespaceMember", 1);
        nh_core_appendToList(&NamespaceMember_p->Children, RegularOperation.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(NamespaceMember_p))
    }

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "readonly"))
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult AttributeRest = nh_webidl_parseAttributeRest(&LocalParser);

        if (AttributeRest.Node_p != NULL)  
        {
            nh_webidl_ParseNode *NamespaceMember_p = nh_webidl_allocateNonTerminalParseNode("NamespaceMember", 2);
            nh_core_appendToList(&NamespaceMember_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&NamespaceMember_p->Children, AttributeRest.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(NamespaceMember_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseNamespaceMembers(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *NamespaceMembers_p = nh_webidl_allocateNonTerminalParseNode("NamespaceMembers", 3);
    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(Parser_p);
    nh_webidl_ParseResult NamespaceMember = nh_webidl_parseNamespaceMember(Parser_p);

    if (NamespaceMember.Node_p != NULL) {
        nh_webidl_ParseResult NamespaceMembers = nh_webidl_parseNamespaceMembers(Parser_p);
        nh_core_appendToList(&NamespaceMembers_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&NamespaceMembers_p->Children, NamespaceMember.Node_p);
        nh_core_appendToList(&NamespaceMembers_p->Children, NamespaceMembers.Node_p);
    }
    else {nh_webidl_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(NamespaceMembers_p))
}

static nh_webidl_ParseResult nh_webidl_parseNamespace(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.p, "namespace") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.p[0] == '{') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 3);
        nh_webidl_ParseResult NamespaceMembers = nh_webidl_parseNamespaceMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.p[0] == '}' && LocalParser.Tokens_p[1].String.p[0] == ';') 
        {
            nh_webidl_ParseNode *Namespace_p = nh_webidl_allocateNonTerminalParseNode("Namespace", 6);
            nh_core_appendToList(&Namespace_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&Namespace_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&Namespace_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            nh_core_appendToList(&Namespace_p->Children, NamespaceMembers.Node_p);
            nh_core_appendToList(&Namespace_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&Namespace_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(Namespace_p))
        }

        nh_webidl_freeParseNode(NamespaceMembers.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parsePartialInterfaceMembers(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *PartialInterfaceMembers_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceMembers", 3);
    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(Parser_p);
    nh_webidl_ParseResult PartialInterfaceMember = nh_webidl_parsePartialInterfaceMember(Parser_p);

    if (PartialInterfaceMember.Node_p != NULL) {
        nh_webidl_ParseResult PartialInterfaceMembers = nh_webidl_parsePartialInterfaceMembers(Parser_p);
        nh_core_appendToList(&PartialInterfaceMembers_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&PartialInterfaceMembers_p->Children, PartialInterfaceMember.Node_p);
        nh_core_appendToList(&PartialInterfaceMembers_p->Children, PartialInterfaceMembers.Node_p);
    }
    else {nh_webidl_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceMembers_p))
}

static nh_webidl_ParseResult nh_webidl_parsePartialInterfaceRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 1 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[1].String.p[0] == '{') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
        nh_webidl_ParseResult PartialInterfaceMembers = nh_webidl_parsePartialInterfaceMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.p[0] == '}' && LocalParser.Tokens_p[1].String.p[0] == ';') 
        {
            nh_webidl_ParseNode *PartialInterfaceRest_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceRest", 5);
            nh_core_appendToList(&PartialInterfaceRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&PartialInterfaceRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&PartialInterfaceRest_p->Children, PartialInterfaceMembers.Node_p);
            nh_core_appendToList(&PartialInterfaceRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&PartialInterfaceRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceRest_p))
        }

        nh_webidl_freeParseNode(PartialInterfaceMembers.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parsePartialInterfaceOrPartialMixin(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult PartialInterfaceRest = nh_webidl_parsePartialInterfaceRest(Parser_p);

    if (PartialInterfaceRest.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceOrPartialMixin_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceOrPartialMixin", 1);
        nh_core_appendToList(&PartialInterfaceOrPartialMixin_p->Children, PartialInterfaceRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceOrPartialMixin_p))
    }
    
    nh_webidl_ParseResult MixinRest = nh_webidl_parseMixinRest(Parser_p);

    if (MixinRest.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialInterfaceOrPartialMixin_p = nh_webidl_allocateNonTerminalParseNode("PartialInterfaceOrPartialMixin", 1);
        nh_core_appendToList(&PartialInterfaceOrPartialMixin_p->Children, MixinRest.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialInterfaceOrPartialMixin_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDictionaryMemberRest(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "required")) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser); 

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER && LocalParser.Tokens_p[1].String.p[0] == ';') 
        {
            nh_webidl_ParseNode *DictionaryMemberRest_p = nh_webidl_allocateNonTerminalParseNode("DictionaryMemberRest", 4);
            nh_core_appendToList(&DictionaryMemberRest_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&DictionaryMemberRest_p->Children, TypeWithExtendedAttributes.Node_p);
            nh_core_appendToList(&DictionaryMemberRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&DictionaryMemberRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(DictionaryMemberRest_p))
        }

        nh_webidl_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }
    
    nh_webidl_Parser LocalParser = *Parser_p;
    nh_webidl_ParseResult Type = nh_webidl_parseType(&LocalParser); 

    if (Type.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        nh_webidl_Token *Identifier_p = &LocalParser.Tokens_p[0];
        LocalParser = nh_webidl_advanceParser(LocalParser, 1);
        nh_webidl_ParseResult Default = nh_webidl_parseDefault(&LocalParser); 

        if (LocalParser.unparsed > 0 && LocalParser.Tokens_p[0].String.p[0] == ';')
        {
            nh_webidl_ParseNode *DictionaryMemberRest_p = nh_webidl_allocateNonTerminalParseNode("DictionaryMemberRest", 4);
            nh_core_appendToList(&DictionaryMemberRest_p->Children, Type.Node_p);
            nh_core_appendToList(&DictionaryMemberRest_p->Children, nh_webidl_allocateTerminalParseNode(Identifier_p));
            nh_core_appendToList(&DictionaryMemberRest_p->Children, Default.Node_p);
            nh_core_appendToList(&DictionaryMemberRest_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 1);
            NH_WEBIDL_END(nh_webidl_initParseResult(DictionaryMemberRest_p))
        }

        nh_webidl_freeParseNode(Default.Node_p);
        nh_webidl_freeParseNode(Type.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDictionaryMember(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Parser LocalParser = *Parser_p;
    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(&LocalParser);
    nh_webidl_ParseResult DictionaryMemberRest = nh_webidl_parseDictionaryMemberRest(&LocalParser);

    if (DictionaryMemberRest.Node_p != NULL) {
        nh_webidl_ParseNode *DictionaryMember_p = nh_webidl_allocateNonTerminalParseNode("DictionaryMember", 2);
        nh_core_appendToList(&DictionaryMember_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&DictionaryMember_p->Children, DictionaryMemberRest.Node_p);
        *Parser_p = LocalParser;
        NH_WEBIDL_END(nh_webidl_initParseResult(DictionaryMember_p))
    }
    else {nh_webidl_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDictionaryMembers(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *DictionaryMembers_p = nh_webidl_allocateNonTerminalParseNode("DictionaryMembers", 2);
    nh_webidl_ParseResult DictionaryMember = nh_webidl_parseDictionaryMember(Parser_p);

    if (DictionaryMember.Node_p != NULL) {
        nh_webidl_ParseResult DictionaryMembers = nh_webidl_parseDictionaryMembers(Parser_p);
        nh_core_appendToList(&DictionaryMembers_p->Children, DictionaryMember.Node_p);
        nh_core_appendToList(&DictionaryMembers_p->Children, DictionaryMembers.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(DictionaryMembers_p))
}

static nh_webidl_ParseResult nh_webidl_parsePartialDictionary(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.p, "dictionary") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.p[0] == '{') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 3);
        nh_webidl_ParseResult DictionaryMembers = nh_webidl_parseDictionaryMembers(&LocalParser); 

        if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.p[0] == '}' && LocalParser.Tokens_p[1].String.p[0] == ';') 
        {
            nh_webidl_ParseNode *PartialDictionary_p = nh_webidl_allocateNonTerminalParseNode("PartialDictionary", 6);
            nh_core_appendToList(&PartialDictionary_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&PartialDictionary_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&PartialDictionary_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
            nh_core_appendToList(&PartialDictionary_p->Children, DictionaryMembers.Node_p);
            nh_core_appendToList(&PartialDictionary_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&PartialDictionary_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(PartialDictionary_p))
        }

        nh_webidl_freeParseNode(DictionaryMembers.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parsePartialDefinition(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "interface")) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult PartialInterfaceOrPartialMixin = nh_webidl_parsePartialInterfaceOrPartialMixin(&LocalParser); 

        if (PartialInterfaceOrPartialMixin.Node_p != NULL) 
        {
            nh_webidl_ParseNode *PartialDefinition_p = nh_webidl_allocateNonTerminalParseNode("PartialDefinition", 2);
            nh_core_appendToList(&PartialDefinition_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&PartialDefinition_p->Children, PartialInterfaceOrPartialMixin.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(PartialDefinition_p))
        }
    }

    nh_webidl_ParseResult PartialDictionary = nh_webidl_parsePartialDictionary(Parser_p);

    if (PartialDictionary.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialDefinition_p = nh_webidl_allocateNonTerminalParseNode("PartialDefinition", 1);
        nh_core_appendToList(&PartialDefinition_p->Children, PartialDictionary.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialDefinition_p))
    }

    nh_webidl_ParseResult Namespace = nh_webidl_parseNamespace(Parser_p);

    if (Namespace.Node_p != NULL) 
    {
        nh_webidl_ParseNode *PartialDefinition_p = nh_webidl_allocateNonTerminalParseNode("PartialDefinition", 1);
        nh_core_appendToList(&PartialDefinition_p->Children, Namespace.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(PartialDefinition_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parsePartial(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "partial")) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult PartialDefinition = nh_webidl_parsePartialDefinition(&LocalParser); 

        if (PartialDefinition.Node_p != NULL) 
        {
            nh_webidl_ParseNode *Partial_p = nh_webidl_allocateNonTerminalParseNode("Partial", 2);
            nh_core_appendToList(&Partial_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&Partial_p->Children, PartialDefinition.Node_p);
            *Parser_p = LocalParser;
            NH_WEBIDL_END(nh_webidl_initParseResult(Partial_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDictionary(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.p, "dictionary") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 2);
        nh_webidl_ParseResult Inheritance = nh_webidl_parseInheritance(&LocalParser); 

        if (LocalParser.unparsed > 2 && LocalParser.Tokens_p[0].String.p[0] == '{')
        {
            nh_webidl_Token *OpenBracket_p = &LocalParser.Tokens_p[0];
            LocalParser = nh_webidl_advanceParser(LocalParser, 1);
            nh_webidl_ParseResult DictionaryMembers = nh_webidl_parseDictionaryMembers(&LocalParser); 

            if (LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.p[0] == '}' && LocalParser.Tokens_p[1].String.p[0] == ';') 
            {
                nh_webidl_ParseNode *Dictionary_p = nh_webidl_allocateNonTerminalParseNode("Dictionary", 7);
                nh_core_appendToList(&Dictionary_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
                nh_core_appendToList(&Dictionary_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
                nh_core_appendToList(&Dictionary_p->Children, Inheritance.Node_p);
                nh_core_appendToList(&Dictionary_p->Children, nh_webidl_allocateTerminalParseNode(OpenBracket_p));
                nh_core_appendToList(&Dictionary_p->Children, DictionaryMembers.Node_p);
                nh_core_appendToList(&Dictionary_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
                nh_core_appendToList(&Dictionary_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
                *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
                NH_WEBIDL_END(nh_webidl_initParseResult(Dictionary_p))
            }
            nh_webidl_freeParseNode(DictionaryMembers.Node_p);
        }
        nh_webidl_freeParseNode(Inheritance.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseEnumValueListComma(
    nh_webidl_Parser *Parser_p
);

static nh_webidl_ParseResult nh_webidl_parseEnumValueListString(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *EnumValueListString_p = nh_webidl_allocateNonTerminalParseNode("EnumValueListString", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_STRING) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult EnumValueListComma = nh_webidl_parseEnumValueListComma(&LocalParser);
        nh_core_appendToList(&EnumValueListString_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        nh_core_appendToList(&EnumValueListString_p->Children, EnumValueListComma.Node_p);
        *Parser_p = LocalParser;
    }

NH_WEBIDL_END(nh_webidl_initParseResult(EnumValueListString_p))
}

static nh_webidl_ParseResult nh_webidl_parseEnumValueListComma(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *EnumValueListComma_p = nh_webidl_allocateNonTerminalParseNode("EnumValueListComma", 2);

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].String.p[0] == ',') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult EnumValueListString = nh_webidl_parseEnumValueListString(&LocalParser);
        nh_core_appendToList(&EnumValueListComma_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        nh_core_appendToList(&EnumValueListComma_p->Children, EnumValueListString.Node_p);
        *Parser_p = LocalParser;
    }

NH_WEBIDL_END(nh_webidl_initParseResult(EnumValueListComma_p))
}

static nh_webidl_ParseResult nh_webidl_parseEnumValueList(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_STRING) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult EnumValueListComma = nh_webidl_parseEnumValueListComma(&LocalParser);

        nh_webidl_ParseNode *EnumValueList_p = nh_webidl_allocateNonTerminalParseNode("EnumValueList", 2);
        nh_core_appendToList(&EnumValueList_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        nh_core_appendToList(&EnumValueList_p->Children, EnumValueListComma.Node_p);

        *Parser_p = LocalParser;

        NH_WEBIDL_END(nh_webidl_initParseResult(EnumValueList_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseEnum(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 2 && !strcmp(Parser_p->Tokens_p[0].String.p, "enum") && Parser_p->Tokens_p[1].type == NH_WEBIDL_TOKEN_IDENTIFIER && Parser_p->Tokens_p[2].String.p[0] == '{') 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 3);
        nh_webidl_ParseResult EnumValueList = nh_webidl_parseEnumValueList(&LocalParser); 

        if (EnumValueList.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].String.p[0] == '}' && LocalParser.Tokens_p[1].String.p[0] == ';') 
        {
            nh_webidl_ParseNode *Enum_p = nh_webidl_allocateNonTerminalParseNode("Enum", 6);
            nh_core_appendToList(&Enum_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&Enum_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&Enum_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
            nh_core_appendToList(&Enum_p->Children, EnumValueList.Node_p);
            nh_core_appendToList(&Enum_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&Enum_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(Enum_p))
        }
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseTypedef(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 0 && !strcmp(Parser_p->Tokens_p[0].String.p, "typedef")) 
    {
        nh_webidl_Parser LocalParser = nh_webidl_advanceParser(*Parser_p, 1);
        nh_webidl_ParseResult TypeWithExtendedAttributes = nh_webidl_parseTypeWithExtendedAttributes(&LocalParser); 

        if (TypeWithExtendedAttributes.Node_p != NULL && LocalParser.unparsed > 1 && LocalParser.Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER && LocalParser.Tokens_p[1].String.p[0] == ';') 
        {
            nh_webidl_ParseNode *Typedef_p = nh_webidl_allocateNonTerminalParseNode("Typedef", 4);
            nh_core_appendToList(&Typedef_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
            nh_core_appendToList(&Typedef_p->Children, TypeWithExtendedAttributes.Node_p);
            nh_core_appendToList(&Typedef_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[0]));
            nh_core_appendToList(&Typedef_p->Children, nh_webidl_allocateTerminalParseNode(&LocalParser.Tokens_p[1]));
            *Parser_p = nh_webidl_advanceParser(LocalParser, 2);
            NH_WEBIDL_END(nh_webidl_initParseResult(Typedef_p))
        }

        nh_webidl_freeParseNode(TypeWithExtendedAttributes.Node_p);
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseIncludesStatement(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    if (Parser_p->unparsed > 3 
    &&  Parser_p->Tokens_p[0].type == NH_WEBIDL_TOKEN_IDENTIFIER 
    &&  !strcmp(Parser_p->Tokens_p[1].String.p, "includes")
    &&  Parser_p->Tokens_p[2].type == NH_WEBIDL_TOKEN_IDENTIFIER 
    &&  Parser_p->Tokens_p[3].String.p[0] == ';')
    {
        nh_webidl_ParseNode *IncludesStatement_p = nh_webidl_allocateNonTerminalParseNode("IncludesStatement", 4);
        nh_core_appendToList(&IncludesStatement_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[0]));
        nh_core_appendToList(&IncludesStatement_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[1]));
        nh_core_appendToList(&IncludesStatement_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[2]));
        nh_core_appendToList(&IncludesStatement_p->Children, nh_webidl_allocateTerminalParseNode(&Parser_p->Tokens_p[3]));
        *Parser_p = nh_webidl_advanceParser(*Parser_p, 4);
        NH_WEBIDL_END(nh_webidl_initParseResult(IncludesStatement_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDefinition(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseResult CallbackOrInterfaceOrMixin = nh_webidl_parseCallbackOrInterfaceOrMixin(Parser_p);

    if (CallbackOrInterfaceOrMixin.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Definition_p = nh_webidl_allocateNonTerminalParseNode("Definition", 1);
        nh_core_appendToList(&Definition_p->Children, CallbackOrInterfaceOrMixin.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Definition_p))
    }

    nh_webidl_ParseResult Namespace = nh_webidl_parseNamespace(Parser_p);

    if (Namespace.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Definition_p = nh_webidl_allocateNonTerminalParseNode("Definition", 1);
        nh_core_appendToList(&Definition_p->Children, Namespace.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Definition_p))
    }

    nh_webidl_ParseResult Partial = nh_webidl_parsePartial(Parser_p);

    if (Partial.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Definition_p = nh_webidl_allocateNonTerminalParseNode("Definition", 1);
        nh_core_appendToList(&Definition_p->Children, Partial.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Definition_p))
    }

    nh_webidl_ParseResult Dictionary = nh_webidl_parseDictionary(Parser_p);

    if (Dictionary.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Definition_p = nh_webidl_allocateNonTerminalParseNode("Definition", 1);
        nh_core_appendToList(&Definition_p->Children, Dictionary.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Definition_p))
    }

    nh_webidl_ParseResult Enum = nh_webidl_parseEnum(Parser_p);

    if (Enum.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Definition_p = nh_webidl_allocateNonTerminalParseNode("Definition", 1);
        nh_core_appendToList(&Definition_p->Children, Enum.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Definition_p))
    }

    nh_webidl_ParseResult Typedef = nh_webidl_parseTypedef(Parser_p);

    if (Typedef.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Definition_p = nh_webidl_allocateNonTerminalParseNode("Definition", 1);
        nh_core_appendToList(&Definition_p->Children, Typedef.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Definition_p))
    }

    nh_webidl_ParseResult IncludesStatement = nh_webidl_parseIncludesStatement(Parser_p);

    if (IncludesStatement.Node_p != NULL) 
    {
        nh_webidl_ParseNode *Definition_p = nh_webidl_allocateNonTerminalParseNode("Definition", 1);
        nh_core_appendToList(&Definition_p->Children, IncludesStatement.Node_p);
        NH_WEBIDL_END(nh_webidl_initParseResult(Definition_p))
    }

NH_WEBIDL_END(nh_webidl_initParseResult(NULL))
}

static nh_webidl_ParseResult nh_webidl_parseDefinitions(
    nh_webidl_Parser *Parser_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_ParseNode *Definitions_p = nh_webidl_allocateNonTerminalParseNode("Definitions", 3);

    nh_webidl_ParseResult ExtendedAttributeList = nh_webidl_parseExtendedAttributeList(Parser_p);
    nh_webidl_ParseResult Definition = nh_webidl_parseDefinition(Parser_p);

    if (Definition.Node_p != NULL) 
    {
        nh_core_appendToList(&Definitions_p->Children, ExtendedAttributeList.Node_p);
        nh_core_appendToList(&Definitions_p->Children, Definition.Node_p);
        nh_webidl_ParseResult Definitions = nh_webidl_parseDefinitions(Parser_p);
        nh_core_appendToList(&Definitions_p->Children, Definitions.Node_p);
    }
    else {nh_webidl_freeParseNode(ExtendedAttributeList.Node_p);}

NH_WEBIDL_END(nh_webidl_initParseResult(Definitions_p))
}

// PARSE ===========================================================================================

nh_webidl_FragmentParseResult nh_webidl_parse(
    NH_BYTE *logName_p, NH_BYTE *fragment_p)
{
NH_WEBIDL_BEGIN()

    nh_Array Tokens = nh_webidl_tokenizeFragment(logName_p, fragment_p);

    nh_webidl_Parser Parser;
    Parser.Tokens_p = (nh_webidl_Token*)Tokens.p;
    Parser.unparsed = Tokens.length;

    nh_webidl_ParseResult Definitions = nh_webidl_parseDefinitions(&Parser);

    nh_webidl_FragmentParseResult Result;
    Result.Root_p = Definitions.Node_p;
    Result.SyntaxErrors = Definitions.SyntaxErrors;
    Result.Tokens = Tokens;

    nh_webidl_logParseTree(logName_p, Result.Root_p);

NH_WEBIDL_END(Result)
}

// UNPARSE =========================================================================================

static void nh_webidl_unparseParseNode(
    nh_webidl_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_webidl_unparseParseNode(Node_p->Children.pp[i]);
    }

    nh_core_freeList(&Node_p->Children, NH_FALSE);
    nh_core_free(Node_p);

NH_WEBIDL_SILENT_END()
}

void nh_webidl_unparse(
    nh_webidl_FragmentParseResult ParseResult)
{
NH_WEBIDL_BEGIN()

    nh_webidl_freeTokens(ParseResult.Tokens);
    nh_webidl_unparseParseNode(ParseResult.Root_p);

NH_WEBIDL_SILENT_END()
}

// INIT ============================================================================================

nh_webidl_FragmentParseResult nh_webidl_initFragmentParseResult()
{
NH_WEBIDL_BEGIN()

    nh_webidl_FragmentParseResult Result;
    Result.Root_p = NULL;
    Result.SyntaxErrors = nh_core_initList(8);
    Result.Tokens = nh_core_initArray(sizeof(nh_webidl_Token), 64);

NH_WEBIDL_END(Result)
}

// GET =============================================================================================

NH_WEBIDL_RESULT nh_webidl_getParseNodes(
    nh_webidl_ParseNode *Node_p, NH_WEBIDL_PARSE_NODE type, nh_List *Nodes_p)
{
NH_WEBIDL_BEGIN()

    if (Node_p->type == type) {
        nh_core_appendToList(Nodes_p, Node_p);
    }

    for (int i = 0; i < Node_p->Children.size; ++i) {
        nh_webidl_getParseNodes(Node_p->Children.pp[i], type, Nodes_p);
    }

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

