#ifndef PATCHES_H
#define PATCHES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022 Dajo Frey
 * Published under MIT.
 */

#include "../../lib/nhapi/nhdocs.h"

#endif

/** @addtogroup bin_nhdocs_functions
 *  @{
 */

    int createPatchPages(
        NH_BYTE *path_p, NH_BYTE *source_p, NH_BYTE *destination_p
    );

/** @} */

#endif
