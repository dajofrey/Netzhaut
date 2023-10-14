#ifndef HEADER_H
#define HEADER_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

#include "../../lib/nhapi/nhdocs.h"

#endif

/** @addtogroup bin_nhdocs_functions
 *  @{
 */

    int generateHeader(
        nh_docs_Changelog *Changelog_p, NH_BYTE *path_p
    );

/** @} */

#endif
