#ifndef NH_GFX_SURFACE_REQUIREMENTS_H
#define NH_GFX_SURFACE_REQUIREMENTS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS
    
/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"
#include "../OpenGL/SurfaceRequirements.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_gfx_SurfaceRequirements {
        nh_opengl_SurfaceRequirements OpenGL;
    } nh_gfx_SurfaceRequirements;

/** @} */

/** @addtogroup lib_nh-gfx_typedefs
 *  @{
 */

    typedef nh_gfx_SurfaceRequirements *(*nh_gfx_getSurfaceRequirements_f)(
    );

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_gfx_createSurfaceRequirements(
    );

    NH_API_RESULT nh_gfx_freeSurfaceRequirements(
    );

/** @} */

#endif 
