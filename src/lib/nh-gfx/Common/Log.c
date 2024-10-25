// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Log.h"

#include "../../nh-core/System/Logger.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// LOG =============================================================================================

NH_API_RESULT _nh_gfx_logBegin(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_begin(file_p, function_p);
}

NH_API_RESULT _nh_gfx_logEnd(
    const char *file_p, const char *function_p)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return NH_SUCCESS;}
//    return _nh_end(file_p, function_p);
}

NH_API_RESULT _nh_gfx_logDiagnosticEnd(
    const char *file_p, const char *function_p, NH_API_RESULT result, int line)
{
//    if (!NH_CONFIG.Flags.Log.Flow.html) {return result;}
//    _nh_diagnosticEnd(file_p, function_p, result, line);
//    return result;
}

void nh_gfx_logFont(
    nh_gfx_Font *Font_p)
{
    char *node_p = Font_p->internal ? "nh-gfx:Fonts:Internal" : "nh-gfx:Fonts:System";

    char message_p[1024];
    sprintf(message_p, "%s | %s | %s", Font_p->Family.name_p, Font_p->Style.name_p, Font_p->id_p);
    nh_core_sendLogMessage(node_p, NULL, message_p);

    memset(message_p, 0, 1024);
    sprintf(message_p, "  generic :");

    int genericCount = 0;
    for (int i = 0; i < NH_GFX_GENERIC_FONT_FAMILY_COUNT; ++i) {
        if (Font_p->Family.generic_p[i]) {
            sprintf(message_p + strlen(message_p), " %s", NH_GFX_GENERIC_FONT_FAMILY_NAMES_PP[i]);
            genericCount++;
        }
    }

    if (genericCount == 0) {sprintf(message_p + strlen(message_p), " none");}
    nh_core_sendLogMessage(node_p, NULL, message_p);

    memset(message_p, 0, 1024);
    sprintf(message_p, "  weight  : %d", Font_p->Style.weight);
    nh_core_sendLogMessage(node_p, NULL, message_p);

    memset(message_p, 0, 1024);
    sprintf(message_p, "  italic  : %s", Font_p->Style.italic ? "true" : "false");
    nh_core_sendLogMessage(node_p, NULL, message_p);

    memset(message_p, 0, 1024);
    sprintf(message_p, "  oblique : %s", Font_p->Style.oblique ? "true" : "false");
    nh_core_sendLogMessage(node_p, NULL, message_p);

    return;
}

void nh_gfx_logVulkanValidation(
    char *message_p)
{
    nh_core_sendLogMessage("nh-gfx:VulkanValidation", NULL, message_p);
    return;
}
           
void nh_gfx_logOpenGLCommand(
    char *format_p, ...)
{
//    va_list args;
//    va_start(args, format_p);
//    vprintf(format_p, args);
//    va_end(args);

    return;
}
 
