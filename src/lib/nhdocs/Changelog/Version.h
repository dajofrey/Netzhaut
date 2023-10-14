#ifndef NH_DOCS_CHANGELOG_VERSION_H
#define NH_DOCS_CHANGELOG_VERSION_H

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

    typedef nh_docs_Version (*nh_docs_getVersion_f)(
        nh_docs_Changelog *Changelog_p, nh_docs_Context *Context_p, nh_docs_Commit *LastCommit_p
    );

    typedef nh_docs_Version (*nh_docs_getRevision_f)(
        nh_docs_Changelog *Changelog_p, nh_docs_Commit *LastCommit_p
    );

/** @} */

/** @addtogroup lib_nhdocs_functions
 *  @{
 */

    NH_DOCS_SCOPE_E nh_docs_getScopeValue(
        NH_BYTE *scope_p
    );

    /**
     * Don't free the result!
     */
    NH_BYTE *nh_docs_getScopeString(
        NH_DOCS_SCOPE_E scope
    );

/** @} */

#endif // NH_DOCS_CHANGELOG_VERSION_H
