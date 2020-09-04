// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Input.h"
#include "../Header/Macros.h"

#include NH_DEBUG

// IMPLEMENT ======================================================================================

NH_RESULT NH_IO_Win_getInput(
    Nh_Window *Window_p) 
{
NH_BEGIN()

#if defined(_WIN32) || defined (WIN32)
    
    MSG msg = {0};
    while (GetMessage(&msg, Window_p->WSI.Win.hwnd, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

