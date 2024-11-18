// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Depth.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// FUNCTIONS =======================================================================================

static float nh_css_getDepth(
    nh_css_Fragment *Fragment_p)
{
    float depth = 0.9999f;

    Fragment_p = Fragment_p->Parent_p;
    while (Fragment_p) {
        Fragment_p = Fragment_p->Parent_p;
        depth -= 0.0001f;
    }

    return depth;
}

void nh_css_getFragmentDepth(
    nh_css_Fragment *Fragment_p)
{
    Fragment_p->ContentBox.depth = nh_css_getDepth(Fragment_p);

    for (int i = 0; i < Fragment_p->Children.size; ++i) {
        nh_css_getFragmentDepth(Fragment_p->Children.pp[i]);
    }

    return;
}
