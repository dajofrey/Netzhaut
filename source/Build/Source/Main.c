// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Docs.h"
#include "../Header/Macros.h"
#include "../Header/Utils.h"
#include "../Header/Download.h" 
#include "../Header/Externals.h"
#include "../Header/Library.h"
#include "../Header/Browser.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <string.h>

// DATA ============================================================================================

NH_BOOL BUILD_NOTHING  = NH_FALSE;
NH_BOOL GENERATE_DOCS  = NH_FALSE;
NH_BOOL BUILD_BROWSER  = NH_FALSE;
NH_BOOL INSTALL        = NH_FALSE;
NH_BOOL DEBUG_MODE     = NH_FALSE;
NH_BOOL VULKAN_SHADERS = NH_FALSE;
NH_BOOL QUIET          = NH_FALSE;

static const char help_p[] = "\
By default (no arguments) Netzhaut Meta Build System will build Netzhaut as a shared library.\n\
This behavior can be expanded by using the following commands:\n\
\n\
--docs    Generate documentation.\n\
--browser Build browser executable.\n\
--install Install shared library. Install browser if --browser was specified.\n\
--debug   Activate better debug capabilies.\n\
--vulkan  Compile Vulkan shaders.\n\
--quite   Suppress all output to the standard output channel.\n\
\n\
The following commands suppress the building process:\n\
\n\
-v | --version | version Print project version and other info.\n\
-h | --help    | help    Print this help text.\n\
";

// TTY =============================================================================================

const char *Nh_Bld_getHelp()
{
NH_BEGIN()
NH_END(help_p)
}

static void Nh_Bld_printHelp()
{
NH_BEGIN()

    printf("\n%s\n", help_p);

NH_SILENT_END()
}

static void Nh_Bld_printVersion()
{
NH_BEGIN()

    puts("");
    puts("Name       : Netzhaut Meta Build System");

#ifdef __unix__
    puts("Target     : Linux");
#elif defined(_WIN32)
    puts("Target     : Windows");
#endif

    if (!strcmp(NH_PRE_RELEASE_VERSION, "none")) {printf("Revision   : %s\n", NH_VERSION);}
    else {printf("Revision   : %s-%s\n", NH_VERSION, NH_PRE_RELEASE_VERSION);}

    printf("Build Time : %s %s\n", __DATE__, __TIME__);

    switch (__STDC_VERSION__)
    {
        case 199409L : puts("C Version  : C95 with GNU extensions"); break;
        case 199901L : puts("C Version  : C99 with GNU extensions"); break;
        case 201112L : puts("C Version  : C11 with GNU extensions"); break;
        case 201710L : puts("C Version  : C18 with GNU extensions"); break;

        default : puts("Standard   : unknown"); break;
    }

    puts("");

NH_SILENT_END()
}

static void Nh_Bld_handleTTY(
    int argc, char **argv_pp)
{
NH_BEGIN()

    for (int i = 1; i < argc; ++i) 
    {
        if (!strcmp(argv_pp[i], "-v") || !strcmp(argv_pp[i], "--version") || !strcmp(argv_pp[i], "version")) {
            Nh_Bld_printVersion();
            BUILD_NOTHING = NH_TRUE;
        }
        if (!strcmp(argv_pp[i], "-h") || !strcmp(argv_pp[i], "--help") || !strcmp(argv_pp[i], "help")) {
            Nh_Bld_printHelp();
            BUILD_NOTHING = NH_TRUE;
        }
        if (!strcmp(argv_pp[i], "--docs")) {GENERATE_DOCS = NH_TRUE;}
        if (!strcmp(argv_pp[i], "--browser")) {BUILD_BROWSER = NH_TRUE;}
        if (!strcmp(argv_pp[i], "--install")) {INSTALL = NH_TRUE;}
        if (!strcmp(argv_pp[i], "--debug")) {DEBUG_MODE = NH_TRUE;}
        if (!strcmp(argv_pp[i], "--vulkan")) {VULKAN_SHADERS = NH_TRUE;}
    }

NH_SILENT_END()
}

// MAIN ============================================================================================

static NH_RESULT Nh_Bld_main(
    int argc, char **argv_pp)
{
NH_BEGIN()

    Nh_Bld_handleTTY(argc, argv_pp);

    if (BUILD_NOTHING) {NH_END(NH_SUCCESS)}

#include NH_CUSTOM_CHECK

    if (GENERATE_DOCS) {
        puts("\nNETZHAUT: \e[1;32mGENERATE DOCS\e[0m\n");
        NH_CHECK(NH_BUILD_ERROR_GENERATE_DOCUMENTS_FAILURE, Nh_Bld_generateDocs())
    }

    puts("\nNETZHAUT: \e[1;32mDOWNLOAD\e[0m\n");
    NH_CHECK(NH_BUILD_ERROR_DOWNLOAD_FAILED, Nh_Bld_download())

    puts("\nNETZHAUT: \e[1;32mBUILD EXTERNALS\e[0m\n");
    NH_CHECK(NH_BUILD_ERROR_BUILD_EXTERNALS_FAILED, Nh_Bld_buildExternals())

    if (INSTALL) {
        puts("\nNETZHAUT: \e[1;32mINSTALL HELPER LIBRARY\e[0m\n");
        NH_CHECK(NH_BUILD_ERROR_INSTALL_HELPER_LIBRARY_FAILED, Nh_Bld_installHelperLibrary())
    }

    puts("\nNETZHAUT: \e[1;32mCOPY EXTERNAL HEADER\e[0m\n");
    NH_CHECK(NH_BUILD_ERROR_COPY_EXTERNAL_HEADER_FAILED, Nh_Bld_copyExternalHeader())

    puts("\nNETZHAUT: \e[1;32mCOPY PUBLIC LIBRARY HEADER\e[0m\n");
    NH_CHECK(NH_BUILD_ERROR_COPY_PUBLIC_LIBRARY_HEADER_FAILED, Nh_Bld_copyPublicLibraryHeader())

    puts("\nNETZHAUT: \e[1;32mCREATE LIBRARY INCLUDES\e[0m\n");
    NH_CHECK(NH_BUILD_ERROR_CREATE_LIBRARY_INCLUDES_FAILED, Nh_Bld_createLibraryIncludes())

    puts("\nNETZHAUT: \e[1;32mBUILD LIBRARY\e[0m\n");
    NH_CHECK(NH_BUILD_ERROR_BUILD_LIBRARY_FAILED, Nh_Bld_buildLibrary())

    if (INSTALL) {
        puts("\nNETZHAUT: \e[1;32mINSTALL LIBRARY\e[0m\n");
        NH_CHECK(NH_BUILD_ERROR_INSTALL_LIBRARY_FAILED, Nh_Bld_installLibrary())
    }

    if (BUILD_BROWSER) {
        puts("\nNETZHAUT: \e[1;32mBUILD BROWSER\e[0m\n");
        NH_CHECK(NH_BUILD_ERROR_BUILD_BROWSER_FAILED, Nh_Bld_buildBrowser())
    }

    if (INSTALL && BUILD_BROWSER) {
        puts("\nNETZHAUT: \e[1;32mINSTALL BROWSER\e[0m\n");
        NH_CHECK(NH_BUILD_ERROR_INSTALL_BROWSER_FAILED, Nh_Bld_installBrowser())
    }

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

int main(
    int argc, char **argv_pp)
{
NH_BEGIN()

    NH_RESULT result = Nh_Bld_main(argc, argv_pp);

    if (result) {
        printf("NETZHAUT: \e[1;31mERROR\e[0m %s\n", NH_RESULTS_PP[result]); \
        puts("\nNETZHAUT: \e[1;31mBUILD FAILED\e[0m\n");
        NH_END(result)
    }
    else if (!BUILD_NOTHING) {
        puts("\nNETZHAUT: \e[1;32mBUILD SUCCESS\e[0m\n");
    }

NH_END(NH_SUCCESS)
} 

