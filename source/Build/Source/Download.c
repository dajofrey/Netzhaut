// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Download.h"
#include "../Header/Macros.h"
#include "../Header/Utils.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DATA ============================================================================================

static NH_RESULT Nh_Bld_downloadFromGithub(
    const char *owner_p, const char *repo_p)
{
NH_BEGIN()

    if (!Nh_Bld_canRunCommand("wget")) {NH_END(NH_BUILD_ERROR_WGET_NOT_FOUND)}
    if (!Nh_Bld_canRunCommand("unzip")) {NH_END(NH_BUILD_ERROR_UNZIP_NOT_FOUND)}

#include NH_CUSTOM_CHECK

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    char zip_p[2048] = {'\0'};
    sprintf(zip_p, "%s%s", topDir_p, "/data/build/tmp.zip");

    char command_p[2048] = {'\0'};
    sprintf(command_p, "wget -O %s https://github.com/%s/%s/archive/master.zip", zip_p, owner_p, repo_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_WGET_EXECUTION_FAILED)}

    memset(command_p, '\0', sizeof(char) * 2048);
    sprintf(command_p, "unzip -o %s -d %s/external/downloads/", zip_p, topDir_p);

    status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_UNZIP_EXECUTION_FAILED)}

    remove(zip_p);

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

// https://github.com/KhronosGroup/glslang.git
// https://github.com/g-truc/gli.git
// https://github.com/KhronosGroup/Vulkan-Headers.git
// https://github.com/zeux/volk.git
// https://git.sv.nongnu.org/r/freetype/freetype2.git

NH_RESULT Nh_Bld_download()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    if (!Nh_Bld_canFindSharedLib("freetype")) {
        NH_CHECK(NH_BUILD_ERROR_CANT_DOWNLOAD_FREETYPE, NH_BUILD_ERROR_NOT_IMPLEMENTED)
    }
    if (!Nh_Bld_canFindSharedLib("ssl")) { // part of OpenSSL
        NH_CHECK(NH_BUILD_ERROR_CANT_DOWNLOAD_OPENSSL, NH_BUILD_ERROR_NOT_IMPLEMENTED)
    }

    NH_CHECK(NH_BUILD_ERROR_CANT_DOWNLOAD_VULKAN_HEADERS, Nh_Bld_downloadFromGithub("KhronosGroup", "Vulkan-Headers"))
    NH_CHECK(NH_BUILD_ERROR_CANT_DOWNLOAD_VOLK, Nh_Bld_downloadFromGithub("zeux", "volk"))

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

