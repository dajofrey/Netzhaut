// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Config.h"
#include "../Header/Macros.h"
#include "../Header/Memory.h"
#include "../Header/HashMap.h"
#include "../Header/Output.h"

#include "../../JavaScript/Header/Parser.h"
#include "../../CSS/Header/Sheet.h"
#include "../../HTML/Header/Document.h"
#include "../../TTY/Header/Terminal.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

// DATA ============================================================================================

Nh_Config Config;

// DECLARE =========================================================================================

static NH_RESULT Nh_initSettings(
);
static void Nh_initInternalFlags(
);

// CREATE AND DESTROY ==============================================================================

NH_RESULT Nh_initConfig()
{
NH_BEGIN()

    NH_INIT_LIST(Config.InternalURNSyntax)
    NH_CHECK(Nh_createInternalURNSyntax())

    NH_CHECK(Nh_initSettings())
    Nh_initInternalFlags();
    Nh_initLogFlags();

NH_END(NH_SUCCESS)
}

void Nh_freeConfig()
{
NH_BEGIN()

    Nh_destroyList(&Config.InternalURNSyntax, true);
    Nh_free(Config.Settings.internalURNSchemeName_p);
    Nh_free(Config.Settings.startpage_p);

NH_SILENT_END()
}

// GET =============================================================================================

Nh_Config *Nh_getConfig()
{
NH_BEGIN()
NH_END(&Config)
}

// HELPER ==========================================================================================

void Nh_initLogFlags()
{
NH_BEGIN()

    Config.Flags.Log.init          = false; 
    Config.Flags.Log.mouse         = false; 
    Config.Flags.Log.vulkan        = false; 
    Config.Flags.Log.script        = false; 
    Config.Flags.Log.listener      = false; 
    Config.Flags.Log.event         = false; 
    Config.Flags.Log.jsinterpreter = false; 
    Config.Flags.Log.jsparser      = false; 
    Config.Flags.Log.sheet         = false; 
    Config.Flags.Log.cssparser     = false; 
    Config.Flags.Log.document      = false; 
    Config.Flags.Log.htmlparser    = false; 
    Config.Flags.Log.receive       = NH_FALSE; 
    Config.Flags.Log.send          = NH_FALSE; 
    Config.Flags.Log.margin        = NH_FALSE;

NH_SILENT_END()
}

static void Nh_initInternalFlags()
{
NH_BEGIN()

    Config.Flags.Internal.shutdown = false;

NH_SILENT_END()
}

static NH_RESULT Nh_initSettings()
{
NH_BEGIN()

    Config.Settings.gamma = 2.2f;
    Config.Settings.inputBufferSize = 255;
    Config.Settings.internalURNSchemeName_p = Nh_allocateChars("nh");

NH_END(NH_SUCCESS)
}

