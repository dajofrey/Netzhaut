// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Tree.h"
#include "../Header/Macros.h"
#include "../Header/Log.h"

#include "../../Core/Header/Tab.h"
#include "../../Core/Header/String.h"
#include "../../CSS/Header/Arrange.h"
#include "../../Graphics/Header/Node.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// FLAT ============================================================================================

static NH_RESULT Nh_HTML_createFlatTree(
    Nh_HTML_Tree *Tree_p, Nh_HTML_Node *Node_p, NH_BOOL unformatted)
{
NH_BEGIN()

    NH_CHECK(Nh_addListItem(unformatted ? &Tree_p->Flat.Unformatted : &Tree_p->Flat.Formatted, Node_p))
    Nh_List *Children_p = unformatted ? &Node_p->Children.Unformatted : &Node_p->Children.Formatted;

    for (int i = 0; i < Children_p->count; ++i) {
        NH_CHECK(Nh_HTML_createFlatTree(Tree_p, Nh_getListItem(Children_p, i), unformatted))
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_recreateFlatTree(
    Nh_HTML_Tree *Tree_p, Nh_HTML_Node *Node_p, NH_BOOL unformatted)
{
NH_BEGIN()

    Nh_destroyList(unformatted ? &Tree_p->Flat.Unformatted : &Tree_p->Flat.Formatted, NH_FALSE);
    NH_CHECK(Nh_HTML_createFlatTree(Tree_p, Node_p, unformatted))
    Nh_HTML_logTree(Tree_p, unformatted);

NH_END(NH_SUCCESS)
}

// FORMATTED =======================================================================================

static NH_RESULT Nh_HTML_computeFormattedTreeRecursively(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) 
    {
        Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Children.Unformatted, i);

        if (Child_p->Computed.Properties.Position.display == NH_CSS_DISPLAY_NONE) {continue;}

        if (Nh_HTML_isTextNode(Child_p)) {NH_CHECK(Nh_HTML_createTextNodes(Tab_p, Node_p, Child_p))}
        else if (!Nh_HTML_isMetaNode(Child_p)) {NH_CHECK(Nh_addListItem(&Node_p->Children.Formatted, Child_p))}

        NH_CHECK(Nh_HTML_computeFormattedTreeRecursively(Tab_p, Child_p))
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_computeFormattedTree(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_computeFormattedTreeRecursively(Tab_p, Tab_p->Document.Tree.Root_p))

    NH_CHECK(Nh_HTML_recreateFlatTree(&Tab_p->Document.Tree, Tab_p->Document.Tree.Root_p, NH_FALSE))
    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_FALSE))

    for (int i = 0; i < Tab_p->Document.Tree.Flat.Formatted.count; ++i) {
        NH_CHECK(Nh_Gfx_createNode(Tab_p, Nh_getListItem(&Tab_p->Document.Tree.Flat.Formatted, i)))
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_destroyFormattedTree(
    Nh_HTML_Tree *Tree_p, Nh_GPU *GPU_p)
{
NH_BEGIN()

    for (int i = 0; i < Tree_p->Flat.Formatted.count; ++i) {
        Nh_Gfx_destroyNode(GPU_p, Nh_getListItem(&Tree_p->Flat.Formatted, i));
    }

    Nh_HTML_destroyFormattedTextNodes(Tree_p);

    for (int i = 0; i < Tree_p->Flat.Formatted.count; ++i) 
    {
        Nh_HTML_Node *Node_p = Nh_getListItem(&Tree_p->Flat.Formatted, i);
        Nh_destroyList(&Node_p->Children.Formatted, false);
    }

    Nh_destroyList(&Tree_p->Flat.Formatted, false);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_recomputeFormattedTree(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_destroyFormattedTree(&Tab_p->Document.Tree, &Tab_p->Window_p->GPU))
    NH_CHECK(Nh_HTML_computeFormattedTree(Tab_p))

NH_END(NH_SUCCESS)
}

// STRINGIFY =======================================================================================

static NH_RESULT Nh_HTML_stringifyElement(
    Nh_HTML_Node *Node_p, Nh_String *String_p, int depth, NH_BOOL unformatted)
{
NH_BEGIN()

#define INDENT() for (int ind = 0; ind < depth; ++ind) {NH_CHECK(Nh_appendToString(String_p, "  "))}

    if (Node_p->tag == NH_HTML_TAG_HTML) {
        INDENT() NH_CHECK(Nh_appendFormatToString(String_p, unformatted ? "\e[1;32mhtml\e[0m { // UNFORMATTED\n" : "\e[1;32mhtml\e[0m { // FORMATTED\n"))
    } else {
        INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "\e[1;32m%s\e[0m {\n", NH_HTML_TAGS_PP[Node_p->tag]))
    }

    if (Node_p->text_p != NULL) {
        INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  %s\n", Node_p->text_p))
    }

    for (int i = 0; i < Node_p->Attributes.count; ++i) {
        Nh_HTML_Attribute *Attribute_p = Nh_HTML_getAttribute(&Node_p->Attributes, i);
        INDENT() NH_CHECK(Nh_appendFormatToString(String_p, "  %s: %s\n", Nh_HTML_getAttributeName(Attribute_p->type), Attribute_p->value_p))
    }
    for (int i = 0; i < (unformatted ? Node_p->Children.Unformatted.count : Node_p->Children.Formatted.count); ++i) {
        NH_CHECK(Nh_HTML_stringifyElement(
            Nh_getListItem(unformatted ? &Node_p->Children.Unformatted : &Node_p->Children.Formatted, i), 
            String_p, depth + 1, unformatted
        ))
    }
        
    INDENT() NH_CHECK(Nh_appendToString(String_p, "}\n"))

NH_END(NH_SUCCESS)
}

char *Nh_HTML_stringifyTree(
    Nh_HTML_Tree *Tree_p, NH_BOOL unformatted)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_String *String_p = Nh_allocateString(NULL);

    NH_CHECK(NULL, Nh_HTML_stringifyElement(Tree_p->Root_p, String_p, 0, unformatted))

    char *chars_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);

#include NH_DEFAULT_CHECK

NH_END(chars_p)
}

