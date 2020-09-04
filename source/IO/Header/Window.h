#ifndef NH_IO_WINDOW_H
#define NH_IO_WINDOW_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../X11/Header/Window.h"
#include "../Win/Header/Window.h"

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/Thread.h"

#endif

/** @addtogroup IOEnums Enums
 *  \ingroup IO
 *  @{
 */

    typedef enum NH_IO_TYPE {
        NH_IO_TYPE_X11,
        NH_IO_TYPE_WIN,
    } NH_IO_TYPE;

/** @} */

/** @addtogroup IOStructs Structs
 *  \ingroup IO
 *  @{
 */

    typedef struct NH_IO_Window {
        NH_IO_TYPE type;
        NH_IO_X11_Window X11;
        NH_IO_Win_Window Win;
    } NH_IO_Window;

/** @} */

/** @addtogroup IOFunctions Functions
 *  \ingroup IO
 *  @{
 */

    NH_RESULT Nh_IO_createWindow(
        char *title_p, int width, int height, NH_IO_Window *Window_p
    );
    
    NH_RESULT Nh_IO_destroyWindow(
        NH_IO_Window *Window_p
    );
    
    NH_RESULT Nh_IO_setWindowBackgroundColor(
        NH_IO_Window *Window_p, float color_p[4]
    );
    
    NH_RESULT Nh_IO_setWindowTitle(
        NH_IO_Window *Window_p, char *title_p
    );
    
    NH_RESULT Nh_IO_setMouseCursor(
        NH_IO_Window *Window_p, char *title_p
    );

    NH_IO_TYPE Nh_IO_getType(
    );

/** @} */

#endif 
