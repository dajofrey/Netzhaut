// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Data.h"
#include "Descriptor.h"
#include "Uniform.h"
#include "Buffer.h"
#include "Text.h"

#include "../Main/Vertices.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-gfx/Base/Viewport.h"

#include <string.h>
#include <stdlib.h>

// FUNCTIONS =======================================================================================

static nh_renderer_VulkanData *nh_renderer_vk_allocateData()
{
    nh_renderer_VulkanData *Data_p = nh_core_allocate(sizeof(nh_renderer_VulkanData));
    NH_CORE_CHECK_MEM_2(NULL, Data_p)

    Data_p->Buffers     = nh_core_initArray(sizeof(nh_gfx_VulkanBuffer), NH_RENDERER_VULKAN_BUFFER_COUNT);
    Data_p->Uniforms    = nh_core_initArray(sizeof(nh_gfx_VulkanBuffer), NH_RENDERER_VK_UNIFORM_COUNT);
    Data_p->Descriptors = nh_core_initArray(sizeof(nh_gfx_VulkanDescriptorSet), 2);

    return Data_p;
}

static NH_API_RESULT nh_renderer_createVulkanTextFragmentData(
    nh_css_Fragment *Fragment_p, nh_gfx_Viewport *Viewport_p)
{
    NH_CORE_CHECK_NULL(Viewport_p)
    NH_CORE_CHECK_NULL(Fragment_p)

    nh_gfx_VulkanDriver *Driver_p = &Viewport_p->Surface_p->Vulkan.GPU_p->Driver;

    int x = Fragment_p->Block.Position.x;
    int y = Fragment_p->Block.Position.y;
    float z = Fragment_p->Block.depth;

    for (int i = 0; i < Fragment_p->Text.Values.Text.Segments.length; ++i)
    {
        nh_gfx_TextSegment *Segment_p = &((nh_gfx_TextSegment*)Fragment_p->Text.Values.Text.Segments.p)[i];
        if (Segment_p->length <= 0) {continue;}

        float *vertices_p = NULL;
        uint32_t *indices_p = NULL;

        NH_CORE_CHECK(nh_renderer_getTextVertices(Viewport_p, Segment_p, &x, y, &z, &vertices_p, &indices_p))
        NH_CORE_CHECK(nh_renderer_createVulkanText(
            Viewport_p->Surface_p->Vulkan.GPU_p, Fragment_p, Segment_p, true, vertices_p, indices_p, i
        ))
    
        nh_core_free(vertices_p);
        nh_core_free(indices_p);
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_renderer_vk_createBoxFragmentData(
    nh_css_Fragment *Fragment_p, nh_gfx_Viewport *Viewport_p)
{
    NH_CORE_CHECK_NULL(Viewport_p)
    NH_CORE_CHECK_NULL(Fragment_p)

    nh_gfx_VulkanDriver *Driver_p = &Viewport_p->Surface_p->Vulkan.GPU_p->Driver;

    if (Fragment_p->Box.Values.BackgroundColor.a > 0.0f) {
        NH_CORE_CHECK(nh_renderer_createVulkanBackgroundVertices(Viewport_p, Driver_p, Fragment_p))
        NH_CORE_CHECK(nh_renderer_createVulkanColorUniform(Driver_p, Fragment_p, NH_RENDERER_VK_UNIFORM_BACKGROUND))
        NH_CORE_CHECK(nh_renderer_createVulkanColorDescriptor(Driver_p, Fragment_p, NH_RENDERER_VK_DESCRIPTOR_BACKGROUND))
    }

    if (Fragment_p->Box.Values.borderTopStyle == NH_CSS_LINE_STYLE_SOLID)
    {
        NH_CORE_CHECK(nh_renderer_createVulkanBorderVertices(Viewport_p, Driver_p, Fragment_p, NH_RENDERER_VULKAN_BUFFER_TOP_BORDER_VERTICES))
        NH_CORE_CHECK(nh_renderer_createVulkanColorUniform(Driver_p, Fragment_p, NH_RENDERER_VK_UNIFORM_TOP_BORDER))
        NH_CORE_CHECK(nh_renderer_createVulkanColorDescriptor(Driver_p, Fragment_p, NH_RENDERER_VK_DESCRIPTOR_TOP_BORDER))
    }

    if (Fragment_p->Box.Values.borderRightStyle == NH_CSS_LINE_STYLE_SOLID)
    {
        NH_CORE_CHECK(nh_renderer_createVulkanBorderVertices(Viewport_p, Driver_p, Fragment_p, NH_RENDERER_VULKAN_BUFFER_RIGHT_BORDER_VERTICES))
        NH_CORE_CHECK(nh_renderer_createVulkanColorUniform(Driver_p, Fragment_p, NH_RENDERER_VK_UNIFORM_RIGHT_BORDER))
        NH_CORE_CHECK(nh_renderer_createVulkanColorDescriptor(Driver_p, Fragment_p, NH_RENDERER_VK_DESCRIPTOR_RIGHT_BORDER))
    }

    if (Fragment_p->Box.Values.borderBottomStyle == NH_CSS_LINE_STYLE_SOLID)
    {
        NH_CORE_CHECK(nh_renderer_createVulkanBorderVertices(Viewport_p, Driver_p, Fragment_p, NH_RENDERER_VULKAN_BUFFER_BOTTOM_BORDER_VERTICES))
        NH_CORE_CHECK(nh_renderer_createVulkanColorUniform(Driver_p, Fragment_p, NH_RENDERER_VK_UNIFORM_BOTTOM_BORDER))
        NH_CORE_CHECK(nh_renderer_createVulkanColorDescriptor(Driver_p, Fragment_p, NH_RENDERER_VK_DESCRIPTOR_BOTTOM_BORDER))
    }

    if (Fragment_p->Box.Values.borderLeftStyle == NH_CSS_LINE_STYLE_SOLID)
    {
        NH_CORE_CHECK(nh_renderer_createVulkanBorderVertices(Viewport_p, Driver_p, Fragment_p, NH_RENDERER_VULKAN_BUFFER_LEFT_BORDER_VERTICES))
        NH_CORE_CHECK(nh_renderer_createVulkanColorUniform(Driver_p, Fragment_p, NH_RENDERER_VK_UNIFORM_LEFT_BORDER))
        NH_CORE_CHECK(nh_renderer_createVulkanColorDescriptor(Driver_p, Fragment_p, NH_RENDERER_VK_DESCRIPTOR_LEFT_BORDER))
    }

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_renderer_vk_createFragmentData(
    nh_css_Fragment *Fragment_p, nh_gfx_Viewport *Viewport_p)
{
    Fragment_p->data_p = nh_renderer_vk_allocateData();
    NH_CORE_CHECK_MEM(Fragment_p->data_p)

    if (Fragment_p->type == NH_CSS_FRAGMENT_BOX) {
        NH_CORE_CHECK(nh_renderer_vk_createBoxFragmentData(Fragment_p, Viewport_p))
    }
    else {
        NH_CORE_CHECK(nh_renderer_createVulkanTextFragmentData(Fragment_p, Viewport_p))
    }

    for (int i = 0; i < Fragment_p->Children.size; ++i) {
        NH_CORE_CHECK(nh_renderer_vk_createFragmentData(Fragment_p->Children.pp[i], Viewport_p))
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_renderer_createFragmentTreeVulkanData(
    nh_css_FragmentTree *Tree_p, nh_gfx_Viewport *Viewport_p)
{
    return nh_renderer_vk_createFragmentData(Tree_p->Root_p, Viewport_p);
}

//// UPDATE ==========================================================================================
//
//NH_API_RESULT nh_vk_updateNodeProperty(
//    nh_gfx_VulkanGPU *GPU_p, nh_html_Node *Node_p, nh_renderer_GenericProperty *Property_p)
//{
//    if (GPU_p == NULL || Node_p == NULL) {NH_DIAGNOSTIC_END(NH_SUCCESS)}
//
//    switch (Property_p->type)
//    {
//        case NH_RENDERER_PROPERTY_BACKGROUND_COLOR : 
// 
//            NH_CHECK(nh_gfx_mapVulkanDeviceMemory(
//                &GPU_p->Driver, 
//                &nh_vk_getUniform(Node_p, NH_VK_UNIFORM_BACKGROUND)->DeviceMemory, 
//                Node_p->Properties.Background.color_p, 
//                sizeof(float) * 4,
//                sizeof(float) * 16 
//            ))
//            break;
// 
//        case NH_RENDERER_PROPERTY_COLOR : 
// 
//            NH_CHECK(nh_gfx_mapVulkanDeviceMemory(
//                &GPU_p->Driver, 
//                &nh_vk_getUniform(Node_p, NH_VK_UNIFORM_TEXT_SDF_FS)->DeviceMemory, 
//                Node_p->Properties.Text.color_p, 
//                sizeof(float) * 4,
//                0
//            ))
//            break;
//    }
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}

// DESTROY =========================================================================================

//void nh_renderer_vk_destroyRenderResources(
//    nh_gfx_Viewport *Viewport_p, nh_renderer_FormattingNodeFragment *Fragment_p)
//{
//    nh_gfx_VulkanDriver *Driver_p = &Viewport_p->Surface_p->Vulkan.GPU_p->Driver;
//
//    nh_renderer_destroyVulkanBufferArray(Driver_p, Fragment_p);
//    nh_renderer_destroyVulkanUniformArray(Driver_p, Fragment_p);
//    nh_renderer_destroyVulkanDescriptorArray(Driver_p, Fragment_p);        
//
//NH_RENDERER_SILENT_END()
//}

