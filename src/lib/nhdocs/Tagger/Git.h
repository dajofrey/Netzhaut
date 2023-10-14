#ifndef NH_DOCS_TAGGER_GIT_H
#define NH_DOCS_TAGGER_GIT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhdocs_functions
 *  @{
 */

    nh_docs_Context *nh_docs_getContextsUsingGit(
        int *count_p, NH_BYTE *indexOrId_p
    );
    
    nh_docs_Commit *nh_docs_createCommitUsingGit(
        NH_BYTE *indexOrId_p
    );

/** @} */

#endif // NH_DOCS_TAGGER_GIT_H
