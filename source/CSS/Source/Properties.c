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

static void Nh_CSS_configureProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Properties.count; ++i) {
        Nh_CSS_configureGenericProperty(
            Node_p, Nh_CSS_getProperty(&Node_p->Properties, i)
        );
    }

    Nh_CSS_setDefaultTextProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultBackgroundProperties(Tab_p->Window_p, Node_p);
    Nh_CSS_setDefaultBorderProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultPositionProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultMarginProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultPaddingProperties(Tab_p, Node_p);
    Nh_CSS_setDefaultListProperties(Node_p);

NH_SILENT_END()
}

static void Nh_CSS_configureProperty(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Properties.count; ++i) {
        if (Nh_CSS_getProperty(&Node_p->Properties, i)->type == type) {
            Nh_CSS_configureGenericProperty(
                Node_p, Nh_CSS_getProperty(&Node_p->Properties, i)
            );
        }
    }

    Nh_CSS_setDefaultTextProperty(Tab_p, Node_p, type);
    Nh_CSS_setDefaultBackgroundProperty(Tab_p->Window_p, Node_p, NULL, type);
    Nh_CSS_setDefaultBorderProperty(Tab_p, Node_p, type);
    Nh_CSS_setDefaultPositionProperty(Tab_p, Node_p, type);
    Nh_CSS_setDefaultMarginProperty(Tab_p, Node_p, type);
    Nh_CSS_setDefaultPaddingProperty(Tab_p, Node_p, type);
    Nh_CSS_setDefaultListProperty(Node_p, type);

NH_SILENT_END()
}

// COMPUTE =========================================================================================

NH_RESULT Nh_CSS_computeProperties(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL init)
{
NH_BEGIN()

    if (init) {Nh_CSS_initProperties(&Node_p->Computed.Properties);}

    NH_CSS_GenericProperty *Properties_pp[NH_CSS_PROPERTY_COUNT];
    for (int i = 0; i < NH_CSS_PROPERTY_COUNT; ++i) {Properties_pp[i] = NULL;}

    Nh_CSS_configureProperties(Tab_p, Node_p);
    Nh_CSS_getGenericProperties(Tab_p, Node_p, Properties_pp);

    // em length has text-font dependency, so we calculate text first
    Nh_CSS_computeTextProperties(Tab_p, Node_p, Properties_pp);
    
    Nh_CSS_computeBackgroundProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computeBorderProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computePositionProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computeMarginProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computePaddingProperties(Tab_p, Node_p, Properties_pp);
    Nh_CSS_computeListProperties(Node_p, Properties_pp);

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

