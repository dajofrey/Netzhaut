// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Image.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Memory.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <string.h>
#include <ctype.h>

// IMAGE LIST ======================================================================================

NH_RESULT Nh_CSS_addImage(
    Nh_List *Images_p, Nh_Gfx_Texture *Texture_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Images_p)

    NH_CSS_Image *Image_p = Nh_allocate(sizeof(NH_CSS_Image));
    NH_CHECK_MEM(Image_p)

    Image_p->buffer     = -1;
    Image_p->descriptor = -1;
    Image_p->uniform    = -1;
    Image_p->Texture_p  = Texture_p;

    Nh_CSS_setDefaultBackgroundProperty(NULL, NULL, Image_p, NH_CSS_PROPERTY_BACKGROUND_SIZE);
    Nh_CSS_setDefaultBackgroundProperty(NULL, NULL, Image_p, NH_CSS_PROPERTY_BACKGROUND_ORIGIN);
    Nh_CSS_setDefaultBackgroundProperty(NULL, NULL, Image_p, NH_CSS_PROPERTY_BACKGROUND_POSITION);

    Nh_addListItem(Images_p, Image_p);

NH_END(NH_SUCCESS)
}

NH_CSS_Image *Nh_CSS_getImage(
    Nh_List *Images_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(Images_p, index))
}

void Nh_CSS_destroyImages(
    Nh_List *Images_p)
{
NH_BEGIN()

    Nh_destroyList(Images_p, true);
    
NH_SILENT_END()
}

