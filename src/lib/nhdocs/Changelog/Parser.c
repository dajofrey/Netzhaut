// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Parser.h"
#include "Changelog.h"
#include "Version.h"
#include "Tokenizer.h"

#include "../Common/Macros.h"

#include "../../nhcore/System/Memory.h"
#include "../../nhcore/Util/File.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

// CREATE PARSE-TREE ===============================================================================

typedef struct nh_docs_CommitParser {
    nh_docs_Token *Token_p;
    nh_docs_CommitNode *Node_p;
    NH_DOCS_COMMIT_NODE_E type;
} nh_docs_CommitParser;

static NH_DOCS_RESULT nh_docs_initCommitNode(
    nh_docs_CommitNode *Node_p, NH_DOCS_COMMIT_NODE_E type, nh_docs_CommitNode *Parent_p)
{
NH_DOCS_BEGIN()

    Node_p->type = type;
    Node_p->text_p = NULL;
    Node_p->length = 0;
    Node_p->Children_pp = NULL;
    Node_p->children = 0;
    Node_p->Parent_p = Parent_p;

NH_DOCS_END(NH_DOCS_SUCCESS)
}

static nh_docs_CommitNode *nh_docs_createCommitNode(
    NH_DOCS_COMMIT_NODE_E type, nh_docs_CommitNode *Parent_p)
{
NH_DOCS_BEGIN()

    nh_docs_CommitNode *Node_p = malloc(sizeof(nh_docs_CommitNode));
    NH_DOCS_CHECK_NULL_2(NULL, Node_p)

    nh_docs_initCommitNode(Node_p, type, Parent_p);

    if (!Parent_p) {NH_DOCS_END(Node_p)}

    if (!Parent_p->Children_pp) {
        Parent_p->Children_pp = malloc(sizeof(nh_docs_CommitNode*));
        NH_DOCS_CHECK_NULL_2(NULL, Parent_p->Children_pp)
        Parent_p->Children_pp[0] = Node_p;
        Parent_p->children = 1;
    }
    else {
        Parent_p->Children_pp =
            realloc(Parent_p->Children_pp, sizeof(nh_docs_CommitNode*) * (Parent_p->children + 1));
        NH_DOCS_CHECK_NULL_2(NULL, Parent_p->Children_pp)
        Parent_p->Children_pp[Parent_p->children] = Node_p;
        Parent_p->children++;
    }

NH_DOCS_END(Node_p)
}

static NH_DOCS_COMMIT_NODE_E nh_docs_getChildType(
    nh_docs_CommitParser *Parser_p)
{
NH_DOCS_BEGIN()

    NH_DOCS_COMMIT_NODE_E type = NH_DOCS_COMMIT_NODE_UNDEFINED;

    if (Parser_p->Node_p->type == NH_DOCS_COMMIT_NODE_ROOT) 
    {
        if (!strcmp(Parser_p->Token_p->string_p, "date")) {
            type = NH_DOCS_COMMIT_NODE_DATE;
        }
        else if (!strcmp(Parser_p->Token_p->string_p, "id")) {
            type = NH_DOCS_COMMIT_NODE_ID;
        }
        else if (!strcmp(Parser_p->Token_p->string_p, "author")) {
            type = NH_DOCS_COMMIT_NODE_AUTHOR;
        }
        else if (!strcmp(Parser_p->Token_p->string_p, "message")) {
            type = NH_DOCS_COMMIT_NODE_MESSAGE;
        }
        else if (!strcmp(Parser_p->Token_p->string_p, "merge")) {
            type = NH_DOCS_COMMIT_NODE_MERGE;
        }
    }

    if (Parser_p->Node_p->type == NH_DOCS_COMMIT_NODE_WORKLOAD) 
    {
        if (!strcmp(Parser_p->Token_p->string_p, "bin")) {
            type = NH_DOCS_COMMIT_NODE_WORKLOAD_BIN;
        }
        else if (!strcmp(Parser_p->Token_p->string_p, "lib")) {
            type = NH_DOCS_COMMIT_NODE_WORKLOAD_LIB;
        }
        else if (!strcmp(Parser_p->Token_p->string_p, "scope")) {
            type = NH_DOCS_COMMIT_NODE_WORKLOAD_SCOPE;
        }
        else if (!strcmp(Parser_p->Token_p->string_p, "message")) {
            type = NH_DOCS_COMMIT_NODE_WORKLOAD_MESSAGE;
        }
    }

    if (Parser_p->Node_p->type == NH_DOCS_COMMIT_NODE_AUTHOR) 
    {
        if (!strcmp(Parser_p->Token_p->string_p, "contact")) {
            type = NH_DOCS_COMMIT_NODE_AUTHOR_CONTACT;
        }
        else if (!strcmp(Parser_p->Token_p->string_p, "name")) {
            type = NH_DOCS_COMMIT_NODE_AUTHOR_NAME;
        }
    }

NH_DOCS_END(type)
}

static NH_DOCS_RESULT nh_docs_parseChild(
    nh_docs_CommitParser *Parser_p) 
{
NH_DOCS_BEGIN()

    if (!Parser_p->Token_p->string_p) {
         NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)       
    }

    NH_DOCS_COMMIT_NODE_E type = nh_docs_getChildType(Parser_p);

    if (type == NH_DOCS_COMMIT_NODE_UNDEFINED) {
        NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)
    }

    nh_docs_CommitNode *Node_p = nh_docs_createCommitNode(type, Parser_p->Node_p);
    NH_DOCS_CHECK_NULL(Node_p)

    ++(Parser_p->Token_p);
    ++(Parser_p->Token_p);

    if (Parser_p->Token_p->type != NH_DOCS_TOKEN_STRING) {
        NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)
    }

    Node_p->text_p = Parser_p->Token_p->string_p;

    ++(Parser_p->Token_p);

    while (Parser_p->Token_p->type == NH_DOCS_TOKEN_COMMA) {
        Node_p = nh_docs_createCommitNode(type, Parser_p->Node_p);
        NH_DOCS_CHECK_NULL(Node_p)
        ++(Parser_p->Token_p);
        Node_p->text_p = Parser_p->Token_p->string_p;
        ++(Parser_p->Token_p);
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_parseToken(
    nh_docs_CommitParser *Parser_p
);

static NH_DOCS_RESULT nh_docs_parseParent(
    nh_docs_CommitParser *Parser_p) 
{
NH_DOCS_BEGIN()

    if (!Parser_p->Token_p->string_p) {
        NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)
    }

    Parser_p->type = NH_DOCS_COMMIT_NODE_UNDEFINED;

    if (!strcmp(Parser_p->Token_p->string_p, "commit")) {
        Parser_p->type = NH_DOCS_COMMIT_NODE_ROOT;
    }
    else if (!strcmp(Parser_p->Token_p->string_p, "workload")) {
        Parser_p->type = NH_DOCS_COMMIT_NODE_WORKLOAD;
    }
    else if (!strcmp(Parser_p->Token_p->string_p, "author")) {
        Parser_p->type = NH_DOCS_COMMIT_NODE_AUTHOR;
    }

    if (Parser_p->type == NH_DOCS_COMMIT_NODE_UNDEFINED) {
        NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)
    }

    Parser_p->Node_p = nh_docs_createCommitNode(Parser_p->type, Parser_p->Node_p);
    NH_DOCS_CHECK_NULL(Parser_p->Node_p)

    ++(Parser_p->Token_p);
    ++(Parser_p->Token_p);

    while (Parser_p->Token_p->type != NH_DOCS_TOKEN_CURLY_BRACKET_RIGHT) {
        NH_DOCS_CHECK(nh_docs_parseToken(Parser_p))
    }

    ++(Parser_p->Token_p);

    if (Parser_p->Node_p->Parent_p) {
        Parser_p->Node_p = Parser_p->Node_p->Parent_p;
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_parseToken(
    nh_docs_CommitParser *Parser_p)
{
NH_DOCS_BEGIN()

    if (Parser_p->Token_p->type != NH_DOCS_TOKEN_IDENTIFIER) {
        NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)
    }

    switch ((Parser_p->Token_p+1)->type) {
        case NH_DOCS_TOKEN_CURLY_BRACKET_LEFT :
            NH_DOCS_END(nh_docs_parseParent(Parser_p))
        case NH_DOCS_TOKEN_COLON :
            NH_DOCS_END(nh_docs_parseChild(Parser_p))
        default :
            NH_DOCS_END(NH_DOCS_ERROR_BAD_STATE)
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

nh_docs_CommitNode *nh_docs_parseCommits(
    NH_BYTE *path_p)
{
NH_DOCS_BEGIN()

    NH_BYTE *data_p = nh_core_getFileData(path_p, NULL);

    nh_docs_Tokenizer Tokenizer;
    nh_docs_tokenize(&Tokenizer, data_p, strlen(data_p));

    nh_docs_CommitParser Parser;
    Parser.Token_p = Tokenizer.Tokens_p;
    Parser.Node_p = malloc(sizeof(nh_docs_CommitNode));
    Parser.type = NH_DOCS_COMMIT_NODE_UNDEFINED;
    NH_DOCS_CHECK_NULL_2(NULL, Parser.Node_p)

    nh_docs_initCommitNode(Parser.Node_p, NH_DOCS_COMMIT_NODE_UNDEFINED, NULL);

    while (Parser.Token_p->type != NH_DOCS_TOKEN_EOF) {
        NH_DOCS_CHECK_2(NULL, nh_docs_parseToken(&Parser))
    }

    nh_core_free(data_p);

NH_DOCS_END((nh_docs_CommitNode*)Parser.Node_p)
}

// PARSE PARSE-TREE ================================================================================

static NH_DOCS_RESULT nh_docs_getAuthor(
    nh_docs_Author *Author_p, nh_docs_CommitNode *Node_p)
{
NH_DOCS_BEGIN()

    memset(Author_p->name_p, 0, NH_DOCS_MAX_AUTHOR_NAME);
    memset(Author_p->contact_p, 0, NH_DOCS_MAX_AUTHOR_CONTACT);

    for (int i = 0; i < Node_p->children; ++i) {
        nh_docs_CommitNode *Child_p = Node_p->Children_pp[i];
        switch (Child_p->type) {
            case NH_DOCS_COMMIT_NODE_AUTHOR_NAME :
                NH_DOCS_CHECK_2(NH_DOCS_ERROR_BAD_STATE, strlen(Child_p->text_p) >= NH_DOCS_MAX_AUTHOR_NAME)
                strcpy(Author_p->name_p, Child_p->text_p);
                break;
            case NH_DOCS_COMMIT_NODE_AUTHOR_CONTACT :
                NH_DOCS_CHECK_2(NH_DOCS_ERROR_BAD_STATE, strlen(Child_p->text_p) >= NH_DOCS_MAX_AUTHOR_CONTACT)
                strcpy(Author_p->contact_p, Child_p->text_p);
                break;
            default : break;
        }
    }

NH_DOCS_DIAGNOSTIC_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_getWorkload(
    nh_docs_Workload *Workload_p, nh_docs_CommitNode *Node_p)
{
NH_DOCS_BEGIN()

    memset(Workload_p, 0, sizeof(nh_docs_Workload));

    int bins = 0, libs = 0, lines = 0;

    for (int i = 0; i < Node_p->children; ++i) {
        nh_docs_CommitNode *Child_p = Node_p->Children_pp[i];
        switch (Child_p->type) {
            case NH_DOCS_COMMIT_NODE_WORKLOAD_BIN :
                ++bins;
                break;
            case NH_DOCS_COMMIT_NODE_WORKLOAD_LIB :
                ++libs;
                break;
            case NH_DOCS_COMMIT_NODE_WORKLOAD_MESSAGE :
                ++lines;
                break;
            case NH_DOCS_COMMIT_NODE_WORKLOAD_SCOPE :
                NH_DOCS_CHECK_2(NH_DOCS_ERROR_BAD_STATE, strlen(Child_p->text_p) >= NH_DOCS_MAX_WORKLOAD_SCOPE)
                Workload_p->scope = nh_docs_getScopeValue(Child_p->text_p);
                break;
            default : break;
        }
    }

    if (bins)
    {
        Workload_p->bins_pp = malloc(sizeof(NH_BYTE*)*bins);
        NH_DOCS_CHECK_NULL(Workload_p->bins_pp)
        for (int i = 0, j = 0; i < Node_p->children; ++i) {
            if (Node_p->Children_pp[i]->type == NH_DOCS_COMMIT_NODE_WORKLOAD_BIN) {
                Workload_p->bins_pp[j++] = Node_p->Children_pp[i]->text_p;
            }
        }
        Workload_p->bins = bins;
    }

    if (libs)
    {
        Workload_p->libs_pp = malloc(sizeof(NH_BYTE*)*libs);
        NH_DOCS_CHECK_NULL(Workload_p->libs_pp)
        for (int i = 0, j = 0; i < Node_p->children; ++i) {
            if (Node_p->Children_pp[i]->type == NH_DOCS_COMMIT_NODE_WORKLOAD_LIB) {
                Workload_p->libs_pp[j++] = Node_p->Children_pp[i]->text_p;
            }
        }
        Workload_p->libs = libs;
    }

    if (lines)
    {
        Workload_p->Message.lines_pp = malloc(sizeof(NH_BYTE*)*lines);
        NH_DOCS_CHECK_NULL(Workload_p->Message.lines_pp)
        for (int i = 0, j = 0; i < Node_p->children; ++i) {
            if (Node_p->Children_pp[i]->type == NH_DOCS_COMMIT_NODE_MESSAGE) {
                Workload_p->Message.lines_pp[j++] = Node_p->Children_pp[i]->text_p;
            }
        }
        Workload_p->Message.lines = lines;
    }

NH_DOCS_DIAGNOSTIC_END(NH_DOCS_SUCCESS)
}

static NH_DOCS_RESULT nh_docs_parseCommitNode(
    nh_docs_CommitNode *Root_p, nh_docs_Commit *Commit_p)
{
NH_DOCS_BEGIN()

    memset(Commit_p, 0, sizeof(nh_docs_Commit));

    int workloads = 0, lines = 0, ids = 0;

    for (int i = 0; i < Root_p->children; ++i) {
        nh_docs_CommitNode *Child_p = Root_p->Children_pp[i];
        switch (Child_p->type) {
            case NH_DOCS_COMMIT_NODE_ID :
                NH_DOCS_CHECK_2(NH_DOCS_ERROR_BAD_STATE, strlen(Child_p->text_p) >= NH_DOCS_MAX_COMMIT_ID)
                strcpy(Commit_p->id_p, Child_p->text_p);
                break;
            case NH_DOCS_COMMIT_NODE_DATE :
                NH_DOCS_CHECK_2(NH_DOCS_ERROR_BAD_STATE, strlen(Child_p->text_p) >= NH_DOCS_MAX_COMMIT_DATE)
                strcpy(Commit_p->date_p, Child_p->text_p);
                break;
            case NH_DOCS_COMMIT_NODE_AUTHOR :
                NH_DOCS_CHECK(nh_docs_getAuthor(&Commit_p->Author, Root_p->Children_pp[i]))
                break;
            case NH_DOCS_COMMIT_NODE_WORKLOAD :
                ++workloads;
                break;
            case NH_DOCS_COMMIT_NODE_MESSAGE :
                ++lines;
                break;
            case NH_DOCS_COMMIT_NODE_MERGE :
                ++ids;
                break;
            default : break;
        }
    }

    if (workloads)
    {
        Commit_p->Workloads_p = malloc(sizeof(nh_docs_Workload)*workloads);
        NH_DOCS_CHECK_NULL(Commit_p->Workloads_p)

        for (int i = 0, count = 0; i < Root_p->children; ++i) {
            if (Root_p->Children_pp[i]->type == NH_DOCS_COMMIT_NODE_WORKLOAD) {
                NH_DOCS_CHECK(nh_docs_getWorkload(Commit_p->Workloads_p+count, Root_p->Children_pp[i]))
                count++;
            } 
        }

        Commit_p->workloads = workloads;
    }

    if (lines)
    {
        Commit_p->Message.lines_pp = malloc(sizeof(NH_BYTE*)*lines);
        NH_DOCS_CHECK_NULL(Commit_p->Message.lines_pp)
        for (int i = 0, j = 0; i < Root_p->children; ++i) {
            if (Root_p->Children_pp[i]->type == NH_DOCS_COMMIT_NODE_MESSAGE) {
                Commit_p->Message.lines_pp[j++] = Root_p->Children_pp[i]->text_p;
            }
        }
        Commit_p->Message.lines = lines;
    }

    if (ids)
    {
        Commit_p->Merge.ids_pp = malloc(sizeof(NH_BYTE*)*ids);
        NH_DOCS_CHECK_NULL(Commit_p->Merge.ids_pp)
        for (int i = 0, j = 0; i < Root_p->children; ++i) {
            if (Root_p->Children_pp[i]->type == NH_DOCS_COMMIT_NODE_MESSAGE) {
                Commit_p->Merge.ids_pp[j++] = Root_p->Children_pp[i]->text_p;
            }
        }
        Commit_p->Merge.ids = ids;
    }

NH_DOCS_END(NH_DOCS_SUCCESS)
}

nh_docs_Commit *nh_docs_getCommits(
    nh_docs_CommitNode *Root_p, int *commits_p)
{
NH_DOCS_BEGIN()

    if (Root_p->children <= 0) {NH_DOCS_END(NULL)}

    nh_docs_Commit *Commits_p = malloc(sizeof(nh_docs_Commit)*Root_p->children);
    NH_DOCS_CHECK_NULL_2(NULL, Commits_p)

    for (int i = 0; i < Root_p->children; ++i) {
        NH_DOCS_CHECK_2(NULL, nh_docs_parseCommitNode(Root_p->Children_pp[i], Commits_p+i))
    } 

    if (commits_p) {*commits_p = Root_p->children;}

NH_DOCS_END(Commits_p)
}

