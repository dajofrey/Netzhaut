// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Pipelines.h"
#include "../Header/Driver.h"
#include "../Header/Surface.h"
#include "../Header/GPU.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"

#include "../../../API/Header/Vulkan.h"
#include "../../../Core/Header/System.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

// DECLARE ========================================================================================

typedef struct PipelineInfo {
    VkViewport Viewport;
    VkRect2D Scissor;
    VkDynamicState dynamic_p[2];
    VkPipelineViewportStateCreateInfo ViewportState;
    VkPipelineDynamicStateCreateInfo DynamicState;
    VkPipelineDepthStencilStateCreateInfo DepthStencilState;
    VkPipelineRasterizationStateCreateInfo RasterizationState;
    VkPipelineMultisampleStateCreateInfo MultisampleState;
    VkPipelineColorBlendStateCreateInfo ColorBlendState;
    VkPipelineColorBlendAttachmentState ColorBlendAttachment;
} PipelineInfo;

static void Nh_Vk_preparePipelineInfo(
    PipelineInfo *Info_p
);

static NH_RESULT Nh_Vk_createColorPipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p
);  
static NH_RESULT Nh_Vk_createTextSDFPipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p
);  
static NH_RESULT Nh_Vk_createImagePipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p
);  
static NH_RESULT Nh_Vk_createBackgroundImagePipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p
);  
static NH_RESULT Nh_Vk_createCanvasRenderingContext2DArcPipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p
);  

// IMPLEMENT ======================================================================================

NH_RESULT Nh_Vk_createPipelines(
    Nh_Vk_GPU *GPU_p)
{
NH_BEGIN()

    PipelineInfo Info;
    Nh_Vk_preparePipelineInfo(&Info);

    NH_CHECK(Nh_Vk_createColorPipeline(GPU_p, &Info))
    NH_CHECK(Nh_Vk_createTextSDFPipeline(GPU_p, &Info))
    NH_CHECK(Nh_Vk_createImagePipeline(GPU_p, &Info))
    NH_CHECK(Nh_Vk_createBackgroundImagePipeline(GPU_p, &Info))
    NH_CHECK(Nh_Vk_createCanvasRenderingContext2DArcPipeline(GPU_p, &Info))

NH_END(NH_SUCCESS)
}

void Nh_Vk_destroyPipelines(
    Nh_Vk_GPU *GPU_p)
{
NH_BEGIN()

    for (int i = 0; i < NH_VK_PIPELINE_COUNT; ++i) {
        Nh_Vk_destroyPipeline(&GPU_p->Driver, &GPU_p->Pipelines_p[i]);
    }

NH_SILENT_END()
}

// PREPARE =========================================================================================

static void Nh_Vk_preparePipelineInfo(
    PipelineInfo *Info_p)
{
NH_BEGIN()

// viewport
    Info_p->dynamic_p[0] = VK_DYNAMIC_STATE_VIEWPORT;
    Info_p->dynamic_p[1] = VK_DYNAMIC_STATE_SCISSOR;

    VkPipelineDynamicStateCreateInfo DynamicState = 
    {
        .sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates    = Info_p->dynamic_p,
    };
    Info_p->DynamicState = DynamicState; 

    Info_p->Viewport.x        = 0.0f;
    Info_p->Viewport.y        = 0.0f;
    Info_p->Viewport.width    = 0.0f;
    Info_p->Viewport.height   = 0.0f;    
    Info_p->Viewport.minDepth = 0.0f;
    Info_p->Viewport.maxDepth = 1.0f;
    
    VkExtent2D Extent2D = 
    { 
        .width = 0.0f, 
        .height = 0.0f
    };
    VkOffset2D Offset2D = 
    { 
        .x = 0, 
        .y = 0
    };
    Info_p->Scissor.offset = Offset2D;
    Info_p->Scissor.extent = Extent2D;

    VkPipelineViewportStateCreateInfo ViewportState = 
    {
        .sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports    = &Info_p->Viewport,
        .scissorCount  = 1,
        .pScissors     = &Info_p->Scissor,
    };
    Info_p->ViewportState = ViewportState;

// other
    VkStencilOpState OpState = {};

    VkPipelineDepthStencilStateCreateInfo DepthStencilState = 
    {
        .sType                 = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
        .depthTestEnable       = VK_TRUE,
        .depthWriteEnable      = VK_TRUE,
        .depthCompareOp        = VK_COMPARE_OP_LESS,
        .depthBoundsTestEnable = VK_FALSE,
        .minDepthBounds        = 0.0f,
        .maxDepthBounds        = 1.0f,
        .stencilTestEnable     = VK_FALSE,
        .front                 = OpState,
        .back                  = OpState,
    };
    Info_p->DepthStencilState = DepthStencilState;

    VkPipelineRasterizationStateCreateInfo RasterizationState = 
    {
        .sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable        = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode             = VK_POLYGON_MODE_FILL,
        .lineWidth               = 1.0f,
        .cullMode                = VK_CULL_MODE_NONE,
        .frontFace               = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        .depthBiasEnable         = VK_FALSE
    };
    Info_p->RasterizationState = RasterizationState;

    VkPipelineColorBlendAttachmentState ColorBlendAttachment = 
    {
        .blendEnable         = VK_TRUE,
        .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
        .dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
        .colorBlendOp        = VK_BLEND_OP_ADD,
        .srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
        .dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
        .alphaBlendOp        = VK_BLEND_OP_ADD,
        .colorWriteMask      = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT
    };
    Info_p->ColorBlendAttachment = ColorBlendAttachment;
    
    VkPipelineColorBlendStateCreateInfo ColorBlendState = 
    {
        .sType             = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable     = VK_FALSE,
        .logicOp           = VK_LOGIC_OP_COPY,
        .attachmentCount   = 1,
        .pAttachments      = &Info_p->ColorBlendAttachment, 
        .blendConstants[0] = 0.0f,
        .blendConstants[1] = 0.0f,
        .blendConstants[2] = 0.0f,
        .blendConstants[3] = 0.0f,
    };
    Info_p->ColorBlendState = ColorBlendState;

    VkPipelineMultisampleStateCreateInfo MultisampleState = 
    {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .sampleShadingEnable = VK_FALSE,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
    };
    Info_p->MultisampleState = MultisampleState;

NH_SILENT_END()
}

// COLOR ===========================================================================================

static NH_RESULT Nh_Vk_createColorPipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p)  
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p = &GPU_p->Driver;

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
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout))
    
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
        #include "Shaders/Color.vert.inc"
    };
    VkShaderModule vertShaderModule;
    if (Nh_Vk_createShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule) != NH_SUCCESS) {
        NH_END(NH_ERROR_BAD_STATE)
    }
    
    const uint32_t fs_code[] = 
    {
        #include "Shaders/Color.frag.inc"
    };
    VkShaderModule fragShaderModule; 
    if (Nh_Vk_createShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule) != NH_SUCCESS) {
        NH_END(NH_ERROR_BAD_STATE)
    }
    
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

    NH_CHECK(Nh_Vk_createPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, &GPU_p->Pipelines_p[NH_VK_PIPELINE_COLOR]))

    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

// SDF TEXT ========================================================================================

static NH_RESULT Nh_Vk_createTextSDFPipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p)  
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p = &GPU_p->Driver;

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
    NH_CHECK_VULKAN(Driver_p->Func.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout))

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
        #include "Shaders/Text_SDF.vert.inc"
    };
    const uint32_t fs_code[] = 
    {
        #include "Shaders/Text_SDF.frag.inc"
    };
    NH_CHECK(Nh_Vk_createShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule))
    NH_CHECK(Nh_Vk_createShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule))

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

    NH_CHECK(Nh_Vk_createPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, &GPU_p->Pipelines_p[NH_VK_PIPELINE_TEXT_SDF]))

    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

// IMAGE ===========================================================================================

static NH_RESULT Nh_Vk_createImagePipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p)  
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p = &GPU_p->Driver;

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
    if (Driver_p->Func.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout) != VK_SUCCESS) {
        NH_END(NH_ERROR_BAD_STATE)
    }

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
        #include "Shaders/Image.vert.inc"
    };
    VkShaderModule vertShaderModule;
    NH_CHECK(Nh_Vk_createShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule))
    
    const uint32_t fs_code[] = 
    {
        #include "Shaders/Image.frag.inc"
    };
    VkShaderModule fragShaderModule; 
    NH_CHECK(Nh_Vk_createShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule))
    
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

    NH_CHECK(Nh_Vk_createPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, &GPU_p->Pipelines_p[NH_VK_PIPELINE_IMAGE]))

    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

// BACKGROUND IMAGE ================================================================================

static NH_RESULT Nh_Vk_createBackgroundImagePipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p)  
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p = &GPU_p->Driver;

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
    if (Driver_p->Func.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout) != VK_SUCCESS) {
        NH_END(NH_ERROR_BAD_STATE)
    }

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
        #include "Shaders/BackgroundImage.vert.inc"
    };
    VkShaderModule vertShaderModule;
    NH_CHECK(Nh_Vk_createShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule))
    
    const uint32_t fs_code[] = 
    {
        #include "Shaders/BackgroundImage.frag.inc"
    };
    VkShaderModule fragShaderModule; 
    NH_CHECK(Nh_Vk_createShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule))
    
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

    NH_CHECK(Nh_Vk_createPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, &GPU_p->Pipelines_p[NH_VK_PIPELINE_BACKGROUND_IMAGE]))

    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

// PATH 2D ARC =====================================================================================

static NH_RESULT Nh_Vk_createCanvasRenderingContext2DArcPipeline(
    Nh_Vk_GPU *GPU_p, PipelineInfo *Info_p)  
{
NH_BEGIN()

    Nh_Vk_Driver *Driver_p = &GPU_p->Driver;

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
    if (Driver_p->Func.vkCreateDescriptorSetLayout(Driver_p->Device, &LayoutInfo, VK_NULL_HANDLE, &Layout) != VK_SUCCESS) {
        NH_END(NH_ERROR_BAD_STATE)
    }

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
        #include "Shaders/Canvas2D/Arc.vert.inc"
    };
    VkShaderModule vertShaderModule;
    NH_CHECK(Nh_Vk_createShaderModule(Driver_p, vs_code, sizeof(vs_code), &vertShaderModule))
    
    const uint32_t fs_code[] = 
    {
        #include "Shaders/Canvas2D/Arc.frag.inc"
    };
    VkShaderModule fragShaderModule; 
    NH_CHECK(Nh_Vk_createShaderModule(Driver_p, fs_code, sizeof(fs_code), &fragShaderModule))
    
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

    NH_CHECK(Nh_Vk_createPipeline(Driver_p, NH_VK_PIPELINE_GRAPHICS, &PipelineLayoutInfo, &PipelineInfo, &GPU_p->Pipelines_p[NH_VK_PIPELINE_CANVAS_RENDERING_CONTEXT_2D_ARC]))

    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, fragShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyShaderModule(Driver_p->Device, vertShaderModule, VK_NULL_HANDLE);
    Driver_p->Func.vkDestroyDescriptorSetLayout(Driver_p->Device, Layout, VK_NULL_HANDLE);

NH_END(NH_SUCCESS)
}

