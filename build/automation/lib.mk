UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    OS := macOS
endif
ifeq ($(UNAME_S),Linux)
    OS := Linux
endif

ifeq ($(OS),macOS)
    INSTALL_NAME_FLAG = -Wl,-install_name,@rpath/$@
else
    INSTALL_NAME_FLAG =
endif

# Define the compiler and compile flags
CC = gcc
CFLAGS = -g -fPIC -std=gnu99 -Wl,-rpath,$(CURDIR)/lib -Werror=implicit-function-declaration
ifeq ($(OS),macOS)
    CFLAGS += -Wl,-undefined,dynamic_lookup
    OBJC_FLAGS = -fobjc-arc -fmodules -framework Cocoa -framework QuartzCore -fsanitize=address,undefined,alignment
    OBJC = clang
endif

# Define the linker and linker flags
LINKER = gcc
LINKER_FLAGS_NH_API = -ldl
LINKER_FLAGS_NH_CORE = -v -lm -ldl
ifeq ($(OS),macOS)
    LINKER_FLAGS_NH_GFX = -L/opt/homebrew/lib -lfreetype -lharfbuzz -Lexternal/freetype-gl -lfreetype-gl -framework OpenGL
    LINKER_FLAGS_FREETYPE_GL = -L/opt/homebrew/lib -lfreetype -lharfbuzz
    LINKER_FLAGS_NH_NETWORK = -L/opt/homebrew/lib -lssl
else
    LINKER_FLAGS_NH_GFX = -lfreetype -lharfbuzz -Lexternal/volk -l:libvolk.a -Lexternal/freetype-gl -lfreetype-gl -lGL
    LINKER_FLAGS_FREETYPE_GL = -L/usr/include/freetype2 -lfreetype -lharfbuzz
    LINKER_FLAGS_NH_NETWORK = -lssl
endif
ifeq ($(OS),Linux)
    LINKER_FLAGS_NH_WSI += -lX11 -lX11-xcb -lXcursor -lxkbcommon -lxkbcommon-x11
    LINKER_FLAGS_NH_GFX += -lX11 -lXrender -lXext
endif
ifeq ($(OS),macOS)
    LINKER_FLAGS_NH_WSI += -framework Cocoa -framework QuartzCore
    LINKER_FLAGS_NH_GFX += -framework Cocoa -framework QuartzCore
endif

# Define the source file directory for each library
SRC_DIR_NH_API = src/lib/nh-api
SRC_DIR_NH_CORE = src/lib/nh-core
SRC_DIR_NH_WSI = src/lib/nh-wsi
SRC_DIR_NH_HTML = src/lib/nh-html
SRC_DIR_NH_DOM = src/lib/nh-dom
SRC_DIR_NH_NETWORK = src/lib/nh-network
SRC_DIR_NH_WEBIDL = src/lib/nh-webidl
SRC_DIR_NH_ECMASCRIPT = src/lib/nh-ecmascript
SRC_DIR_NH_ENCODING = src/lib/nh-encoding
SRC_DIR_NH_GFX = src/lib/nh-gfx
SRC_DIR_NH_RENDERER = src/lib/nh-renderer
SRC_DIR_NH_CSS = src/lib/nh-css
SRC_DIR_NH_URL = src/lib/nh-url
SRC_DIR_NH_MONITOR = src/lib/nh-monitor
SRC_DIR_VOLK= external/volk
SRC_DIR_FREETYPE_GL= external/freetype-gl

# List of source files for each library
SRC_FILES_NH_API = \
    nh-api.c \
    nh-core.c \
    nh-wsi.c \
    nh-gfx.c \
    nh-html.c \
    nh-css.c \
    nh-renderer.c \
    nh-ecmascript.c \
    nh-encoding.c \
    nh-monitor.c \

SRC_FILES_NH_CORE = \
    Loader/Library.c \
    Loader/Reload.c \
    Loader/Repository.c \
    Loader/Loader.c \
    System/System.c \
    System/Thread.c \
    System/Channel.c \
    System/Memory.c \
    System/Process.c \
    Util/MediaType.c \
    Util/HashMap.c \
    Util/Time.c \
    Util/String.c \
    Util/RingBuffer.c \
    Util/LinkedList.c \
    Util/Math.c \
    Util/File.c \
    Util/List.c \
    Util/Array.c \
    Util/Stack.c \
    Util/BigInt.c \
    Util/ArrayList.c \
    Util/Debug.c \
    Config/Parser.c \
    Config/Tokenizer.c \
    Config/Config.c \
    Config/Updater.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Result.c \
    Common/IndexMap.c \
    Common/Log.c \
    Common/About.c \
    Common/Config.c \
    External/c_hashmap/hashmap.c \

SRC_FILES_NH_WSI = \
    Window/Window.c \
    Window/WindowSettings.c \
    Window/Event.c \
    Window/Listener.c \
    Common/Log.c \
    Common/Result.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Config.c \
    Common/About.c \

ifeq ($(OS),Linux)
    SRC_FILES_NH_WSI += \
        Platforms/X11/Init.c \
        Platforms/X11/Window.c \
        Platforms/X11/WindowSettings.c
endif

ifeq ($(OS),macOS)
    SRC_FILES_NH_WSI += \
        Platforms/Cocoa/Window.m \
        Platforms/Cocoa/WindowSettings.m
endif

SRC_FILES_NH_HTML = \
#    Parser/Parser.c \
#    Parser/NamedCharacterReferences.c \
#    Parser/Elements.c \
#    Parser/Tokenizer.c \
#    Parser/TreeConstruction.c \
#    Parser/TreeConstructionHelper.c \
#    Parser/Algorithms.c \
#    Parser/AdoptionAgencyAlgorithm.c \
#    Interfaces/Document.c \
#    Interfaces/HTMLElement.c \
#    Interfaces/HTMLImageElement.c \
#    Common/Log.c \
#    Common/Result.c \
#    Common/IndexMap.c \
#    Common/Terminate.c \
#    Common/Initialize.c \
#    Common/About.c \

SRC_FILES_NH_DOM = \
    Interfaces/NodeList.c \
    Interfaces/EventTarget.c \
    Interfaces/Node.c \
    Interfaces/Document.c \
    Interfaces/DocumentType.c \
    Interfaces/Comment.c \
    Interfaces/Element.c \
    Interfaces/NamedNodeMap.c \
    Interfaces/CharacterData.c \
    Interfaces/Text.c \
    Interfaces/Attr.c \
    Common/Result.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/About.c \

SRC_FILES_NH_NETWORK = \
    Common/Log.c \
    Common/Result.c \
    Common/About.c \
 
SRC_FILES_NH_WEBIDL = \
    Runtime/Tokenizer.c \
    Runtime/Parser.c \
    Runtime/Builtin.c \
    Runtime/Definitions.c \
    Runtime/Runtime.c \
    Runtime/Object.c \
    Runtime/Type.c \
    Runtime/String.c \
    Common/Log.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/IndexMap.c \
    Common/Result.c \
    Common/About.c \

SRC_FILES_NH_ECMASCRIPT = \
    Engine/Lexer.c \
    Engine/Agent.c \
    Engine/Script.c \
    Engine/Parser.c \
    Engine/Realm.c \
    Engine/InternalSlots.c \
    Engine/ObjectType.c \
    Engine/Type.c \
    Engine/BuiltinFunctionObject.c \
    Engine/Completion.c \
    Engine/Properties.c \
    Engine/Environment.c \
    Engine/OrdinaryFunctionObject.c \
    Engine/Evaluation.c \
    Engine/ExecutionContext.c \
    Engine/CompletionOrReference.c \
    Engine/Reference.c \
    Engine/TestAndCompare.c \
    Engine/PropertyDescriptor.c \
    Engine/Instantiation.c \
    Engine/OrdinaryObject.c \
    StaticSemantics/Misc.c \
    StaticSemantics/DeclaredNames.c \
    StaticSemantics/BoundNames.c \
    StaticSemantics/ScopedDeclarations.c \
    StaticSemantics/ParameterLists.c \
    Intrinsics/GlobalObject.c \
    Intrinsics/Object.c \
    Intrinsics/Function.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Log.c \
    Common/IndexMap.c \
    Common/Result.c \
    Common/About.c \

SRC_FILES_NH_ENCODING = \
    Base/Encodings.c \
    Base/UnicodeDataHelper.c \
    Base/UnicodeData.gen.c \
    Base/String.c \
    Encodings/UTF8.c \
    Encodings/UTF32.c \
    Common/Result.c \
    Common/IndexMap.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Log.c \
    Common/About.c \

SRC_FILES_NH_GFX = \
    OpenGL/Surface.c \
    OpenGL/CommandBuffer.c \
    OpenGL/Data.c \
    OpenGL/API.c \
    OpenGL/Commands.c \
    OpenGL/Viewport.c \
    OpenGL/Render.c \
    OpenGL/OpenGL.c \
    Fonts/FontManager.c \
    Fonts/FontFamily.c \
    Fonts/FontStyle.c \
    Fonts/HarfBuzz.c \
    Fonts/Text.c \
    Base/Texture.c \
    Base/Surface.c \
    Base/SurfaceRequirements.c \
    Base/Viewport.c \
    Common/Result.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Log.c \
    Common/Config.c \
    Common/About.c \
    Common/IndexMap.c \

ifeq ($(OS),Linux)
    SRC_FILES_NH_GFX += \
        OpenGL/ContextX11.c \
        Vulkan/Host.c \
        Vulkan/GPU.c \
        Vulkan/Driver.c \
        Vulkan/Helper.c \
        Vulkan/Texture.c \
        Vulkan/Render.c \
        Vulkan/Surface.c \
        Vulkan/Vulkan.c \
        Vulkan/Viewport.c
endif

ifeq ($(OS),macOS)
    SRC_FILES_NH_GFX += \
        OpenGL/ContextCocoa.m
endif

SRC_FILES_NH_RENDERER = \
#    Main/Renderer.c \
#    Main/BoxTriangulation.c \
#    Main/Vertices.c \
#    Vulkan/Pipeline.c \
#    Vulkan/Buffer.c \
#    Vulkan/Uniform.c \
#    Vulkan/Descriptor.c \
#    Vulkan/Record.c \
#    Vulkan/Data.c \
#    Vulkan/Text.c \
#    Common/Result.c \
#    Common/Log.c \
#    Common/Initialize.c \
#    Common/Terminate.c \
#    Common/About.c \

SRC_FILES_NH_CSS = \
#    Parser/Tokenizer.c \
#    Parser/Parser.c \
#    Parser/SelectorParser.c \
#    Parser/TokenParser.c \
#    Parser/ComponentParser.c \
#    Properties/Match.c \
#    Properties/Values.c \
#    Properties/Properties.c \
#    Properties/Compute.c \
#    Properties/Color.c \
#    Properties/Filter.c \
#    Properties/Cascade.c \
#    Main/LayoutEngine.c \
#    Main/SourceTree.c \
#    Main/BoxTree.c \
#    Main/FragmentTree.c \
#    Main/FormattingContext.c \
#    Main/Canvas.c \
#    Layout/BoxModel.c \
#    Layout/LayoutValues.c \
#    Layout/Block.c \
#    Layout/Inline.c \
#    Layout/Depth.c \
#    Layout/Float.c \
#    Interfaces/Document.c \
#    Interfaces/StyleSheet.c \
#    Interfaces/StyleSheetList.c \
#    Interfaces/RuleList.c \
#    Interfaces/StyleRule.c \
#    Interfaces/StyleDeclaration.c \
#    Interfaces/CounterStyleRule.c \
#    Interfaces/Rule.c \
    Common/Result.c \
    Common/Log.c \
    Common/Initialize.c \
    Common/IndexMap.c \
    Common/About.c \
 
SRC_FILES_NH_URL = \
    Main/URLParser.c \
    Main/HostParser.c \
    Main/Helper.c \
    Main/PercentEncode.c \
    IDNA/IDNAMappingTable.c \
    IDNA/IDNAProcessing.c \
    Common/Log.c \
    Common/Result.c \
    Common/Initialize.c \
    Common/About.c \
    Common/Test.c \

SRC_FILES_NH_MONITOR = \
    Logger.c \
    Monitor.c \
    Client.c \
    Server.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Config.c \

SRC_FILES_VOLK = volk.c

SRC_FILES_FREETYPE_GL = \
    utf8-utils.c \
    vector.c \
    texture-atlas.c \
    texture-font.c \
    distance-field.c \
    edtaa3func.c \
    platform.c \

# Object files derived from source files for each library
OBJ_FILES_NH_API = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_API)/, $(SRC_FILES_NH_API)))
OBJ_FILES_NH_CORE = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_CORE)/, $(SRC_FILES_NH_CORE)))
OBJ_FILES_NH_WSI = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_WSI)/, $(SRC_FILES_NH_WSI)))
OBJ_FILES_NH_WSI += $(patsubst %.m, %.o, $(addprefix $(SRC_DIR_NH_WSI)/, $(wildcard *.m)))
OBJ_FILES_NH_HTML = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_HTML)/, $(SRC_FILES_NH_HTML)))
OBJ_FILES_NH_DOM = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_DOM)/, $(SRC_FILES_NH_DOM)))
OBJ_FILES_NH_NETWORK = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_NETWORK)/, $(SRC_FILES_NH_NETWORK)))
OBJ_FILES_NH_WEBIDL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_WEBIDL)/, $(SRC_FILES_NH_WEBIDL)))
OBJ_FILES_NH_ECMASCRIPT = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_ECMASCRIPT)/, $(SRC_FILES_NH_ECMASCRIPT)))
OBJ_FILES_NH_ENCODING = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_ENCODING)/, $(SRC_FILES_NH_ENCODING)))
OBJ_FILES_NH_GFX = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_GFX)/, $(SRC_FILES_NH_GFX)))
OBJ_FILES_NH_GFX += $(patsubst %.m, %.o, $(addprefix $(SRC_DIR_NH_GFX)/, $(wildcard *.m)))
OBJ_FILES_NH_RENDERER = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_RENDERER)/, $(SRC_FILES_NH_RENDERER)))
OBJ_FILES_NH_CSS = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_CSS)/, $(SRC_FILES_NH_CSS)))
OBJ_FILES_NH_URL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_URL)/, $(SRC_FILES_NH_URL)))
OBJ_FILES_NH_MONITOR = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_MONITOR)/, $(SRC_FILES_NH_MONITOR)))
OBJ_FILES_VOLK = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_VOLK)/, $(SRC_FILES_VOLK)))
OBJ_FILES_FREETYPE_GL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_FREETYPE_GL)/, $(SRC_FILES_FREETYPE_GL)))

# Names of the shared libraries
LIB_NH_API = lib/libnh-api.so
LIB_NH_CORE = lib/libnh-core.so
LIB_NH_WSI = lib/libnh-wsi.so
LIB_NH_HTML = lib/libnh-html.so
LIB_NH_DOM = lib/libnh-dom.so
LIB_NH_NETWORK = lib/libnh-network.so
LIB_NH_WEBIDL = lib/libnh-webidl.so
LIB_NH_ECMASCRIPT = lib/libnh-ecmascript.so
LIB_NH_ENCODING = lib/libnh-encoding.so
LIB_NH_GFX = lib/libnh-gfx.so
LIB_NH_RENDERER = lib/libnh-renderer.so
LIB_NH_CSS = lib/libnh-css.so
LIB_NH_URL = lib/libnh-url.so
LIB_NH_MONITOR = lib/libnh-monitor.so
LIB_VOLK = external/volk/libvolk.a
LIB_FREETYPE_GL = external/freetype-gl/libfreetype-gl.so

# Keep this as default (first) target. 
ifeq ($(OS),macOS)
    all: $(LIB_NH_API) $(LIB_NH_CORE) $(LIB_NH_WSI) $(LIB_NH_HTML) $(LIB_NH_DOM) $(LIB_NH_NETWORK) $(LIB_NH_WEBIDL) $(LIB_NH_ECMASCRIPT) $(LIB_NH_ENCODING) $(LIB_FREETYPE_GL) $(LIB_NH_GFX) $(LIB_NH_RENDERER) $(LIB_NH_URL) $(LIB_NH_CSS) $(LIB_NH_MONITOR)
else
    all: $(LIB_NH_API) $(LIB_NH_CORE) $(LIB_NH_WSI) $(LIB_NH_HTML) $(LIB_NH_DOM) $(LIB_NH_NETWORK) $(LIB_NH_WEBIDL) $(LIB_NH_ECMASCRIPT) $(LIB_NH_ENCODING) $(LIB_VOLK) $(LIB_FREETYPE_GL) $(LIB_NH_GFX) $(LIB_NH_RENDERER) $(LIB_NH_URL) $(LIB_NH_CSS) $(LIB_NH_MONITOR)
endif

# Build targets for each library
lib-nh-api: $(LIB_NH_API)
lib-nh-core: $(LIB_NH_CORE)
lib-nh-wsi: $(LIB_NH_WSI)
lib-nh-html: $(LIB_NH_HTML)
lib-nh-dom: $(LIB_NH_DOM)
lib-nh-network: $(LIB_NH_NETWORK)
lib-nh-webidl: $(LIB_NH_WEBIDL)
lib-nh-ecmascript: $(LIB_NH_ECMASCRIPT)
lib-nh-encoding: $(LIB_NH_ENCODING)
ifeq ($(OS),macOS)
    lib-nh-gfx: $(LIB_FREETYPE_GL) $(LIB_NH_GFX)
else
    lib-nh-gfx: $(LIB_VOLK) $(LIB_FREETYPE_GL) $(LIB_NH_GFX)
endif
lib-nh-renderer: $(LIB_NH_RENDERER)
lib-nh-css: $(LIB_NH_CSS)
lib-nh-url: $(LIB_NH_URL)
lib-nh-monitor: $(LIB_NH_MONITOR)

create_lib_dir:
	mkdir -p lib
download_termoskanne: 
	git submodule update --init --checkout --force external/TTyr/

# Custom compiler flags
$(OBJ_FILES_NH_CORE): CFLAGS += -lexternal
$(OBJ_FILES_NH_HTML): CFLAGS += -Iexternal
$(OBJ_FILES_NH_CSS): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_NH_API): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2
$(OBJ_FILES_NH_RENDERER): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_NH_WSI): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2
ifeq ($(OS),macOS)
    $(OBJ_FILES_NH_GFX): CFLAGS += -Iexternal -I/opt/homebrew/include/freetype2 -I/opt/homebrew/include/harfbuzz
else
    $(OBJ_FILES_NH_GFX): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -I/usr/include/freetype2 -I/usr/include/harfbuzz -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
endif
$(OBJ_FILES_NH_RENDERER): CFLAGS += -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_VOLK): CFLAGS += -ldl -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface -DVOLK_VULKAN_H_PATH=\"../Vulkan-Headers/include/vulkan/vulkan.h\"
ifeq ($(OS),macOS)
    $(OBJ_FILES_FREETYPE_GL): CFLAGS += -I/opt/homebrew/include/freetype2 -I/opt/homebrew/include/harfbuzz
else
    $(OBJ_FILES_FREETYPE_GL): CFLAGS += -I/usr/include/freetype2 -I/usr/include/harfbuzz
endif
$(OBJ_FILES_NH_MONITOR): CFLAGS += -Isrc/lib

ifeq ($(OS),Linux)
    LINKER_FLAGS_NH_API += -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
endif

ifeq ($(OS),Linux)
    $(OBJ_FILES_NH_WSI): CFLAGS += -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
endif

# Rule to compile source files into object files
%.o: $(SRC_DIR_NH_API)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_CORE)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_WSI)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_HTML)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_DOM)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_NETWORK)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_WEBIDL)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_ECMASCRIPT)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_ENCODING)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_GFX)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_RENDERER)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_CSS)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_URL)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NH_MONITOR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SRC_DIR_VOLK)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR_FREETYPE_GL)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Add rule for Objective-C files
%.o: $(SRC_DIR_NH_WSI)/%.m
	$(OBJC) $(CFLAGS) $(OBJC_FLAGS) -c $< -o $@

ifeq ($(OS),macOS)
    INSTALL_NAME = -Wl,-install_name,@rpath/$(1)
else
    INSTALL_NAME =
endif

# Rule to link object files into the shared libraries
$(LIB_NH_API): create_lib_dir $(OBJ_FILES_NH_API)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-api.so) $(OBJ_FILES_NH_API) $(LINKER_FLAGS_NH_API)
$(LIB_NH_CORE): create_lib_dir $(OBJ_FILES_NH_CORE)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-core.so) $(OBJ_FILES_NH_CORE) $(LINKER_FLAGS_NH_CORE)
$(LIB_NH_WSI): create_lib_dir $(OBJ_FILES_NH_WSI)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-wsi.so) $(OBJ_FILES_NH_WSI) $(LINKER_FLAGS_NH_WSI)
$(LIB_NH_HTML): create_lib_dir $(OBJ_FILES_NH_HTML)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-html.so) $(OBJ_FILES_NH_HTML) $(LINKER_FLAGS_NH_HTML)
$(LIB_NH_DOM): create_lib_dir $(OBJ_FILES_NH_DOM)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-dom.so) $(OBJ_FILES_NH_DOM) $(LINKER_FLAGS_NH_DOM)
$(LIB_NH_NETWORK): create_lib_dir $(OBJ_FILES_NH_NETWORK)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-network.so) $(OBJ_FILES_NH_NETWORK) $(LINKER_FLAGS_NH_NETWORK)
$(LIB_NH_WEBIDL): create_lib_dir $(OBJ_FILES_NH_WEBIDL)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-webidl.so) $(OBJ_FILES_NH_WEBIDL) $(LINKER_FLAGS_NH_WEBIDL)
$(LIB_NH_ECMASCRIPT): create_lib_dir $(OBJ_FILES_NH_ECMASCRIPT)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-ecmascript.so) $(OBJ_FILES_NH_ECMASCRIPT) $(LINKER_FLAGS_NH_ECMASCRIPT)
$(LIB_NH_ENCODING): create_lib_dir $(OBJ_FILES_NH_ENCODING)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-encoding.so) $(OBJ_FILES_NH_ENCODING) $(LINKER_FLAGS_NH_ENCODING)
$(LIB_NH_GFX): create_lib_dir $(OBJ_FILES_NH_GFX)
	$(LINKER) $(CFLAGS) -Wl,-rpath,'$(CURDIR)/external/freetype-gl' -shared -o $@ $(call INSTALL_NAME,libnh-gfx.so) $(OBJ_FILES_NH_GFX) $(LINKER_FLAGS_NH_GFX)
$(LIB_NH_RENDERER): create_lib_dir $(OBJ_FILES_NH_RENDERER)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-renderer.so) $(OBJ_FILES_NH_RENDERER) $(LINKER_FLAGS_NH_RENDERER)
$(LIB_NH_CSS): create_lib_dir $(OBJ_FILES_NH_CSS)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-css.so) $(OBJ_FILES_NH_CSS) $(LINKER_FLAGS_NH_CSS)
$(LIB_NH_URL): create_lib_dir $(OBJ_FILES_NH_URL)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-url.so) $(OBJ_FILES_NH_URL) $(LINKER_FLAGS_NH_URL)
$(LIB_NH_MONITOR): create_lib_dir download_termoskanne $(OBJ_FILES_NH_MONITOR)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libnh-monitor.so) $(OBJ_FILES_NH_MONITOR) $(LINKER_FLAGS_NH_MONITOR)
$(LIB_VOLK): $(OBJ_FILES_VOLK)
	ar rcs $@ $^ 
$(LIB_FREETYPE_GL): $(OBJ_FILES_FREETYPE_GL)
	$(LINKER) $(CFLAGS) -shared -o $@ $(call INSTALL_NAME,libfreetype-gl.so) $(OBJ_FILES_FREETYPE_GL) $(LINKER_FLAGS_FREETYPE_GL)

# Clean rule
clean:
	find . -name "*.o" -type f -delete
	rm -f $(LIB_NH_API) 
	rm -f $(LIB_NH_CORE) 
	rm -f $(LIB_NH_WSI) 
	rm -f $(LIB_NH_HTML)
	rm -f $(LIB_NH_DOM)
	rm -f $(LIB_NH_NETWORK)
	rm -f $(LIB_NH_WEBIDL)
	rm -f $(LIB_NH_ECMASCRIPT)
	rm -f $(LIB_NH_ENCODING)
	rm -f $(LIB_NH_GFX)
	rm -f $(LIB_NH_RENDERER)
	rm -f $(LIB_NH_CSS)
	rm -f $(LIB_NH_URL)
	rm -f $(LIB_NH_MONITOR)
	rm -f $(LIB_VOLK)
	rm -f $(LIB_FREETYPE_GL)
	rm -rf lib

.PHONY: all clean create_lib_dir download_termoskanne
