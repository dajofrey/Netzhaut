#ifndef NH_VK_DESCRIPTORS_H
#define NH_VK_DESCRIPTORS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../HTML/Header/Document.h"
#include "../../../JavaScript/DOM/Header/CanvasRenderingContext2D.h"
#include "../../../API/Header/Netzhaut.h"
#include "../../../Core/Header/System.h"
#include "../../../CSS/Header/Image.h"

#endif

/** @addtogroup VulkanEnums Enums
 *  \ingroup Vulkan
 *  @{
 */

    typedef enum NH_VK_DESCRIPTOR {
        NH_VK_DESCRIPTOR_BACKGROUND,       
        NH_VK_DESCRIPTOR_TOP_BORDER,       
        NH_VK_DESCRIPTOR_RIGHT_BORDER,     
        NH_VK_DESCRIPTOR_BOTTOM_BORDER,    
        NH_VK_DESCRIPTOR_LEFT_BORDER,      
        NH_VK_DESCRIPTOR_TEXT_SDF,         
        NH_VK_DESCRIPTOR_IMAGE,         
        NH_VK_DESCRIPTOR_BACKGROUND_IMAGE,
    } NH_VK_DESCRIPTOR;

/** @} */

/** @addtogroup VulkanFunctions Functions
 *  \ingroup Vulkan
 *  @{
 */

    NH_RESULT Nh_Vk_createColorDescriptor(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_DESCRIPTOR type
    );
    NH_RESULT Nh_Vk_createImageDescriptor(
        Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p
    );
    NH_RESULT Nh_Vk_createBackgroundImageDescriptor(
        Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p
    );

    NH_RESULT Nh_Vk_createCanvasRenderingContext2DDescriptor(
        Nh_Tab *Tab_p, Nh_Vk_Driver *Driver_p, NH_JS_CanvasRenderingContext2D *Context_p,
        NH_Vk_Buffer *Uniform_p, size_t size
    );
   
    void Nh_Vk_destroyDescriptorList(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p
    );
    void Nh_Vk_destroyDescriptor(
        Nh_Vk_Driver *Driver_p, Nh_HTML_Node *Node_p, NH_VK_DESCRIPTOR type
    );
    
    NH_Vk_DescriptorSet *Nh_Vk_getDescriptor(
        Nh_HTML_Node *Node_p, NH_VK_DESCRIPTOR type
    );
    NH_Vk_DescriptorSet *Nh_Vk_getDescriptorFromIndex(
        Nh_HTML_Node *Node_p, int index
    );

/** @} */

#endif
