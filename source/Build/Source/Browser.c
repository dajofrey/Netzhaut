// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Browser.h"
#include "../Header/Macros.h"
#include "../Header/Utils.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// BUILD ===========================================================================================

NH_RESULT Nh_Bld_buildBrowser()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    static char command_p[2048] = {'\0'};
    sprintf(command_p, "gcc -std=gnu99 -Wl,-rpath=/usr/local/lib:%s/bin -o%s/bin/netzhaut_browser -lNetzhaut %s/source/Examples/Browser.c", topDir_p, topDir_p, topDir_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_GCC_EXECUTION_FAILED)}

NH_END(NH_SUCCESS)
}

// INSTALL =========================================================================================

NH_RESULT Nh_Bld_installBrowser()
{
NH_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    chdir(topDir_p);

    const char *homedir_p = Nh_Bld_getHomeDirectory();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/applications/", homedir_p);
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("data/examples/browser/netzhaut.desktop", dest_p, NH_FALSE, NH_TRUE, NH_FALSE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/32x32/apps", homedir_p);
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("data/examples/browser/icons/32x32/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/64x64/apps", homedir_p);
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("data/examples/browser/icons/64x64/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/128x128/apps", homedir_p);
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("data/examples/browser/icons/128x128/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/256x256/apps", homedir_p);
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("data/examples/browser/icons/256x256/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/512x512/apps", homedir_p);
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("data/examples/browser/icons/512x512/netzhaut.png", dest_p, NH_FALSE, NH_TRUE, NH_TRUE))

    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("bin/netzhaut_browser", "/usr/local/bin", NH_FALSE, NH_TRUE, NH_TRUE))

    chdir(wrkDir_p);

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

