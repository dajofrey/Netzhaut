// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Helper.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/Config.h"

#include NH_DEBUG

#include <string.h>
#include <ctype.h>
#include <math.h>

// LENGTH ==========================================================================================

// https://developer.mozilla.org/en-US/docs/W../CSS/length
int Nh_CSS_getLengthInPixel(
    Nh_HTML_Node *Node_p, Nh_Tab *Tab_p, char *str_p)
{
NH_BEGIN()

    // absolute lengths
    if (strstr(str_p, "cm")) 
    {
        float cm = strtof(str_p, NULL); // TODO might be problematic https:/*/stackoverflow.com/*questions/*7951019/*how-to-convert-string-to-float
        NH_END((int)(cm * (96.0f / 2.54)))
    } 
    else if (strstr(str_p, "mm")) 
    {
        float mm = strtof(str_p, NULL);
        NH_END((int)(mm * ((96.0f / 2.56) / 10.0f)))
    }
    else if (strstr(str_p, "in")) 
    {
        float in = strtof(str_p, NULL);
        NH_END((int)(in * 96.0f))
    }
    else if (strstr(str_p, "px")) 
    {
        float px = strtof(str_p, NULL);
        NH_END((int)px)
    }
    else if (strstr(str_p, "pt")) 
    {
        float pt = strtof(str_p, NULL);
        NH_END((int)(pt * ((1.0f / 72.0f) * 96.0f)))
    }
    else if (strstr(str_p, "pc")) 
    {
        float pc = strtof(str_p, NULL);
        NH_END((int)(pc * ((1.0f / 6.0f) * 96.0f)))
    }

    // font-relative lengths
    else if (strstr(str_p, "em")) 
    {
        float em = strtof(str_p, NULL);
        NH_END(Node_p != NULL ? (int)(em * ((Node_p->Computed.Properties.Text.fontSize * ((float)Tab_p->Info.pxSize_p[1]))/2.0f)) : -1)
    }
    else if (strstr(str_p, "ex")) 
    {
        float ex = strtof(str_p, NULL);
        NH_END(Node_p != NULL ? (int)(ex * ((Node_p->Computed.Properties.Text.fontSize * ((float)Tab_p->Info.pxSize_p[1]))/2.0f) * 0.5f) : -1)
    }
    else if (strstr(str_p, "ch")) 
    {
        float ch = strtof(str_p, NULL);
        NH_END(Node_p != NULL ? (int)(ch * ((Node_p->Computed.Properties.Text.fontSize * ((float)Tab_p->Info.pxSize_p[1]))/2.0f) * 0.5f) : -1)
    }
    else if (strstr(str_p, "rem")) {

    }

    // viewport-percentage lengths
    else if (strstr(str_p, "vh")) 
    {
        float vh = strtof(str_p, NULL);
        NH_END(Tab_p != NULL ?(int)((vh/100.0f) * ((float)Tab_p->Info.pxSize_p[1])) : -1)
    }
    else if (strstr(str_p, "vw")) 
    {
        float vw = strtof(str_p, NULL);
        NH_END(Tab_p != NULL ?(int)((vw/100.0f) * ((float)Tab_p->Info.pxSize_p[0])) : -1)
    }
    else if (strstr(str_p, "vi")) {

    }
    else if (strstr(str_p, "vb")) {

    }
    else if (strstr(str_p, "vmin") || strstr(str_p, "vmax")) 
    {
        float vh = strtof(str_p, NULL);
        int h = Tab_p != NULL ?(int)((vh/100.0f) * ((float)Tab_p->Info.pxSize_p[1])) : -1;
        float vw = strtof(str_p, NULL);
        int w = Tab_p != NULL ?(int)((vw/100.0f) * ((float)Tab_p->Info.pxSize_p[0])) : -1;
        
        if (strstr(str_p, "vmin")) {NH_END(w < h ? w : h)}
        else {NH_END(w > h ? w : h)}
    }

// can't handle

NH_END(-1)
}

// FLOW ============================================================================================

bool Nh_CSS_respectFlow(
    Nh_HTML_Node *Node_p, NH_BOOL plain)
{
NH_BEGIN()

    bool flow = Node_p->Computed.Properties.Position.position != NH_CSS_POSITION_ABSOLUTE && Node_p->Computed.Properties.Position.position != NH_CSS_POSITION_FIXED;

    Nh_HTML_Node *Parent_p = plain == NH_TRUE ? Node_p->Parent_p : Node_p->Parent_p;
    while (Parent_p != NULL) {
        flow = Parent_p->Computed.Properties.Position.position != NH_CSS_POSITION_ABSOLUTE && Parent_p->Computed.Properties.Position.position != NH_CSS_POSITION_FIXED;
        if (!flow) {NH_END(false)}
        Parent_p = plain == NH_TRUE ? Parent_p->Parent_p : Parent_p->Parent_p;
    }

    if ((Node_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE
    ||   Node_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE_BLOCK)
    &&   (plain == NH_TRUE ? Node_p->Children.Unformatted.count > 0 : Node_p->Children.Formatted.count > 0)) {NH_END(false)}

NH_END(true)
}

// PRECEDENCE ======================================================================================

int Nh_CSS_getPrecedence(
    NH_CSS_SELECTOR selector)
{
NH_BEGIN()

    switch (selector)
    {
        case NH_CSS_SELECTOR_UNIVERSAL                   : NH_END(20)
        case NH_CSS_SELECTOR_TYPE                        : NH_END(5)
        case NH_CSS_SELECTOR_CLASS                       : NH_END(3)
        case NH_CSS_SELECTOR_ID                          : NH_END(2)
        case NH_CSS_SELECTOR_ATTRIBUTE                   : NH_END(0)
        case NH_CSS_SELECTOR_PSEUDO_CLASS                : NH_END(1)
        case NH_CSS_SELECTOR_DESCENDANT_COMBINATOR       : 
        case NH_CSS_SELECTOR_CHILD_COMBINATOR            : 
        case NH_CSS_SELECTOR_GENERAL_SIBLING_COMBINATOR  :
        case NH_CSS_SELECTOR_ADJACENT_SIBLING_COMBINATOR :
        case NH_CSS_SELECTOR_COLUMN_COMBINATOR           : NH_END(4)
    }
    
NH_END(30)
}

// STRIP ===========================================================================================

void Nh_CSS_strip( // TODO improve robustness
    NH_CSS_STRIP type, char *in_p, char *out_p)
{
NH_BEGIN()

    switch (type)
    {
        case NH_CSS_STRIP_URL :
        {
            in_p = in_p + 4; // jump over 'url('
            while (in_p[0] == ' ') {in_p = in_p + 1;} // jump over white spaces
            
            strcpy(out_p, in_p);
   
            // clip at ')'
            int end = 0;
            for (int i = 0; i < strlen(out_p); ++i) {
                if (out_p[i] == ')') {end = i; break;}
            }
            out_p[end] = '\0';
 
            break;
        }
    }

NH_SILENT_END()
}

