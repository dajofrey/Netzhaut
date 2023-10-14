// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Binaries.h"
#include "Util.h"
#include "Main.h"

#include "../Common/Macro.h"

#include NH_MAKE_FLOW
#include NH_MAKE_CUSTOM_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// BUILD BINARY ====================================================================================

NH_MAKE_RESULT nh_make_buildnhterminal()
{
NH_MAKE_BEGIN()

    char projDir_p[2048] = {'\0'};
    NH_MAKE_CHECK(NH_MAKE_ERROR_GET_PROJECT_DIRECTORY, nh_make_getProjectDir(projDir_p, 2048))

    // set -no-pie because of https://stackoverflow.com/questions/41398444/gcc-creates-mime-type-application-x-sharedlib-instead-of-application-x-applicati
    static char command_p[2048] = {'\0'};
    sprintf(command_p, "gcc -std=gnu99 -Wl,-rpath=%s/lib:/usr/local/lib -o%s/bin/nhterminal -no-pie -L%s/lib -lnetzhaut %s/src/bin/nhterminal/Terminal.c", projDir_p, projDir_p, projDir_p, projDir_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_GCC_EXECUTION_FAILED)}

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_buildnhwebbrowser()
{
NH_MAKE_BEGIN()

    char projDir_p[2048] = {'\0'};
    NH_MAKE_CHECK(NH_MAKE_ERROR_GET_PROJECT_DIRECTORY, nh_make_getProjectDir(projDir_p, 2048))

    // set -no-pie because of https://stackoverflow.com/questions/41398444/gcc-creates-mime-type-application-x-sharedlib-instead-of-application-x-applicati
    static char command_p[2048] = {'\0'};
    sprintf(command_p, "gcc -std=gnu99 -Wl,-rpath=%s/lib:/usr/local/lib -o%s/bin/nhwebbrowser -no-pie -L%s/lib -lnetzhaut %s/src/bin/nhwebbrowser/WebBrowser.c", projDir_p, projDir_p, projDir_p, projDir_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_MAKE_DIAGNOSTIC_END(NH_MAKE_ERROR_GCC_EXECUTION_FAILED)}

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// INSTALL BINARY ==================================================================================

NH_MAKE_RESULT nh_make_installnhterminal()
{
NH_MAKE_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    NH_MAKE_CHECK_NULL(NH_MAKE_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    NH_MAKE_CHECK(NH_MAKE_ERROR_GET_PROJECT_DIRECTORY, nh_make_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = nh_make_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/applications/", homedir_p);
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("src/bin/nhterminal/Common/Data/nhterminal.desktop", dest_p, NH_MAKE_FALSE, NH_MAKE_FALSE))
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("bin/nhterminal", "/usr/local/bin", NH_MAKE_FALSE, NH_MAKE_TRUE))

    chdir(wrkDir_p);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

NH_MAKE_RESULT nh_make_installnhwebbrowser()
{
NH_MAKE_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    NH_MAKE_CHECK_NULL(NH_MAKE_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    NH_MAKE_CHECK(NH_MAKE_ERROR_GET_PROJECT_DIRECTORY, nh_make_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = nh_make_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/applications/", homedir_p);
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("src/bin/nhwebbrowser/Common/Data/nhwebbrowser.desktop", dest_p, NH_MAKE_FALSE, NH_MAKE_FALSE))
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("bin/nhwebbrowser", "/usr/local/bin", NH_MAKE_FALSE, NH_MAKE_TRUE))

    chdir(wrkDir_p);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

// INSTALL LOGO ====================================================================================

NH_MAKE_RESULT nh_make_installLogo()
{
NH_MAKE_BEGIN()

    char wrkDir_p[2048] = {'\0'};
    NH_MAKE_CHECK_NULL(NH_MAKE_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char projDir_p[2048] = {'\0'};
    NH_MAKE_CHECK(NH_MAKE_ERROR_GET_PROJECT_DIRECTORY, nh_make_getProjectDir(projDir_p, 2048))

    chdir(projDir_p);

    const char *homedir_p = nh_make_getHomeDir();
    char dest_p[512] = {'\0'};

    sprintf(dest_p, "%s/.local/share/icons/hicolor/32x32/apps", homedir_p);
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("docs/Logo/32x32/netzhaut.png", dest_p, NH_MAKE_FALSE, NH_MAKE_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/64x64/apps", homedir_p);
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("docs/Logo/64x64/netzhaut.png", dest_p, NH_MAKE_FALSE, NH_MAKE_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/128x128/apps", homedir_p);
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("docs/Logo/128x128/netzhaut.png", dest_p, NH_MAKE_FALSE, NH_MAKE_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/256x256/apps", homedir_p);
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("docs/Logo/256x256/netzhaut.png", dest_p, NH_MAKE_FALSE, NH_MAKE_TRUE))

    memset(dest_p, '\0', sizeof(char) * 512);
    sprintf(dest_p, "%s/.local/share/icons/hicolor/512x512/apps", homedir_p);
    NH_MAKE_CHECK(NH_MAKE_ERROR_COPY_FAILED, nh_make_copy("docs/Logo/512x512/netzhaut.png", dest_p, NH_MAKE_FALSE, NH_MAKE_TRUE))

    chdir(wrkDir_p);

NH_MAKE_DIAGNOSTIC_END(NH_MAKE_SUCCESS)
}

