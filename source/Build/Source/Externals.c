// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Externals.h"
#include "../Header/Macros.h"
#include "../Header/Utils.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_Bld_createStaticLibraryUsingAr(
    const char *objects_p, const char *out_p
);

// EXTERNALS =======================================================================================

static NH_RESULT Nh_Bld_buildXXD(
    char *wrkDir_p, char *topDir_p)
{
NH_BEGIN()

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", topDir_p, "/external/xxd");
    chdir(path_p);

    int status = system("gcc -o ../bin/xxd xxd.c");
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_GCC_EXECUTION_FAILED)}

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Bld_buildHelperLibrary(
    char *wrkDir_p, char *topDir_p)
{
NH_BEGIN()

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", topDir_p, "/external");
    chdir(path_p);

#include NH_CUSTOM_CHECK

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_EXTERNAL_BIN_OBJECT_DIRECTORY, Nh_Bld_createDirectory("bin/object"))

#ifdef __unix__

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("freetype-gl/utf8-utils.c", "bin/object/utf8-utils.o", NULL))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("freetype-gl/vector.c", "bin/object/vector.o", NULL))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("freetype-gl/texture-atlas.c", "bin/object/texture-atlas.o", NULL))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("freetype-gl/texture-font.c", "bin/object/texture-font.o", "-I/usr/include/freetype2"))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("freetype-gl/distance-field.c", "bin/object/distance-field.o", NULL))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("freetype-gl/edtaa3func.c", "bin/object/edtaa3func.o", NULL))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("freetype-gl/platform.c", "bin/object/platform.o", NULL))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("C-Thread-Pool/thpool.c", "bin/object/thpool.o", NULL))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("c_hashmap/hashmap.c", "bin/object/hashmap.o", NULL))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC("downloads/volk-master/volk.c", "bin/object/volk.o", "-DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface"))

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_STATIC_LIB, Nh_Bld_createStaticLibraryUsingAr(
        "                               \
            bin/object/utf8-utils.o     \
            bin/object/vector.o         \
            bin/object/texture-font.o   \
            bin/object/texture-atlas.o  \
            bin/object/distance-field.o \
            bin/object/edtaa3func.o     \
            bin/object/platform.o       \
            bin/object/thpool.o         \
            bin/object/hashmap.o        \
            bin/object/volk.o           \
        ",
        "bin/libNetzhautHelper.a"
    ))

#elif defined(WIN_32)

#endif

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

// BUILD ===========================================================================================

NH_RESULT Nh_Bld_buildExternals()
{
NH_BEGIN()

    if (!Nh_Bld_canRunCommand("gcc")) {NH_END(NH_BUILD_ERROR_GCC_NOT_FOUND)}
    if (!Nh_Bld_canRunCommand("ar")) {NH_END(NH_BUILD_ERROR_AR_NOT_FOUND)}

#include NH_CUSTOM_CHECK

    char wrkDir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", topDir_p, "/external");

    chdir(path_p);

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_EXTERNAL_BIN_DIRECTORY, Nh_Bld_createDirectory("bin"))

    NH_CHECK(NH_BUILD_ERROR_BUILD_XXD_FAILED, Nh_Bld_buildXXD(wrkDir_p, topDir_p))
    NH_CHECK(NH_BUILD_ERROR_BUILD_HELPER_LIBRARY_FAILED, Nh_Bld_buildHelperLibrary(wrkDir_p, topDir_p))

#include NH_DEFAULT_CHECK

    chdir(wrkDir_p);

NH_END(NH_SUCCESS)
}

// COPY ============================================================================================

NH_RESULT Nh_Bld_copyExternalHeader()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    char wrkDir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    char external_p[1024] = {'\0'};
    sprintf(external_p, "%s/source/API/Header/External", topDir_p);

    chdir(topDir_p);

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_EXTERNAL_DIRECTORY, Nh_Bld_createDirectory(external_p))

    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("external/downloads/volk-master/volk.h", "source/API/Header/External", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("external/downloads/Vulkan-Headers-master/include/vulkan/*", "source/API/Header/External", NH_FALSE, NH_FALSE, NH_FALSE))

    chdir(wrkDir_p);

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

// INSTALL =========================================================================================

NH_RESULT Nh_Bld_installHelperLibrary()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    char wrkDir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    chdir(topDir_p);

    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("external/bin/libNetzhautHelper.a", "/usr/local/lib", NH_FALSE, NH_TRUE, NH_TRUE))

    chdir(wrkDir_p);

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Bld_createStaticLibraryUsingAr(
    const char *objects_p, const char *out_p)
{
NH_BEGIN()

    char command_p[1024] = {'\0'};
    sprintf(command_p, "ar rcs %s %s", out_p, objects_p);
    
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_AR_EXECUTION_FAILED)}

NH_END(NH_SUCCESS)
}

