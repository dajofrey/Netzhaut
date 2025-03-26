// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Config.h"

#include "../Config/Config.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

// FUNCTIONS =======================================================================================

static NH_API_RESULT nh_core_getSetting(
    nh_core_Config *Config_p, int option, char *option_p)
{
    nh_core_List *Setting_p = nh_core_getGlobalConfigSetting(NULL, NH_MODULE_CORE, option_p);

    NH_CORE_CHECK_NULL(Setting_p)

    switch (option) {
        case 0 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->loaderUnload = atoi(Setting_p->pp[0]) == 1;
            break;
        case 1 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->monitor_on = atoi(Setting_p->pp[0]) == 1;
            break;
        case 2 :
            if (Setting_p->size != 1) {return NH_API_ERROR_BAD_STATE;}
            Config_p->dump_on = atoi(Setting_p->pp[0]) == 1;
            break;
    }

    return NH_API_SUCCESS;
}

static nh_core_Config nh_core_getStaticConfig() 
{ 
    nh_core_Config Config; 
    memset(&Config, 0, sizeof(nh_core_Config)); 

    static const char *options_pp[] = {
        "loader.unload",
        "debug.monitor_on",
        "debug.dump_on",
    };

    int options = sizeof(options_pp)/sizeof(options_pp[0]);

    for (int i = 0; i < options; ++i) { 
        nh_core_getSetting(&Config, i, options_pp[i]); 
    } 

    return Config; 
} 

static int nh_core_consoleInRawMode() {
    struct termios term;
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        return -1;
    }
    if (!(term.c_lflag & ICANON) && !(term.c_lflag & ECHO) &&
        !(term.c_iflag & (IXON | ICRNL | INLCR | IGNCR)) &&
        !(term.c_oflag & OPOST)) {
        return 1;
    } else {
        return 0;
    }
}

nh_core_Config nh_core_getConfig()
{
    nh_core_Config Config = nh_core_getStaticConfig();

    if (nh_core_consoleInRawMode() == 1) {
        Config.dump_on = false;
    }

    return Config;
}
