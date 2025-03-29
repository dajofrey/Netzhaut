// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Parser.h"
#include "Tokenizer.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/File.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

// CREATE PARSE-TREE ===============================================================================

typedef struct nh_ConfigParser {
    nh_ConfigToken *Token_p;
    nh_core_RawConfig *Config_p;
    nh_core_RawConfig *GlobalConfig_p;
} nh_ConfigParser;

static NH_API_RESULT nh_core_parseRawConfigValues(
    nh_ConfigParser *Parser_p, nh_core_List *Values_p) 
{
    *Values_p = nh_core_initList(4);

    while (Parser_p->Token_p->type != NH_CONFIG_TOKEN_SEMICOLON && Parser_p->Token_p->type != NH_CONFIG_TOKEN_EOF) {
        switch (Parser_p->Token_p->type) {
            case NH_CONFIG_TOKEN_STRING :
            case NH_CONFIG_TOKEN_IDENTIFIER :
            {
                char *value_p = malloc(sizeof(char)*(strlen(Parser_p->Token_p->string_p)+1));
                NH_CORE_CHECK_MEM(value_p)
                strcpy(value_p, Parser_p->Token_p->string_p);
                nh_core_appendToList(Values_p, value_p);
                break;
            }
        }
        ++(Parser_p->Token_p);
    }

    ++(Parser_p->Token_p);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_parseConfigSetting(
    nh_ConfigParser *Parser_p) 
{
    if (!Parser_p->Token_p->string_p) {return NH_API_ERROR_BAD_STATE;}

    nh_core_RawConfigSetting *Setting_p = malloc(sizeof(nh_core_RawConfigSetting));
    NH_CORE_CHECK_NULL(Setting_p)

    memset(Setting_p, 0, sizeof(nh_core_RawConfigSetting));
    char *string_p = Parser_p->Token_p->string_p;

    int c = 0, c2 = 0;
    for (c = 0; c < strlen(string_p) && string_p[c] != '.'; ++c);
    for (c2 = 0; c2 < strlen(string_p) && string_p[c2] != '_'; ++c2);

    // First, handle potential namespace.
    if (c2 < c) {
        string_p[c2] = 0; 
        strcpy(Setting_p->namespace_p, string_p);
        string_p[c2] = '_';
        string_p += c2+1;
    }

    for (c = 0; c < strlen(string_p) && string_p[c] != '.'; ++c);
    if (c == strlen(string_p)) {return NH_API_ERROR_BAD_STATE;}

    string_p[c] = 0;
    int module = nh_core_getModuleIndex(string_p);
    string_p[c] = '.';

    Setting_p->module = module;
    Setting_p->name_p = malloc(sizeof(char)*(strlen(string_p)+1));
    NH_CORE_CHECK_MEM(Setting_p->name_p)
    memset(Setting_p->name_p, 0, sizeof(char)*(strlen(string_p)+1));
 
    if (module >= 0) {
        strcpy(Setting_p->name_p, string_p+c+1);
    } else {
        strcpy(Setting_p->name_p, string_p);
    }

    if (Parser_p->GlobalConfig_p) {
        for (int i = 0; i < Parser_p->GlobalConfig_p->Settings.size; ++i) {
            nh_core_RawConfigSetting *DefaultSetting_p = Parser_p->GlobalConfig_p->Settings.pp[i];
            if (!DefaultSetting_p->Default_p && DefaultSetting_p->module == Setting_p->module && !strcmp(DefaultSetting_p->name_p, Setting_p->name_p)) {
                Setting_p->Default_p = DefaultSetting_p;
                break;
            }
        }
    }

    ++(Parser_p->Token_p);
    if (Parser_p->Token_p->type != NH_CONFIG_TOKEN_COLON) {return NH_API_ERROR_BAD_STATE;}
    ++(Parser_p->Token_p);
 
    NH_CORE_CHECK(nh_core_parseRawConfigValues(Parser_p, &Setting_p->Values))
    nh_core_appendToList(&Parser_p->Config_p->Settings, Setting_p);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_core_parseConfigToken(
    nh_ConfigParser *Parser_p)
{
    if (Parser_p->Token_p->type != NH_CONFIG_TOKEN_IDENTIFIER) {
        return NH_API_ERROR_BAD_STATE;
    }

    switch ((Parser_p->Token_p+1)->type) {
        case NH_CONFIG_TOKEN_COLON :
            return nh_core_parseConfigSetting(Parser_p);
        case NH_CONFIG_TOKEN_EOF :
            ++(Parser_p->Token_p);
            break;
        default :
            return NH_API_ERROR_BAD_STATE;
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_parseRawConfig(
    nh_core_RawConfig *Config_p, char *data_p, int length, nh_core_RawConfig *GlobalConfig_p)
{
    nh_ConfigTokenizer Tokenizer;
    nh_core_tokenizeConfig(&Tokenizer, data_p, length);

    nh_ConfigParser Parser;
    Parser.Token_p = Tokenizer.Tokens_p;
    Parser.Config_p = Config_p;
    Parser.GlobalConfig_p = GlobalConfig_p;

    while (Parser.Token_p->type != NH_CONFIG_TOKEN_EOF) {
        NH_CORE_CHECK(nh_core_parseConfigToken(&Parser))
    }

    nh_core_freeConfigTokenizer(&Tokenizer);

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_core_freeRawConfig(
    nh_core_RawConfig *Config_p)
{
    for (int i = 0; i < Config_p->Settings.size; ++i) {
        nh_core_RawConfigSetting *Setting_p = Config_p->Settings.pp[i];
        if (Setting_p->Default_p == NULL) {
            nh_core_free(Setting_p->name_p);
        } 
        nh_core_freeList(&Setting_p->Values, true);
    } 

    nh_core_freeList(&Config_p->Settings, true);

    memset(Config_p, 0, sizeof(nh_core_RawConfig));

    return NH_API_SUCCESS;
}
