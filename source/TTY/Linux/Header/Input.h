#ifndef NH_TTY_LINUX_INPUT_H
#define NH_TTY_LINUX_INPUT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Terminal.h"

#endif
 
/** @addtogroup TTYFunctions Functions
 *  \ingroup TTY
 *  @{
 */

    int Nh_TTY_Linux_readKey(
    );
    
    int Nh_TTY_Linux_getCursorPosition(
        int *rows, int *cols
    ); 

/** @} */

#endif 
