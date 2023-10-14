#ifndef VERSION_H
#define VERSION_H

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

    int generateVersionInAboutFile(
        nh_docs_Context *Context_p, nh_docs_Version Version
    );

/** @} */

#endif // VERSION_H
