// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Library.h"
#include "../Header/Macros.h"
#include "../Header/Utils.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <pthread.h>
#include <dirent.h>

// DECLARE =========================================================================================

static NH_RESULT Nh_Bld_createSharedLibraryUsingGCC(
    const char *objects_p, const char *out_p, const char *extra_p
);

// OBJECTS =========================================================================================

#include NH_CUSTOM_CHECK

#define CREATE(in, out, extra) \
    NH_CHECK((void*)NH_BUILD_ERROR_CANT_CREATE_OBJECT_FILE, Nh_Bld_createPICObjectFileUsingGCC(in, out, extra))

static void *Nh_Bld_buildCore(
    void *extra_p)
{
NH_BEGIN()

    CREATE("Core/Source/System.c", "../bin/object/CoreSystem.o", extra_p)
    CREATE("Core/Source/Window.c", "../bin/object/CoreWindow.o", extra_p)
    CREATE("Core/Source/Memory.c", "../bin/object/CoreMemory.o", extra_p)
    CREATE("Core/Source/Thread.c", "../bin/object/CoreThread.o", extra_p)
    CREATE("Core/Source/HashMap.c", "../bin/object/CoreHashMap.o", extra_p)
    CREATE("Core/Source/Sync.c", "../bin/object/CoreSync.o", extra_p)
    CREATE("Core/Source/Font.c", "../bin/object/CoreFont.o", extra_p)
    CREATE("Core/Source/Tab.c", "../bin/object/CoreTab.o", extra_p)
    CREATE("Core/Source/Time.c", "../bin/object/CoreTime.o", extra_p)
    CREATE("Core/Source/Config.c", "../bin/object/CoreConfig.o", extra_p)
    CREATE("Core/Source/Callbacks.c", "../bin/object/CoreCallbacks.o", extra_p)
    CREATE("Core/Source/String.c", "../bin/object/CoreString.o", extra_p)
    CREATE("Core/Source/RingIterator.c", "../bin/object/CoreRingIterator.o", extra_p)
    CREATE("Core/Source/List.c", "../bin/object/CoreList.o", extra_p)
    CREATE("Core/Source/Math.c", "../bin/object/CoreMath.o", extra_p)
    CREATE("Core/Source/File.c", "../bin/object/CoreFile.o", extra_p)
    CREATE("Core/Source/URI.c", "../bin/object/CoreURI.o", extra_p)
    CREATE("Core/Source/MediaType.c", "../bin/object/CoreMediaType.o", extra_p)
    CREATE("Core/Source/Output.c", "../bin/object/CoreOutput.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildIO(
    void *extra_p)
{
NH_BEGIN()

    CREATE("IO/Source/Window.c", "../bin/object/IOWindow.o", extra_p)
    CREATE("IO/Source/Input.c", "../bin/object/IOInput.o", extra_p)
    CREATE("IO/Source/Mouse.c", "../bin/object/IOMouse.o", extra_p)
    CREATE("IO/Source/Keyboard.c", "../bin/object/IOKeyboard.o", extra_p)
    CREATE("IO/Source/Log.c", "../bin/object/IOLog.o", extra_p)
    CREATE("IO/Win/Source/Window.c", "../bin/object/IOWinWindow.o", extra_p)
    CREATE("IO/Win/Source/Input.c", "../bin/object/IOWinInput.o", extra_p)
    CREATE("IO/X11/Source/Window.c", "../bin/object/IOX11Window.o", extra_p)
    CREATE("IO/X11/Source/Input.c", "../bin/object/IOX11Input.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildGraphics(
    void *extra_p)
{
NH_BEGIN()

    CREATE("Graphics/Source/Tab.c", "../bin/object/GfxTab.o", extra_p)
    CREATE("Graphics/Source/Text.c", "../bin/object/GfxText.o", extra_p)
    CREATE("Graphics/Source/Node.c", "../bin/object/GfxNode.o", extra_p)
    CREATE("Graphics/Source/Texture.c", "../bin/object/GfxTexture.o", extra_p)
    CREATE("Graphics/Source/Window.c", "../bin/object/GfxWindow.o", extra_p)
    CREATE("Graphics/Source/Log.c", "../bin/object/GfxLog.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Host.c", "../bin/object/GfxVkHost.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Node.c", "../bin/object/GfxVkNode.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Driver.c", "../bin/object/GfxVkDriver.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Surface.c", "../bin/object/GfxVkSurface.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Texture.c", "../bin/object/GfxVkTexture.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Utils.c", "../bin/object/GfxVkUtils.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Buffers.c", "../bin/object/GfxVkBuffers.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Text.c", "../bin/object/GfxVkText.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Record.c", "../bin/object/GfxVkRecord.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Pipelines.c", "../bin/object/GfxVkPipelines.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Uniforms.c", "../bin/object/GfxVkUniforms.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Descriptors.c", "../bin/object/GfxVkDescriptors.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Render.c", "../bin/object/GfxVkRender.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Tab.c", "../bin/object/GfxVkTab.o", extra_p)
    CREATE("Graphics/Vulkan/Source/GPU.c", "../bin/object/GfxVkGPU.o", extra_p)
    CREATE("Graphics/Vulkan/Source/Vulkan.c", "../bin/object/GfxVkVulkan.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildCSS(
    void *extra_p)
{
NH_BEGIN()

    CREATE("CSS/Source/Arrange.c", "../bin/object/CSSArrange.o", extra_p)
    CREATE("CSS/Source/Parser.c", "../bin/object/CSSParser.o", extra_p)
    CREATE("CSS/Source/Properties.c", "../bin/object/CSSProperties.o", extra_p)
    CREATE("CSS/Source/Functions.c", "../bin/object/CSSFunctions.o", extra_p)
    CREATE("CSS/Source/Helper.c", "../bin/object/CSSHelper.o", extra_p)
    CREATE("CSS/Source/Background.c", "../bin/object/CSSBackground.o", extra_p)
    CREATE("CSS/Source/Border.c", "../bin/object/CSSBorder.o", extra_p)
    CREATE("CSS/Source/Position.c", "../bin/object/CSSPosition.o", extra_p)
    CREATE("CSS/Source/List.c", "../bin/object/CSSList.o", extra_p)
    CREATE("CSS/Source/Text.c", "../bin/object/CSSText.o", extra_p)
    CREATE("CSS/Source/Box.c", "../bin/object/CSSBox.o", extra_p)
    CREATE("CSS/Source/Associate.c", "../bin/object/CSSAssociate.o", extra_p)
    CREATE("CSS/Source/Sheet.c", "../bin/object/CSSSheet.o", extra_p)
    CREATE("CSS/Source/Data.c", "../bin/object/CSSData.o", extra_p)
    CREATE("CSS/Source/Margin.c", "../bin/object/CSSMargin.o", extra_p)
    CREATE("CSS/Source/Padding.c", "../bin/object/CSSPadding.o", extra_p)
    CREATE("CSS/Source/Log.c", "../bin/object/CSSLog.o", extra_p)
    CREATE("CSS/Source/Input.c", "../bin/object/CSSInput.o", extra_p)
    CREATE("CSS/Source/Image.c", "../bin/object/CSSImage.o", extra_p)
    CREATE("CSS/Source/Selector.c", "../bin/object/CSSSelector.o", extra_p)
    CREATE("CSS/Source/GenericProperty.c", "../bin/object/CSSGenericProperty.o", extra_p)
    CREATE("CSS/Source/Animation.c", "../bin/object/CSSAnimation.o", extra_p)
    CREATE("CSS/Source/BoxTriangles.c", "../bin/object/CSSBoxTriangles.o", extra_p)
    CREATE("CSS/Source/Vertices.c", "../bin/object/CSSVertices.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildHTML(
    void *extra_p)
{
NH_BEGIN()

    CREATE("HTML/Source/Document.c", "../bin/object/HTMLDocument.o", extra_p)
    CREATE("HTML/Source/Attribute.c", "../bin/object/HTMLAttribute.o", extra_p)
    CREATE("HTML/Source/Text.c", "../bin/object/HTMLText.o", extra_p)
    CREATE("HTML/Source/Parser.c", "../bin/object/HTMLParser.o", extra_p)
    CREATE("HTML/Source/Log.c", "../bin/object/HTMLLog.o", extra_p)
    CREATE("HTML/Source/Input.c", "../bin/object/HTMLInput.o", extra_p)
    CREATE("HTML/Source/Node.c", "../bin/object/HTMLNode.o", extra_p)
    CREATE("HTML/Source/Tree.c", "../bin/object/HTMLTree.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildJavaScript1(
    void *extra_p)
{
NH_BEGIN()

    CREATE("JavaScript/Source/Parser.c", "../bin/object/JSParser.o", extra_p)
    CREATE("JavaScript/Source/Script.c", "../bin/object/JSScript.o", extra_p)
    CREATE("JavaScript/Source/Data.c", "../bin/object/JSData.o", extra_p)
    CREATE("JavaScript/Source/Interpreter.c", "../bin/object/JSInterpreter.o", extra_p)
    CREATE("JavaScript/Source/Debug.c", "../bin/object/JSDebug.o", extra_p)
    CREATE("JavaScript/Source/Loader.c", "../bin/object/JSLoader.o", extra_p)
    CREATE("JavaScript/Source/Variable.c", "../bin/object/JSVariable.o", extra_p)
    CREATE("JavaScript/Source/Input.c", "../bin/object/JSInput.o", extra_p)
    CREATE("JavaScript/Source/Result.c", "../bin/object/JSResult.o", extra_p)
    CREATE("JavaScript/Source/Memory.c", "../bin/object/JSMemory.o", extra_p)
    CREATE("JavaScript/Core/Source/Math.c", "../bin/object/JSCoreMath.o", extra_p)
    CREATE("JavaScript/Core/Source/Object.c", "../bin/object/JSCoreObject.o", extra_p)
    CREATE("JavaScript/Core/Source/Function.c", "../bin/object/JSCoreFunction.o", extra_p)
    CREATE("JavaScript/Core/Source/String.c", "../bin/object/JSCoreString.o", extra_p)
    CREATE("JavaScript/DOM/Source/Location.c", "../bin/object/JSDOMLocation.o", extra_p)
    CREATE("JavaScript/DOM/Source/Document.c", "../bin/object/JSDOMDocument.o", extra_p)
    CREATE("JavaScript/DOM/Source/Console.c", "../bin/object/JSDOMConsole.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildJavaScript2(
    void *extra_p)
{
NH_BEGIN()

    CREATE("JavaScript/DOM/Source/HTMLElement.c", "../bin/object/JSDOMHTMLElement.o", extra_p)
    CREATE("JavaScript/DOM/Source/Window.c", "../bin/object/JSDOMWindow.o", extra_p)
    CREATE("JavaScript/DOM/Source/NhTab.c", "../bin/object/JSDOMNhTab.o", extra_p)
    CREATE("JavaScript/DOM/Source/Event.c", "../bin/object/JSDOMEvent.o", extra_p)
    CREATE("JavaScript/DOM/Source/HTMLCollection.c", "../bin/object/JSDOMHTMLCollection.o", extra_p)
    CREATE("JavaScript/DOM/Source/CanvasRenderingContext2D.c", "../bin/object/JSDOMCanvasRenderingContext2D.o", extra_p)
    CREATE("JavaScript/DOM/Source/HTMLCanvasElement.c", "../bin/object/JSDOMHTMLCanvasElement.o", extra_p)
    CREATE("JavaScript/DOM/Source/Node.c", "../bin/object/JSDOMNode.o", extra_p)
    CREATE("JavaScript/DOM/Source/EventTarget.c", "../bin/object/JSDOMEventTarget.o", extra_p)
    CREATE("JavaScript/DOM/Source/Element.c", "../bin/object/JSDOMElement.o", extra_p)
    CREATE("JavaScript/DOM/Source/Path2D.c", "../bin/object/JSDOMPath2D.o", extra_p)
    CREATE("JavaScript/DOM/Source/UIEvent.c", "../bin/object/JSDOMUIEvent.o", extra_p)
    CREATE("JavaScript/DOM/Source/MouseEvent.c", "../bin/object/JSDOMMouseEvent.o", extra_p)
    CREATE("JavaScript/DOM/Source/WheelEvent.c", "../bin/object/JSDOMWheelEvent.o", extra_p)
    CREATE("JavaScript/DOM/Source/NhWindow.c", "../bin/object/JSDOMNhWindow.o", extra_p)
    CREATE("JavaScript/DOM/Source/EventListener.c", "../bin/object/JSDOMEventListener.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildTTY(
    void *extra_p)
{
NH_BEGIN()

    CREATE("TTY/Source/Terminal.c", "../bin/object/TTYTerminal.o", extra_p)
    CREATE("TTY/Source/Row.c", "../bin/object/TTYRow.o", extra_p)
    CREATE("TTY/Source/Input.c", "../bin/object/TTYInput.o", extra_p)
    CREATE("TTY/Source/Draw.c", "../bin/object/TTYDraw.o", extra_p)
    CREATE("TTY/Source/Command.c", "../bin/object/TTYCommand.o", extra_p)
    CREATE("TTY/Source/Query.c", "../bin/object/TTYQuery.o", extra_p)
    CREATE("TTY/Linux/Source/Terminal.c", "../bin/object/TTYLinuxTerminal.o", extra_p)
    CREATE("TTY/Linux/Source/Input.c", "../bin/object/TTYLinuxInput.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildNetwork(
    void *extra_p)
{
NH_BEGIN()

    CREATE("Network/Source/URL.c", "../bin/object/NetURL.o", extra_p)
    CREATE("Network/Source/Socket.c", "../bin/object/NetSocket.o", extra_p)
    CREATE("Network/Source/HTTP.c", "../bin/object/NetHTTP.o", extra_p)
    CREATE("Network/Source/SSL.c", "../bin/object/NetSSL.o", extra_p)
    CREATE("Network/Source/Network.c", "../bin/object/NetNetwork.o", extra_p)
    CREATE("Network/Source/Log.c", "../bin/object/NetLog.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

static void *Nh_Bld_buildAPI(
    void *extra_p)
{
NH_BEGIN()

    CREATE("API/Source/Netzhaut.c", "../bin/object/APINetzhaut.o", extra_p)
    CREATE("API/Source/Results.c", "../bin/object/APIResults.o", extra_p)

NH_END((void*)NH_SUCCESS)
}

#undef CREATE

static NH_RESULT Nh_Bld_buildObjectFiles(
    char *wrkDir_p, char *topDir_p)
{
NH_BEGIN()

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", topDir_p, "/source");

    chdir(topDir_p);

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_BIN_DIRECTORY, Nh_Bld_createDirectory("bin"))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_BIN_OBJECT_DIRECTORY, Nh_Bld_createDirectory("bin/object"))

    chdir(path_p);

#ifdef __unix__

    char *extra_p = "-I/usr/include/freetype2 -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface";

    pthread_t thread_p[10];
    void *status_p;

    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[0], NULL, Nh_Bld_buildCore, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[1], NULL, Nh_Bld_buildIO, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[2], NULL, Nh_Bld_buildGraphics, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[3], NULL, Nh_Bld_buildCSS, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[4], NULL, Nh_Bld_buildHTML, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[5], NULL, Nh_Bld_buildJavaScript1, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[6], NULL, Nh_Bld_buildJavaScript2, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[7], NULL, Nh_Bld_buildTTY, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[8], NULL, Nh_Bld_buildNetwork, extra_p))
    NH_CHECK(NH_BUILD_ERROR_THREAD_CREATION_FAILED, pthread_create(&thread_p[9], NULL, Nh_Bld_buildAPI, extra_p))

    pthread_join(thread_p[0], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_CORE_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[1], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_IO_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[2], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_GRAPHICS_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[3], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_CSS_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[4], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_HTML_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[5], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_JAVASCRIPT_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[6], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_JAVASCRIPT_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[7], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_TTY_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[8], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_NETWORK_OBJECT_FILES, (NH_RESULT)status_p)
    pthread_join(thread_p[9], &status_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_API_OBJECT_FILES, (NH_RESULT)status_p)

#elif defined(WIN_32)

    char *extra_p = "-I/usr/include/freetype2 -L/usr/local/lib -lfreetype -DVK_VERSION_1_2 -DVK_USE_PLATFORM_WIN32_KHR -DVK_KHR_win32_surface";

#endif

    chdir(wrkDir_p);

NH_END(NH_SUCCESS)
}

#include NH_DEFAULT_CHECK

// LINK ============================================================================================

static NH_RESULT Nh_Bld_createLibrary(
    char *wrkDir_p, char *topDir_p)
{
NH_BEGIN()

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", topDir_p, "/bin/object");

    chdir(path_p);

#ifdef __unix__

    int index = 0;
    static char objects_p[16384] = {'\0'};

    DIR *dir;
    struct dirent *ent;
    
    if ((dir = opendir(path_p)) != NULL) 
    {
      while ((ent = readdir(dir)) != NULL) {
        for (int i = 0; ent->d_name[i] != '\0' && ent->d_name[0] != '.'; ++i) {
            objects_p[index++] = ent->d_name[i];
        }
        objects_p[index++] = ' ';
      }
      closedir(dir);
    } 
    else {NH_END(NH_BUILD_ERROR_CANT_OPEN_DIR)}

    char extra_p[1024] = {'\0'};
    sprintf(extra_p, "-Wl,-rpath=%s/external/bin:/usr/local/lib -lm -lssl -lfreetype -lX11 -lX11-xcb -lXcursor -lxkbcommon -lxkbcommon-x11 -lNetzhautHelper", topDir_p);

    NH_CHECK(Nh_Bld_createSharedLibraryUsingGCC(objects_p, "../libNetzhaut.so", extra_p))

#endif 

    chdir(wrkDir_p);

NH_END(NH_SUCCESS)
}

// BUILD ===========================================================================================

NH_RESULT Nh_Bld_buildLibrary()
{
NH_BEGIN()

    if (!Nh_Bld_canRunCommand("gcc")) {NH_END(NH_BUILD_ERROR_GCC_NOT_FOUND)}

#include NH_CUSTOM_CHECK

    char wrkDir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_LIBRARY_OBJECT_FILES, Nh_Bld_buildObjectFiles(wrkDir_p, topDir_p))
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_LIBRARY, Nh_Bld_createLibrary(wrkDir_p, topDir_p))

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

// INCLUDES ========================================================================================

static NH_RESULT Nh_Bld_createIncludeFileUsingXXD(
    const char *topDir_p, const char *in_p, const char *out_p, const char *name_p)
{
NH_BEGIN()

    printf("NETZHAUT: CREATE %s/%s\n", topDir_p, out_p);

    char command_p[1024] = {'\0'};
    sprintf(command_p, "./xxd -i %s/%s %s/%s %s", topDir_p, in_p, topDir_p, out_p, name_p);

    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_XXD_EXECUTION_FAILED)}

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Bld_createLibraryIncludes()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    char wrkDir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", topDir_p, "/external/bin");

    chdir(path_p);

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/js/selecttext.js", "source/JavaScript/Header/Scripts/SelectText.h", "SelectTextJS")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/js/selectoption.js", "source/JavaScript/Header/Scripts/SelectOption.h", "SelectOptionJS")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/js/scroll.js", "source/JavaScript/Header/Scripts/Scroll.h", "ScrollJS")
    );

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/css/misc.css", "source/CSS/Header/Sheets/Misc.h", "MiscCSS")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/css/selectoption.css", "source/CSS/Header/Sheets/SelectOption.h", "SelectOptionCSS")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/css/header.css", "source/CSS/Header/Sheets/Header.h", "HeaderCSS")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/css/list.css", "source/CSS/Header/Sheets/List.h", "ListCSS")
    );

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/icons/expand.png", "source/CSS/Header/Icons/Expand.h", "ExpandPNG")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/icons/disc.png", "source/CSS/Header/Icons/Disc.h", "DiscPNG")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/icons/circle.png", "source/CSS/Header/Icons/Circle.h", "CirclePNG")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/icons/square.png", "source/CSS/Header/Icons/Square.h", "SquarePNG")
    );

    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/index.html", "source/HTML/Header/TestSuite/Index.h", "IndexHTML")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/background.html", "source/HTML/Header/TestSuite/Background.h", "BackgroundHTML")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/border.html", "source/HTML/Header/TestSuite/Border.h", "BorderHTML")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/canvas.html", "source/HTML/Header/TestSuite/Canvas.h", "CanvasHTML")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/image.html", "source/HTML/Header/TestSuite/Image.h", "ImageHTML")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/input.html", "source/HTML/Header/TestSuite/Input.h", "InputHTML")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/list.html", "source/HTML/Header/TestSuite/List.h", "ListHTML")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/selector.html", "source/HTML/Header/TestSuite/Selector.h", "SelectorHTML")
    );
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_FILE, Nh_Bld_createIncludeFileUsingXXD(
        topDir_p, "data/testsuite/text.html", "source/HTML/Header/TestSuite/Text.h", "TextHTML")
    );

    chdir(wrkDir_p);

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

// COPY ============================================================================================

NH_RESULT Nh_Bld_copyPublicLibraryHeader()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    char wrkDir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    char include_p[1024] = {'\0'};
    sprintf(include_p, "%s/include", topDir_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_DIRECTORY, Nh_Bld_createDirectory(include_p))

    sprintf(include_p, "%s/include/Netzhaut", topDir_p);
    NH_CHECK(NH_BUILD_ERROR_CANT_CREATE_INCLUDE_DIRECTORY, Nh_Bld_createDirectory(include_p))

    chdir(topDir_p);

    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("source/API/Header/Netzhaut.h", "include/Netzhaut", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("source/API/Header/Vulkan.h", "include/Netzhaut", NH_FALSE, NH_FALSE, NH_FALSE))
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("source/API/Header/External/", "include/Netzhaut", NH_TRUE, NH_FALSE, NH_FALSE))

    chdir(wrkDir_p);

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

// INSTALL =========================================================================================

NH_RESULT Nh_Bld_installLibrary()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    char wrkDir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkDir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    chdir(topDir_p);

    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("bin/libNetzhaut.so", "/usr/local/lib", NH_FALSE, NH_TRUE, NH_TRUE))
    NH_CHECK(NH_BUILD_ERROR_COPY_FAILED, Nh_Bld_copy("include/Netzhaut", "/usr/local/include", NH_TRUE, NH_TRUE, NH_TRUE))

    chdir(wrkDir_p);

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

// HELPER ==========================================================================================

static NH_RESULT Nh_Bld_createSharedLibraryUsingGCC(
    const char *objects_p, const char *out_p, const char *extra_p)
{
NH_BEGIN()

    static char command_p[16384] = {'\0'};
    sprintf(command_p, "gcc -shared %s %s -o %s", objects_p, extra_p, out_p);
    
    int status = system(command_p);
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_GCC_EXECUTION_FAILED)}

NH_END(NH_SUCCESS)
}

