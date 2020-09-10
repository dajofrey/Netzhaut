// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Font.h"
#include "../Header/File.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"
#include "../Header/Config.h"
#include "../Header/String.h"
#include "../Header/External/freetype.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <string.h>
#include <stdio.h>

#ifdef __unix__ 	
    #include <dirent.h>
#elif defined(_WIN32) || defined(WIN32)
#endif

// DECLARE =========================================================================================

typedef struct Nh_FontManager {
    Nh_Font *Default_p;
    FT_Library Library;
    Nh_List Fonts;
    Nh_List Prototypes;
} Nh_FontManager;

// DATA ============================================================================================

Nh_FontManager FontManager;

// CREATE ==========================================================================================

NH_RESULT Nh_initFontManager()
{
NH_BEGIN()

    FontManager.Default_p = NULL;
    NH_INIT_LIST(FontManager.Fonts)
    NH_INIT_LIST(FontManager.Prototypes)

NH_END(NH_SUCCESS)
}

void Nh_destroyFontManager()
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.count; ++i) {
        Nh_Font *Font_p = Nh_getListItem(&FontManager.Fonts, i);
        Nh_freeURI(Font_p->URI);
        Nh_free(Font_p->familyName_p);
        Nh_free(Font_p->styleName_p);
    }

    for (int i = 0; i < FontManager.Prototypes.count; ++i) {
        Nh_Font *Prototype_p = Nh_getListItem(&FontManager.Prototypes, i);
        Nh_free(Prototype_p->familyName_p);
        Nh_free(Prototype_p->styleName_p);
    }

    Nh_destroyList(&FontManager.Fonts, true);
    Nh_destroyList(&FontManager.Prototypes, true);

NH_SILENT_END()
}

// ADD =============================================================================================

NH_RESULT Nh_addInitialFonts()
{
NH_BEGIN()

    Nh_initExternalFontLib();

#ifdef __unix__
    NH_CHECK(
        Nh_addFontDirectory("/usr/share/fonts/"),
        Nh_addFontDirectory("/usr/local/share/fonts/"),
        Nh_addFontDirectory("~/.fonts/")
    )
#elif defined(_WIN32) || defined (WIN32)
    exit(0);
#endif

    Nh_closeExternalFontLib();

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_addInitialFontPrototypes()
{
NH_BEGIN()

    NH_CHECK(
        Nh_addFontPrototype("Nimbus Roman", "Regular"),
        Nh_addFontPrototype("Arial", "Regular"),          
        Nh_addFontPrototype("Verdana", "Regular"),        
        Nh_addFontPrototype("Cantarell", "Regular"),      
        Nh_addFontPrototype("Georgia", "Regular"),        
        Nh_addFontPrototype("Droid Sans", "Regular"),     
        Nh_addFontPrototype("Noto Sans Mono", "Regular")
    ) 

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_addFontDirectory(
    char *dirPath_p)
{
NH_BEGIN()

#ifdef __unix__ 

    struct dirent *entry_p = NULL;
    char fullPath_p[1024] = {'\0'};

    DIR *dir_p = opendir(dirPath_p); 
    if (dir_p == NULL) {NH_END(NH_SUCCESS)}
  
    while ((entry_p = readdir(dir_p)) != NULL) 
    {
        if (dirPath_p[strlen(dirPath_p) - 1] != '/') {
            sprintf(fullPath_p, "%s/%s", dirPath_p, entry_p->d_name);
        } else {
            sprintf(fullPath_p, "%s%s", dirPath_p, entry_p->d_name);
        }

        NH_RESULT result = Nh_addFont(fullPath_p);
        if (result != NH_SUCCESS && result != NH_ERROR_INVALID_FILE_FORMAT) {closedir(dir_p); NH_END(result)}

        if (strcmp(entry_p->d_name, "..") != 0 && strcmp(entry_p->d_name, ".") != 0) {
            NH_CHECK(Nh_addFontDirectory(fullPath_p))
        }
    }
  
    closedir(dir_p);    

#elif defined(_WIN32) || defined(WIN32)

    printf("windows not supported\n");
    exit(0);

#endif

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_addFont(
    char *absolutePath_p)
{
NH_BEGIN()

    if (!Nh_isRegularFile(absolutePath_p)) {NH_END(NH_ERROR_INVALID_FILE_FORMAT)}

    Nh_URI URI = Nh_createURI(absolutePath_p, NULL, NULL, NULL, -1);
    long size = 0;
    char *data_p = Nh_getData(URI, &size);

    if (data_p == NULL) {
        Nh_freeURI(URI);
        return NH_ERROR_INVALID_FILE_FORMAT;
    } 

    FT_Face Face; 
    int error = FT_New_Memory_Face(FontManager.Library, data_p, size, 0, &Face);
    if (error == FT_Err_Unknown_File_Format) {NH_END(NH_ERROR_INVALID_FILE_FORMAT)}
    else if (error) {NH_END(NH_SUCCESS)}

    Nh_Font *Font_p = Nh_allocate(sizeof(Nh_Font));
    NH_CHECK_MEM(Font_p)

    Font_p->URI = URI;
    Font_p->familyName_p = Nh_allocateChars(Face->family_name);
    NH_CHECK_MEM(Font_p->familyName_p)
    Font_p->styleName_p = Nh_allocateChars(Face->style_name);
    NH_CHECK_MEM(Font_p->styleName_p)

    NH_CHECK(Nh_addListItem(&FontManager.Fonts, Font_p))

    FT_Done_Face(Face);
    Nh_freeData(URI);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_addFontPrototype(
    char *familyName_p, char *styleName_p)
{
NH_BEGIN()

    Nh_Font *Font_p = Nh_allocate(sizeof(Nh_Font));
    NH_CHECK_MEM(Font_p)

    Font_p->familyName_p = NULL;
    Font_p->styleName_p  = NULL;

    if (familyName_p != NULL) 
    {
        Font_p->familyName_p = Nh_allocate(sizeof(char) * (strlen(familyName_p) + 1));
        NH_CHECK_MEM(Font_p->familyName_p)
        strcpy(Font_p->familyName_p, familyName_p);
    }

    if (styleName_p != NULL)
    {
        Font_p->styleName_p = Nh_allocate(sizeof(char) * (strlen(styleName_p) + 1));
        NH_CHECK_MEM(Font_p->styleName_p)
        strcpy(Font_p->styleName_p, styleName_p);
    }

    NH_CHECK(Nh_addListItem(&FontManager.Prototypes, Font_p))

NH_END(NH_SUCCESS)
}

// GET =============================================================================================

Nh_Font *Nh_getFontFromPrototype(
    Nh_Font *Prototype_p)
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.count; ++i) 
    {
        Nh_Font *Font_p = Nh_getListItem(&FontManager.Fonts, i);
       
        if (!strcmp(Font_p->familyName_p, Prototype_p->familyName_p)
        &&  !strcmp(Font_p->styleName_p,  Prototype_p->styleName_p ))
        {
            NH_END(Font_p)
        }
    }

NH_END(NULL)
}

Nh_Font *Nh_getFontFromFamilyName(
    char *familyName_p)
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.count; ++i) 
    {
        Nh_Font *Font_p = Nh_getListItem(&FontManager.Fonts, i);
        if (!strcmp(Font_p->familyName_p, familyName_p)) {NH_END(Font_p)}
    }

NH_END(NULL)
}

Nh_Font *Nh_getFontFromStyleName(
    char *styleName_p)
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.count; ++i) 
    {
        Nh_Font *Font_p = Nh_getListItem(&FontManager.Fonts, i);
        if (!strcmp(Font_p->styleName_p, styleName_p)) {NH_END(Font_p)}
    }

NH_END(NULL)
}

Nh_Font *Nh_getFontFromURI(
    Nh_URI URI)
{
NH_BEGIN()

    for (int i = 0; i < FontManager.Fonts.count; ++i) 
    {
        Nh_Font *Font_p = Nh_getListItem(&FontManager.Fonts, i);
        if (Nh_equivalentURI(URI, Font_p->URI)) {NH_END(Font_p)}
    }

NH_END(NULL)
}

Nh_Font *Nh_getDefaultFont()
{
NH_BEGIN()

    if (FontManager.Default_p == NULL)
    {
        for (int i = 0; i < FontManager.Prototypes.count; ++i) 
        {
            Nh_Font *Prototype_p = Nh_getListItem(&FontManager.Prototypes, i);
            Nh_Font *Font_p = Nh_getFontFromPrototype(Prototype_p); 
            if (Font_p != NULL) {
                FontManager.Default_p = Font_p;
                NH_END(Font_p)
            }
        }
    }
    else {NH_END(FontManager.Default_p)}

printf("crab\n");
exit(0);

NH_END(NULL)
}

// EXTERNAL ========================================================================================

NH_RESULT Nh_initExternalFontLib()
{
NH_BEGIN()

    int error = FT_Init_FreeType(&FontManager.Library);
    if (error) {NH_END(NH_ERROR_BAD_STATE)}

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_closeExternalFontLib()
{
NH_BEGIN()

    FT_Done_FreeType(FontManager.Library);

NH_END(NH_SUCCESS)
}

