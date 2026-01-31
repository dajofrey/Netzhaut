// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Navigate.h"

#include "../Parser/Parser.h"
#include "../Interfaces/Document.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"

#include "../../nh-css/Main/LayoutEngine.h"
#include "../../nh-webidl/Runtime/Object.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

typedef struct nh_html_SourceSnapshotParams {
    bool hasTransientActivation;
    int sandboxingFlags;
    bool allowsDownloading;
    nh_html_PolicyContainer SourcePolicyContainer;
} nh_html_SourceSnapshotParams;

nh_html_SourceSnapshotParams nh_html_snapshotSourceSnapshotParams(
    nh_ecmascript_Object *Document_p)
{
    nh_html_SourceSnapshotParams SourceSnapshotParams;
    SourceSnapshotParams.hasTransientActivation = ...;
    return SourceSnapshotParams;
}

NH_API_RESULT *nh_html_navigate(
    nh_html_Navigable *Navigable_p, nh_url_URL URL, nh_ecmascript_Object *SourceDocument_p, void *DocumentResource_p)
{
    char *cspNavigationType_p = "other";
    nh_html_SourceSnapshotParams SourceSnapshotParams = nh_html_snapshotSourceSnapshotParams;
    nh_html_Origin InitiatorOriginSnapshot = SourceDocument_p->origin
    ...

    nh_crypto_UUID NavigationId = nh_crypto_generateRandomUUID();
    
    return NH_API_SUCCESS; 
}
