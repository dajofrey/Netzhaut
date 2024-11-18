// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Vertices.h"
#include "BoxTriangulation.h"

#include "../Common/Log.h"

#include "../../nh-gfx/Base/Viewport.h"
#include "../../nh-gfx/Fonts/HarfBuzz.h"

#include "../../nh-core/System/Memory.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// CLIPBOX =========================================================================================

nh_renderer_ClipBox nh_renderer_convertToClipBox(
    nh_gfx_Viewport *Viewport_p, nh_css_PixelBox PixelBox)
{
    #define CLIP_LENGTH(VALUE) (VALUE + 1.0f)
    #define PIXEL_TO_CLIP(PIXEL, VIEWPORT, basedOnWidth)                                            \
    (                                                                                             \
        basedOnWidth ? ((((float)PIXEL) / ((float)VIEWPORT->Settings.Size.width))) * 2.0f - 1.0f  \
                     : ((((float)PIXEL) / ((float)VIEWPORT->Settings.Size.height))) * 2.0f - 1.0f \
    )

    nh_renderer_ClipBox ClipBox = {0};

    ClipBox.Position.x = PIXEL_TO_CLIP(PixelBox.Position.x, Viewport_p, true);
    ClipBox.Position.y = PIXEL_TO_CLIP(PixelBox.Position.y, Viewport_p, false);

    ClipBox.Size.width  = CLIP_LENGTH(PIXEL_TO_CLIP(PixelBox.Size.width, Viewport_p, true));
    ClipBox.Size.height = CLIP_LENGTH(PIXEL_TO_CLIP(PixelBox.Size.height, Viewport_p, false));

    ClipBox.depth = PixelBox.depth;

    return ClipBox;
}

// HELPER ==========================================================================================

//static void nh_renderer_getTextureTriangles(
//    nh_Triangle Triangles_p[2], nh_renderer_ClipBox Box)
//{
//    Triangles_p[0].V1.x = Box.Position.x;
//    Triangles_p[0].V1.y = Box.Position.y;
//    Triangles_p[0].V1.z = Box.depth;
//    Triangles_p[0].V1.u = 0.0f;
//    Triangles_p[0].V1.v = 0.0f;
//
//    Triangles_p[0].V2.x = Box.Position.x;
//    Triangles_p[0].V2.y = NH_HTML_MAX_Y(Box);
//    Triangles_p[0].V2.z = Box.depth;
//    Triangles_p[0].V2.u = 0.0f;
//    Triangles_p[0].V2.v = 1.0f;
//
//    Triangles_p[0].V3.x = NH_HTML_MAX_X(Box);
//    Triangles_p[0].V3.y = NH_HTML_MAX_Y(Box);
//    Triangles_p[0].V3.z = Box.depth;
//    Triangles_p[0].V3.u = 1.0f;
//    Triangles_p[0].V3.v = 1.0f;
//
//    Triangles_p[1].V1.x = Box.Position.x;
//    Triangles_p[1].V1.y = Box.Position.y;
//    Triangles_p[1].V1.z = Box.depth;
//    Triangles_p[1].V1.u = 0.0f;
//    Triangles_p[1].V1.v = 0.0f;
//
//    Triangles_p[1].V2.x = NH_HTML_MAX_X(Box);
//    Triangles_p[1].V2.y = NH_HTML_MAX_Y(Box);
//    Triangles_p[1].V2.z = Box.depth;
//    Triangles_p[1].V2.u = 1.0f;
//    Triangles_p[1].V2.v = 1.0f;
//
//    Triangles_p[1].V3.x = NH_HTML_MAX_X(Box);
//    Triangles_p[1].V3.y = Box.Position.y;
//    Triangles_p[1].V3.z = Box.depth;
//    Triangles_p[1].V3.u = 1.0f;
//    Triangles_p[1].V3.v = 0.0f;
//
//NH_CORE_SILENT_END()
//}

// GET =============================================================================================

int nh_renderer_getBackgroundVertices(
    nh_gfx_Viewport *Viewport_p, nh_css_Fragment *Fragment_p, float *vertices_p, int cornerTriangleCount)
{
    int triangleCount = 0;
    nh_Triangle Triangles_p[1024] = {0.0f};

    nh_css_PixelBox MarginBox = nh_css_getMarginBox(Fragment_p);
    nh_renderer_ClipBox PaddingBox = nh_renderer_convertToClipBox(Viewport_p, nh_css_getPaddingBox(&MarginBox, &Fragment_p->Box.Values)); 
    nh_renderer_ClipBox ContentBox = nh_renderer_convertToClipBox(Viewport_p, Fragment_p->ContentBox); 
    
    float radii_p[4] = {0, 0, 0, 0};
//    radii_p[0] = Fragment_p->Values.borderTopLeftRadius;
//    radii_p[1] = Fragment_p->Values.borderTopRightRadius;
//    radii_p[2] = Fragment_p->Values.borderBottomLeftRadius;
//    radii_p[3] = Fragment_p->Values.borderBottomRightRadius;
    
    triangleCount = nh_renderer_triangulateBox(PaddingBox, ContentBox, radii_p, cornerTriangleCount, "top", Triangles_p, true, triangleCount, PaddingBox.depth);
    triangleCount = nh_renderer_triangulateBox(PaddingBox, ContentBox, radii_p, cornerTriangleCount, "right", Triangles_p, true, triangleCount, PaddingBox.depth);
    triangleCount = nh_renderer_triangulateBox(PaddingBox, ContentBox, radii_p, cornerTriangleCount, "bottom", Triangles_p, true, triangleCount, PaddingBox.depth);
    triangleCount = nh_renderer_triangulateBox(PaddingBox, ContentBox, radii_p, cornerTriangleCount, "left", Triangles_p, true, triangleCount, PaddingBox.depth);

    if (vertices_p != NULL) {
//        nh_scrollTriangles(Viewport_p, Triangles_p, triangleCount);
        nh_trianglesToArray(Triangles_p, vertices_p, triangleCount, false);
    }

    return triangleCount * 9;
}

//NH_API_RESULT nh_renderer_getBackgroundImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_renderer_FormattingNodeFragment *Fragment_p, float vertices_p[30], NH_RENDERER_Image *Image_p, 
//    float subtractFromZ)
//{
//    nh_renderer_ClipBox Box = nh_renderer_convertToClipBox(Viewport_p, nh_renderer_getBackgroundImageBox(Viewport_p, Fragment_p, Image_p, subtractFromZ));
//
//    nh_Triangle Triangles_p[2] = {0.0f};
//    nh_renderer_getTextureTriangles(Triangles_p, Box);
//
//    nh_scrollTriangles(Viewport_p, Triangles_p, 2);
//    nh_trianglesToArray(Triangles_p, vertices_p, 2, true);
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}

#define PIXEL_TO_CLIP_LENGTH(PIXEL, VIEWPORT, basedOnWidth)                            \
(                                                                                      \
    basedOnWidth ? ((((float)PIXEL) / ((float)VIEWPORT->Settings.Size.width))) * 2.0f  \
                 : ((((float)PIXEL) / ((float)VIEWPORT->Settings.Size.height))) * 2.0f \
)

int nh_renderer_getBorderVertices(
    nh_gfx_Viewport *Viewport_p, nh_css_Fragment *Fragment_p, float *vertices_p, char *side_p, 
    int cornerTriangleCount)
{
    nh_css_PixelBox MarginBox = nh_css_getMarginBox(Fragment_p);
    nh_renderer_ClipBox PaddingBox = nh_renderer_convertToClipBox(Viewport_p, nh_css_getPaddingBox(&MarginBox, &Fragment_p->Box.Values)); 
    nh_renderer_ClipBox BorderBox  = nh_renderer_convertToClipBox(Viewport_p, nh_css_getBorderBox(&MarginBox, &Fragment_p->Box.Values));

    nh_Triangle Triangles_p[111] = {0.0f};

// TODO radii second value
    float radii_p[4] = {0.0f};
    radii_p[0] = PIXEL_TO_CLIP_LENGTH(Fragment_p->Box.Values.borderTopLeftRadius, Viewport_p, true);
    radii_p[1] = PIXEL_TO_CLIP_LENGTH(Fragment_p->Box.Values.borderTopRightRadius, Viewport_p, true);
    radii_p[2] = PIXEL_TO_CLIP_LENGTH(Fragment_p->Box.Values.borderBottomLeftRadius, Viewport_p, true);
    radii_p[3] = PIXEL_TO_CLIP_LENGTH(Fragment_p->Box.Values.borderBottomRightRadius, Viewport_p, true);

    float offsetZ = 0.001f;
    int triangleCount = nh_renderer_triangulateBox(BorderBox, PaddingBox, radii_p, cornerTriangleCount, side_p, Triangles_p, false, 0, BorderBox.depth - offsetZ);

//    nh_scrollTriangles(Viewport_p, Triangles_p, triangleCount);
    nh_trianglesToArray(Triangles_p, vertices_p, triangleCount, false);

    return triangleCount * 9;
}

//NH_API_RESULT nh_renderer_getImageVertices(
//    nh_gfx_Viewport *Viewport_p, nh_renderer_FormattingNodeFragment *Fragment_p, float vertices_p[30], float subtractFromZ)
//{
//    nh_renderer_ClipBox ContentBox = nh_renderer_convertToClipBox(Viewport_p, nh_renderer_getContentPixelBox(Fragment_p, 0));
//    ContentBox.depth -= subtractFromZ;
//
//    nh_Triangle Triangles_p[2] = {0.0f};
//    nh_renderer_getTextureTriangles(Triangles_p, ContentBox);
//
//    nh_scrollTriangles(Viewport_p, Triangles_p, 2);
//    nh_trianglesToArray(Triangles_p, vertices_p, 2, true);
//
//NH_DIAGNOSTIC_END(NH_SUCCESS)
//}

NH_API_RESULT nh_renderer_getTextVertices(
    nh_gfx_Viewport *Viewport_p, nh_gfx_TextSegment *Segment_p, int *x_p, int y, float *z_p,
    float **vertices_pp, uint32_t **indices_pp)
{
    unsigned int glyphs = 0;
    nh_gfx_HarfBuzzBuffer Buffer = nh_gfx_createHarfBuzzBuffer(Segment_p->FontInstance_p, Segment_p->codepoints_p, Segment_p->length);
    nh_gfx_HarfBuzzGlyphInfo *Infos_p = nh_gfx_getHarfBuzzGlyphInfos(Buffer, &glyphs);

    nh_Vertex *Vertices_p = nh_core_allocate(sizeof(nh_Vertex) * 4 * glyphs);
    uint32_t *indices_p   = nh_core_allocate(sizeof(uint32_t) * 6 * glyphs);
    NH_CORE_CHECK_NULL(Vertices_p)
    NH_CORE_CHECK_NULL(indices_p)

    nh_css_PixelBox GlyphBox;
    GlyphBox.Position.x = *x_p;
    GlyphBox.Position.y = y + (int)Segment_p->FontInstance_p->descender + Segment_p->FontInstance_p->fontSize;

    for (int i = 0, offset = 0, indicesIndex = 0; i < glyphs; ++i) 
    {
        nh_gfx_Glyph Glyph = nh_gfx_getGlyph(Segment_p->FontInstance_p, Infos_p[i].id);

        float xOffset  = Infos_p[i].xOffset/(float)(Segment_p->FontInstance_p->hres*64);
        float yAdvance = Infos_p[i].yAdvance/(float)(64);
        float yOffset  = Infos_p[i].yOffset/(float)(64);

        GlyphBox.Position.x += xOffset + Glyph.xOffset;
        GlyphBox.Position.y -= yOffset + Glyph.yOffset;
        GlyphBox.Size.width  = Glyph.width;
        GlyphBox.Size.height = Glyph.height;

        nh_renderer_ClipBox GlyphClipBox = nh_renderer_convertToClipBox(Viewport_p, GlyphBox);

        float depth = *z_p - (0.000001f * (i + 1));

        Vertices_p[offset].x = GlyphClipBox.Position.x; 
        Vertices_p[offset].y = GlyphClipBox.Position.y;
        Vertices_p[offset].z = depth;
        Vertices_p[offset].u = Glyph.u0;
        Vertices_p[offset].v = Glyph.v0;

        Vertices_p[offset + 1].x = GlyphClipBox.Position.x; 
        Vertices_p[offset + 1].y = GlyphClipBox.Position.y + GlyphClipBox.Size.height; 
        Vertices_p[offset + 1].z = depth;
        Vertices_p[offset + 1].u = Glyph.u0;
        Vertices_p[offset + 1].v = Glyph.v1;

        Vertices_p[offset + 2].x = GlyphClipBox.Position.x + GlyphClipBox.Size.width; 
        Vertices_p[offset + 2].y = GlyphClipBox.Position.y + GlyphClipBox.Size.height; 
        Vertices_p[offset + 2].z = depth;
        Vertices_p[offset + 2].u = Glyph.u1;
        Vertices_p[offset + 2].v = Glyph.v1;

        Vertices_p[offset + 3].x = GlyphClipBox.Position.x + GlyphClipBox.Size.width; 
        Vertices_p[offset + 3].y = GlyphClipBox.Position.y;
        Vertices_p[offset + 3].z = depth;
        Vertices_p[offset + 3].u = Glyph.u1;
        Vertices_p[offset + 3].v = Glyph.v0;

        uint32_t letterIndices_p[6] = {0, 1, 2, 0, 2, 3};
        for (int j = 0; j < 6; ++j) {indices_p[indicesIndex++] = offset + letterIndices_p[j];}
        offset += 4;

        GlyphBox.Position.x -= xOffset + Glyph.xOffset;
        GlyphBox.Position.x += Infos_p[i].xAdvance;
        GlyphBox.Position.y += yOffset + Glyph.yOffset; 
        GlyphBox.Position.y += yAdvance; 
    }

    *x_p = GlyphBox.Position.x;
    *z_p -= 0.000001f * glyphs;

    float *vertices_p = nh_core_allocate(sizeof(float) * 20 * glyphs);
    NH_CORE_CHECK_MEM(vertices_p)
    nh_verticesToArray(Vertices_p, vertices_p, 4 * glyphs, true, 0);

    if (vertices_pp != NULL) {*vertices_pp = vertices_p;}
    if (indices_pp != NULL) {*indices_pp = indices_p;}

    nh_core_free(Infos_p);
    nh_core_free(Vertices_p);

    nh_gfx_destroyHarfBuzzBuffer(Buffer);

    return NH_API_SUCCESS;
}

