#ifndef NH_VK_GPU_H
#define NH_VK_GPU_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Helper.h"
#include "Driver.h"
#include "../../nh-core/Util/List.h"

#endif

/** @addtogroup lib_nh-gfx_structs
 *  @{
 */

    typedef struct nh_vk_GPU {
        char *name_p;                       
        nh_vk_Driver Driver;
        nh_List Textures;
        struct {
            nh_vk_Pipeline *Pipelines_p;
        } Renderer;
        struct {
            nh_vk_Pipeline *Pipelines_p;
        } Term;
    } nh_vk_GPU;

/** @} */

/** @addtogroup lib_nh-gfx_functions
 *  @{
 */

    NH_API_RESULT nh_vk_initGPUs(
        nh_List *GPUs_p, nh_vk_Host *Host_p
    );

    void nh_vk_freeGPUs(
        nh_List *GPUs_p
    );

/** @} */

#endif
