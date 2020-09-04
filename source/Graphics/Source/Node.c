// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Node.h"
#include "../Header/Macros.h"

#include "../Vulkan/Header/Node.h"
#include "../Vulkan/Header/Text.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include "../../Core/Header/Memory.h"
#include "../../CSS/Header/Vertices.h"

// CREATE ==========================================================================================

/**
 * Creates resources for text rendering based on \p Node_p.
 *
 * @param  Tab_p  The tab which contains the document holding \p Node_p.
 * @param  Node_p Must be a text node. 
 * @return        @ref NH_SUCCESS when the text-rendering resources were created, various error codes if not.
 */
static NH_RESULT Nh_Gfx_createText(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (!Nh_HTML_isTextNode(Node_p)) {NH_END(NH_SUCCESS)}

    float *vertices_p = NULL;
    uint32_t *indices_p = NULL;
    NH_CHECK(Nh_CSS_getTextVertices(Tab_p, Node_p, &vertices_p, &indices_p))

    switch (Tab_p->Window_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_createText(Tab_p, Node_p, true, vertices_p, indices_p))
    }

    Nh_free(vertices_p);
    Nh_free(indices_p);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Gfx_createNode(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_HTML || Nh_HTML_isMetaNode(Node_p)) {NH_END(NH_SUCCESS)}

    if (Nh_HTML_isTextNode(Node_p)) {NH_CHECK(Nh_Gfx_createText(Tab_p, Node_p))}

    switch (Tab_p->Window_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_createNode(Tab_p, Node_p))
    }

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_Gfx_destroyNode(
    Nh_GPU *GPU_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_HTML || Nh_HTML_isMetaNode(Node_p)) {NH_SILENT_END()}

    switch (GPU_p->API)
    {
        case NH_API_VULKAN : Nh_Vk_destroyNode(GPU_p->Pointer, Node_p);
    }

NH_SILENT_END()
}

// UPDATE ==========================================================================================

NH_RESULT Nh_Gfx_updateNodeProperty(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_HTML || Nh_HTML_isMetaNode(Node_p)) {NH_END(NH_SUCCESS)}

    switch (Tab_p->Window_p->GPU.API)
    {
        case NH_API_VULKAN : NH_CHECK(Nh_Vk_updateNodeProperty(Tab_p, Node_p, Property_p))
    }

NH_END(NH_SUCCESS)
}


