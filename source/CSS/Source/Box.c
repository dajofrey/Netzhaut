// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Box.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"
#include "../Header/Arrange.h"

#include "../../Core/Header/Tab.h"
#include "../../HTML/Header/Node.h"

#include NH_DEBUG
#include NH_CSS_UTILS
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>
#include <math.h>

// DECLARE ==========================================================================================

static void Nh_CSS_getEnclosingBoxRecursively(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_Box *Enclosing_p
);

static inline void Nh_CSS_transformToFixedSize(
    Nh_Window *Window_p, Nh_CSS_Box *Box_p, bool x, bool y
);
static inline void Nh_CSS_transformToAdaptiveSize(
    Nh_Window *Window_p, Nh_CSS_Box *Box_p, bool x, bool y
);

// ENCLOSING BOX ===================================================================================

void Nh_CSS_getEnclosingBox(
    Nh_Tab *Tab_p, Nh_CSS_Box *Box_p)
{
NH_BEGIN()

//    Nh_HTML_Node *Body_p = Nh_HTML_getNode(Tab_p, NH_HTML_TAG_BODY);
//    if (Body_p == NULL || Body_p->tag != NH_HTML_TAG_BODY) {NH_SILENT_END()}
//
//    Nh_CSS_Box Enclosing;
//    Enclosing.TopLeft.z = Enclosing.BottomRight.z = 0.99999f;
//    Enclosing.TopLeft.x = Enclosing.TopLeft.y = 2.0f;
//    Enclosing.BottomRight.x = Enclosing.BottomRight.y = 0.0f;
//
//    Nh_CSS_getEnclosingBoxRecursively(Tab_p, Body_p, &Enclosing);
//
//    Enclosing.TopLeft.x -= 1.0f;
//    Enclosing.TopLeft.y -= 1.0f;
//    Enclosing.BottomRight.x -= 1.0f;
//    Enclosing.BottomRight.y -= 1.0f;
//
//    *Box_p = Enclosing;

NH_END()
}

static void Nh_CSS_getEnclosingBoxRecursively(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_Box *Enclosing_p)
{
NH_BEGIN()

//    Nh_CSS_Box Box = Nh_CSS_getDefaultBox();
//
//    bool multiInline = false, discardUpper = false;
//
//    for (int i = 0; i < Node_p->Children.count; ++i) 
//    {
//        Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Children, i);
//        bool isInline = Child_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE || Child_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE_BLOCK;
//
//             if (isInline && multiInline) {discardUpper = true;}
//        else if (isInline)                {multiInline = true;}
//        else if (multiInline)             {multiInline = false; discardUpper = false;}
//
//        bool updateTarget = false;
//        Nh_CSS_arrange(Tab_p, Node_p, Child_p, &Box, &Box, &updateTarget);
//
//        Box.TopLeft.x += 1.0f;
//        Box.TopLeft.y += 1.0f;
//        Box.BottomRight.x += 1.0f;
//        Box.BottomRight.y += 1.0f;
//
//        if (!discardUpper && (Box.TopLeft.x < Enclosing_p->TopLeft.x)) {Enclosing_p->TopLeft.x = Box.TopLeft.x;}
//        if (!discardUpper && (Box.TopLeft.y < Enclosing_p->TopLeft.y)) {Enclosing_p->TopLeft.y = Box.TopLeft.y;}
//        if (Box.BottomRight.x > Enclosing_p->BottomRight.x) {Enclosing_p->BottomRight.x = Box.BottomRight.x;}
//        if (Box.BottomRight.y > Enclosing_p->BottomRight.y) {Enclosing_p->BottomRight.y = Box.BottomRight.y;}
//
//        Nh_CSS_getEnclosingBoxRecursively(Tab_p, Child_p, Enclosing_p);
//    }

NH_END()
}

// BACKGROUND IMAGE BOX ============================================================================

Nh_CSS_Box Nh_CSS_getBackgroundImageBox(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_Image *Image_p, float subtractFromZ)
{
NH_BEGIN()

    Nh_CSS_Box Box;

    switch (Image_p->Background.origin)
    {
        case NH_CSS_BACKGROUND_ORIGIN_PADDING_BOX : Box = Nh_CSS_getPaddingBox(Node_p); break;
        case NH_CSS_BACKGROUND_ORIGIN_BORDER_BOX  : Box = Nh_CSS_getBorderBox(Node_p); break;
        case NH_CSS_BACKGROUND_ORIGIN_CONTENT_BOX : Box = Nh_CSS_getContentBox(Node_p); break;
    }

    Nh_CSS_resize(Tab_p, Node_p, &Box);

    float containerWidth  = NH_CSS_NORMALIZED_LENGTH(Box.BottomRight.x) - NH_CSS_NORMALIZED_LENGTH(Box.TopLeft.x);
    float containerHeight = NH_CSS_NORMALIZED_LENGTH(Box.BottomRight.y) - NH_CSS_NORMALIZED_LENGTH(Box.TopLeft.y);

    // size
    float imageWidth  = Nh_Gfx_getTextureWidth(Tab_p->Window_p, Image_p->Texture_p);
    float imageHeight = Nh_Gfx_getTextureHeight(Tab_p->Window_p, Image_p->Texture_p);

    if (Image_p->Background.Size_p[0].type == NH_CSS_BACKGROUND_SIZE_PERCENT) {
        imageWidth = (Image_p->Background.Size_p[0].value / 100.0f) * containerWidth; 
    }
    if (Image_p->Background.Size_p[1].type == NH_CSS_BACKGROUND_SIZE_PERCENT) {
        imageHeight = (Image_p->Background.Size_p[1].value / 100.0f) * containerHeight; 
    }
    if (Image_p->Background.Size_p[0].type == NH_CSS_BACKGROUND_SIZE_PIXEL) {
        imageWidth = Image_p->Background.Size_p[0].value; 
    }
    if (Image_p->Background.Size_p[1].type == NH_CSS_BACKGROUND_SIZE_PIXEL) {
        imageHeight = Image_p->Background.Size_p[1].value; 
    }

    // position
    float positionX = Box.TopLeft.x;
    float positionY = Box.TopLeft.y;

    float emptyWidth  = containerWidth - imageWidth;
    float emptyHeight = containerHeight - imageHeight;

    if (Image_p->Background.Position_p[0].type == NH_CSS_BACKGROUND_POSITION_PERCENT && emptyWidth > 0.0f) {
        positionX += (Image_p->Background.Position_p[0].value / 100.0f) * emptyWidth;
    }
    else {
        positionX += Image_p->Background.Position_p[0].value;
    }

    if (Image_p->Background.Position_p[1].type == NH_CSS_BACKGROUND_POSITION_PERCENT && emptyHeight > 0.0f) {
        positionY += (Image_p->Background.Position_p[1].value / 100.0f) * emptyHeight;
    }
    else {
        positionY += Image_p->Background.Position_p[1].value;
    }

    Box.TopLeft.x = positionX;
    Box.BottomRight.x = positionX + imageWidth;
    Box.TopLeft.y = positionY;
    Box.BottomRight.y = positionY + imageHeight;

    Box.TopLeft.z -= subtractFromZ;
    Box.BottomRight.z -= subtractFromZ;

NH_END(Box)
}

// DEFAULT BOXES ===================================================================================

Nh_CSS_Box Nh_CSS_getContentBox(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_Box Box = Nh_CSS_getPaddingBox(Node_p); 

    Box.TopLeft.x += Node_p->Computed.Properties.Padding.left;
    Box.TopLeft.y += Node_p->Computed.Properties.Padding.top;
    Box.BottomRight.x -= Node_p->Computed.Properties.Padding.right;
    Box.BottomRight.y -= Node_p->Computed.Properties.Padding.bottom;

NH_END(Box)
}

Nh_CSS_Box Nh_CSS_getPaddingBox(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_Box Box = Nh_CSS_getBorderBox(Node_p);

    if (Node_p->Computed.Properties.Border.Style.left != NH_CSS_BORDER_STYLE_NONE) {
        Box.TopLeft.x += Node_p->Computed.Properties.Border.Width.left;
    }
    if (Node_p->Computed.Properties.Border.Style.top != NH_CSS_BORDER_STYLE_NONE) {
        Box.TopLeft.y += Node_p->Computed.Properties.Border.Width.top;
    }
    if (Node_p->Computed.Properties.Border.Style.right != NH_CSS_BORDER_STYLE_NONE) {
        Box.BottomRight.x -= Node_p->Computed.Properties.Border.Width.right;
    }
    if (Node_p->Computed.Properties.Border.Style.bottom != NH_CSS_BORDER_STYLE_NONE) {
        Box.BottomRight.y -= Node_p->Computed.Properties.Border.Width.bottom;
    }

NH_END(Box)
}

Nh_CSS_Box Nh_CSS_getBorderBox(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_Box Box = Node_p->Computed.Margin; 

    Box.TopLeft.x += Node_p->Computed.Properties.Margin.left;
    Box.TopLeft.y += Node_p->Computed.Properties.Margin.top;

    Box.BottomRight.x -= Node_p->Computed.Properties.Margin.right;
    Box.BottomRight.y -= Node_p->Computed.Properties.Margin.bottom;

NH_END(Box)
}

// CONVERT =========================================================================================

void Nh_CSS_getBoxOffsetRelToTab(
    Nh_Tab *Tab_p, Nh_CSS_Box *Box_p, int xy_p[2])
{
NH_BEGIN()

    if (Tab_p == NULL || Box_p == NULL || xy_p == NULL) {NH_END()}

    float relativeX = (Box_p->TopLeft.x + 1.0f) / 2.0f;
    float relativeY = (Box_p->TopLeft.y + 1.0f) / 2.0f;

    xy_p[0] = Tab_p->Info.pxSize_p[0] * relativeX;
    xy_p[1] = Tab_p->Info.pxSize_p[1] * relativeY;

NH_END()
}

void Nh_CSS_getBoxSize(
    Nh_Tab *Tab_p, Nh_CSS_Box *Box_p, int wh_p[2])
{
NH_BEGIN()

    if (Tab_p == NULL || Box_p == NULL || wh_p == NULL) {NH_END()}

    int offset_p[2];
    Nh_CSS_getBoxOffsetRelToTab(Tab_p, Box_p, offset_p);

    float relativeX = (Box_p->BottomRight.x + 1.0f) / 2.0f;
    float relativeY = (Box_p->BottomRight.y + 1.0f) / 2.0f;

    wh_p[0] = (Tab_p->Info.pxSize_p[0] * relativeX) - offset_p[0];
    wh_p[1] = (Tab_p->Info.pxSize_p[1] * relativeY) - offset_p[1];

NH_END()
}

void Nh_CSS_getBoxOffsetRelToWindow(
    Nh_Tab *Tab_p, Nh_CSS_Box *Box_p, int xy_p[2])
{
NH_BEGIN()

    if (Tab_p == NULL || Box_p == NULL || xy_p == NULL) {NH_END()}

    int offset_p[2];
    Nh_CSS_getBoxOffsetRelToTab(Tab_p, Box_p, offset_p);

    float relativeX = Tab_p->Info.relativeOffset_p[0];
    float relativeY = Tab_p->Info.relativeOffset_p[1];

    xy_p[0] = (Tab_p->Window_p->Info.pxSize_p[0] * relativeX) + offset_p[0];
    xy_p[1] = (Tab_p->Window_p->Info.pxSize_p[1] * relativeY) + offset_p[1];

NH_END()
}

// RESIZE ==========================================================================================

void Nh_CSS_resize(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_Box *Box_p)
{
NH_BEGIN()

    switch (Node_p->Computed.Properties.Position.Width.type)
    {
        case NH_CSS_SIZE_AUTO :
            
            switch (Node_p->Computed.Properties.Position.display)
            {
                case NH_CSS_DISPLAY_BLOCK : 
                case NH_CSS_DISPLAY_LIST_ITEM : 
//                    Nh_CSS_transformToAdaptiveSize(Tab_p->Window_p, Box_p, true, false);  
                    Nh_CSS_transformToFixedSize(Tab_p->Window_p, Box_p, true, true); 
                    break;
                case NH_CSS_DISPLAY_INLINE : 
                case NH_CSS_DISPLAY_INLINE_BLOCK :
                    Nh_CSS_transformToFixedSize(Tab_p->Window_p, Box_p, true, true); 
                    break;
            }

            break;
    }

    Box_p->BottomRight.x -= NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(Tab_p->Info.pxScrollOffset_p[0], Tab_p));
    Box_p->TopLeft.x     -= NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(Tab_p->Info.pxScrollOffset_p[0], Tab_p));

    Box_p->BottomRight.y -= NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(Tab_p->Info.pxScrollOffset_p[1], Tab_p));
    Box_p->TopLeft.y     -= NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(Tab_p->Info.pxScrollOffset_p[1], Tab_p));

NH_SILENT_END()
}

// HELPER ==========================================================================================

static inline void Nh_CSS_transformToFixedSize(
    Nh_Window *Window_p, Nh_CSS_Box *Box_p, bool x, bool y)
{
NH_BEGIN()

    float upperHeight = NH_CSS_NORMALIZED_LENGTH(Box_p->TopLeft.y);
    float lowerHeight = NH_CSS_NORMALIZED_LENGTH(Box_p->BottomRight.y);

    float upperWidth = NH_CSS_NORMALIZED_LENGTH(Box_p->TopLeft.x);
    float lowerWidth = NH_CSS_NORMALIZED_LENGTH(Box_p->BottomRight.x);

    if (x) {
        Box_p->TopLeft.x -= upperWidth - upperWidth / Window_p->Internal.resizeFactors_p[0];
        Box_p->BottomRight.x -= lowerWidth - lowerWidth / Window_p->Internal.resizeFactors_p[0];
    }
    if (y) {
        Box_p->TopLeft.y -= upperHeight - upperHeight / Window_p->Internal.resizeFactors_p[1];
        Box_p->BottomRight.y -= lowerHeight - lowerHeight / Window_p->Internal.resizeFactors_p[1];
    }

NH_SILENT_END()
}

static inline void Nh_CSS_transformToAdaptiveSize(
    Nh_Window *Window_p, Nh_CSS_Box *Box_p, bool x, bool y)
{
NH_BEGIN()

    float upperHeight = Box_p->TopLeft.y + 1.0f;
    float lowerHeight = fabs(Box_p->BottomRight.y - 1.0f);

    float upperWidth = Box_p->TopLeft.x + 1.0f;
    float lowerWidth = fabs(Box_p->BottomRight.x - 1.0f);
    
    if (x) {
        Box_p->TopLeft.x -= upperWidth;
        Box_p->TopLeft.x += upperWidth * (1.0f / Window_p->Internal.resizeFactors_p[0]);
        Box_p->BottomRight.x += lowerWidth;
        Box_p->BottomRight.x -= lowerWidth * (1.0f / Window_p->Internal.resizeFactors_p[0]);
    }
    if (y) {
        Box_p->TopLeft.y -= upperHeight;
        Box_p->TopLeft.y += upperHeight * (1.0f / Window_p->Internal.resizeFactors_p[1]);
        Box_p->BottomRight.y += lowerHeight;
        Box_p->BottomRight.y -= lowerHeight * (1.0f / Window_p->Internal.resizeFactors_p[1]);
    }

NH_SILENT_END()
}

Nh_CSS_Box Nh_CSS_initBox()
{
NH_BEGIN()

    Nh_CSS_Box Box;
    Box.TopLeft.z = Box.BottomRight.z = 1.0f;
    Box.TopLeft.x = Box.TopLeft.y = Box.BottomRight.x = Box.BottomRight.y = -1.0f;

NH_END(Box)
}

