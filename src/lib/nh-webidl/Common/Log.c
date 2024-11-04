// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"

#include "../Runtime/Definitions.h"
#include "../../nh-core/Logger/Logger.h"

#include <string.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_webidl_logTokens(
    char *fragmentName_p, nh_core_Array *Tokens_p, bool dirty)
{
    char message_p[1023] = {0};
    char node_p[255] = {0};

    for (int i = 0; i < Tokens_p->length; ++i) 
    {
        nh_webidl_Token *Token_p = &((nh_webidl_Token*)Tokens_p->p)[i];
        nh_core_String String = nh_core_initString(64);
        nh_core_appendToString(&String, Token_p->String.p, Token_p->String.length);

        for (int i = 0; i < String.length; ++i) {
            if (String.p[i] == '\n' || String.p[i] == '\r') {String.p[i] = ' ';}
        }

        sprintf(node_p, dirty ? "nh-webidl:Parser:%s:TokensDirty" : "nh-webidl:Parser:%s:TokensClean", fragmentName_p);
        sprintf(message_p, "%s [%s]", NH_WEBIDL_TOKENS_PP[Token_p->type], String.p);

        nh_core_sendLogMessage(node_p, NULL, message_p);

        nh_core_freeString(&String);

        memset(message_p, 0, 1023);
        memset(node_p, 0, 255);
    }

    return NH_API_SUCCESS;
}

#define MAX_DEPTH 1024

static NH_API_RESULT nh_webidl_logParseTreeRecursively(
    char *fragmentName_p, nh_webidl_ParseNode *ParseNode_p, nh_webidl_ParseNode *Parent_p, int depth, 
    bool *branch_p, char *message_p, char *indent_p, char *node_p)
{
    if (depth >= MAX_DEPTH) {return NH_API_ERROR_BAD_STATE;}

    int offset;
    for (offset = 0; offset < depth; ++offset) {
        indent_p[offset] = branch_p[offset] == true ? '|' : ' ';
    }

    if (ParseNode_p->Token_p == NULL) {
        sprintf(message_p, "%s%s", indent_p, NH_WEBIDL_PARSE_NODE_NAMES_PP[ParseNode_p->type]);
    }
    else {
        sprintf(message_p, "%sTERMINAL %s", indent_p, ParseNode_p->Token_p->String.p);
    }

    sprintf(node_p, "nh-webidl:Parser:%s:ParseTree", fragmentName_p);
    nh_core_sendLogMessage(node_p, NULL, message_p);

    branch_p[depth] = true;
    if (Parent_p != NULL && ParseNode_p == Parent_p->Children.pp[Parent_p->Children.size - 1]) {
        branch_p[depth - 1] = false;
    }

    memset(message_p, 0, 2047);
    memset(indent_p, 0, MAX_DEPTH);
    memset(node_p, 0, 255);

    for (int i = 0; i < ParseNode_p->Children.size; ++i) {
        nh_webidl_logParseTreeRecursively(
            fragmentName_p, ParseNode_p->Children.pp[i], ParseNode_p, depth + 1, branch_p, message_p, 
            indent_p, node_p
        );
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_webidl_logParseTree(
    char *fragmentName_p, nh_webidl_ParseNode *ParseNode_p)
{
    char indent_p[MAX_DEPTH] = {'\0'};
    char message_p[2047] = {'\0'};
    char node_p[255] = {0};

    bool branch_p[MAX_DEPTH];
    memset(branch_p, false, MAX_DEPTH);

    nh_webidl_logParseTreeRecursively(
        fragmentName_p, ParseNode_p, NULL, 0, branch_p, message_p, indent_p, node_p
    );

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_webidl_logFragment(
    char *specification_p, nh_webidl_Fragment *Fragment_p)
{
    char message_p[1023] = {'\0'};

    for (int i = 0; i < Fragment_p->Interfaces.length; ++i) 
    {
        nh_webidl_Interface *Interface_p = &((nh_webidl_Interface*)Fragment_p->Interfaces.p)[i];
        char className_p[255] = {'\0'};
        sprintf(className_p, Interface_p->partial ? "%s (partial)" : "%s", Interface_p->name_p);

        char node_p[255] = {0};
        sprintf(node_p, "nh-webidl:%s:Interfaces:%s", specification_p, className_p);

        if (Interface_p->Inheritance_p) 
        {
            sprintf(
                message_p, "Inherits %s (%s)", Interface_p->Inheritance_p->interface_p, Interface_p->Inheritance_p->specification_p ? Interface_p->Inheritance_p->specification_p : Interface_p->Specification_p->name_p
            );
            nh_core_sendLogMessage(node_p, NULL, message_p);
            memset(message_p, 0, 1023);
        }

        nh_core_sendLogMessage(node_p, NULL, "Members:");

        for (int j = 0; j < Interface_p->Members.length; ++j) 
        {
            nh_webidl_InterfaceMember *InterfaceMember_p = &((nh_webidl_InterfaceMember*)Interface_p->Members.p)[j];
            sprintf(
                message_p, "  %s %s", NH_WEBIDL_PARSE_NODE_NAMES_PP[InterfaceMember_p->Node_p->type], InterfaceMember_p->name_p == NULL ? "null" : InterfaceMember_p->name_p
            );
            nh_core_sendLogMessage(node_p, NULL, message_p);
            memset(message_p, 0, 1023);
        }
    }

    return NH_API_SUCCESS;
}
