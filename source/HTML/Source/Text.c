// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Text.h"
#include "../Header/Document.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/External/freetype-gl.h"
#include "../../Core/Header/String.h"

#include "../../CSS/Header/Box.h"
#include "../../CSS/Header/Associate.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS
#include NH_CSS_UTILS

#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <limits.h>
#include <float.h>

// DECLARE ========================================================================================

static uint32_t Nh_HTML_getLineBreaks(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, uint32_t *lineBreaks_p
);

static void Nh_HTML_generateTextVertices( 
    Nh_Gfx_Texture *Texture_p, Nh_HTML_Node *Node_p
);
// set position to upper left corner
static void Nh_HTML_normalizeTextVertices(
    Nh_Gfx_Texture *Texture_p, Nh_HTML_Node *Node_p
);

// INIT ============================================================================================

void Nh_HTML_initText(
    Nh_HTML_Text *Text_p)
{
NH_BEGIN()

    Text_p->yOffset = 0.0f;
    Text_p->width = 0.0f;
    Text_p->vertices_p = NULL;
    Text_p->indices_p = NULL;

NH_SILENT_END()
}

// CHECK ===========================================================================================

NH_BOOL Nh_HTML_isTextNode(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()
NH_END(Node_p != NULL && Node_p->tag == NH_HTML_TAG_TEXT && !Nh_HTML_isMetaNode(Node_p))
}

// CREATE ==========================================================================================

NH_RESULT Nh_HTML_createNormalizedText(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p, Node_p)
    if (!Nh_HTML_isTextNode(Node_p)) {NH_END(NH_SUCCESS)}

    Node_p->Computed.Text.vertices_p = Nh_allocate(sizeof(float) * 20 * strlen(Node_p->text_p));
    NH_CHECK_MEM(Node_p->Computed.Text.vertices_p)
    
    Node_p->Computed.Text.indices_p = Nh_allocate(sizeof(uint32_t) * 6 * strlen(Node_p->text_p));
    NH_CHECK_MEM(Node_p->Computed.Text.indices_p)

    Nh_Gfx_Texture *Texture_p = Nh_Gfx_initTexture(Tab_p, &Node_p->Computed.Properties.Text.Font_p->URI, NULL);
    NH_CHECK_NULL(Texture_p)

    Nh_HTML_generateTextVertices(Texture_p, Node_p);

    Node_p->Computed.Text.width = NH_CSS_NORMALIZED_LENGTH(Node_p->Computed.Text.vertices_p[strlen(Node_p->text_p) * 20 - 10]);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_createTextNodes(
    Nh_Tab *Tab_p, Nh_HTML_Node *Parent_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Tab_p, Parent_p, Node_p)

    if (!Nh_HTML_isTextNode(Node_p) || Parent_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_NONE) {
        NH_END(NH_ERROR_BAD_STATE)
    }

    uint32_t *linebreaks_p = Nh_allocate(sizeof(uint32_t) * Nh_HTML_getLineBreaks(Tab_p, Node_p, NULL));
    NH_CHECK_MEM(linebreaks_p)
    linebreaks_p[0] = 0;
    int lineCount = Nh_HTML_getLineBreaks(Tab_p, Node_p, linebreaks_p);

    for (int i = 0; lineCount > 1 && i < lineCount; ++i) 
    {     
        Nh_HTML_Node *Child_p = Nh_allocate(sizeof(Nh_HTML_Node));
        NH_CHECK_MEM(Child_p)

        Nh_HTML_initNode(Child_p, Parent_p, NH_HTML_TAG_TEXT);
        Nh_CSS_associateSheets(Tab_p, Child_p);
        Nh_HTML_computeNode(Tab_p, Child_p, NH_FALSE);

        int start = linebreaks_p[i];
        int end = i + 1 < lineCount ? linebreaks_p[i + 1] : strlen(Node_p->text_p);

        Child_p->text_p = Nh_allocate(sizeof(char) * (end - start + 1));
        NH_CHECK_MEM(Child_p->text_p)
        memcpy(Child_p->text_p, Node_p->text_p + start, (end - start) * sizeof(char));
        Child_p->text_p[end - start] = '\0';

        float width1 = NH_CSS_NORMALIZED_LENGTH(Node_p->Computed.Text.vertices_p[start * 20 - 10]);
        float width2 = NH_CSS_NORMALIZED_LENGTH(Node_p->Computed.Text.vertices_p[end * 20 - 10]);

        Child_p->Computed.Text.width = width2 - width1;
        Child_p->Computed.Text.yOffset = Node_p->Computed.Text.yOffset;
        Child_p->Computed.Text.vertices_p = &Node_p->Computed.Text.vertices_p[start * 20];
        Child_p->Computed.Text.indices_p = Node_p->Computed.Text.indices_p;

        NH_CHECK(Nh_addListItem(&Parent_p->Children.Formatted, Child_p))

        if (i + 1 < lineCount) 
        {
            Child_p = Nh_allocate(sizeof(Nh_HTML_Node));
            NH_CHECK_MEM(Child_p)
            Nh_HTML_initNode(Child_p, Parent_p, NH_HTML_TAG_BR);
            Nh_CSS_associateSheets(Tab_p, Child_p);
            Nh_HTML_computeNode(Tab_p, Child_p, NH_FALSE);
            NH_CHECK(Nh_addListItem(&Parent_p->Children.Formatted, Child_p))
        }
    }
    if (lineCount <= 1) {NH_CHECK(Nh_addListItem(&Parent_p->Children.Formatted, Node_p))}

    Node_p->Computed.Text.width = NH_CSS_NORMALIZED_LENGTH(Node_p->Computed.Text.vertices_p[strlen(Node_p->text_p) * 20 - 10]);

    Nh_free(linebreaks_p);

NH_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

//NH_RESULT Nh_HTML_updateText(
//    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
//{
//NH_BEGIN()
//
//    NH_CHECK_NULL(Tab_p, Node_p)
//    if (!Nh_HTML_isTextNode(Node_p)) {NH_END(NH_SUCCESS)}
//
//    Nh_HTML_destroyText(Node_p);
//    NH_CHECK(Nh_HTML_createNormalizedText(Tab_p, Node_p))
//    NH_CHECK(Nh_HTML_createTextNodes(Tab_p, Node_p))
//   
//NH_END(NH_SUCCESS)
//}
//
// DESTROY =========================================================================================

void Nh_HTML_destroyText(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (!Nh_HTML_isTextNode(Node_p)) {NH_SILENT_END()}

    Node_p->Computed.Text.yOffset = 0.0f;
    Node_p->Computed.Text.width = 0.0f;

    Nh_free(Node_p->Computed.Text.vertices_p);
    Nh_free(Node_p->Computed.Text.indices_p);

    Node_p->Computed.Text.vertices_p = NULL;
    Node_p->Computed.Text.indices_p  = NULL;

NH_SILENT_END()
}

void Nh_HTML_destroyFormattedTextNodes(
    Nh_HTML_Tree *Tree_p)
{
NH_BEGIN()

    Nh_List List;
    NH_INIT_LIST(List)

    for (int i = 0; i < Tree_p->Flat.Formatted.count; ++i) 
    {
        NH_BOOL destroy = NH_TRUE;
        Nh_HTML_Node *Node_p = Nh_getListItem(&Tree_p->Flat.Formatted, i);

        for (int j = 0; j < Tree_p->Flat.Unformatted.count; ++j) {
            Nh_HTML_Node *Tmp_p = Nh_getListItem(&Tree_p->Flat.Unformatted, j);
            if (Tmp_p == Node_p) {destroy = NH_FALSE;}
        }

        if (destroy) {
            Nh_HTML_destroyNode(Node_p, NH_FALSE);
            Nh_addListItem(&List, Node_p);
        }
    }

    for (int i = 0; i < List.count; ++i) {
        Nh_destroyListItemFromPointer(&Tree_p->Flat.Formatted, Nh_getListItem(&List, i), false);
    }
    Nh_destroyList(&List, true);

NH_SILENT_END()
}

// LINE BREAKS =====================================================================================

static uint32_t Nh_HTML_getLineBreaks(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, uint32_t *lineBreaks_p)
{
NH_BEGIN()

#define UNLIMITED_WIDTH FLT_MAX

    Nh_HTML_Node *BlockNode_p = Node_p;
    while (BlockNode_p != NULL && BlockNode_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE) {
        BlockNode_p = BlockNode_p->Parent_p;
    }

    Nh_CSS_Box ContentBox = Nh_CSS_getContentBox(BlockNode_p);
    Nh_CSS_resize(Tab_p, BlockNode_p, &ContentBox);

    Nh_HTML_Node *Previous_p = NULL;
    for (int i = 0; i < Node_p->Parent_p->Children.Formatted.count; ++i) {
        Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Parent_p->Children.Formatted, i);
        if (Child_p == Node_p) {break;}
        Previous_p = Child_p;
    }

    float offset = 0.0f;
    if (Previous_p != NULL) {
        Nh_CSS_Box ContentBox2 = Nh_CSS_getContentBox(Previous_p);
        Nh_CSS_resize(Tab_p, Previous_p, &ContentBox2);
        offset = NH_CSS_NORMALIZED_LENGTH(ContentBox2.BottomRight.x);
    }

//    if (Node_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE
//    ||  Node_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE_BLOCK) {
//        ContentBox.BottomRight.x = UNLIMITED_WIDTH;
//    }

    uint32_t lineCount = 1;
    int glyph = 0, word = 0;
    float xReset = 0.0f;

    float width = NH_CSS_NORMALIZED_LENGTH(ContentBox.BottomRight.x) * Tab_p->Window_p->Internal.resizeFactors_p[0];
    float diff  = NH_CSS_NORMALIZED_LENGTH(ContentBox.BottomRight.x) - width;

    switch (Node_p->Computed.Properties.Position.Width.type)
    {
        case NH_CSS_SIZE_AUTO :

            for (int i = 0; i < (strlen(Node_p->text_p) * 20); i += 20)
            {
                if ((offset + NH_CSS_NORMALIZED_LENGTH(Node_p->Computed.Text.vertices_p[i + 15]) - xReset) > (NH_CSS_NORMALIZED_LENGTH(ContentBox.BottomRight.x) - diff)) {
                    if (lineBreaks_p != NULL) {lineBreaks_p[lineCount] = word;}
		    ++lineCount;
		    xReset = (NH_CSS_NORMALIZED_LENGTH(Node_p->Computed.Text.vertices_p[(word*20) + 15]) - NH_CSS_NORMALIZED_LENGTH(ContentBox.TopLeft.x));
		}
		if (Node_p->text_p[glyph++] == ' ') {word = glyph;}
            }

            break;

        case NH_CSS_SIZE_MIN_CONTENT :

            for (int i = 0; i < (strlen(Node_p->text_p) * 20); ++i)
            {
                if (i % 20 == 0) { // for each glyph
                    if (Node_p->text_p[glyph++] == ' ') 
                    {
                        if (lineBreaks_p != NULL) {lineBreaks_p[lineCount] = glyph;}
                        ++lineCount;
                    }
                }
            }

            break;
    }

NH_END(lineCount)
}

// VERTICES ========================================================================================

static void Nh_HTML_generateTextVertices( 
    Nh_Gfx_Texture *Texture_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    vec2 pen = {{0.0f, 0.0f}};
    size_t i;
    int vertexIndex = 0, indicesIndex = 0, indexOffset = 0;

    float *vertices_p = Node_p->Computed.Text.vertices_p;
    uint32_t *indices_p = Node_p->Computed.Text.indices_p;

    float z = 0.5f;

    for(i = 0; i < strlen(Node_p->text_p); ++i)
    {
        texture_glyph_t *glyph = texture_font_get_glyph(Texture_p->data_pp[1], Node_p->text_p + i);
        if(glyph != NULL)
        {
            float kerning = 0.0f;
            if(i > 0) {kerning = texture_glyph_get_kerning(glyph, Node_p->text_p + i - 1);}
            pen.x += kerning;

            float x0  = pen.x + glyph->offset_x;
            float y0  = pen.y - glyph->offset_y;
            float x1  = x0 + glyph->width;
            float y1  = y0 + glyph->height;
            float s0 = glyph->s0;
            float t0 = glyph->t0;
            float s1 = glyph->s1;
            float t1 = glyph->t1;

            vertices_p[vertexIndex++] = x0;
            vertices_p[vertexIndex++] = y0;
            vertices_p[vertexIndex++] = z;
            vertices_p[vertexIndex++] = s0;
            vertices_p[vertexIndex++] = t0;

            vertices_p[vertexIndex++] = x0;
            vertices_p[vertexIndex++] = y1;
            vertices_p[vertexIndex++] = z;
            vertices_p[vertexIndex++] = s0;
            vertices_p[vertexIndex++] = t1;

            vertices_p[vertexIndex++] = x1;
            vertices_p[vertexIndex++] = y1;
            vertices_p[vertexIndex++] = z;
            vertices_p[vertexIndex++] = s1;
            vertices_p[vertexIndex++] = t1;

            vertices_p[vertexIndex++] = x1;
            vertices_p[vertexIndex++] = y0;
            vertices_p[vertexIndex++] = z;
            vertices_p[vertexIndex++] = s1;
            vertices_p[vertexIndex++] = t0;

            uint32_t letterIndices_p[6] = {0, 1, 2, 0, 2, 3};
            for (int i = 0; i < 6; ++i) {indices_p[indicesIndex++] = indexOffset + letterIndices_p[i];}
            indexOffset += 4;

            pen.x += glyph->advance_x;

            z -= 0.000001f;
        }
    }

    Nh_HTML_normalizeTextVertices(Texture_p, Node_p);

NH_SILENT_END()
}

static void Nh_HTML_normalizeTextVertices(
    Nh_Gfx_Texture *Texture_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    float y0 = 0.0f;
    float yMin = 9999999.9f;

    for(int i = 0; i < (strlen(Node_p->text_p) * 20); ++i)
    {
        if (i % 20 == 0) 
        {
            y0 = Node_p->Computed.Text.vertices_p[i + 1];
            if (y0 < yMin) {yMin = y0;}
        }
    }
    
    float scale = (*((float*)Texture_p->data_pp[2])) / Node_p->Computed.Properties.Text.fontSize; 
    Node_p->Computed.Text.yOffset = Node_p->Computed.Properties.Text.fontSize + (yMin / scale);
    float xOffset = 1.0f + (Node_p->Computed.Text.vertices_p[0] / scale);
    float yOffset = 1.0f + (yMin / scale); 
    
    for(int i = 0; i < (strlen(Node_p->text_p) * 20); ++i)
    {
        if (i % 5 == 0)
        {
            Node_p->Computed.Text.vertices_p[i]     /= scale;
            Node_p->Computed.Text.vertices_p[i + 1] /= scale;
            Node_p->Computed.Text.vertices_p[i]     -= xOffset;
            Node_p->Computed.Text.vertices_p[i + 1] -= yOffset - Node_p->Computed.Text.yOffset;
        }
    }

NH_SILENT_END()
}

// GET =============================================================================================

static NH_RESULT Nh_HTML_getTextRecursively(
    Nh_HTML_Node *Node_p, Nh_String *String_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i)  
    {
        Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Children.Unformatted, i);
        if (Nh_HTML_isTextNode(Child_p)) {NH_CHECK(Nh_appendToString(String_p, Child_p->text_p))}
        NH_CHECK(Nh_HTML_getTextRecursively(Child_p, String_p))
    }

NH_END(NH_SUCCESS)
}

char *Nh_HTML_getText(
    Nh_HTML_Node *Node_p, NH_BOOL recursive)
{
NH_BEGIN()

    char *result_p = NULL;

    Nh_String *String_p = Nh_allocateString(NULL);

    if (!recursive) {    
        for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
            Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Children.Unformatted, i);
            if (Nh_HTML_isTextNode(Child_p)) {
                if (Nh_appendToString(String_p, Child_p->text_p) != NH_SUCCESS) {NH_END(NULL)}
            }
        }
    }
    else if (Nh_HTML_getTextRecursively(Node_p, String_p) != NH_SUCCESS) {NH_END(NULL)}

    result_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);
   
NH_END(result_p)
}

