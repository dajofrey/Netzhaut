// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Window.h"
#include "../Header/Macros.h"

#include NH_DEBUG

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#if defined(_WIN32) || defined (WIN32)
    #include <windows.h>
    #include <windowsx.h>
    #include <winuser.h>
    #include <Shellapi.h>
#endif

// CALLBACK DECLARATION ============================================================================

#if defined(_WIN32) || defined (WIN32)
    LRESULT CALLBACK WindowProc_(
        NHND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam
    );
#endif

// CREATE ==========================================================================================

NH_RESULT NH_IO_Win_createWindow(
    char *title, int width, int height, NH_IO_Win_Window *Window_p)
{
NH_BEGIN()

#if defined(_WIN32) || defined (WIN32)

    Window_p->hinstance = (HINSTANCE) GetModuleHandle(NULL);

    const wchar_t CLASS_NAME[]  = L"Sample Window Class"; 
    WNDCLASS wc = {0};
    Window_p->wc = wc;	
    Window_p->wc.lpfnWndProc   = WindowProc_;
    Window_p->wc.hInstance     = Window_p->hinstance;
    Window_p->wc.lpszClassName = CLASS_NAME;
    RegisterClass(&Window_p->wc);

    Window_p->hwnd = CreateWindowEx(
        0,
        CLASS_NAME,           // Window class
        TEXT(title), // Window text
        WS_OVERLAPPEDWINDOW,  // Window style
        
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        
        NULL, // Parent window    
        NULL, // Menu
        Window_p->hinstance, // Window.handle
        (LPVOID) Window_p // Additional application data
    );
    
    if (Window_p->hwnd == NULL) {
        NH_END(NH_ERROR_BAD_STATE)
    }

    DragAcceptFiles(Window_p->hwnd, true);
    ShowWindow(Window_p->hwnd, 3);

    Window_p->type = NH_IO_TYPE_WIN;

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

// DESTROY =========================================================================================

NH_RESULT NH_IO_Win_destroyWindow(
    NH_IO_Win_Window *Window_p)
{
NH_BEGIN()

#if defined(_WIN32) || defined (WIN32)

    const wchar_t CLASS_NAME[]  = L"Sample Window Class";  
    DestroyWindow(Window_p->hwnd);
    UnregisterClassA(CLASS_NAME, Window_p->hinstance);

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

// SET =============================================================================================

NH_RESULT NH_IO_Win_setWindowBackgroundColor(
    NH_IO_Win_Window *Window_p, float color_p[4])
{
NH_BEGIN()

#if defined(_WIN32) || defined (WIN32)

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

NH_RESULT NH_IO_Win_setWindowTitle(
    NH_IO_Win_Window *Window_p, char *title_p)
{
NH_BEGIN()

#if defined(_WIN32) || defined (WIN32)

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

NH_RESULT NH_IO_Win_setMouseCursor(
    NH_IO_Win_Window *Window_p, char *title_p)
{
NH_BEGIN()

#if defined(_WIN32) || defined (WIN32)

    NH_END(NH_SUCCESS)

#endif

NH_END(NH_ERROR_BAD_STATE)
}

// CALLBACK =======================================================================================

#if defined(_WIN32) || defined (WIN32)

    LRESULT CALLBACK WindowProc_(
        NHND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        Nh_Window *Window_p = GetWindowLongPtr(hwnd, GWLP_USERDATA);
        POINTS pts = MAKEPOINTS(lParam);
        
        switch (uMsg)
        {
            case WM_NCCREATE :
                SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG) (( (CREATESTRUCT *) lParam)->lpCreateParams) );
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
    		
            case WM_CLOSE :
                return 0;
    
            case WM_DESTROY :
                PostQuitMessage(0);
                return 0;
    
            case WM_PAINT :
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
                EndPaint(hwnd, &ps);
                return 0;
    	
            case WM_DROPFILES :
                HDROP hDropInfo = (HDROP) wParam;
                UINT buffsize = 500;
                char buf[500];
                DragQueryFile(hDropInfo, 0, buf, buffsize);
                return 0;
    
           case WM_MOUSEMOVE :
               return 0;
               	
           case WM_LBUTTONDOWN :
               return 0; 
               	
           case WM_LBUTTONUP :
               return 0; 
               	
           case WM_RBUTTONDOWN :
               return 0;
               	
           default :
               return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

#endif

