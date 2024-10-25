// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Win32.h"

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

// INPUT ===========================================================================================

NH_API_RESULT nh_Win32_getInput(
    nh_Win32_Window *Window_p)
{
#if defined(_WIN32) || defined (WIN32)
    
    MSG msg = {0};
    while (GetMessage(&msg, Windpow_p->hwnd, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return NH_API_SUCCESS;

#endif

    return NH_API_ERROR_BAD_STATE;
}

// WINDOW ==========================================================================================

NH_API_RESULT nh_Win32_createWindow(
    nh_Win32_Window *Window_p, nh_wsi_WindowConfiguration *Configuration_p, 
    nh_gfx_SurfaceRequirements *Requirements_p)
{
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
        return NH_API_ERROR_BAD_STATE;
    }

    DragAcceptFiles(Window_p->hwnd, true);
    ShowWindow(Window_p->hwnd, 3);

    Window_p->type = NH_WSI_TYPE_WIN32;

    return NH_API_SUCCESS;

#endif

    return NH_API_ERROR_BAD_STATE;
}

// DESTROY =========================================================================================

NH_API_RESULT nh_Win32_destroyWindow(
    nh_Win32_Window *Window_p)
{
#if defined(_WIN32) || defined (WIN32)

    const wchar_t CLASS_NAME[]  = L"Sample Window Class";  
    DestroyWindow(Window_p->hwnd);
    UnregisterClassA(CLASS_NAME, Window_p->hinstance);

    return NH_API_SUCCESS;

#endif

    return NH_API_ERROR_BAD_STATE;
}

// SET =============================================================================================

NH_API_RESULT nh_Win32_setWindowBackgroundColor(
    nh_Win32_Window *Window_p, nh_css_Color Color)
{
#if defined(_WIN32) || defined (WIN32)

    return NH_API_SUCCESS;

#endif

    return NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_Win32_setWindowTitle(
    nh_Win32_Window *Window_p, char *title_p)
{
#if defined(_WIN32) || defined (WIN32)

    return NH_API_SUCCESS;

#endif

    return NH_API_ERROR_BAD_STATE;
}

NH_API_RESULT nh_Win32_setMouseCursor(
    nh_Win32_Window *Window_p, int type)
{
#if defined(_WIN32) || defined (WIN32)

    return NH_API_SUCCESS;

#endif

    return NH_API_ERROR_BAD_STATE;
}

// CALLBACK =======================================================================================

#if defined(_WIN32) || defined (WIN32)

    LRESULT CALLBACK WindowProc_(
        NHND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        nh_Window *Window_p = GetWindowLongPtr(hwnd, GWLP_USERDATA);
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

