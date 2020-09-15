// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Text.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"
#include "../Header/Color.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Memory.h"

#include NH_DEBUG

#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

static inline void setDirection(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void setTextAlign(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void setColor(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void setFontSize(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void setWritingMode(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);

static inline void setFontFamily(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);
static inline void setFontWeight(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p
);

// IMPLEMENT =======================================================================================

NH_RESULT Nh_CSS_setDefaultTextProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_setDefaultTextProperty(Tab_p, Node_p, NH_CSS_PROPERTY_DIRECTION);
    Nh_CSS_setDefaultTextProperty(Tab_p, Node_p, NH_CSS_PROPERTY_TEXT_ALIGN);
    Nh_CSS_setDefaultTextProperty(Tab_p, Node_p, NH_CSS_PROPERTY_COLOR);
    Nh_CSS_setDefaultTextProperty(Tab_p, Node_p, NH_CSS_PROPERTY_FONT_SIZE);
    Nh_CSS_setDefaultTextProperty(Tab_p, Node_p, NH_CSS_PROPERTY_WRITING_MODE);
    Nh_CSS_setDefaultTextProperty(Tab_p, Node_p, NH_CSS_PROPERTY_FONT);

NH_END(NH_SUCCESS)
}

void Nh_CSS_setDefaultTextProperty(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type)
{
NH_BEGIN()

    switch (type)
    {
        case NH_CSS_PROPERTY_DIRECTION : 
            Node_p->Computed.Properties.Text.direction = NH_CSS_DIRECTION_LTR; break;

        case NH_CSS_PROPERTY_TEXT_ALIGN :
            Node_p->Computed.Properties.Text.align = NH_CSS_TEXT_ALIGN_LEFT; break;
        
        case NH_CSS_PROPERTY_COLOR :
            for (int i = 0; i < 3; ++i) {Node_p->Computed.Properties.Text.color_p[i] = 0.0f;}
            Node_p->Computed.Properties.Text.color_p[3] = 1.0;
            break;

        case NH_CSS_PROPERTY_FONT_SIZE : ;
            float relative = 16.0f / ((float)Tab_p->Info.pxSize_p[1]);
            Node_p->Computed.Properties.Text.fontSize = relative *  2.0f;
            break;

        case NH_CSS_PROPERTY_WRITING_MODE :
            Node_p->Computed.Properties.Text.writingMode = NH_CSS_WRITING_MODE_HORIZONTAL_TB; break;

        case NH_CSS_PROPERTY_FONT :
            Node_p->Computed.Properties.Text.Font_p = Nh_getDefaultFont(); break;
    }

NH_SILENT_END()
}

NH_RESULT Nh_CSS_computeTextProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

    setFontSize(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_FONT_SIZE]);
    setDirection(Node_p, Properties_pp[NH_CSS_PROPERTY_LIST_STYLE_TYPE]);
    setTextAlign(Node_p, Properties_pp[NH_CSS_PROPERTY_DISPLAY]);
    setColor(Node_p, Properties_pp[NH_CSS_PROPERTY_COLOR]);
    setWritingMode(Node_p, Properties_pp[NH_CSS_PROPERTY_WRITING_MODE]);

    setFontFamily(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_FONT_FAMILY]);
    setFontWeight(Tab_p, Node_p, Properties_pp[NH_CSS_PROPERTY_FONT_WEIGHT]);

NH_END(NH_SUCCESS)
}

// IMPLEMENT PRIVATE ===============================================================================

inline static void setDirection(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        if (!strcmp(Property_p->values_pp[0], "rtl")) {
            Node_p->Computed.Properties.Text.direction = NH_CSS_DIRECTION_RTL;
        }
    }

NH_SILENT_END()
}

inline static void setTextAlign(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    Node_p->Computed.Properties.Text.align = 
        Node_p->Computed.Properties.Text.direction == NH_CSS_DIRECTION_LTR ? NH_CSS_TEXT_ALIGN_LEFT : NH_CSS_TEXT_ALIGN_RIGHT;

    if (Property_p != NULL) 
    {
             if (!strcmp(Property_p->values_pp[0], "left"))         {Node_p->Computed.Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_LEFT;} 
        else if (!strcmp(Property_p->values_pp[0], "right"))        {Node_p->Computed.Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_RIGHT;} 
        else if (!strcmp(Property_p->values_pp[0], "center"))       {Node_p->Computed.Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_CENTER;} 
        else if (!strcmp(Property_p->values_pp[0], "justify"))      {Node_p->Computed.Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_JUSTIFY;} 
        else if (!strcmp(Property_p->values_pp[0], "justify-all"))  {Node_p->Computed.Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_JUSTIFY_ALL;} 
        else if (!strcmp(Property_p->values_pp[0], "start"))        {Node_p->Computed.Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_START;} 
        else if (!strcmp(Property_p->values_pp[0], "end"))          {Node_p->Computed.Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_END;} 
        else if (!strcmp(Property_p->values_pp[0], "match-parent")) {Node_p->Computed.Properties.Text.writingMode = NH_CSS_TEXT_ALIGN_MATCH_PARENT;}
    }

NH_SILENT_END()
}

inline static void setColor(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        char color_p[64];
        strcpy(color_p, Property_p->values_pp[0]);
        color_p[strlen(Property_p->values_pp[0])] = '\0';
        Nh_CSS_getColor(color_p, Node_p->Computed.Properties.Text.color_p);
    }

NH_SILENT_END()
}

inline static void setFontSize(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        int pixel = 16;

        Nh_HTML_Node *Parent_p = Node_p->Parent_p;

             if (!strcmp(Property_p->values_pp[0], "xx-small"))  {pixel -= 9;} 
        else if (!strcmp(Property_p->values_pp[0], "x-small"))   {pixel -= 6;} 
        else if (!strcmp(Property_p->values_pp[0], "small"))     {pixel -= 3;} 
        else if (!strcmp(Property_p->values_pp[0], "medium"))    {pixel -= 0;} 
        else if (!strcmp(Property_p->values_pp[0], "large"))     {pixel += 5;} 
        else if (!strcmp(Property_p->values_pp[0], "x-large"))   {pixel += 10;} 
        else if (!strcmp(Property_p->values_pp[0], "xx-large"))  {pixel += 15;} 
        else if (!strcmp(Property_p->values_pp[0], "xxx-large")) {pixel += 20;} 
        else if (!strcmp(Property_p->values_pp[0], "smaller")) 
        {
            if (Parent_p != NULL) {
                float relative = (5.0f / ((float)Tab_p->Info.pxSize_p[1])) * 2.0f;
                Node_p->Computed.Properties.Text.fontSize = Parent_p->Computed.Properties.Text.fontSize - relative;
                NH_SILENT_END()
            }
        } 
        else if (!strcmp(Property_p->values_pp[0], "larger")) 
        {
            if (Parent_p != NULL) {
                float relative = (5.0f / ((float)Tab_p->Info.pxSize_p[1])) * 2.0f;
                Node_p->Computed.Properties.Text.fontSize = Parent_p->Computed.Properties.Text.fontSize + relative;
                NH_SILENT_END()
            }
        } 
        else if (!strcmp(Property_p->values_pp[0], "inherit")) 
        {
            if (Parent_p != NULL) {
                Node_p->Computed.Properties.Text.fontSize = Parent_p->Computed.Properties.Text.fontSize;
                NH_SILENT_END()
            }
        } 
        else if (strstr(Property_p->values_pp[0], "em")) 
        {
            float em = strtof(Property_p->values_pp[0], NULL);
            if (Parent_p != NULL) {
                pixel = em * (Parent_p->Computed.Properties.Text.fontSize * ((float)Tab_p->Info.pxSize_p[1])) / 2.0f;
            }
        }
        else if (strstr(Property_p->values_pp[0], "ex")) 
        {
            float ex = strtof(Property_p->values_pp[0], NULL);
            if (Parent_p != NULL) {
                pixel = ex * ((Parent_p->Computed.Properties.Text.fontSize * ((float)Tab_p->Info.pxSize_p[1])) / 2.0f) * 0.5; // TODO approximation
            }
        }
        else if (!strcmp(Property_p->values_pp[0], "initial")) {}
        else if (!strcmp(Property_p->values_pp[0], "unset")) {} 
        else {
            pixel = Nh_CSS_getLengthInPixel(Node_p, Tab_p, Property_p->values_pp[0]);
            if (pixel == -1) {pixel = 16;}
        }
        
        float relative = ((float)pixel) / ((float)Tab_p->Info.pxSize_p[1]);
        Node_p->Computed.Properties.Text.fontSize = relative *  2.0f;
    }

NH_SILENT_END()
}

inline static void setWritingMode(
    Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
             if (!strcmp(Property_p->values_pp[0], "vertical-rl")) {Node_p->Computed.Properties.Text.writingMode = NH_CSS_WRITING_MODE_VERTICAL_RL;} 
        else if (!strcmp(Property_p->values_pp[0], "vertical-lr")) {Node_p->Computed.Properties.Text.writingMode = NH_CSS_WRITING_MODE_VERTICAL_LR;} 
        else if (!strcmp(Property_p->values_pp[0], "sideways-rl")) {Node_p->Computed.Properties.Text.writingMode = NH_CSS_WRITING_MODE_SIDEWAYS_RL;} 
        else if (!strcmp(Property_p->values_pp[0], "sideways-lr")) {Node_p->Computed.Properties.Text.writingMode = NH_CSS_WRITING_MODE_SIDEWAYS_LR;}
    }

NH_SILENT_END()
}

inline static void setFontFamily(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        Nh_Font *Font_p = NULL;

        for (int i = 0; i < Property_p->valueCount; ++i)
        {
            if (!strcmp(Property_p->values_pp[i], "serif")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "sans-serif")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "monospace")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "cursive")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "fantasy")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "system-ui")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "math")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "emoji")) {
                break;
            } else if (!strcmp(Property_p->values_pp[i], "fangsong")) {
                break;
            } else {
                Font_p = Nh_getFontFromFamilyName(Property_p->values_pp[i]);
                if (Font_p != NULL) {break;}
            }
        }
   
        if (Font_p != NULL) {Node_p->Computed.Properties.Text.Font_p = Font_p;}
    }

NH_SILENT_END()
}

static inline void setFontWeight(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_CSS_GenericProperty *Property_p)
{
NH_BEGIN()

    if (Property_p != NULL) 
    {
        if (!strcmp(Property_p->values_pp[0], "bold"))
        {
            Nh_Font *Font_p = Node_p->Computed.Properties.Text.Font_p;
            Nh_Font Prototype = {
                .familyName_p = Font_p->familyName_p,
                .styleName_p  = "Bold",
            };
            Font_p = Nh_getFontFromPrototype(&Prototype);
            if (Font_p != NULL) {Node_p->Computed.Properties.Text.Font_p = Font_p;}
        }
    }

NH_SILENT_END()
}

