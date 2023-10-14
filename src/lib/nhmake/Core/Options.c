// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Options.h"
#include "Thread.h"
#include "File.h"
#include "Build.h"

#include "../Parser/Functions.h"
#include "../Parser/Variables.h"
#include "../Common/Macros.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// HELPER ==========================================================================================

static NH_BYTE *nh_make_offsetBuildArgumentPrefix(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    nh_make_Variable *Prefix_p = nh_make_getVariable(&Runtime_p->VariableArray, "PREFIX");

    if (Prefix_p) {
        int offset = 0;
        for (offset = 0; offset < strlen(Prefix_p->values_pp[0]) && offset < strlen(name_p) 
            && Prefix_p->values_pp[0][offset] == name_p[offset]; ++offset) {};
        if (offset == strlen(Prefix_p->values_pp[0])) {
            NH_MAKE_END(name_p + offset)
        }
    }

NH_MAKE_END(name_p)
}

// GET OPTION ======================================================================================

static nh_make_Option *nh_make_getOptionFromBlock(
    nh_make_Runtime *Runtime_p, nh_make_Block *Block_p, NH_BYTE *name_p, NH_BYTE **argv_pp, int args)
{
NH_MAKE_BEGIN()

    for (int d = 0; d < Block_p->definitions; ++d) 
    {
        switch (Block_p->Definitions_p[d].type)
        {
            case NH_MAKE_DEFINITION_IF :
                if (nh_make_compareIf(Runtime_p, &Block_p->Definitions_p[d].If, NH_MAKE_TRUE)) {
                    nh_make_Option *Option_p = nh_make_getOptionFromBlock(Runtime_p, &Block_p->Definitions_p[d].If.Block_p->Block, name_p, argv_pp, args);
                    if (Option_p) {NH_MAKE_END(Option_p)}
                }
                break;

            case NH_MAKE_DEFINITION_OPTION :
                if (!strcmp(Block_p->Definitions_p[d].Option.name_p, name_p)
                &&  Block_p->Definitions_p[d].Option.arguments == args) {
                    NH_MAKE_BOOL sameArguments = NH_MAKE_TRUE;
                    for (int i = 0; i < args; ++i) {
                        NH_BYTE *argument_p = Block_p->Definitions_p[d].Option.arguments_pp[i];
                        NH_BYTE *offsetArgument_p = argument_p;
                        if (Block_p->Definitions_p[d].Option.name_p[0] == 'b' || Block_p->Definitions_p[d].Option.name_p[0] == 't' || Block_p->Definitions_p[d].Option.name_p[0] == 'i') {
                            offsetArgument_p = nh_make_offsetBuildArgumentPrefix(Runtime_p, argument_p);
                        }
                        if (strcmp(argument_p, argv_pp[i]) && strcmp(offsetArgument_p, argv_pp[i])) {
                            sameArguments = NH_MAKE_FALSE; 
                            break;
                        }
                    }
                    if (sameArguments) {
                        NH_MAKE_END(&Block_p->Definitions_p[d].Option)
                    }
                }
                break;
        }
    }

NH_MAKE_END(NULL)
}

static nh_make_Option *nh_make_getOptionFromParser(
    nh_make_Runtime *Runtime_p, nh_make_Parser *Parser_p, NH_BYTE *name_p, NH_BYTE **argv_pp, int args)
{
NH_MAKE_BEGIN()

    nh_make_Block Block;
    Block.definitions = Parser_p->definitions;
    Block.Definitions_p = Parser_p->Definitions_p;

NH_MAKE_END(nh_make_getOptionFromBlock(Runtime_p, &Block, name_p, argv_pp, args))
}

static nh_make_Option *nh_make_getShortOptions(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p, NH_BYTE **argv_pp, int args, int *options_p)
{
NH_MAKE_BEGIN()

    *options_p = 0;

    nh_make_Option *Options_p = NULL;
    NH_BYTE c = 0;

    for (int i = 0; i < strlen(name_p); ++i) 
    {
        if (i + 1 < strlen(name_p)) {
            c = name_p[i+1];
            name_p[i+1] = 0;
        }

        nh_make_Option *Option_p = NULL;
        for (int j = 0; j < Runtime_p->ParserArray.length; ++j) {
            nh_make_Parser *Parser_p = &Runtime_p->ParserArray.Parsers_p[j];
            Option_p = nh_make_getOptionFromParser(Runtime_p, Parser_p, name_p+i, argv_pp, args);
            if (Option_p) {break;}
        }

        if (Option_p) {
            if (*options_p == 0) {
                Options_p = Option_p;
            }
            else if (*options_p == 1) {
                nh_make_Option *Tmp_p = malloc(sizeof(nh_make_Option)*2);
                Tmp_p[0] = *Options_p;
                Tmp_p[1] = *Option_p;
                Options_p = Tmp_p;
            }
            else {
                Options_p = realloc(Options_p, sizeof(nh_make_Option)*(*options_p+1));
                Options_p[*options_p] = *Option_p;
            }
            *options_p += 1;
        }

        if (c != 0) {
            name_p[i+1] = c;
            c = 0;
        }
    }

NH_MAKE_END(Options_p)
}

static nh_make_Option *nh_make_getOption(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p, int args, NH_BYTE **argv_pp)
{
NH_MAKE_BEGIN()

    nh_make_Option *Option_p = NULL;

    for (int j = 0; j < Runtime_p->ParserArray.length; ++j) {
        nh_make_Parser *Parser_p = &Runtime_p->ParserArray.Parsers_p[j];
        Option_p = nh_make_getOptionFromParser(Runtime_p, Parser_p, name_p, argv_pp, args);
        if (Option_p) {break;}
    }

NH_MAKE_END(Option_p)
}

// OPTION NAME =====================================================================================

static NH_MAKE_BOOL nh_make_optionNameExistsInBlock(
    nh_make_Runtime *Runtime_p, nh_make_Block *Block_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    for (int d = 0; d < Block_p->definitions; ++d) {
        switch (Block_p->Definitions_p[d].type) {
            case NH_MAKE_DEFINITION_OPTION :
                if (!strcmp(Block_p->Definitions_p[d].Option.name_p, name_p)) {
                    NH_MAKE_END(NH_MAKE_TRUE)
                }
                break;
            case NH_MAKE_DEFINITION_IF :
                if (nh_make_compareIf(Runtime_p, &Block_p->Definitions_p[d].If, NH_MAKE_TRUE)) {
                    if (nh_make_optionNameExistsInBlock(Runtime_p, &Block_p->Definitions_p[d].If.Block_p->Block, name_p)) {
                        NH_MAKE_END(NH_MAKE_TRUE)
                    }
                }
                break;
        }
          
    } 

NH_MAKE_END(NH_MAKE_FALSE)
}

static NH_MAKE_BOOL nh_make_optionNameExists(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    for (int i = 0; i < Runtime_p->ParserArray.length; ++i) {
        nh_make_Parser *Parser_p = &Runtime_p->ParserArray.Parsers_p[i];
        nh_make_Block Block;
        Block.definitions = Parser_p->definitions;
        Block.Definitions_p = Parser_p->Definitions_p;
        if (nh_make_optionNameExistsInBlock(Runtime_p, &Block, name_p)) {
            NH_MAKE_END(NH_MAKE_TRUE)
        } 
    }

NH_MAKE_END(NH_MAKE_FALSE)
}

// EXECUTE OPTION ==================================================================================

static NH_MAKE_RESULT nh_make_printOption(
    nh_make_Option *Option_p)
{
NH_MAKE_BEGIN()

    NH_BYTE args_p[1024]; // TODO malloc
    memset(args_p, 0, 1024);
    for (int i = 0; i < Option_p->arguments; ++i) {
        sprintf(args_p + strlen(args_p), "%s ", Option_p->arguments_pp[i]); 
    }

    nh_make_operationf(Option_p->longOption ? "--%s %s'%s'" : "-%s %s'%s'", Option_p->name_p, args_p, Option_p->description_p);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_executeOption(
    nh_make_Runtime *Runtime_p, nh_make_Option *Option_p)
{
NH_MAKE_BEGIN()

    NH_MAKE_CHECK(nh_make_printOption(Option_p))

    if (!Option_p->Block_p) {
        if (!strcmp(Option_p->name_p, "b")) {
            NH_MAKE_CHECK(nh_make_build(Runtime_p, Option_p->arguments_pp[0]))
        }
        else if (!strcmp(Option_p->name_p, "i")) {
        }
        else if (!strcmp(Option_p->name_p, "t")) {
            NH_MAKE_CHECK(nh_make_test(Runtime_p, Option_p->arguments_pp[0]))
        }
        else {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_BAD_STATE)}
    }
    else {
        NH_MAKE_CHECK(nh_make_executeBlock(Runtime_p, &Option_p->Block_p->Block))
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// BUILD OPTIONS ===================================================================================

static NH_MAKE_BOOL nh_make_buildOptionOverride(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    for (int j = 0; j < Runtime_p->ParserArray.length; ++j)
    {
        nh_make_Parser *Parser_p = &Runtime_p->ParserArray.Parsers_p[j];

        for (int i = 0; i < Parser_p->definitions; ++i) 
        {
            nh_make_Definition *Definition_p = &Parser_p->Definitions_p[i];

            if (Definition_p->type == NH_MAKE_DEFINITION_OPTION 
            &&  !Definition_p->Option.longOption
            &&  !strcmp(Definition_p->Option.name_p, "b") 
            &&  Definition_p->Option.arguments == 1 
            &&  (!strcmp(Definition_p->Option.arguments_pp[0], name_p) || !strcmp(nh_make_offsetBuildArgumentPrefix(Runtime_p, Definition_p->Option.arguments_pp[0]), name_p))) {
                NH_MAKE_END(NH_MAKE_TRUE)
            }
        }
    }

NH_MAKE_END(NH_MAKE_FALSE)
}

NH_MAKE_RESULT nh_make_addBuildOption(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    if (nh_make_buildOptionOverride(Runtime_p, name_p)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)}

    static NH_BYTE *option_p = "b";

    nh_make_Parser *Parser_p = &Runtime_p->ParserArray.Parsers_p[0];

    Parser_p->Definitions_p = realloc(Parser_p->Definitions_p, sizeof(nh_make_Definition) * (Parser_p->definitions + 1));
    NH_MAKE_CHECK_NULL(Parser_p->Definitions_p)

    nh_make_Definition *Definition_p = &Parser_p->Definitions_p[Parser_p->definitions++];

    NH_BYTE *description_p = malloc(strlen(name_p) + 7);
    NH_MAKE_CHECK_NULL(description_p)
    sprintf(description_p, "build %s", name_p);

    NH_BYTE *argument_p = malloc(strlen(name_p) + 1);
    NH_MAKE_CHECK_NULL(argument_p)
    strcpy(argument_p, name_p);

    NH_BYTE **arguments_pp = malloc(sizeof(NH_BYTE*));
    NH_MAKE_CHECK_NULL(arguments_pp)
    arguments_pp[0] = argument_p;

    Definition_p->type = NH_MAKE_DEFINITION_OPTION;
    Definition_p->Option.arguments     = 1;
    Definition_p->Option.arguments_pp  = arguments_pp;
    Definition_p->Option.longOption    = NH_MAKE_FALSE;
    Definition_p->Option.name_p        = option_p;
    Definition_p->Option.description_p = description_p;
    Definition_p->Option.Block_p       = NULL;

    nh_make_messagef("Generate Option '-b %s'", name_p);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_addTestOption(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p)
{
NH_MAKE_BEGIN()

    static NH_BYTE *option_p = "t";

    nh_make_Parser *Parser_p = &Runtime_p->ParserArray.Parsers_p[0];

    Parser_p->Definitions_p = realloc(Parser_p->Definitions_p, sizeof(nh_make_Definition) * (Parser_p->definitions + 1));
    NH_MAKE_CHECK_NULL(Parser_p->Definitions_p)

    nh_make_Definition *Definition_p = &Parser_p->Definitions_p[Parser_p->definitions++];

    NH_BYTE *description_p = malloc(strlen(name_p) + 7);
    NH_MAKE_CHECK_NULL(description_p)
    sprintf(description_p, "test %s", name_p);

    NH_BYTE *argument_p = malloc(strlen(name_p) + 1);
    NH_MAKE_CHECK_NULL(argument_p)
    strcpy(argument_p, name_p);

    NH_BYTE **arguments_pp = malloc(sizeof(NH_BYTE*));
    NH_MAKE_CHECK_NULL(arguments_pp)
    arguments_pp[0] = argument_p;

    Definition_p->type = NH_MAKE_DEFINITION_OPTION;
    Definition_p->Option.arguments     = 1;
    Definition_p->Option.arguments_pp  = arguments_pp;
    Definition_p->Option.longOption    = NH_MAKE_FALSE;
    Definition_p->Option.name_p        = option_p;
    Definition_p->Option.description_p = description_p;
    Definition_p->Option.Block_p       = NULL;

    nh_make_messagef("Generate Option '-t %s'", name_p);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// PARSE ===========================================================================================

static NH_MAKE_RESULT nh_make_parseShortOption(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p, int optArgsCount, NH_BYTE **optArgs_pp, int *parsedOptArgs_p)
{
NH_MAKE_BEGIN()

    for (int i = 0; i < optArgsCount; ++i) 
    {
        int options = 0;
        nh_make_Option *Options_p = nh_make_getShortOptions(Runtime_p, name_p, &optArgs_pp[i], 1, &options);
        if (Options_p) {
            for (int j = 0; j < options; ++j) {
                NH_MAKE_CHECK(nh_make_executeOption(Runtime_p, Options_p+j))
            }
            if (options > 1) {free(Options_p);}
        }
        *parsedOptArgs_p += 1;
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

static NH_MAKE_RESULT nh_make_parseLongOption(
    nh_make_Runtime *Runtime_p, NH_BYTE *name_p, int optArgsCount, NH_BYTE **optArgs_pp, int *parsedOptArgs_p)
{
NH_MAKE_BEGIN()

    nh_make_Option *Option_p = NULL;

    // Get trivial option.
    if (optArgsCount == 0) {
        Option_p = nh_make_getOption(Runtime_p, name_p, 0, NULL);
        if (!Option_p) {
            NH_MAKE_END(NH_MAKE_ERROR_INVALID_OPTION)
        }
        *parsedOptArgs_p = 0;
    }

    // Get option which matches the most arguments.
    for (int i = optArgsCount; i > 0; --i) {
        Option_p = nh_make_getOption(Runtime_p, name_p, i, optArgs_pp);
        if (Option_p) {
            *parsedOptArgs_p = i;
            break;
        }
    }

    if (!Option_p) {
        NH_MAKE_END(NH_MAKE_ERROR_INVALID_OPTION)
    }

    NH_MAKE_CHECK(nh_make_executeOption(Runtime_p, Option_p))
 
NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_parseOption(
    nh_make_Runtime *Runtime_p, int optionLength, NH_BYTE **option_pp, int *advance_p)
{
NH_MAKE_BEGIN()

    for (int i = 0, advance = 0; i < optionLength; i += advance) 
    {
        NH_BYTE **optArgs_pp = NULL;
        int optArgsCount = 0;
        if (i == 0) {
            if (optionLength > 1) {
                optArgs_pp = option_pp+1;
                optArgsCount = optionLength-1;
            }
        }
        else {
            optArgs_pp = option_pp+i;
            optArgsCount = optionLength - i;
        } 

        int parsedOptArgs = 0;

        if (option_pp[0][0] == '-' && option_pp[0][1] != '-') {
            NH_MAKE_CHECK(nh_make_parseShortOption(Runtime_p, option_pp[0]+1, optArgsCount, optArgs_pp, &parsedOptArgs))
        }
        else if (option_pp[0][0] == '-' && option_pp[0][1] == '-') {
            NH_MAKE_CHECK(nh_make_parseLongOption(Runtime_p, option_pp[0]+2, optArgsCount, optArgs_pp, &parsedOptArgs))
        }
        else {
            NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_INVALID_OPTION)
        }

        advance = i ? parsedOptArgs : parsedOptArgs + 1;
        *advance_p += advance;
    }

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

