#ifndef NH_ECMASCRIPT_INTRINSICS_PARSER_H
#define NH_ECMASCRIPT_INTRINSICS_PARSER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Intrinsics.h"
#include "Tokenizer.h"

#include "../Common/Includes.h"
#include "../Engine/Object.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/String.h"

nh_ecmascript_IntrinsicTemplate* nh_ecmascript_parseIntrinsicTemplate(
    nh_ecmascript_Token *tokens,
    int count
);

#endif
