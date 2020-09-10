// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Margin.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_CSS_UTILS

#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

static inline bool Nh_CSS_setMarginBlock(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline bool Nh_CSS_setMarginBlockStart(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline bool Nh_CSS_setMarginBlockEnd(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);

static inline bool Nh_CSS_setMargin(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setMarginBottom(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setMarginLeft(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setMarginRight(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);
static inline void Nh_CSS_setMarginTop(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p
);

static inline void Nh_CSS_collapseMargin(
    Nh_HTML_Node *Node_p
);
static inline Nh_HTML_Node *Nh_CSS_getMarginCollapsePartner(
    Nh_HTML_Node *Node_p
);

// INIT ============================================================================================

void Nh_CSS_setDefaultMarginProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_setDefaultMarginProperty(Tab_p, Node_p, NH_CSS_PROPERTY_MARGIN_BOTTOM);
    Nh_CSS_setDefaultMarginProperty(Tab_p, Node_p, NH_CSS_PROPERTY_MARGIN_LEFT);
    Nh_CSS_setDefaultMarginProperty(Tab_p, Node_p, NH_CSS_PROPERTY_MARGIN_RIGHT);
    Nh_CSS_setDefaultMarginProperty(Tab_p, Node_p, NH_CSS_PROPERTY_MARGIN_TOP);

NH_SILENT_END()
}

void Nh_CSS_setDefaultMarginProperty(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type)
{
NH_BEGIN()

    switch (type)
    {
        case NH_CSS_PROPERTY_MARGIN_BOTTOM :
            Node_p->Computed.Properties.Margin.bottom = 0.0f;
            break;
        case NH_CSS_PROPERTY_MARGIN_LEFT :
            Node_p->Computed.Properties.Margin.left = 0.0f;
            break;
        case NH_CSS_PROPERTY_MARGIN_RIGHT :
            Node_p->Computed.Properties.Margin.right = 0.0f;
            break;
        case NH_CSS_PROPERTY_MARGIN_TOP :
            Node_p->Computed.Properties.Margin.top = 0.0f;
            break;
    }

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computeMarginProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    if (!Nh_CSS_setMarginBlock(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_BLOCK]))
    {
        Nh_CSS_setMarginBlockStart(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_BLOCK_START]);
        Nh_CSS_setMarginBlockEnd(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_BLOCK_END]);
    }
    
    if (!Nh_CSS_setMargin(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_MARGIN])) 
    {
        Nh_CSS_setMarginBottom(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_BOTTOM]);
        Nh_CSS_setMarginLeft(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_LEFT]);
        Nh_CSS_setMarginRight(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_RIGHT]);
        Nh_CSS_setMarginTop(Node_p, Tab_p, Properties_pp[NH_CSS_PROPERTY_MARGIN_TOP]);
    }

    Nh_CSS_collapseMargin(Node_p);

NH_END(NH_SUCCESS)
}

// MARGIN ==========================================================================================

static inline bool Nh_CSS_setMarginBlock(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        Nh_CSS_GenericProperty SingleValue;
        SingleValue.values_pp[0] = Property_p->values_pp[0];
        
        if (Property_p->valueCount == 2) 
        {
            Nh_CSS_setMarginBlockStart(Node_p, Tab_p, &SingleValue);
            SingleValue.values_pp[0] = Property_p->values_pp[1];
            Nh_CSS_setMarginBlockEnd(Node_p, Tab_p, &SingleValue);
        } 
        else if (Property_p->valueCount == 1) 
        {
            Nh_CSS_setMarginBlockStart(Node_p, Tab_p, &SingleValue);
            Nh_CSS_setMarginBlockEnd(Node_p, Tab_p, &SingleValue);
        }

        NH_END(true)
    }

NH_END(false)
}

static inline bool Nh_CSS_setMarginBlockStart(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        if (Node_p->Computed.Properties.Text.writingMode == NH_CSS_WRITING_MODE_HORIZONTAL_TB) {
            Nh_CSS_setMarginTop(Node_p, Tab_p, Property_p);
        }

        NH_END(true)
    }

NH_END(false)
}

static inline bool Nh_CSS_setMarginBlockEnd(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        if (Node_p->Computed.Properties.Text.writingMode == NH_CSS_WRITING_MODE_HORIZONTAL_TB) {
            Nh_CSS_setMarginBottom(Node_p, Tab_p, Property_p);
        }

        NH_END(true)
    }

NH_END(false)
}

static inline bool Nh_CSS_setMargin(
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
                Nh_CSS_setMarginBottom(Node_p, Tab_p, &SingleValue);
                Nh_CSS_setMarginLeft(Node_p, Tab_p, &SingleValue);
                Nh_CSS_setMarginRight(Node_p, Tab_p, &SingleValue);
                Nh_CSS_setMarginTop(Node_p, Tab_p, &SingleValue);
                break;
            
            case 2:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setMarginTop(Node_p, Tab_p, &SingleValue);
                Nh_CSS_setMarginBottom(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setMarginRight(Node_p, Tab_p, &SingleValue);
                Nh_CSS_setMarginLeft(Node_p, Tab_p, &SingleValue);
                break;
            
            case 3:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setMarginTop(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setMarginRight(Node_p, Tab_p, &SingleValue);
                Nh_CSS_setMarginLeft(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setMarginBottom(Node_p, Tab_p, &SingleValue);
                break;
            
            case 4:
                SingleValue.values_pp[0] = Property_p->values_pp[0];
                Nh_CSS_setMarginTop(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[1];
                Nh_CSS_setMarginRight(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[2];
                Nh_CSS_setMarginBottom(Node_p, Tab_p, &SingleValue);
                SingleValue.values_pp[0] = Property_p->values_pp[3];
                Nh_CSS_setMarginLeft(Node_p, Tab_p, &SingleValue);
                break;
        }
        
        Nh_free(SingleValue.values_pp);
        NH_END(true)
    }

NH_END(false)
}

static inline void Nh_CSS_setMarginBottom(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Computed.Properties.Margin.bottom = 
                NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p));
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMarginLeft(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Computed.Properties.Margin.left = 
                NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p));
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMarginRight(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Computed.Properties.Margin.right = 
                NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_X_PIXEL(pixel, Tab_p));
        }
    }

NH_SILENT_END()
}

static inline void Nh_CSS_setMarginTop(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
        if (pixel != -1) {
            Node_p->Computed.Properties.Margin.top = 
                NH_CSS_NORMALIZED_LENGTH(NH_CSS_NORMALIZE_Y_PIXEL(pixel, Tab_p));
        }
    }

NH_SILENT_END()
}

// MARGIN COLLAPSE =================================================================================

// https://developer.mozilla.org/en-US/docs/W../CSS/CSS_Box_Model/Mastering_margin_collapsing
static inline void Nh_CSS_collapseMargin(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Node *CollapsePartner_p = Nh_CSS_getMarginCollapsePartner(Node_p);

    if (CollapsePartner_p != NULL)
    {
        if (CollapsePartner_p == Node_p->Parent_p) 
        {
            if (Node_p->Computed.Properties.Margin.top >= CollapsePartner_p->Computed.Properties.Margin.top)
            {
                // the body margin should not be affected by this
                if (Node_p->Parent_p->tag != NH_HTML_TAG_BODY) {
                    CollapsePartner_p->Computed.Properties.Margin.top = 0.0f;
                }
            } 
            else {Node_p->Computed.Properties.Margin.top = 0.0f;}

        } else if (Node_p->Computed.Properties.Margin.top > CollapsePartner_p->Computed.Properties.Margin.bottom) {
            CollapsePartner_p->Computed.Properties.Margin.bottom = 0.0f;
        } 
        else {Node_p->Computed.Properties.Margin.top = 0.0f;}
    } 

NH_SILENT_END()
}

static inline Nh_HTML_Node *Nh_CSS_getMarginCollapsePartner(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->Parent_p == NULL) {NH_END(NULL)}

    Nh_HTML_Node *CollapsePartner_p = NULL;

    if (Node_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE
    ||  Node_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE_BLOCK)
    {
        for (int i = 0; i < Node_p->Parent_p->Children.Unformatted.count; ++i) 
        {
            Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Parent_p->Children.Unformatted, i);

            if (Child_p == Node_p) {break;}
            if (Child_p->Computed.Properties.Position.display != NH_CSS_DISPLAY_INLINE
            ||  Child_p->Computed.Properties.Position.display != NH_CSS_DISPLAY_INLINE_BLOCK) {
                CollapsePartner_p = Nh_getListItem(&Node_p->Parent_p->Children.Unformatted, i);
            }
        }
        if (CollapsePartner_p == NULL) {CollapsePartner_p = Node_p->Parent_p;}
    }
    else
    {
        for (int i = 0; i < Node_p->Parent_p->Children.Unformatted.count; ++i) 
        {
            Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Parent_p->Children.Unformatted, i);
            if (Child_p == Node_p) {break;}
            CollapsePartner_p = Nh_getListItem(&Node_p->Parent_p->Children.Unformatted, i);
        }
        if (CollapsePartner_p == NULL) {CollapsePartner_p = Node_p->Parent_p;}
    }

NH_END(CollapsePartner_p)
}

