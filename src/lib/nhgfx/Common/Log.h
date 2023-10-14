#ifndef NH_GFX_COMMON_LOG_H
#define NH_GFX_COMMON_LOG_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "Result.h"

#include "../Fonts/FontManager.h"

#endif

/** @addtogroup lib_nhgfx_functions
 *  @{
 */

    NH_GFX_RESULT _nh_gfx_logBegin(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_GFX_RESULT _nh_gfx_logEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p
    );
    
    NH_GFX_RESULT _nh_gfx_logDiagnosticEnd(
        const NH_BYTE *file_p, const NH_BYTE *function_p, NH_GFX_RESULT result, int line
    );

    void nh_gfx_logFont(
        nh_gfx_Font *Font_p 
    );

    void nh_gfx_logVulkanValidation(
        NH_BYTE *message_p
    );

    void nh_gfx_logOpenGLCommand(
        NH_BYTE *format_p, ...
    );

/** @} */

#endif // NH_GFX_COMMON_LOG_H 
