#ifndef CHANGELOGS_H
#define CHANGELOGS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022 Dajo Frey
 * Published under MIT.
 */

#include "../../lib/nhapi/nhdocs.h"

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @addtogroup bin_nhdocs_functions
 *  @{
 */

    int generateChangelogPages(
        nh_docs_Changelog *Changelog_p, NH_BYTE *main_p, NH_BYTE *pages_p
    );

/** @} */

#endif // CHANGELOGS_H
