// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Properties.h"
#include "../Header/Background.h"
#include "../Header/Border.h"
#include "../Header/Position.h"
#include "../Header/Padding.h"
#include "../Header/Margin.h"
#include "../Header/List.h"
#include "../Header/Text.h"
#include "../Header/Macros.h"
#include "../Header/Associate.h"
#include "../Header/Helper.h"
#include "../Header/GenericProperty.h"
#include "../Header/Image.h"

#include "../../Graphics/Header/Node.h"

#include "../../Core/Header/Memory.h"
#include "../../HTML/Header/Document.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>

// INIT ============================================================================================

void Nh_CSS_initProperties(
    NH_CSS_Properties *Properties_p)
{
NH_BEGIN()

    NH_INIT_LIST(Properties_p->Background.Images)

NH_SILENT_END()
}

// SET DEFAULT =====================================================================================

static void Nh_CSS_defaultProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    Nh_CSS_setDefaultTextProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultBackgroundProperties(Tab_p->Window_p, Node_p);
    Nh_CSS_setDefaultBorderProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultPositionProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultMarginProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultPaddingProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultListProperties(Node_p);

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computeNodeProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL init, NH_BOOL *recompute_p)
{
NH_BEGIN()

    Nh_CSS_GenericProperty *Properties_pp[NH_CSS_PROPERTY_COUNT];
    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {Properties_pp[i] = NULL;}

    Nh_CSS_configureGenericProperties(Node_p);
    Nh_CSS_getGenericProperties(Tab_p, Node_p, Properties_pp);

    if (init) {Nh_CSS_initProperties(&Node_p->Computed.Properties);}
    Nh_CSS_defaultProperties(Tab_p, Node_p);

    // em length has text-font dependency, so we calculate text first
    Nh_CSS_computeTextProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computeBackgroundProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computeBorderProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computePositionProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computeMarginProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computePaddingProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computeListProperties(Node_p, Properties_pp);

    if (recompute_p != NULL) {*recompute_p = NH_FALSE;}

    for (int i = 0; i < Node_p->Properties.count; ++i)
    {
        Nh_CSS_GenericProperty *Property_p = Nh_getListItem(&Node_p->Properties, i);
        if (Property_p->update && Property_p->triggerRecompute && recompute_p != NULL) {
            *recompute_p = NH_TRUE;
        }
        Property_p->update = NH_FALSE;
    }

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_computeProperties(
    Nh_Tab *Tab_p, NH_BOOL init, NH_BOOL *recompute_p)
{
NH_BEGIN()

    NH_BOOL recompute = NH_FALSE;
    if (recompute_p != NULL) {*recompute_p = NH_FALSE;}

    for (int i = 0; i < Tab_p->Document.Tree.Flat.Unformatted.count; ++i) {
        NH_CHECK(Nh_CSS_computeNodeProperties(Tab_p, Nh_getListItem(&Tab_p->Document.Tree.Flat.Unformatted, i), NH_FALSE, &recompute))
        if (recompute == NH_TRUE && recompute_p != NULL) {*recompute_p = NH_TRUE;}
    }

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_CSS_destroyProperties(
    NH_CSS_Properties *Properties_p)
{
NH_BEGIN()

    Nh_CSS_destroyImages(&Properties_p->Background.Images);

NH_SILENT_END()
}

