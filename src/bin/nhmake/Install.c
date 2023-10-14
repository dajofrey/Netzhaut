// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under MIT.
 */

// INCLUDE =========================================================================================

#include "Build.h"
#include "Util.h"
#include "Unicode.h"
#include "Main.h"
#include "WebIDL.h"

#include "../UI/Message.h"
#include "../Common/Macro.h"

#include NH_MAKE_FLOW
#include NH_MAKE_CUSTOM_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>

// INSTALL =========================================================================================

SM_RESULT sm_installnhterminal()
{
SM_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    SM_CHECK_NULL(SM_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    SM_CHECK(SM_ERROR_GET_PROJECT_DIRECTORY, sm_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = sm_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/applications/", homedir_p);
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("src/bin/nhterminal/Common/Data/nhterminal.desktop", dest_p, SM_FALSE, SM_FALSE))
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("bin/nhterminal", "/usr/local/bin", SM_FALSE, SM_TRUE))

    chdir(wrkDir_p);

SM_DIAGNOSTIC_END(SM_SUCCESS)
}

SM_RESULT sm_installnhwebbrowser()
{
SM_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    SM_CHECK_NULL(SM_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    SM_CHECK(SM_ERROR_GET_PROJECT_DIRECTORY, sm_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = sm_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/applications/", homedir_p);
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("src/bin/nhwebbrowser/Common/Data/nhwebbrowser.desktop", dest_p, SM_FALSE, SM_FALSE))
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("bin/nhwebbrowser", "/usr/local/bin", SM_FALSE, SM_TRUE))

    chdir(wrkDir_p);

SM_DIAGNOSTIC_END(SM_SUCCESS)
}

// INSTALL LOGO ====================================================================================

SM_RESULT sm_installLogo()
{
SM_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    SM_CHECK_NULL(SM_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    SM_CHECK(SM_ERROR_GET_PROJECT_DIRECTORY, sm_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = sm_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/icons/hicolor/32x32/apps", homedir_p);
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("docs/Logo/32x32/netzhaut.png", dest_p, SM_FALSE, SM_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/64x64/apps", homedir_p);
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("docs/Logo/64x64/netzhaut.png", dest_p, SM_FALSE, SM_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/128x128/apps", homedir_p);
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("docs/Logo/128x128/netzhaut.png", dest_p, SM_FALSE, SM_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/256x256/apps", homedir_p);
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("docs/Logo/256x256/netzhaut.png", dest_p, SM_FALSE, SM_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/512x512/apps", homedir_p);
    SM_CHECK(SM_ERROR_COPY_FAILED, sm_copy("docs/Logo/512x512/netzhaut.png", dest_p, SM_FALSE, SM_TRUE))

    chdir(wrkDir_p);

SM_DIAGNOSTIC_END(SM_SUCCESS)
}

