// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Position.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"

#include "../../HTML/Header/Document.h"

#include NH_DEBUG
#include NH_CSS_UTILS

#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

static inline void Nh_CSS_setPosition(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setDisplay(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);

static inline void Nh_CSS_setLeft(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setRight(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setTop(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setBottom(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);

static inline void Nh_CSS_setWidth(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setHeight(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setMinWidth(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setMinHeight(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);

// flex
static inline void Nh_CSS_setFlexBasis(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setFlexDirection(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setFlexGrow(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setFlexShrink(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setFlexWrap(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);

static inline void Nh_CSS_setOrder(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setAlignContent(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);

// IMPLEMENT =======================================================================================

void Nh_CSS_setDefaultPositionProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_POSITION);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_DISPLAY);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_BOTTOM);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_LEFT);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_RIGHT);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_TOP);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_WIDTH);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_HEIGHT);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_MIN_WIDTH);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_MIN_HEIGHT);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_FLEX_BASIS);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_FLEX_DIRECTION);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_FLEX_GROW);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_FLEX_SHRINK);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_FLEX_WRAP);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_ORDER);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, NH_CSS_PROPERTY_ALIGN_CONTENT);

NH_SILENT_END()
}

void Nh_CSS_setDefaultPositionProperty(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type)
{
NH_BEGIN()

    switch (type)
    {
        case NH_CSS_PROPERTY_POSITION :
            Node_p->Computed.Properties.Position.position = NH_CSS_POSITION_STATIC;
            break;

        case NH_CSS_PROPERTY_DISPLAY :
        {
            Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_BLOCK;
            Nh_HTML_Node *Parent_p = Node_p->Parent_p;
            if (Parent_p != NULL) {
                if (Parent_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_FLEX) {
                    Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_FLEX;
                }
                if (Parent_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_NONE) {
                    Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_NONE;
                }
            }
            break;
        }

        case NH_CSS_PROPERTY_TOP :
            Node_p->Computed.Properties.Position.Top.type = NH_CSS_SIZE_AUTO;
            break;

        case NH_CSS_PROPERTY_BOTTOM :
            Node_p->Computed.Properties.Position.Bottom.type = NH_CSS_SIZE_AUTO;
            break;

        case NH_CSS_PROPERTY_LEFT :
            Node_p->Computed.Properties.Position.Left.type = NH_CSS_SIZE_AUTO;
            break;

        case NH_CSS_PROPERTY_RIGHT :
            Node_p->Computed.Properties.Position.Right.type = NH_CSS_SIZE_AUTO;
            break;

        case NH_CSS_PROPERTY_WIDTH :
            Node_p->Computed.Properties.Position.Width.type = NH_CSS_SIZE_AUTO;
            if (Node_p->tag == NH_HTML_TAG_CANVAS) {
                Node_p->Computed.Properties.Position.Width.type = NH_CSS_SIZE__LENGTH;
                Node_p->Computed.Properties.Position.Width.value = 
                    NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(300.0f, Tab_p));
            }
            break;

        case NH_CSS_PROPERTY_HEIGHT :
            Node_p->Computed.Properties.Position.Height.type = NH_CSS_SIZE_AUTO;
            if (Node_p->tag == NH_HTML_TAG_CANVAS) {
                Node_p->Computed.Properties.Position.Height.type = NH_CSS_SIZE__LENGTH;
                Node_p->Computed.Properties.Position.Height.value = 
                    NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(150.0f, Tab_p));
            }
            break;

        case NH_CSS_PROPERTY_FLEX_BASIS :
            Node_p->Computed.Properties.Position.FlexBasis.type = NH_CSS_FLEX_BASIS_AUTO;
            break;

        case NH_CSS_PROPERTY_FLEX_DIRECTION :
            Node_p->Computed.Properties.Position.flexDirection = NH_CSS_FLEX_DIRECTION_ROW;
            break;

        case NH_CSS_PROPERTY_FLEX_GROW :
            Node_p->Computed.Properties.Position.flexGrow = 0.0f;
            break;

        case NH_CSS_PROPERTY_FLEX_SHRINK :
            Node_p->Computed.Properties.Position.flexShrink = 1.0f;
            break;

        case NH_CSS_PROPERTY_FLEX_WRAP :
            Node_p->Computed.Properties.Position.flexWrap = NH_CSS_FLEX_WRAP_NOWRAP;
            break;

        case NH_CSS_PROPERTY_ORDER :
            Node_p->Computed.Properties.Position.order = 0;
            break;

        case NH_CSS_PROPERTY_ALIGN_CONTENT :
            Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_NORMAL;
            break;

        case NH_CSS_PROPERTY_MIN_WIDTH :
            Node_p->Computed.Properties.Position.MinWidth.type = NH_CSS_SIZE_AUTO;
            break;

        case NH_CSS_PROPERTY_MIN_HEIGHT :
            Node_p->Computed.Properties.Position.MinHeight.type = NH_CSS_SIZE_AUTO;
            break;
    }

NH_SILENT_END()
}

NH_RESULT Nh_CSS_computePositionProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    Nh_CSS_setPosition(Node_p, Properties_pp[NH_CSS_PROPERTY_POSITION]);
    Nh_CSS_setDisplay(Node_p, Properties_pp[NH_CSS_PROPERTY_DISPLAY]);

    Nh_CSS_setLeft(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_LEFT]);
    Nh_CSS_setRight(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_RIGHT]);
    Nh_CSS_setTop(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_TOP]);
    Nh_CSS_setBottom(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_BOTTOM]);

    Nh_CSS_setHeight(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_HEIGHT]);
    Nh_CSS_setWidth(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_WIDTH]);
    Nh_CSS_setMinHeight(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_MIN_HEIGHT]);
    Nh_CSS_setMinWidth(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_MIN_WIDTH]);

    Nh_CSS_setAlignContent(Node_p, Properties_pp[NH_CSS_PROPERTY_ALIGN_CONTENT]);

NH_END(NH_SUCCESS)
}

// IMPLEMENT PRIVATE ==============================================================================

static inline void Nh_CSS_setPosition(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL)
    {
        char *positionValue = Property_p->values_pp[0];

             if (!strcmp(positionValue, "fixed"))    {Node_p->Computed.Properties.Position.position = NH_CSS_POSITION_FIXED;}
        else if (!strcmp(positionValue, "relative")) {Node_p->Computed.Properties.Position.position = NH_CSS_POSITION_RELATIVE;}
        else if (!strcmp(positionValue, "absolute")) {Node_p->Computed.Properties.Position.position = NH_CSS_POSITION_ABSOLUTE;}
        else if (!strcmp(positionValue, "sticky"))   {Node_p->Computed.Properties.Position.position = NH_CSS_POSITION_STICKY;}
        else                                         {Node_p->Computed.Properties.Position.position = NH_CSS_POSITION_STATIC;}
    } 

NH_SILENT_END()
}

static inline void Nh_CSS_setDisplay(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        char *displayValue = Property_p->values_pp[0];

        if (Node_p->Parent_p != NULL 
        &&  Node_p->Parent_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_NONE) {NH_SILENT_END()}

             if (!strcmp(displayValue, "inline"))             {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_INLINE;}
        else if (!strcmp(displayValue, "block"))              {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_BLOCK;}
        else if (!strcmp(displayValue, "contents"))           {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_CONTENTS;}
        else if (!strcmp(displayValue, "flex"))               {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_FLEX;}
        else if (!strcmp(displayValue, "grid"))               {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_GRID;}
        else if (!strcmp(displayValue, "inline-block"))       {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_INLINE_BLOCK;}
        else if (!strcmp(displayValue, "inline-flex"))        {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_INLINE_FLEX;}
        else if (!strcmp(displayValue, "inline-grid"))        {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_INLINE_GRID;}
        else if (!strcmp(displayValue, "inline-table"))       {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_INLINE_TABLE;}
        else if (!strcmp(displayValue, "list-item"))          {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_LIST_ITEM;}
        else if (!strcmp(displayValue, "run-in"))             {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_RUN_IN;}
        else if (!strcmp(displayValue, "table"))              {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE;}
        else if (!strcmp(displayValue, "table-caption"))      {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE_CAPTION;}
        else if (!strcmp(displayValue, "table-column-group")) {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE_COLUMN_GROUP;}
        else if (!strcmp(displayValue, "table-header-group")) {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE_HEADER_GROUP;}
        else if (!strcmp(displayValue, "table-footer-group")) {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE_FOOTER_GROUP;}
        else if (!strcmp(displayValue, "table-row-group"))    {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE_ROW_GROUP;}
        else if (!strcmp(displayValue, "table-cell"))         {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE_CELL;}
        else if (!strcmp(displayValue, "table-column"))       {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE_COLUMN;}
        else if (!strcmp(displayValue, "table-row"))          {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_TABLE_ROW;}
        else if (!strcmp(displayValue, "none"))               {Node_p->Computed.Properties.Position.display = NH_CSS_DISPLAY_NONE;}
    }

NH_SILENT_END()
}

// POSITION ========================================================================================

static inline void Nh_CSS_setLeft(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()
    
    if (Property_p != NULL)
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Computed.Properties.Position.Left.type  = NH_CSS_SIZE__LENGTH;
            Node_p->Computed.Properties.Position.Left.value = 
                NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p));
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setRight(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL)
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Computed.Properties.Position.Right.type  = NH_CSS_SIZE__LENGTH;
            Node_p->Computed.Properties.Position.Right.value = 
                NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p));
        }
    }
       
NH_SILENT_END()
}

static inline void Nh_CSS_setTop(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL)
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Computed.Properties.Position.Top.type  = NH_CSS_SIZE__LENGTH;
            Node_p->Computed.Properties.Position.Top.value = 
                NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p));
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setBottom(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL)
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Computed.Properties.Position.Bottom.type  = NH_CSS_SIZE__LENGTH;
            Node_p->Computed.Properties.Position.Bottom.value = 
                NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p));
        }
    }

NH_SILENT_END()
}

// CONTENT =========================================================================================

static inline void Nh_CSS_setWidth(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "max-content")) {Node_p->Computed.Properties.Position.Width.type = NH_CSS_SIZE_MAX_CONTENT;}
        else if (strstr(Property_p->values_pp[0], "min-content")) {Node_p->Computed.Properties.Position.Width.type = NH_CSS_SIZE_MIN_CONTENT;}
        else 
        {
            int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
            if (pixel != -1) {
                Node_p->Computed.Properties.Position.Width.type = NH_CSS_SIZE__LENGTH;
                Node_p->Computed.Properties.Position.Width.value = 
                    NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p));
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setHeight(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "max-content")) {Node_p->Computed.Properties.Position.Height.type = NH_CSS_SIZE_MAX_CONTENT;}
        else if (strstr(Property_p->values_pp[0], "min-content")) {Node_p->Computed.Properties.Position.Height.type = NH_CSS_SIZE_MIN_CONTENT;}
        else 
        {
            int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
            if (pixel != -1) {
                Node_p->Computed.Properties.Position.Height.type = NH_CSS_SIZE__LENGTH;
                Node_p->Computed.Properties.Position.Height.value = 
                    NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p));
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMinWidth(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "max-content")) {Node_p->Computed.Properties.Position.MinWidth.type = NH_CSS_SIZE_MAX_CONTENT;}
        else if (strstr(Property_p->values_pp[0], "min-content")) {Node_p->Computed.Properties.Position.MinWidth.type = NH_CSS_SIZE_MIN_CONTENT;}
        else 
        {
            int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
            if (pixel != -1) {
                Node_p->Computed.Properties.Position.MinWidth.type = NH_CSS_SIZE__LENGTH;
                Node_p->Computed.Properties.Position.MinWidth.value = 
                    NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p));
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMinHeight(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "max-content")) {Node_p->Computed.Properties.Position.MinHeight.type = NH_CSS_SIZE_MAX_CONTENT;}
        else if (strstr(Property_p->values_pp[0], "min-content")) {Node_p->Computed.Properties.Position.MinHeight.type = NH_CSS_SIZE_MIN_CONTENT;}
        else 
        {
            int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
            if (pixel != -1) {
                Node_p->Computed.Properties.Position.MinHeight.type = NH_CSS_SIZE__LENGTH;
                Node_p->Computed.Properties.Position.MinHeight.value = 
                    NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p));
            }
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setAlignContent(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
             if (strstr(Property_p->values_pp[0], "center"))         {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_CENTER;}
        else if (strstr(Property_p->values_pp[0], "start"))          {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_START;}
        else if (strstr(Property_p->values_pp[0], "end"))            {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_END;}
        else if (strstr(Property_p->values_pp[0], "flex-start"))     {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_FLEX_START;}
        else if (strstr(Property_p->values_pp[0], "flex-end"))       {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_FLEX_END;}
        else if (strstr(Property_p->values_pp[0], "normal"))         {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_NORMAL;}
        else if (strstr(Property_p->values_pp[0], "baseline"))       {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_BASELINE;}
        else if (strstr(Property_p->values_pp[0], "space-between"))  {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_SPACE_BETWEEN;}
        else if (strstr(Property_p->values_pp[0], "space-around"))   {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_SPACE_AROUND;}
        else if (strstr(Property_p->values_pp[0], "space-evenly"))   {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_SPACE_EVENLY;}
        else if (strstr(Property_p->values_pp[0], "stretch"))        {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_STRETCH;}
        else if (strstr(Property_p->values_pp[0], "first baseline")) {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_FIRST_BASELINE;}
        else if (strstr(Property_p->values_pp[0], "last baseline"))  {Node_p->Computed.Properties.Position.alignContent = NH_CSS_ALIGN_CONTENT_LAST_BASELINE;}
    }

NH_SILENT_END()
}

