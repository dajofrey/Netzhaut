#ifndef NH_WEBIDL_COMMON_LOG_H
#define NH_WEBIDL_COMMON_LOG_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"

#include "../Utils/Tokenizer.h"
#include "../Utils/Parser.h"
#include "../Utils/Global.h"

NH_API_RESULT nh_webidl_logTokens(
    char *fragmentName_p, nh_core_Array *Tokens_p, bool dirty
);

NH_API_RESULT nh_webidl_logParseTree(
    char *fragmentName_p, nh_webidl_ParseNode *ParseNode_p
);

NH_API_RESULT nh_webidl_logFragment(
    char *specification_p, nh_webidl_Fragment *Fragment_p
);

#endif // NH_WEBIDL_COMMON_LOG_H 
