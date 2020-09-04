#ifndef NH_VK_UNIFORMS_H
#define NH_VK_UNIFORMS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../Core/Header/Tab.h"
#include "../../../API/Header/Netzhaut.h"
#include "../../../CSS/Header/Image.h"
#include "../../../JavaScript/DOM/Header/CanvasRenderingContext2D.h"
#include "../../../JavaScript/DOM/Header/Path2D.h"

#endif

/** @addtogroup VulkanEnums Enums
 *  \ingroup Vulkan
 *  @{
 */

    typedef enum NH_VK_UNIFORM {
        NH_VK_UNIFORM_BACKGROUND,       
        NH_VK_UNIFORM_TOP_BORDER,       
        NH_VK_UNIFORM_RIGHT_BORDER,     
        NH_VK_UNIFORM_BOTTOM_BORDER,    
        NH_VK_UNIFORM_LEFT_BORDER,      
        NH_VK_UNIFORM_TEXT_SDF_VS,      
        NH_VK_UNIFORM_TEXT_SDF_FS,      
        NH_VK_UNIFORM_IMAGE,       
        NH_VK_UNIFORM_BACKGROUND_IMAGE,
        NH_VK_UNIFORM_COUNT             
    } NH_VK_UNIFORM;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createColorUniform(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_UNIFORM type
    );
    NH_RESULT Nh_Vk_createImageUniform(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p
    );
    NH_RESULT Nh_Vk_createBackgroundImageUniform(
        Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p
    );

    NH_RESULT Nh_Vk_createCanvasRenderingContext2DArcUniform(
        Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_JS_CanvasRenderingContext2D *Context_p, 
        NH_JS_Path2DArc *Arc_p
    );
   
    void Nh_Vk_destroyUniformList(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p
    );
    void Nh_Vk_destroyUniform(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_UNIFORM type
    );
    
    NH_Vk_Buffer *Nh_Vk_getUniform(
        Nh_HTML_Node *Node_p, NH_VK_UNIFORM type
    );
    NH_Vk_Buffer *Nh_Vk_getUniformFromIndex(
        Nh_HTML_Node *Node_p, int index
    );

/** @} */

#endif
