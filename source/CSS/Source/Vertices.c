// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Vertices.h"
#include "../Header/Macros.h"
#include "../Header/Helper.h"
#include "../Header/BoxTriangles.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include <string.h>

// GET =============================================================================================

int Nh_CSS_getBackgroundVertices(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float *vertices_p, int cornerTriangleCount)
{
NH_BEGIN()

    int triangleCount = 0;

    Nh_CSS_Box BorderBox = Nh_CSS_getBorderBox(Node_p); 
    Nh_CSS_Box ContentBox = Nh_CSS_getContentBox(Node_p); 
    Nh_CSS_resize(Tab_p, Node_p, &BorderBox);
    Nh_CSS_resize(Tab_p, Node_p, &ContentBox);

    Nh_Triangle Triangles_p[111] = {0.0f};

    float radii_p[4] = {0.0f};
    radii_p[0] = Node_p->Computed.Properties.Border.Radii.Values_p[0].topLeft;
    radii_p[1] = Node_p->Computed.Properties.Border.Radii.Values_p[0].topRight;
    radii_p[2] = Node_p->Computed.Properties.Border.Radii.Values_p[0].bottomLeft;
    radii_p[3] = Node_p->Computed.Properties.Border.Radii.Values_p[0].bottomRight;

    triangleCount = Nh_CSS_getBoxTriangles(BorderBox, ContentBox, radii_p, cornerTriangleCount, 0, Triangles_p, true, triangleCount, BorderBox.TopLeft.z);
    triangleCount = Nh_CSS_getBoxTriangles(BorderBox, ContentBox, radii_p, cornerTriangleCount, 1, Triangles_p, true, triangleCount, BorderBox.TopLeft.z);
    triangleCount = Nh_CSS_getBoxTriangles(BorderBox, ContentBox, radii_p, cornerTriangleCount, 2, Triangles_p, true, triangleCount, BorderBox.TopLeft.z);
    triangleCount = Nh_CSS_getBoxTriangles(BorderBox, ContentBox, radii_p, cornerTriangleCount, 3, Triangles_p, true, triangleCount, BorderBox.TopLeft.z);

    Nh_trianglesToArray(Triangles_p, vertices_p, triangleCount);

NH_END(triangleCount * 9)
}

NH_RESULT Nh_CSS_getBackgroundImageVertices(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float vertices_p[30], NH_CSS_Image *Image_p, 
    float subtractFromZ)
{
NH_BEGIN()

    Nh_CSS_Box Box = Nh_CSS_getBackgroundImageBox(Tab_p, Node_p, Image_p, subtractFromZ);

    vertices_p[0] = Box.TopLeft.x;
    vertices_p[1] = Box.TopLeft.y;
    vertices_p[2] = Box.TopLeft.z;

    vertices_p[3] = 0.0f;
    vertices_p[4] = 0.0f;

    vertices_p[5] = Box.TopLeft.x;
    vertices_p[6] = Box.BottomRight.y;
    vertices_p[7] = Box.BottomRight.z;

    vertices_p[8] = 0.0f;
    vertices_p[9] = 1.0f;

    vertices_p[10] = Box.BottomRight.x;
    vertices_p[11] = Box.BottomRight.y;
    vertices_p[12] = Box.BottomRight.z;

    vertices_p[13] = 1.0f;
    vertices_p[14] = 1.0f;

    vertices_p[15] = Box.TopLeft.x;
    vertices_p[16] = Box.TopLeft.y;
    vertices_p[17] = Box.TopLeft.z;

    vertices_p[18] = 0.0f;
    vertices_p[19] = 0.0f;

    vertices_p[20] = Box.BottomRight.x;
    vertices_p[21] = Box.BottomRight.y;
    vertices_p[22] = Box.BottomRight.z;

    vertices_p[23] = 1.0f;
    vertices_p[24] = 1.0f;

    vertices_p[25] = Box.BottomRight.x;
    vertices_p[26] = Box.TopLeft.y;
    vertices_p[27] = Box.TopLeft.z;

    vertices_p[28] = 1.0f;
    vertices_p[29] = 0.0f;

NH_END(NH_SUCCESS)
}

int Nh_CSS_getBorderVertices(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float *vertices_p, NH_CSS_BORDER type, 
    int cornerTriangleCount)
{
NH_BEGIN()

    Nh_CSS_Box PaddingBox = Nh_CSS_getPaddingBox(Node_p); 
    Nh_CSS_Box BorderBox = Nh_CSS_getBorderBox(Node_p);
    Nh_CSS_resize(Tab_p, Node_p, &PaddingBox);
    Nh_CSS_resize(Tab_p, Node_p, &BorderBox);

    Nh_Triangle Triangles_p[111] = {0.0f};

    float radii_p[4] = {0.0f};
    radii_p[0] = Node_p->Computed.Properties.Border.Radii.Values_p[0].topLeft;
    radii_p[1] = Node_p->Computed.Properties.Border.Radii.Values_p[0].topRight;
    radii_p[2] = Node_p->Computed.Properties.Border.Radii.Values_p[0].bottomLeft;
    radii_p[3] = Node_p->Computed.Properties.Border.Radii.Values_p[0].bottomRight;

    int triangleCount = 0;
    float offsetZ = 0.001f;

    switch (type)
    {
        case NH_CSS_BORDER_TOP :
            triangleCount = Nh_CSS_getBoxTriangles(BorderBox, PaddingBox, radii_p, cornerTriangleCount, 0, Triangles_p, false, 0, BorderBox.TopLeft.z - offsetZ);
            break;
        case NH_CSS_BORDER_BOTTOM :
            triangleCount = Nh_CSS_getBoxTriangles(BorderBox, PaddingBox, radii_p, cornerTriangleCount, 1, Triangles_p, false, 0, BorderBox.TopLeft.z - offsetZ);
            break;
        case NH_CSS_BORDER_LEFT :
            triangleCount = Nh_CSS_getBoxTriangles(BorderBox, PaddingBox, radii_p, cornerTriangleCount, 2, Triangles_p, false, 0, BorderBox.TopLeft.z - offsetZ);
            break;
        case NH_CSS_BORDER_RIGHT :
            triangleCount = Nh_CSS_getBoxTriangles(BorderBox, PaddingBox, radii_p, cornerTriangleCount, 3, Triangles_p, false, 0, BorderBox.TopLeft.z - offsetZ);
            break;
        default : NH_END(NH_ERROR_PARAMETERS)
    }

    Nh_trianglesToArray(Triangles_p, vertices_p, triangleCount);

NH_END(triangleCount * 9)
}

NH_RESULT Nh_CSS_getImageVertices(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float vertices_p[30], float subtractFromZ)
{
NH_BEGIN()

    Nh_CSS_Box ContentBox = Nh_CSS_getContentBox(Node_p);
    Nh_CSS_resize(Tab_p, Node_p, &ContentBox);

    ContentBox.TopLeft.z -= subtractFromZ;
    ContentBox.BottomRight.z -= subtractFromZ;

    vertices_p[0] = ContentBox.TopLeft.x;
    vertices_p[1] = ContentBox.TopLeft.y;
    vertices_p[2] = ContentBox.TopLeft.z;

    vertices_p[3] = 0.0f;
    vertices_p[4] = 0.0f;

    vertices_p[5] = ContentBox.TopLeft.x;
    vertices_p[6] = ContentBox.BottomRight.y;
    vertices_p[7] = ContentBox.BottomRight.z;

    vertices_p[8] = 0.0f;
    vertices_p[9] = 1.0f;

    vertices_p[10] = ContentBox.BottomRight.x;
    vertices_p[11] = ContentBox.BottomRight.y;
    vertices_p[12] = ContentBox.BottomRight.z;

    vertices_p[13] = 1.0f;
    vertices_p[14] = 1.0f;

    vertices_p[15] = ContentBox.TopLeft.x;
    vertices_p[16] = ContentBox.TopLeft.y;
    vertices_p[17] = ContentBox.TopLeft.z;

    vertices_p[18] = 0.0f;
    vertices_p[19] = 0.0f;

    vertices_p[20] = ContentBox.BottomRight.x;
    vertices_p[21] = ContentBox.BottomRight.y;
    vertices_p[22] = ContentBox.BottomRight.z;

    vertices_p[23] = 1.0f;
    vertices_p[24] = 1.0f;

    vertices_p[25] = ContentBox.BottomRight.x;
    vertices_p[26] = ContentBox.TopLeft.y;
    vertices_p[27] = ContentBox.TopLeft.z;

    vertices_p[28] = 1.0f;
    vertices_p[29] = 0.0f;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_getTextVertices(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, float **vertices_pp, uint32_t **indices_pp)
{
NH_BEGIN()

    if (!Nh_HTML_isTextNode(Node_p)) {NH_END(NH_ERROR_BAD_STATE)}

    int len = strlen(Node_p->text_p);
    if (len == 0) {NH_END(NH_ERROR_BAD_STATE)}

    float *vertices_p = Nh_allocate(sizeof(float) * len * 20);
    uint32_t *indices_p = Nh_allocate(sizeof(uint32_t) * len * 6);
    NH_CHECK_NULL(vertices_p, indices_p)

    memcpy(vertices_p, Node_p->Computed.Text.vertices_p, sizeof(float) * len * 20);
    memcpy(indices_p, Node_p->Computed.Text.indices_p, sizeof(uint32_t) * len * 6);

    Nh_CSS_Box ContentBox = Nh_CSS_getContentBox(Node_p);
    Nh_CSS_resize(Tab_p, Node_p, &ContentBox);

    float xOffset = NH_CSS_NORMALIZED_LENGTH(ContentBox.TopLeft.x);
    float yOffset = NH_CSS_NORMALIZED_LENGTH(ContentBox.TopLeft.y);
    float subtract = NH_CSS_NORMALIZED_LENGTH(vertices_p[0]);

    for(int i = 0, j = 1; i < len * 20; i += 20, j++)
    {
        vertices_p[i]      -= subtract;
        vertices_p[i + 5]  -= subtract;
        vertices_p[i + 10] -= subtract;
        vertices_p[i + 15] -= subtract;

        vertices_p[i]      += xOffset;
        vertices_p[i + 1]  += yOffset;
        vertices_p[i + 5]  += xOffset;
        vertices_p[i + 6]  += yOffset;
        vertices_p[i + 10] += xOffset;
        vertices_p[i + 11] += yOffset;
        vertices_p[i + 15] += xOffset;
        vertices_p[i + 16] += yOffset;

        vertices_p[i + 2]  = ContentBox.BottomRight.z - (0.000001f * j) - 0.001f;
        vertices_p[i + 7]  = ContentBox.BottomRight.z - (0.000001f * j) - 0.001f;
        vertices_p[i + 12] = ContentBox.BottomRight.z - (0.000001f * j) - 0.001f;
        vertices_p[i + 17] = ContentBox.BottomRight.z - (0.000001f * j) - 0.001f;
    }

    if (vertices_pp != NULL) {*vertices_pp = vertices_p;}
    if (indices_pp != NULL) {*indices_pp = indices_p;}

NH_END(NH_SUCCESS)
}

