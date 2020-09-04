#ifndef NH_TTY_ROW_H
#define NH_TTY_ROW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "Terminal.h"
#include <stddef.h>

#endif

/** @addtogroup TTYStructs Structs
 *  \ingroup TTY
 *  @{
 */

    typedef struct Nh_TTY_Row {
        int size;
        int rsize;
        char *chars_p;
        char *render_p;
    } Nh_TTY_Row;

/** @} */

/** @addtogroup TTYFunctions Functions
 *  \ingroup TTY
 *  @{
 */

    int Nh_TTY_rowCxToRx(
        Nh_TTY_Row *Row_p, int cx
    ); 
    
    void Nh_TTY_addRow(
        Nh_TTY_Tab *Tab_p, const char *line_p, size_t len
    ); 

    void Nh_TTY_destroyRows(
        Nh_TTY_Tab *Tab_p
    );

/** @} */

#endif 
