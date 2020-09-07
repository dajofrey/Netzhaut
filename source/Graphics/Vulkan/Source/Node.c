// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Node.h"
#include "../Header/Driver.h"
#include "../Header/Descriptors.h"
#include "../Header/Uniforms.h"
#include "../Header/Pipelines.h"
#include "../Header/Buffers.h"
#include "../Header/Text.h"
#include "../Header/Texture.h"
#include "../Header/GPU.h"
#include "../Header/Utils.h"
#include "../Header/Macros.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

// DECLARE =========================================================================================

//static NH_RESULT Nh_Vk_destroyCanvasRenderingContext2DResources(
//    Nh_Vk_Driver *Driver_p, NH_JS_CanvasRenderingContext2D *Context_p
//);

// CREATE ==========================================================================================

NH_RESULT Nh_Vk_createNode(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Tab_p == NULL || Node_p == NULL) {NH_END(NH_SUCCESS)}

    Nh_Vk_GPU *GPU_p = Tab_p->Window_p->GPU.Pointer;

    if (NH_HTML_SHOW_IMAGE(Node_p))
    {
        NH_CHECK(
            Nh_Vk_hardLoadTexture(GPU_p, Node_p->Computed.Attributes.Texture_p),
            Nh_Vk_createImageVertices(Tab_p, &GPU_p->Driver, Node_p, 0.0f),
            Nh_Vk_createImageUniform(&GPU_p->Driver, Node_p),
            Nh_Vk_createImageDescriptor(GPU_p, Node_p)
        )
    }
    else 
    {
        for (int i = 0; i < Node_p->Computed.Properties.Background.Images.count; ++i) 
        {
            NH_CSS_Image *Image_p = Nh_CSS_getImage(&Node_p->Computed.Properties.Background.Images, i);

            NH_CHECK(
                Nh_Vk_hardLoadTexture(GPU_p, Image_p->Texture_p),
                Nh_Vk_createBackgroundImageVertices(Tab_p, &GPU_p->Driver, Node_p, Image_p),
                Nh_Vk_createBackgroundImageUniform(Tab_p, &GPU_p->Driver, Node_p, Image_p),                 
                Nh_Vk_createBackgroundImageDescriptor(GPU_p, Node_p, Image_p)
            )
        }
        NH_CHECK(
            Nh_Vk_createBackgroundVertices(Tab_p, &GPU_p->Driver, Node_p),
            Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_BACKGROUND),
            Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_BACKGROUND)
        )
    }

    if (NH_CSS_SHOW_TOP_BORDER(Node_p))
    {
        NH_CHECK(
            Nh_Vk_createBorderVertices(Tab_p, &GPU_p->Driver, Node_p, NH_VK_BUFFER_TOP_BORDER_VERTICES),
            Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_TOP_BORDER),
            Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_TOP_BORDER)
        )
    }

    if (NH_CSS_SHOW_RIGHT_BORDER(Node_p))
    {
        NH_CHECK(
            Nh_Vk_createBorderVertices(Tab_p, &GPU_p->Driver, Node_p, NH_VK_BUFFER_RIGHT_BORDER_VERTICES),
            Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_RIGHT_BORDER),
            Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_RIGHT_BORDER)
        )
    }

    if (NH_CSS_SHOW_BOTTOM_BORDER(Node_p))
    {
        NH_CHECK(
            Nh_Vk_createBorderVertices(Tab_p, &GPU_p->Driver, Node_p, NH_VK_BUFFER_BOTTOM_BORDER_VERTICES),
            Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_BOTTOM_BORDER),
            Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_BOTTOM_BORDER)
        )
    }

    if (NH_CSS_SHOW_LEFT_BORDER(Node_p))
    {
        NH_CHECK(
            Nh_Vk_createBorderVertices(Tab_p, &GPU_p->Driver, Node_p, NH_VK_BUFFER_LEFT_BORDER_VERTICES),
            Nh_Vk_createColorUniform(&GPU_p->Driver, Node_p, NH_VK_UNIFORM_LEFT_BORDER),
            Nh_Vk_createColorDescriptor(&GPU_p->Driver, Node_p, NH_VK_DESCRIPTOR_LEFT_BORDER)
        )
    }

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_Vk_destroyNode(
    Nh_Vk_GPU *GPU_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_Vk_destroyBufferList(&GPU_p->Driver, Node_p);
    Nh_Vk_destroyUniformList(&GPU_p->Driver, Node_p);
    Nh_Vk_destroyDescriptorList(&GPU_p->Driver, Node_p);        
//
//    for (int i = 0; i < Node_p->Injections.count; ++i) {
//        NH_JS_Injection *Injection_p = Nh_getListItem(&Node_p->Injections, i);
//        switch (Injection_p->type)
//        {
//            case NH_JS_INJECTION_CANVAS_RENDERING_CONTEXT_2D :
//                Nh_Vk_destroyCanvasRenderingContext2DResources(&GPU_p->Driver, Injection_p->data_p);
//        }
//    }

NH_SILENT_END()
}

// CANVAS RENDERING CONTEXT 2D =====================================================================

//NH_RESULT Nh_Vk_createCanvasRenderingContext2DResources(
//    Nh_Tab *Tab_p, NH_JS_CanvasRenderingContext2D *Context_p)
//{
//NH_BEGIN()
//
//    int count = 0;
//    Nh_HTML_Node *Node_p = NULL;
//
//    for (int i = 0; i < Tab_p->Document.Tree.Flat.count; ++i) {
//        Node_p = Nh_getListItem(&Tab_p->Document.Tree.Flat, i);
//        if (Nh_JS_getInjection(Node_p, NH_JS_INJECTION_CANVAS_RENDERING_CONTEXT_2D) == Context_p) {break;}
//        Node_p = NULL;
//    }
//
//    NH_CHECK_NULL(Node_p)
//
//    Nh_GPU *GPU_p = &Tab_p->Window_p->GPU;
//    Nh_Vk_Driver *Driver_p = &((Nh_Vk_GPU*)GPU_p->Pointer)->Driver;
//
//    for (int i = 0; i < Context_p->Paths.count; ++i)
//    {
//        NH_JS_Path2D *Path_p = Nh_getListItem(&Context_p->Paths, i);
//        for (int j = 0; j < Path_p->Commands.count; ++j)
//        {
//            if (count++ < Context_p->Vulkan.Uniforms.count) {continue;} // skip already handled commands
//            if (count == 1) {NH_CHECK(Nh_Vk_createImageVertices(Tab_p, Driver_p, Node_p, 0.001f))}
//
//            NH_JS_Path2DCommand *Command_p = Nh_getListItem(&Path_p->Commands, j);
//            switch (Command_p->type)
//            {   
//                case NH_JS_PATH_2D_COMMAND_ARC :
//
//                    NH_CHECK(Nh_Vk_createCanvasRenderingContext2DArcUniform(
//                        Tab_p, Driver_p, Node_p, Context_p, Command_p->data_p
//                    ))
//                    NH_CHECK(Nh_Vk_createCanvasRenderingContext2DDescriptor(
//                        Tab_p, Driver_p, Context_p, Nh_getListItem(&Context_p->Vulkan.Uniforms, j), 29 * sizeof(float)
//                    ))
//                    break;
//
//                default : break;
//            }
//        }
//    }
//
//NH_END(NH_SUCCESS)
//}
//
//static NH_RESULT Nh_Vk_destroyCanvasRenderingContext2DResources(
//    Nh_Vk_Driver *Driver_p, NH_JS_CanvasRenderingContext2D *Context_p)
//{
//NH_BEGIN()
//
//    for (int i = 0; i < Context_p->Vulkan.Descriptors.count; ++i)
//    {
//        NH_Vk_DescriptorSet *Descriptor_p = Nh_getListItem(&Context_p->Vulkan.Descriptors, i);
//        Nh_Vk_destroyDescriptorSet(Driver_p, Descriptor_p, &Driver_p->DescriptorPool_p[0]);
//    }
//    Nh_destroyList(&Context_p->Vulkan.Descriptors, true);
//
//    for (int i = 0; i < Context_p->Vulkan.Uniforms.count; ++i)
//    {
//        NH_Vk_Buffer *Buffer_p = Nh_getListItem(&Context_p->Vulkan.Uniforms, i);
//        Nh_Vk_destroyBuffer(Driver_p, Buffer_p);
//    }
//    Nh_destroyList(&Context_p->Vulkan.Uniforms, true);
//
//NH_END(NH_SUCCESS)
//}
//
