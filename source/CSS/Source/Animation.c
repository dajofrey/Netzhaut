// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Animation.h"
#include "../Header/Macros.h"

#include "../../HTML/Header/Document.h"

#include NH_DEBUG

#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

// IMPLEMENT =======================================================================================

void Nh_CSS_setDefaultAnimationProperties(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

NH_SILENT_END()
}

void Nh_CSS_setDefaultAnimationProperty(
    Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type)
{
NH_BEGIN()

    switch (type)
    {
        default: break;
    }

NH_SILENT_END()
}

NH_RESULT Nh_CSS_computeAnimationProperties(
    Nh_HTML_Node *Node_p, NH_CSS_GenericProperty **Properties_pp)
{
NH_BEGIN()

NH_END(NH_SUCCESS)
}

