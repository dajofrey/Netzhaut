// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Document.h"
#include "../Header/Parser.h"
#include "../Header/Text.h"
#include "../Header/Macros.h"
#include "../Header/Log.h"

#include "../Header/TestSuite/Index.h"
#include "../Header/TestSuite/Background.h"
#include "../Header/TestSuite/Border.h"
#include "../Header/TestSuite/Canvas.h"
#include "../Header/TestSuite/Image.h"
#include "../Header/TestSuite/Input.h"
#include "../Header/TestSuite/Selector.h"
#include "../Header/TestSuite/Text.h"
#include "../Header/TestSuite/List.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/List.h"
#include "../../Core/Header/String.h"
#include "../../Core/Header/HashMap.h"

#include "../../CSS/Header/Input.h"
#include "../../CSS/Header/Associate.h"
#include "../../CSS/Header/Log.h"
#include "../../CSS/Header/Properties.h"
#include "../../CSS/Header/Arrange.h"

#include "../../JavaScript/Header/Input.h"
#include "../../JavaScript/Header/Debug.h"
#include "../../JavaScript/Header/Loader.h"

#include "../../Graphics/Header/Text.h"
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

// INIT ============================================================================================

NH_RESULT Nh_HTML_initDocument(
    char *location_p, Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    Document_p->URI = Nh_createURI(location_p, NULL, NULL);
    Document_p->Tree.Root_p = NULL;

    NH_INIT_LIST(Document_p->Tree.Flat.Unformatted)
    NH_INIT_LIST(Document_p->Tree.Flat.Formatted)

    NH_INIT_LIST(Document_p->Sheets)
    NH_INIT_LIST(Document_p->Scripts)

    NH_CHECK(Nh_HTML_initInput(&Document_p->Input, 1024))

NH_END(NH_SUCCESS)
}

// CREATE ===========================================================================================

NH_RESULT Nh_HTML_createDocument(
    char *location_p, Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_initDocument(location_p, &Tab_p->Document))

    NH_CHECK(Nh_HTML_parseDocument(&Tab_p->Document))
    NH_CHECK(Nh_HTML_recreateFlatTree(&Tab_p->Document.Tree, Tab_p->Document.Tree.Root_p, NH_TRUE))

    NH_CHECK(Nh_CSS_addSheets(&Tab_p->Document, true))
    for (int i = 0; i < Tab_p->Document.Sheets.count; ++i) {
        NH_CHECK(Nh_CSS_parseSheet(Nh_CSS_getSheet(&Tab_p->Document.Sheets, i)))
        Nh_CSS_logSheet(Nh_CSS_getSheet(&Tab_p->Document.Sheets, i));
    }

    NH_CHECK(Nh_JS_addScripts(&Tab_p->Document, true))
    for (int i = 0; i < Tab_p->Document.Scripts.count; ++i) {
        NH_CHECK(Nh_JS_parseScript(Nh_JS_getScript(&Tab_p->Document.Scripts, i)))
        Nh_JS_debugScript(Nh_JS_getScript(&Tab_p->Document.Scripts, i));
    }

NH_END(NH_SUCCESS)
}

// COMPUTE =========================================================================================

NH_RESULT Nh_HTML_computeDocument(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    for (int i = 0; i < Tab_p->Document.Scripts.count; ++i) {
        NH_CHECK(Nh_JS_loadScript(Tab_p, Nh_JS_getScript(&Tab_p->Document.Scripts, i)))
    }

    NH_CHECK(Nh_CSS_associateSheets(Tab_p, NULL))

    for (int i = 0; i < Tab_p->Document.Tree.Flat.Unformatted.count; ++i) {
        NH_CHECK(Nh_HTML_computeNode(
            Tab_p, Nh_getListItem(&Tab_p->Document.Tree.Flat.Unformatted, i), NH_TRUE
        ))
    }

    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_TRUE))
    NH_CHECK(Nh_HTML_computeFormattedTree(Tab_p))

    for (int i = 0; i < Tab_p->Document.Scripts.count; ++i) {
        NH_CHECK(Nh_JS_interpretScript(Nh_JS_getScript(&Tab_p->Document.Scripts, i)))
    }

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

NH_RESULT Nh_HTML_destroyDocument(
    Nh_HTML_Document *Document_p, Nh_GPU *GPU_p, NH_BOOL resize)
{   
NH_BEGIN()

printf("destroy doc\n");
exit(0);
//    NH_CHECK(Nh_HTML_destroyFormattedTree(Document_p, GPU_p))
//
//    if (!resize)
//    {
//        Nh_JS_destroyScriptList(&Document_p->Scripts);
//        Nh_CSS_destroySheetList(&Document_p->Sheets);
//        Nh_HTML_destroyNode(Document_p->Tree.Root_p, NH_TRUE);
//        Nh_HTML_freeInput(&Document_p->Input);
//
//        Nh_destroyList(&Document_p->Tree.Flat, false);
//    }

NH_END(NH_SUCCESS)
}

// UPDATE ==========================================================================================

NH_RESULT Nh_HTML_updateDocument(
    Nh_Tab *Tab_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_processInput(Tab_p))
    NH_CHECK(Nh_CSS_processInput(Tab_p))
    NH_CHECK(Nh_JS_processInput(Tab_p))

NH_END(NH_SUCCESS)
}

// GET =============================================================================================

char *Nh_HTML_getDefaultTestDocument(
    NH_INTERNAL_FILE file)
{
NH_BEGIN()

    switch (file)
    {
        case NH_INTERNAL_FILE_BACKGROUND : NH_END(BackgroundHTML) break; 
        case NH_INTERNAL_FILE_BORDER     : NH_END(BorderHTML) break;
        case NH_INTERNAL_FILE_CANVAS     : NH_END(CanvasHTML) break;
        case NH_INTERNAL_FILE_IMAGE      : NH_END(ImageHTML) break;
        case NH_INTERNAL_FILE_INDEX      : NH_END(IndexHTML) break; 
        case NH_INTERNAL_FILE_INPUT      : NH_END(InputHTML) break;
        case NH_INTERNAL_FILE_SELECTOR   : NH_END(SelectorHTML) break;
        case NH_INTERNAL_FILE_TEXT       : NH_END(TextHTML) break;
        case NH_INTERNAL_FILE_LIST       : NH_END(ListHTML) break;
    }

NH_END(NULL)
}

