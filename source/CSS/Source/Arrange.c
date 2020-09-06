// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// DEFINE ==========================================================================================

#define IS_FLEX_CONTAINER_ELEMENT() if (TargetNode_p->Parent_p != NULL && TargetNode_p->Parent_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_FLEX)
#define IS_FLEX_CONTAINER() if (TargetNode_p != NULL && TargetNode_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_FLEX)
#define DISPLAY_NONE() TargetNode_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_NONE

// INCLUDE =========================================================================================

#include "../Header/Arrange.h"
#include "../Header/Helper.h"
#include "../Header/Macros.h"
#include "../Header/Log.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/String.h"

#include NH_DEBUG
#include NH_CSS_UTILS
#include NH_HTML_UTILS
#include NH_DEFAULT_CHECK

#include <string.h>
#include <ctype.h>
#include <math.h>

// DECLARE ==========================================================================================

static inline void Nh_CSS_initZ(
    Nh_HTML_Node *Node_p, NH_BOOL unformatted
);

static inline void Nh_CSS_initTopLeftY(
    Nh_HTML_Node *Node_p
);
static inline void Nh_CSS_initTopLeftX(
    Nh_HTML_Node *Node_p
);

static inline void Nh_CSS_initBottomRightY(
    Nh_Window *Window_p, Nh_HTML_Node *Node_p
);
static inline void Nh_CSS_initBottomRightX(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL unformatted
);

static inline void Nh_CSS_updateParents(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p
);

//static inline void Nh_CSS_updateTarget(
//    Nh_Tab *Tab_p, Nh_HTML_Node *EndNode_p, Nh_HTML_Node *TargetNode_p, Nh_CSS_Box *TargetMarginBox_p, 
//    Nh_CSS_Box *UpdateTargetMarginBox_p
//);

static inline void Nh_CSS_advance(
    Nh_HTML_Node *Current_p, Nh_HTML_Node *Next_p, NH_BOOL unformatted
);

static inline float Nh_CSS_getContentWidth(
    Nh_Window *Window_p, Nh_HTML_Node *Node_p
);
static inline float Nh_CSS_getContentHeight(
    Nh_Window *Window_p, Nh_HTML_Node *Node_p
);

// ARRANGE =========================================================================================

static NH_RESULT Nh_CSS_arrangeRecursively(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL unformatted)
{
NH_BEGIN()

    if (!Nh_HTML_isMetaNode(Node_p))
    {
        Nh_CSS_initZ(Node_p, unformatted);
        Nh_CSS_initTopLeftX(Node_p);
        Nh_CSS_initTopLeftY(Node_p);
        Nh_CSS_initBottomRightY(Tab_p->Window_p, Node_p);
        Nh_CSS_initBottomRightX(Tab_p, Node_p, unformatted);

        Nh_CSS_updateParents(Tab_p, Node_p);
    }
 
    Nh_List *Children_p = unformatted ? &Node_p->Children.Unformatted : &Node_p->Children.Formatted;

    for (int i = 0; i < Children_p->count; ++i) 
    {
        Nh_HTML_Node *Child_p = Nh_getListItem(Children_p, i);
        if (i == 0) {Child_p->Computed.Margin = Nh_CSS_getContentBox(Node_p);}

        NH_CHECK(Nh_CSS_arrangeRecursively(Tab_p, Child_p, unformatted))
        Nh_CSS_advance(Child_p, Nh_getListItem(Children_p, i + 1), unformatted);
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_arrange(
    Nh_Tab *Tab_p, NH_BOOL unformatted)
{
NH_BEGIN()

    NH_CHECK(Nh_CSS_arrangeRecursively(Tab_p, Tab_p->Document.Tree.Root_p, unformatted))
    NH_CHECK(Nh_CSS_logMargins(&Tab_p->Document, unformatted))

NH_END(NH_SUCCESS)
}

// INIT ============================================================================================

static inline void Nh_CSS_initZ(
    Nh_HTML_Node *Node_p, NH_BOOL unformatted)
{
NH_BEGIN()

    float depth = 1.0f;
    
    Nh_HTML_Node *Parent_p = unformatted == NH_TRUE ? Node_p->Parent_p : Node_p->Parent_p;
    while (Parent_p != NULL) 
    {
        depth -= 0.0001f;
        Parent_p = unformatted == NH_TRUE ? Parent_p->Parent_p : Parent_p->Parent_p;
    }
    
    Node_p->Computed.Margin.TopLeft.z = depth;
    Node_p->Computed.Margin.BottomRight.z = depth;

    if (!Nh_CSS_respectFlow(Node_p, unformatted)) {
        Node_p->Computed.Margin.TopLeft.z -= 0.1f;
        Node_p->Computed.Margin.BottomRight.z -= 0.1f;
    }

NH_SILENT_END()
}

static inline void Nh_CSS_initTopLeftX(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_HTML) {Node_p->Computed.Margin.TopLeft.x = -1.0f;}

NH_SILENT_END()
}

static inline void Nh_CSS_initTopLeftY(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    if (Node_p->tag == NH_HTML_TAG_HTML) {Node_p->Computed.Margin.TopLeft.y = -1.0f;}

//    if (Node_p->Computed.Properties.Position.display != NH_CSS_DISPLAY_INLINE) {NH_SILENT_END()}
//
//    float topWidth = 
//        Node_p->Computed.Properties.Border.Width.top 
//      + Node_p->Computed.Properties.Padding.top
//      + Node_p->Computed.Properties.Margin.top;
//
//    for (int i = 0; Node_p->Parent_p != NULL && i < Node_p->Parent_p->Children.count; ++i) 
//    {
//        Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Parent_p->Children, i);
//        if (Child_p == Node_p) {continue;}
//
//        float childTopWidth = 
//            Child_p->Computed.Properties.Border.Width.top 
//          + Child_p->Computed.Properties.Padding.top
//          + Child_p->Computed.Properties.Margin.top;
//
//        if (topWidth < childTopWidth) {
//            Node_p->Computed.Margin.TopLeft.y += childTopWidth - topWidth;
//            topWidth = childTopWidth;
//        }
//    }

NH_SILENT_END()
}

static inline void Nh_CSS_initBottomRightY(
    Nh_Window *Window_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    // start with 0 height
    Node_p->Computed.Margin.BottomRight.y = Node_p->Computed.Margin.TopLeft.y;
    float contentHeight = 0.0f;

    switch (Node_p->Computed.Properties.Position.display)
    {
        case NH_CSS_DISPLAY_BLOCK :
        case NH_CSS_DISPLAY_LIST_ITEM :
        case NH_CSS_DISPLAY_INLINE :
        case NH_CSS_DISPLAY_INLINE_BLOCK :
        case NH_CSS_DISPLAY_FLEX :

            switch (Node_p->Computed.Properties.Position.Height.type)
            {
                case NH_CSS_SIZE_AUTO    : contentHeight = Nh_CSS_getContentHeight(Window_p, Node_p); break;
                case NH_CSS_SIZE__LENGTH : contentHeight = Node_p->Computed.Properties.Position.Height.value; break;
            }

            float minContentHeight = 0.0f;
            switch (Node_p->Computed.Properties.Position.MinHeight.type)
            {
                case NH_CSS_SIZE_AUTO    : minContentHeight = Nh_CSS_getContentHeight(Window_p, Node_p); break;
                case NH_CSS_SIZE__LENGTH : minContentHeight = Node_p->Computed.Properties.Position.MinHeight.value; break;
            }

            if (contentHeight < minContentHeight) {contentHeight = minContentHeight;}

            break;
    }

    Node_p->Computed.Margin.BottomRight.y += contentHeight;

    Node_p->Computed.Margin.BottomRight.y += Node_p->Computed.Properties.Padding.top;
    Node_p->Computed.Margin.BottomRight.y += Node_p->Computed.Properties.Padding.bottom;

    if (Node_p->Computed.Properties.Border.Style.top != NH_CSS_BORDER_STYLE_NONE) {
        Node_p->Computed.Margin.BottomRight.y += Node_p->Computed.Properties.Border.Width.top;
    }
    if (Node_p->Computed.Properties.Border.Style.bottom != NH_CSS_BORDER_STYLE_NONE) {
        Node_p->Computed.Margin.BottomRight.y += Node_p->Computed.Properties.Border.Width.bottom;
    }

    Node_p->Computed.Margin.BottomRight.y += Node_p->Computed.Properties.Margin.top;
    Node_p->Computed.Margin.BottomRight.y += Node_p->Computed.Properties.Margin.bottom;

NH_SILENT_END()
}

static inline void Nh_CSS_initBottomRightX(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL unformatted)
{
NH_BEGIN()

    // start with 0 width
    Node_p->Computed.Margin.BottomRight.x = Node_p->Computed.Margin.TopLeft.x;

    float contentWidth = 0.0f;

    switch (Node_p->Computed.Properties.Position.display)
    {
        case NH_CSS_DISPLAY_BLOCK     :
        case NH_CSS_DISPLAY_LIST_ITEM :
        case NH_CSS_DISPLAY_FLEX      :

            switch (Node_p->Computed.Properties.Position.Width.type)
            {
                case NH_CSS_SIZE_AUTO        : 
                {
                    switch (Node_p->tag) {
                       // root element fills entire tab width
                       case NH_HTML_TAG_HTML : Node_p->Computed.Margin.BottomRight.x = 1.0f; break;
                       // default is full width of parent content
                       default : Node_p->Computed.Margin.BottomRight.x = unformatted == NH_TRUE ? 
                           Nh_CSS_getContentBox(Node_p->Parent_p).BottomRight.x :
                           Nh_CSS_getContentBox(Node_p->Parent_p).BottomRight.x;
                    }
                    NH_SILENT_END()
                }
                case NH_CSS_SIZE_MIN_CONTENT : contentWidth += Nh_CSS_getContentWidth(Tab_p->Window_p, Node_p); break;
                case NH_CSS_SIZE__LENGTH     : contentWidth += Node_p->Computed.Properties.Position.Width.value; break;
            }

            break;

        case NH_CSS_DISPLAY_INLINE       :
        case NH_CSS_DISPLAY_INLINE_BLOCK :

            if (Node_p->Computed.Properties.Position.Width.type == NH_CSS_SIZE_AUTO) {contentWidth += Nh_CSS_getContentWidth(Tab_p->Window_p, Node_p);}
            break;
    }

    float minContentWidth = 0.0f;
    switch (Node_p->Computed.Properties.Position.MinWidth.type)
    {
        case NH_CSS_SIZE_AUTO    : minContentWidth = Nh_CSS_getContentWidth(Tab_p->Window_p, Node_p); break;
        case NH_CSS_SIZE__LENGTH : minContentWidth = Node_p->Computed.Properties.Position.MinWidth.value; break;
    }

    if (contentWidth < minContentWidth) {contentWidth = minContentWidth;}

    Node_p->Computed.Margin.BottomRight.x += contentWidth;

    Node_p->Computed.Margin.BottomRight.x += Node_p->Computed.Properties.Padding.left;
    Node_p->Computed.Margin.BottomRight.x += Node_p->Computed.Properties.Padding.right;

    if (Node_p->Computed.Properties.Border.Style.left != NH_CSS_BORDER_STYLE_NONE) {
        Node_p->Computed.Margin.BottomRight.x += Node_p->Computed.Properties.Border.Width.left;
    }
    if (Node_p->Computed.Properties.Border.Style.right != NH_CSS_BORDER_STYLE_NONE) {
        Node_p->Computed.Margin.BottomRight.x += Node_p->Computed.Properties.Border.Width.right;
    }

    Node_p->Computed.Margin.BottomRight.x += Node_p->Computed.Properties.Margin.left;
    Node_p->Computed.Margin.BottomRight.x += Node_p->Computed.Properties.Margin.right;

NH_SILENT_END()
}

// TARGET ==========================================================================================

//static inline void Nh_CSS_updateTarget(
//    Nh_Tab *Tab_p, Nh_HTML_Node *EndNode_p, Nh_HTML_Node *TargetNode_p, Nh_CSS_Box *TargetMarginBox_p, 
//    Nh_CSS_Box *UpdatedTargetMarginBox_p)
//{
//NH_BEGIN()
//
//    Nh_CSS_Box UpdatedTargetContentBox;
//    Nh_CSS_getContentBox(Tab_p, TargetNode_p, &UpdatedTargetContentBox, UpdatedTargetMarginBox_p);
//
//    switch (EndNode_p->Computed.Properties.Position.display)
//    {
//        case NH_CSS_DISPLAY_BLOCK     :
//        case NH_CSS_DISPLAY_LIST_ITEM :
//        case NH_CSS_DISPLAY_FLEX      :
//
//            switch (EndNode_p->Computed.Properties.Position.Width.type)
//            {
//                case NH_CSS_SIZE_AUTO        : TargetMarginBox_p->BottomRight.x = UpdatedTargetContentBox.BottomRight.x; break;
//                case NH_CSS_SIZE_MAX_CONTENT : break;
//                case NH_CSS_SIZE_MIN_CONTENT : break;
//                case NH_CSS_SIZE__LENGTH     : break;
//            }
//
//            break;
//
//        case NH_CSS_DISPLAY_INLINE       :
//        case NH_CSS_DISPLAY_INLINE_BLOCK :
//            break;
//    }
//
//NH_SILENT_END()
//}

// PARENT ==========================================================================================

static inline void Nh_CSS_updateParents(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_HTML_Node *Parent_p = Node_p->Parent_p;

    while (Parent_p != NULL) 
    {
        Nh_CSS_Box ParentContentBox = Nh_CSS_getContentBox(Parent_p);
    
        if (ParentContentBox.BottomRight.y < Node_p->Computed.Margin.BottomRight.y) {
            Parent_p->Computed.Margin.BottomRight.y += 
                NH_CSS_NORMALIZED_LENGTH(Node_p->Computed.Margin.BottomRight.y) 
              - NH_CSS_NORMALIZED_LENGTH(ParentContentBox.BottomRight.y);
        }
        if (Parent_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_INLINE
        || (Parent_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_BLOCK && Parent_p->Computed.Properties.Position.Width.type == NH_CSS_SIZE_AUTO)) {
            if (ParentContentBox.BottomRight.x < Node_p->Computed.Margin.BottomRight.x) {
                Parent_p->Computed.Margin.BottomRight.x += 
                    NH_CSS_NORMALIZED_LENGTH(Node_p->Computed.Margin.BottomRight.x) 
                  - NH_CSS_NORMALIZED_LENGTH(ParentContentBox.TopLeft.x);
            }
        }

        Node_p = Parent_p;
        Parent_p = Parent_p->Parent_p;
    }

NH_SILENT_END()
}

// MOVE ============================================================================================

static inline void Nh_CSS_advance(
    Nh_HTML_Node *Current_p, Nh_HTML_Node *Next_p, NH_BOOL unformatted)
{
NH_BEGIN()

#define CURRENT_DISPLAY() Current_p->Computed.Properties.Position.display
#define NEXT_DISPLAY() Next_p->Computed.Properties.Position.display

#define INLINE_TO_INLINE()                                                         \
    Next_p->Computed.Margin.TopLeft.x = Current_p->Computed.Margin.BottomRight.x;   \
    Next_p->Computed.Margin.TopLeft.y = Current_p->Computed.Margin.TopLeft.y;        \
    Next_p->Computed.Margin.BottomRight.x = Current_p->Computed.Margin.BottomRight.x; \
    Next_p->Computed.Margin.BottomRight.y = Current_p->Computed.Margin.TopLeft.y; 

#define BREAK()                                                                                                                         \
    Next_p->Computed.Margin.TopLeft.x = unformatted == NH_TRUE ?                                                                               \
        Current_p->Parent_p != NULL ? Nh_CSS_getContentBox(Current_p->Parent_p).TopLeft.x : -1.0f         \
      : Current_p->Parent_p != NULL ? Nh_CSS_getContentBox(Current_p->Parent_p).TopLeft.x : -1.0f; \
    Next_p->Computed.Margin.TopLeft.y = Current_p->Computed.Margin.BottomRight.y;                                                           \
    Next_p->Computed.Margin.BottomRight.x = Next_p->Computed.Margin.TopLeft.x;                                                               \
    Next_p->Computed.Margin.BottomRight.y = Current_p->Computed.Margin.BottomRight.y; 

    if (Next_p == NULL) {NH_SILENT_END()}

    switch (CURRENT_DISPLAY())
    {
        case NH_CSS_DISPLAY_BLOCK :
        case NH_CSS_DISPLAY_LIST_ITEM :

            switch (NEXT_DISPLAY())
            {
                case NH_CSS_DISPLAY_BLOCK :
                case NH_CSS_DISPLAY_LIST_ITEM :
                case NH_CSS_DISPLAY_INLINE :
                case NH_CSS_DISPLAY_INLINE_BLOCK :

                    BREAK()
                    break;
            }

            break;

        case NH_CSS_DISPLAY_INLINE :
        case NH_CSS_DISPLAY_INLINE_BLOCK :

            switch (NEXT_DISPLAY())
            {
                case NH_CSS_DISPLAY_BLOCK :
                case NH_CSS_DISPLAY_LIST_ITEM :

                    BREAK()
                    break;

                case NH_CSS_DISPLAY_INLINE :
                case NH_CSS_DISPLAY_INLINE_BLOCK :

                    INLINE_TO_INLINE()
                    break;
            }

            break;
    }

#undef BREAK
#undef INLINE_TO_INLINE

NH_SILENT_END()
}

// HELPER ==========================================================================================

static inline float Nh_CSS_getContentWidth(
    Nh_Window *Window_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    float width = 0.0f;

    if (Nh_HTML_isTextNode(Node_p)) {width += Node_p->Computed.Text.width;}

    if (NH_HTML_SHOW_IMAGE(Node_p)) {
        width += Nh_Gfx_getTextureWidth(Window_p, Node_p->Computed.Attributes.Texture_p);
    }

NH_END(width)
}

static inline float Nh_CSS_getContentHeight(
    Nh_Window *Window_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    float height = 0.0f;

    if (Nh_HTML_isTextNode(Node_p)) {
        height += Node_p->Computed.Properties.Text.fontSize + Node_p->Computed.Text.yOffset;
    }

    if (NH_HTML_SHOW_IMAGE(Node_p)) {
        height += Nh_Gfx_getTextureHeight(Window_p, Node_p->Computed.Attributes.Texture_p);
    }

NH_END(height)
}

// STRINGIFY =======================================================================================

static NH_RESULT Nh_CSS_stringifyElement(
    Nh_HTML_Node *Node_p, Nh_String *String_p, int depth, NH_BOOL unformatted)
{
NH_BEGIN()

#define INDENT() for (int ind = 0; ind < depth; ++ind) {NH_CHECK(Nh_appendToString(String_p, "  "))}

    INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "\e[1;32m%s\e[0m {\n", NH_HTML_TAGS_PP[Node_p->tag]))

    if (!Nh_HTML_isMetaNode(Node_p)) 
    {  
        if (Nh_HTML_isTextNode(Node_p)) {INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "\"%s\"\n", Node_p->text_p))}
        INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  x: %f %f\n", Node_p->Computed.Margin.TopLeft.x, Node_p->Computed.Margin.BottomRight.x))
        INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  y: %f\n", Node_p->Computed.Margin.TopLeft.y))
        INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "     %f\n", Node_p->Computed.Margin.BottomRight.y))
        INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  z: %f\n", Node_p->Computed.Margin.TopLeft.z))
    }

    for (int i = 0; i < (unformatted ? Node_p->Children.Unformatted.count : Node_p->Children.Formatted.count); ++i) 
    {
        NH_CHECK(Nh_CSS_stringifyElement(
            Nh_getListItem(unformatted ? &Node_p->Children.Unformatted : &Node_p->Children.Formatted, i), String_p, depth + 1, unformatted
        ))
    }
        
    INDENT() NH_CHECK(Nh_appendToString(String_p, "}\n"))

NH_END(NH_SUCCESS)
}

char *Nh_CSS_stringifyArrangement(
    Nh_HTML_Document *Document_p, NH_BOOL unformatted)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_String *String_p = Nh_allocateString(NULL);

    NH_CHECK(NULL, Nh_CSS_stringifyElement(Document_p->Tree.Root_p, String_p, 0, unformatted))

    char *chars_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);

#include NH_DEFAULT_CHECK

NH_END(chars_p)
}

