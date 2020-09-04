#ifndef NH_FONT_H
#define NH_FONT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "URI.h"
#include "External/freetype.h"

#include "../../API/Header/Netzhaut.h"

#endif

/** @addtogroup CoreStructs Structs
 *  \ingroup Core
 *  @{
 */

    typedef struct Nh_Font {
        Nh_URI URI;
        char *familyName_p;
        char *styleName_p;
    } Nh_Font;

/** @} */

/** @addtogroup CoreFunctions Functions
 *  \ingroup Core
 *  @{
 */

    NH_RESULT Nh_initFontManager(
    );

    void Nh_destroyFontManager(
    );

    NH_RESULT Nh_addInitialFonts(
    );
    
    NH_RESULT Nh_addInitialFontPrototypes(
    );

    NH_RESULT Nh_addFontDirectory(
        char *dirPath_p
    );
    
    NH_RESULT Nh_addFont(
        char *absolutePath_p
    );
    
    NH_RESULT Nh_addFontPrototype(
        char *familyName_p, char *styleName_p
    );
    
    Nh_Font *Nh_getFontFromPrototype(
        Nh_Font *Prototype_p
    );
    
    Nh_Font *Nh_getFontFromPath(
        char *path_p
    );
    
    Nh_Font *Nh_getFontFromFamilyName(
        char *familyName_p
    );
    
    Nh_Font *Nh_getFontFromStyleName(
        char *styleName_p
    );
    
    Nh_Font *Nh_getDefaultFont(
    );
    
    NH_RESULT Nh_initExternalFontLib(
    );
    
    NH_RESULT Nh_closeExternalFontLib(
    );

/** @} */

#endif 
