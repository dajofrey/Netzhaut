#ifndef NH_DOCS_CHANGELOG_CHANGELOG_H
#define NH_DOCS_CHANGELOG_CHANGELOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup lib_nhdocs_typedefs
 *  @{
 */

    typedef nh_docs_Changelog (*nh_docs_parseChangelog_f)(
        NH_BYTE *path_p
    );

    typedef void (*nh_docs_freeChangelog_f)(
        nh_docs_Changelog *Changelog_p
    );

    typedef nh_docs_ChangelogPart (*nh_docs_getChangelogForContext_f)(
       nh_docs_Changelog *Changelog_p, nh_docs_Context Context
    );

    typedef void (*nh_docs_freeChangelogPart_f)(
       nh_docs_ChangelogPart *Part_p
    );

    typedef NH_DOCS_RESULT (*nh_docs_printChangelog_f)(
        nh_docs_Changelog *Changelog_p
    );

    typedef NH_DOCS_RESULT (*nh_docs_stringifyChangelog_f)(
        nh_docs_Changelog *Changelog_p, NH_BYTE *result_p, int length
    );

    typedef NH_DOCS_RESULT (*nh_docs_stringifyCommit_f)(
        nh_docs_Commit *Commit_p, NH_BYTE *commit_p, int length
    );

/** @} */

/** @addtogroup lib_nhdocs_functions
 *  @{
 */

/** @} */

#endif // NH_DOCS_CHANGELOG_CHANGELOG_H
