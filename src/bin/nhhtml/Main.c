/**
 * Netzhaut - Web Browser 
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

#include "nhapi/nhapi.h"
#include "ttyr-api/ttyr-api.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

//static void *getFileData(
//    char *path_p, long *size_p) 
//{
//    FILE *fh = fopen(path_p, "rb");
//    if (fh == NULL) {return NULL;}
//    
//    if (fseek(fh, 0L, SEEK_END) != 0) {return NULL;}
//
//    long size = ftell(fh);
//
//    if (fseek(fh, 0L, SEEK_SET) != 0) {return NULL;}
//
//    if(size <= 0) {
//        fclose(fh);
//        return NULL;
//    }
//
//    char *data_p = (char*)malloc(((size_t)size) + 1); 
//    if (!data_p) {return NULL;}
//    
//    fread(data_p, 1, size, fh);
//    fclose(fh);
//
//    data_p[size] = '\0';
//    if (size_p) {*size_p = size;}
//
//    return data_p;
//}

//static void handleInput(
//    nh_wsi_Window *Window_p, nh_wsi_Event Event)
//{
//    switch (Event.type)
//    {
//        case NH_WSI_EVENT_KEYBOARD :
//            ttyr_api_sendEvent(TTY_p, Event);
//            break;
//    }
//}

static int openMonitor()
{
    static ttyr_tty_TTY *TTY_p = NULL;
    static void *Interface_p = NULL;

    Interface_p = nh_api_createMonitorInterface();
    if (!Interface_p) {return 1;}

    ttyr_api_initialize();

    TTY_p = ttyr_api_openTTY(NULL, Interface_p);
    if (!TTY_p) {return 1;}

    if (ttyr_api_claimStandardIO(TTY_p)) {return 1;}

    return 0;
}

int main(
    int argc, char **argv_pp) 
{
    if (nh_api_initialize(NULL, NULL, 0) != NH_CORE_SUCCESS) {return 1;}

    if (openMonitor()) {return 1;}

//    long size;
//    void *document_p = getFileData(argv_pp[1], &size);
//
//    if (!document_p || !size) {return 1;}

    nh_wsi_Window *Window_p = nh_api_createWindow(NULL, nh_api_getSurfaceRequirements());
    if (!Window_p) {return 1;}

//    nh_api_setWindowEventListener(Window_p, handleInput);

    nh_gfx_Surface *Surface_p = nh_api_createSurface(Window_p, NH_GFX_API_VULKAN);
    if (!Surface_p) {return 1;}

    nh_PixelSize Size;
    Size.width = 700;
    Size.height = 700;

    nh_PixelPosition Position;
    Position.x = 0;
    Position.y = 0;

    nh_gfx_Viewport *Viewport_p = nh_api_createViewport(Surface_p, Position, Size);
    if (!Viewport_p) {return 1;}

    nh_html_DocumentContext *DocumentContext_p = nh_api_createDocumentContext(NH_FALSE);
    if (!DocumentContext_p) {return 1;}

    nh_css_LayoutEngine *LayoutEngine_p = nh_api_createLayoutEngine(DocumentContext_p);
    if (!LayoutEngine_p) {return 1;}

    nh_renderer_Renderer *Renderer_p = nh_api_createRenderer(LayoutEngine_p);
    if (!Renderer_p) {return 1;}

//    if (nh_renderer_addViewport(Renderer_p, Viewport_p)) {return 1;}
//    if (nh_html_loadBytes(DocumentContext_p, document_p, size)) {return 1;}

    while (1) {
        if (!nh_api_run()) {usleep(10000);}
        if (!nh_api_keepRunning()) {break;}
    }

    nh_api_terminate();
}
