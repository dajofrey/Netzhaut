// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Pseudo.h"
#include "../Header/Macros.h"

#include "../../HTML/Header/Node.h"

#include <string.h>
#include <ctype.h>

#include NH_DEBUG

// DATA ============================================================================================

const char *NH_CSS_PSEUDO_CLASSES_PP[] = 
{
   "active",
   "any-link", 
   "blank", 
   "checked",
   "current", 
   "default",
   "defined",
   "dir()", 
   "disabled",
   "drop", 
   "empty",
   "enabled",
   "first",
   "first-child",
   "first-of-type",
   "fullscreen", 
   "future", 
   "focus",
   "focus-visible",
   "focus-within",
   "has()", 
   "host",
   "host()",
   "host-context()",
   "hover",
   "indeterminate",
   "in-range",
   "invalid",
   "is()",
   "lang()",
   "last-child",
   "last-of-type",
   "left",
   "link",
   "local-link",
   "not()",
   "nth-child()",
   "nth-col()",
   "nth-last-child()",
   "nth-last-col()",
   "nth-last-of-type()",
   "nth-of-type()",
   "only-child",
   "only-of-type",
   "optional",
   "out-of-range",
   "past",
   "placeholder-shown",
   "read-only",
   "read-write",
   "required",
   "right",
   "root",
   "scope",
   "target",
   "target-within",
   "user-invalid",
   "valid",
   "visited",
   "where()",
};

const char *NH_CSS_PSEUDO_ELEMENTS_PP[] =
{
    "after",
    "backdrop",
    "before",
    "cue",
    "cue-region",
    "first-letter",
    "first-line",
    "grammar-error",
    "marker", 
    "part",
    "placeholder",
    "selection",
    "slotted",
    "spelling-error",
};

size_t NH_CSS_PSEUDO_CLASSES_PP_COUNT = sizeof(NH_CSS_PSEUDO_CLASSES_PP) / sizeof(NH_CSS_PSEUDO_CLASSES_PP[0]);
size_t NH_CSS_PSEUDO_ELEMENTS_PP_COUNT = sizeof(NH_CSS_PSEUDO_ELEMENTS_PP) / sizeof(NH_CSS_PSEUDO_ELEMENTS_PP[0]);

// DEACTIVATE ======================================================================================

void Nh_CSS_deactivatePseudoClass(
    Nh_HTML_Tree *Tree_p, NH_CSS_PSEUDO_CLASS pseudoClass)
{
NH_BEGIN()

    for (int i = 0; i < Tree_p->Flat.Unformatted.count; ++i) 
    {
        Nh_HTML_Node *Node_p = Nh_getListItem(&Tree_p->Flat.Unformatted, i);
        Node_p->Pseudo.classes_p[pseudoClass] = NH_FALSE;
    }

NH_SILENT_END()
}

void Nh_CSS_activatePseudoClass(
    Nh_HTML_Node *Node_p, NH_CSS_PSEUDO_CLASS pseudoClass)
{
NH_BEGIN()

    Node_p->Pseudo.classes_p[pseudoClass] = NH_TRUE;

NH_SILENT_END()
}

void Nh_CSS_updateInputUnrelatedPseudos(
    Nh_HTML_Tree *Tree_p)
{
NH_BEGIN()

    for (int i = 0; i < Tree_p->Flat.Unformatted.count; ++i) 
    {
        Nh_HTML_Node *Node_p = Nh_getListItem(&Tree_p->Flat.Unformatted, i);

        // first-child
        if (Node_p->Parent_p != NULL && Nh_getListItem(&Node_p->Parent_p->Children.Unformatted, 0) == Node_p) {
            Node_p->Pseudo.classes_p[NH_CSS_PSEUDO_CLASS_FIRST_CHILD] = NH_TRUE;
        }
        else {
            Node_p->Pseudo.classes_p[NH_CSS_PSEUDO_CLASS_FIRST_CHILD] = NH_FALSE;
        }

        // first-of-type 
        Node_p->Pseudo.classes_p[NH_CSS_PSEUDO_CLASS_FIRST_CHILD] = NH_FALSE;

        for (int j = 0; Node_p->Parent_p != NULL && j < Node_p->Parent_p->Children.Unformatted.count; ++j) {
            Nh_HTML_Node *Child_p = Nh_getListItem(&Node_p->Parent_p->Children.Unformatted, j);
            if (Child_p->tag == Node_p->tag) {
                if (Child_p == Node_p) {Node_p->Pseudo.classes_p[NH_CSS_PSEUDO_CLASS_FIRST_OF_TYPE] = NH_TRUE;}
                break;
            }
        }
    }

NH_SILENT_END()
}

