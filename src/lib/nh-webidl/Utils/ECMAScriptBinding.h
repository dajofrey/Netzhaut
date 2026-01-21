#ifndef NH_WEBIDL_UTILS_ECMASCRIPTBINDING_H
#define NH_WEBIDL_UTILS_ECMASCRIPTBINDING_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../../nh-ecmascript/Engine/Realm.h"

NH_API_RESULT nh_webidl_installInterfaces(
    nh_ecmascript_Realm *Realm_p
);

#endif
