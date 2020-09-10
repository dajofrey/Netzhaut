// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Padding.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_CSS_UTILS

#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

static inline void Nh_JS_setPadding(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_JS_setPaddingBottom(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_JS_setPaddingLeft(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_JS_setPaddingRight(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_JS_setPaddingTop(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);

// INIT ============================================================================================

void Nh_CSS_setDefaultPaddingProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_setDefaultPaddingProperty(Tab_p, Node_p, NH_CSS_PROPERTY_PADDING_BOTTOM);
    Nh_CSS_setDefaultPaddingProperty(Tab_p, Node_p, NH_CSS_PROPERTY_PADDING_LEFT);
    Nh_CSS_setDefaultPaddingProperty(Tab_p, Node_p, NH_CSS_PROPERTY_PADDING_RIGHT);
    Nh_CSS_setDefaultPaddingProperty(Tab_p, Node_p, NH_CSS_PROPERTY_PADDING_TOP);

NH_SILENT_END()
}

void Nh_CSS_setDefaultPaddingProperty(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type)
{
NH_BEGIN()

    switch (type)
    {
        case NH_CSS_PROPERTY_PADDING_BOTTOM :
            Node_p->Computed.Properties.Padding.bottom = 0.0f;
            break;
        case NH_CSS_PROPERTY_PADDING_LEFT :
            Node_p->Computed.Properties.Padding.left = 0.0f;
            break;
        case NH_CSS_PROPERTY_PADDING_RIGHT :
            Node_p->Computed.Properties.Padding.right = 0.0f;
            break;
        case NH_CSS_PROPERTY_PADDING_TOP :
            Node_p->Computed.Properties.Padding.top = 0.0f;
            break;
    }

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computePaddingProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    Nh_JS_setPadding(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_PADDING]);
    Nh_JS_setPaddingBottom(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_PADDING_BOTTOM]);
    Nh_JS_setPaddingLeft(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_PADDING_LEFT]);
    Nh_JS_setPaddingRight(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_PADDING_RIGHT]);
    Nh_JS_setPaddingTop(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_PADDING_TOP]);

NH_END(NH_SUCCESS)
}

// PADDING =========================================================================================

static inline void Nh_JS_setPadding(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        Nh_CSS_GenericProperty SingleValue;
        SingleValue.values_pp = Nh_allocate(sizeof(void*));
        
        switch (Property_p->valueCount)
        {
            case 1:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_JS_setPaddingBottom(Node_p, Tab_p, &SingleValue);
                Nh_JS_setPaddingLeft(Node_p, Tab_p, &SingleValue);
                Nh_JS_setPaddingRight(Node_p, Tab_p, &SingleValue);
                Nh_JS_setPaddingTop(Node_p, Tab_p, &SingleValue);
                break;
            
            case 2:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_JS_setPaddingTop(Node_p, Tab_p, &SingleValue);
                Nh_JS_setPaddingBottom(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_JS_setPaddingRight(Node_p, Tab_p, &SingleValue);
                Nh_JS_setPaddingLeft(Node_p, Tab_p, &SingleValue);
                break;
            
            case 3:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_JS_setPaddingTop(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_JS_setPaddingRight(Node_p, Tab_p, &SingleValue);
                Nh_JS_setPaddingLeft(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_JS_setPaddingBottom(Node_p, Tab_p, &SingleValue);
                break;
            
            case 4:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_JS_setPaddingTop(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_JS_setPaddingRight(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_JS_setPaddingBottom(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[3];
                Nh_JS_setPaddingLeft(Node_p, Tab_p, &SingleValue);
                break;
        }
        
        free(SingleValue.values_pp);
    }

NH_SILENT_END()
}

static inline void Nh_JS_setPaddingBottom(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        Node_p->Computed.Properties.Padding.bottom = 
            NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p));
    } 

NH_SILENT_END()
}

static inline void Nh_JS_setPaddingLeft(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        Node_p->Computed.Properties.Padding.left = 
            NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p));
    }

NH_SILENT_END()
}

static inline void Nh_JS_setPaddingRight(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        Node_p->Computed.Properties.Padding.right = 
            NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p));
    } 

NH_SILENT_END()
}

static inline void Nh_JS_setPaddingTop(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        Node_p->Computed.Properties.Padding.top = 
            NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p));
    } 

NH_SILENT_END()
}

