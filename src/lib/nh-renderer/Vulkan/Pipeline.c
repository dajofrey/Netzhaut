// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Pipeline.h"
#include "../../nh-gfx/Common/Macros.h"

// DECLARE ========================================================================================

static NH_API_RESULT nh_renderer_vk_createColorPipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p, nh_gfx_VulkanPipelineInfo *Info_p
);  
static NH_API_RESULT nh_renderer_createVulkanTextSDFPipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p, nh_gfx_VulkanPipelineInfo *Info_p
);  
static NH_API_RESULT nh_renderer_vk_createImagePipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p, nh_gfx_VulkanPipelineInfo *Info_p
);  
static NH_API_RESULT nh_renderer_vk_createBackgroundImagePipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p, nh_gfx_VulkanPipelineInfo *Info_p
);  

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_renderer_createVulkanPipelines(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipelines_p)
{
    nh_gfx_VulkanPipelineInfo Info;
    nh_gfx_prepareVulkanPipelineInfo(&Info);

    NH_CORE_CHECK(nh_renderer_vk_createColorPipeline(Driver_p, &Pipelines_p[NH_RENDERER_VK_PIPELINE_COLOR], &Info))
    NH_CORE_CHECK(nh_renderer_createVulkanTextSDFPipeline(Driver_p, &Pipelines_p[NH_RENDERER_VK_PIPELINE_TEXT_SDF], &Info))
    NH_CORE_CHECK(nh_renderer_vk_createImagePipeline(Driver_p, &Pipelines_p[NH_RENDERER_VK_PIPELINE_IMAGE], &Info))
    NH_CORE_CHECK(nh_renderer_vk_createBackgroundImagePipeline(Driver_p, &Pipelines_p[NH_RENDERER_VK_PIPELINE_BACKGROUND_IMAGE], &Info))

    return NH_API_SUCCESS;
}

void nh_renderer_destroyVulkanPipelines(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipelines_p)
{
    for (int i = 0; i < NH_RENDERER_VK_PIPELINE_COUNT; ++i) {
        nh_gfx_destroyVulkanPipeline(Driver_p, &Pipelines_p[i]);
    }
}

static NH_API_RESULT nh_renderer_vk_createColorPipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p, nh_gfx_VulkanPipelineInfo *Info_p)
{
    // layout
    VkDescriptorSetLayoutBinding LayoutBinding = 
    {
        .binding            = 0,
        .descriptorType     = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .descriptorCount    = 1,
        .stageFlags         = VK_SHADER_STAGE_VERTEX_BIT,
        .pImmutableSamplers = VK_NULL_HANDLE,
    };
    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = 1,
        .pBindings    = &LayoutBinding,
    };
    
    VkDescriptorSetLayout Layout;
    NH_GFX_CHECK_VULKAN_2(NH_API_ERROR_BAD_STATE, Driver_p->Functions.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout))
    
    VkPipelineLayoutCreateInfo PipelineLayoutInfo = 
    {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount         = 1,
        .pSetLayouts            = &Layout,
        .pushConstantRangeCount = 0
    };

    // shaders
    const uint32_t vs_code[] = 
    {
        #include "../Common/Data/GLSL/Main/Color.vert.inc"
    };
    const uint32_t fs_code[] = 
    {
        #include "../Common/Data/GLSL/Main/Color.frag.inc"
    };
    VkShaderModule vertShaderModule;
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule))
    VkShaderModule fragShaderModule; 
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule))
    
    VkPipelineShaderStageCreateInfo vertShaderInfo = 
    {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertShaderModule,
        .pName  = "main"
    };
    VkPipelineShaderStageCreateInfo fragShaderInfo = 
    {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragShaderModule,
        .pName  = "main"
    };
    VkPipelineShaderStageCreateInfo shaderStages[2] = {vertShaderInfo, fragShaderInfo};

    // shader input
    VkVertexInputBindingDescription bindingDescription = 
    {
        .binding   = 0,
        .stride    = sizeof(float) * 3,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };
    VkVertexInputAttributeDescription vertexDescription = 
    {
        .binding  = 0,
        .location = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = 0
    };
    VkPipelineVertexInputStateCreateInfo VertexInput = 
    {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount   = 1,
        .vertexAttributeDescriptionCount = 1,
        .pVertexBindingDescriptions      = &bindingDescription,
        .pVertexAttributeDescriptions    = &vertexDescription
    };
    VkPipelineInputAssemblyStateCreateInfo InputAssembly = 
    {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    // create
    VkGraphicsPipelineCreateInfo PipelineInfo = 
    {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount          = 2,
        .pStages             = shaderStages,
        .pVertexInputState   = &VertexInput,
        .pInputAssemblyState = &InputAssembly,
        .pRasterizationState = &Info_p->RasterizationState,
        .pDepthStencilState  = &Info_p->DepthStencilState,
        .pMultisampleState   = &Info_p->MultisampleState,
        .pColorBlendState    = &Info_p->ColorBlendState,
        .pDynamicState       = &Info_p->DynamicState, 
        .pViewportState      = &Info_p->ViewportState,
        .renderPass          = Driver_p->RenderPass_p[0],
        .subpass             = 0
    };

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, Pipeline_p))

    Driver_p->Functions.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

    return NH_API_SUCCESS;
}

// SDF TEXT ========================================================================================

static NH_API_RESULT nh_renderer_createVulkanTextSDFPipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p, nh_gfx_VulkanPipelineInfo *Info_p)
{
    // layout
    VkDescriptorSetLayoutBinding setLayoutBindings[3];
    VkDescriptorSetLayoutBinding setLayoutBinding1 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .stageFlags      = VK_SHADER_STAGE_VERTEX_BIT,
        .binding         = 0,
        .descriptorCount = 1 
    };
    VkDescriptorSetLayoutBinding setLayoutBinding2 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
        .binding         = 1,
        .descriptorCount = 1 
    };
    VkDescriptorSetLayoutBinding setLayoutBinding3 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
        .binding         = 2,
        .descriptorCount = 1 
    };
    setLayoutBindings[0] = setLayoutBinding1;
    setLayoutBindings[1] = setLayoutBinding2;
    setLayoutBindings[2] = setLayoutBinding3;

    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pBindings    = setLayoutBindings,
        .bindingCount = 3
    };

    VkDescriptorSetLayout Layout;
    NH_GFX_CHECK_VULKAN_2(NH_API_ERROR_BAD_STATE, Driver_p->Functions.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout))

    VkPipelineLayoutCreateInfo PipelineLayoutInfo = 
    {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount         = 1,
        .pSetLayouts            = &Layout,
        .pushConstantRangeCount = 0
    };

    // shader
    VkShaderModule fragShaderModule, vertShaderModule;
    const uint32_t vs_code[] = 
    {
        #include "../Common/Data/GLSL/Main/TextSDF.vert.inc"
    };
    const uint32_t fs_code[] = 
    {
        #include "../Common/Data/GLSL/Main/TextSDF.frag.inc"
    };
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule))
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule))

    VkPipelineShaderStageCreateInfo vertShaderInfo = 
    {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertShaderModule,
        .pName  = "main"
    };
    VkPipelineShaderStageCreateInfo fragShaderInfo = 
    {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragShaderModule,
        .pName  = "main"
    };
    VkPipelineShaderStageCreateInfo shaderStages[2] = {vertShaderInfo, fragShaderInfo};
    
    // shader input
    VkVertexInputBindingDescription vInputBindDescription = 
    {
        .binding   = 0,
        .stride    = sizeof(float) * 5,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
    };

    VkVertexInputAttributeDescription vInputAttribDescriptions[2];
    VkVertexInputAttributeDescription vertexDescription =
    {
        .location = 0,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = 0
    };
		
    VkVertexInputAttributeDescription textureDescription =
    {
        .location = 1,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32_SFLOAT,
        .offset   = sizeof(float) * 3
    };
        
    vInputAttribDescriptions[0] = vertexDescription;
    vInputAttribDescriptions[1] = textureDescription;

    VkPipelineVertexInputStateCreateInfo VertexInput = 
    {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount   = 1,
        .vertexAttributeDescriptionCount = 2,
        .pVertexBindingDescriptions      = &vInputBindDescription,
        .pVertexAttributeDescriptions    = vInputAttribDescriptions
    };
    VkPipelineInputAssemblyStateCreateInfo InputAssembly = 
    {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    // create
    VkGraphicsPipelineCreateInfo PipelineInfo = 
    {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount          = 2,
        .pStages             = shaderStages,
        .pVertexInputState   = &VertexInput,
        .pInputAssemblyState = &InputAssembly,
        .pRasterizationState = &Info_p->RasterizationState,
        .pDepthStencilState  = &Info_p->DepthStencilState,
        .pMultisampleState   = &Info_p->MultisampleState,
        .pColorBlendState    = &Info_p->ColorBlendState,
        .pDynamicState       = &Info_p->DynamicState,
        .pViewportState      = &Info_p->ViewportState,
        .renderPass          = Driver_p->RenderPass_p[0],
        .subpass             = 0
    };

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, Pipeline_p))

    Driver_p->Functions.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_renderer_vk_createImagePipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p, nh_gfx_VulkanPipelineInfo *Info_p)
{
    // layout
    VkDescriptorSetLayoutBinding setLayoutBindings[2];
    VkDescriptorSetLayoutBinding setLayoutBinding1 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .stageFlags      = VK_SHADER_STAGE_VERTEX_BIT,
        .binding         = 0,
        .descriptorCount = 1 
    };
    VkDescriptorSetLayoutBinding setLayoutBinding2 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
        .binding         = 1,
        .descriptorCount = 1 
    };
    setLayoutBindings[0] = setLayoutBinding1;
    setLayoutBindings[1] = setLayoutBinding2;

    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pBindings    = setLayoutBindings,
        .bindingCount = 2
    };

    VkDescriptorSetLayout Layout;
    NH_GFX_CHECK_VULKAN_2(NH_API_ERROR_BAD_STATE, Driver_p->Functions.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout))

    VkPipelineLayoutCreateInfo PipelineLayoutInfo = 
    {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount         = 1,
        .pSetLayouts            = &Layout,
        .pushConstantRangeCount = 0
    };

    // shaders
    const uint32_t vs_code[] = 
    {
        #include "../Common/Data/GLSL/Main/Image.vert.inc"
    };
    const uint32_t fs_code[] = 
    {
        #include "../Common/Data/GLSL/Main/Image.frag.inc"
    };
    VkShaderModule vertShaderModule;
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule))
    VkShaderModule fragShaderModule; 
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule))
    
    VkPipelineShaderStageCreateInfo vertShaderInfo = 
    {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertShaderModule,
        .pName  = "main"
    };
    VkPipelineShaderStageCreateInfo fragShaderInfo = 
    {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragShaderModule,
        .pName  = "main"
    };
    VkPipelineShaderStageCreateInfo shaderStages[2] = {vertShaderInfo, fragShaderInfo};

    // shader input
    VkVertexInputBindingDescription bindingDescription = 
    {
        .binding   = 0,
        .stride    = sizeof(float) * 5,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };
    VkVertexInputAttributeDescription vInputAttribDescriptions[2];
    VkVertexInputAttributeDescription vertexDescription = 
    {
        .binding  = 0,
        .location = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = 0
    };
    VkVertexInputAttributeDescription textureDescription =
    {
        .location = 1,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32_SFLOAT,
        .offset   = sizeof(float) * 3
    };
    vInputAttribDescriptions[0] = vertexDescription;
    vInputAttribDescriptions[1] = textureDescription;

    VkPipelineVertexInputStateCreateInfo VertexInput = 
    {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount   = 1,
        .vertexAttributeDescriptionCount = 2,
        .pVertexBindingDescriptions      = &bindingDescription,
        .pVertexAttributeDescriptions    = vInputAttribDescriptions
    };
    VkPipelineInputAssemblyStateCreateInfo InputAssembly = 
    {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    // create
    VkGraphicsPipelineCreateInfo PipelineInfo = 
    {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount          = 2,
        .pStages             = shaderStages,
        .pVertexInputState   = &VertexInput,
        .pInputAssemblyState = &InputAssembly,
        .pRasterizationState = &Info_p->RasterizationState,
        .pDepthStencilState  = &Info_p->DepthStencilState,
        .pMultisampleState   = &Info_p->MultisampleState,
        .pColorBlendState    = &Info_p->ColorBlendState,
        .pDynamicState       = &Info_p->DynamicState, 
        .pViewportState      = &Info_p->ViewportState,
        .renderPass          = Driver_p->RenderPass_p[0],
        .subpass             = 0
    };

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, Pipeline_p))

    Driver_p->Functions.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_renderer_vk_createBackgroundImagePipeline(
    nh_gfx_VulkanDriver *Driver_p, nh_gfx_VulkanPipeline *Pipeline_p, nh_gfx_VulkanPipelineInfo *Info_p)
{
    // layout
    VkDescriptorSetLayoutBinding setLayoutBindings[2];
    VkDescriptorSetLayoutBinding setLayoutBinding1 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        .stageFlags      = VK_SHADER_STAGE_VERTEX_BIT,
        .binding         = 0,
        .descriptorCount = 1 
    };
    VkDescriptorSetLayoutBinding setLayoutBinding2 =
    {
        .descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        .stageFlags      = VK_SHADER_STAGE_FRAGMENT_BIT,
        .binding         = 1,
        .descriptorCount = 1 
    };
    setLayoutBindings[0] = setLayoutBinding1;
    setLayoutBindings[1] = setLayoutBinding2;

    VkDescriptorSetLayoutCreateInfo LayoutInfo = 
    {
        .sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .pBindings    = setLayoutBindings,
        .bindingCount = 2
    };

    VkDescriptorSetLayout Layout;
    NH_GFX_CHECK_VULKAN_2(NH_API_ERROR_BAD_STATE, Driver_p->Functions.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout))

    VkPipelineLayoutCreateInfo PipelineLayoutInfo = 
    {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount         = 1,
        .pSetLayouts            = &Layout,
        .pushConstantRangeCount = 0
    };

    // shaders
    const uint32_t vs_code[] = 
    {
        #include "../Common/Data/GLSL/Main/BackgroundImage.vert.inc"
    };
    const uint32_t fs_code[] = 
    {
        #include "../Common/Data/GLSL/Main/BackgroundImage.frag.inc"
    };
    VkShaderModule vertShaderModule;
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule))
    VkShaderModule fragShaderModule; 
    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule))
    
    VkPipelineShaderStageCreateInfo vertShaderInfo = 
    {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = VK_SHADER_STAGE_VERTEX_BIT,
        .module = vertShaderModule,
        .pName  = "main"
    };
    VkPipelineShaderStageCreateInfo fragShaderInfo = 
    {
        .sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage  = VK_SHADER_STAGE_FRAGMENT_BIT,
        .module = fragShaderModule,
        .pName  = "main"
    };
    VkPipelineShaderStageCreateInfo shaderStages[2] = {vertShaderInfo, fragShaderInfo};

    // shader input
    VkVertexInputBindingDescription bindingDescription = 
    {
        .binding   = 0,
        .stride    = sizeof(float) * 5,
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
    };
    VkVertexInputAttributeDescription vInputAttribDescriptions[2];
    VkVertexInputAttributeDescription vertexDescription = 
    {
        .binding  = 0,
        .location = 0,
        .format   = VK_FORMAT_R32G32B32_SFLOAT,
        .offset   = 0
    };
    VkVertexInputAttributeDescription textureDescription =
    {
        .location = 1,
        .binding  = 0,
        .format   = VK_FORMAT_R32G32_SFLOAT,
        .offset   = sizeof(float) * 3
    };
    vInputAttribDescriptions[0] = vertexDescription;
    vInputAttribDescriptions[1] = textureDescription;

    VkPipelineVertexInputStateCreateInfo VertexInput = 
    {
        .sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount   = 1,
        .vertexAttributeDescriptionCount = 2,
        .pVertexBindingDescriptions      = &bindingDescription,
        .pVertexAttributeDescriptions    = vInputAttribDescriptions
    };
    VkPipelineInputAssemblyStateCreateInfo InputAssembly = 
    {
        .sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE
    };

    // create
    VkGraphicsPipelineCreateInfo PipelineInfo = 
    {
        .sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount          = 2,
        .pStages             = shaderStages,
        .pVertexInputState   = &VertexInput,
        .pInputAssemblyState = &InputAssembly,
        .pRasterizationState = &Info_p->RasterizationState,
        .pDepthStencilState  = &Info_p->DepthStencilState,
        .pMultisampleState   = &Info_p->MultisampleState,
        .pColorBlendState    = &Info_p->ColorBlendState,
        .pDynamicState       = &Info_p->DynamicState, 
        .pViewportState      = &Info_p->ViewportState,
        .renderPass          = Driver_p->RenderPass_p[0],
        .subpass             = 0
    };

    NH_CORE_CHECK_2(NH_API_ERROR_BAD_STATE, nh_gfx_createVulkanPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, Pipeline_p))

    Driver_p->Functions.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Functions.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

    return NH_API_SUCCESS;
}

