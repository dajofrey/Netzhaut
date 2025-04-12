UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    OS := macOS
endif
ifeq ($(UNAME_S),Linux)
    OS := Linux
endif

# Define the compiler and compile flags
CC = gcc
CFLAGS = -g -fPIC -std=gnu99 -Wl,-rpath,$(CURDIR)/lib -Werror=implicit-function-declaration -fsanitize=address,undefined,alignment

ifeq ($(OS),macOS)
    CFLAGS += -Wl,-undefined,dynamic_lookup
    OBJC_FLAGS = -fobjc-arc -fmodules -framework Cocoa -framework QuartzCore
    OBJC = clang
endif

# Define the linker and linker flags
LD = gcc

LDFLAGS_NHAPI = -ldl
LDFLAGS_NHCORE = -v -lm -ldl -fsanitize=address,undefined,alignment
LDFLAGS_NHNETWORK = -lssl
LDFLAGS_NHGFX = -lfreetype -lharfbuzz -Lexternal/volk -l:libvolk.a -Lexternal/freetype-gl -lfreetype-gl -lGL
LDFLAGS_FREETYPE_GL = -I/usr/include/freetype2 -lfreetype -lharfbuzz

ifeq ($(OS),Linux)
    LDFLAGS_NHWSI += -lX11 -lX11-xcb -lXcursor -lxkbcommon -lxkbcommon-x11
    LDFLAGS_NHGFX += -lX11 -lXrender -lXext
endif

ifeq ($(OS),macOS)
    LDFLAGS_NHWSI += -framework Cocoa -framework QuartzCore
endif

# Define the source file directory for each library
SRC_DIR_NHAPI = src/lib/nh-api
SRC_DIR_NHCORE = src/lib/nh-core
SRC_DIR_NHWSI = src/lib/nh-wsi
SRC_DIR_NHHTML = src/lib/nh-html
SRC_DIR_NHDOM = src/lib/nh-dom
SRC_DIR_NHNETWORK = src/lib/nh-network
SRC_DIR_NHWEBIDL = src/lib/nh-webidl
SRC_DIR_NHECMASCRIPT = src/lib/nh-ecmascript
SRC_DIR_NHENCODING = src/lib/nh-encoding
SRC_DIR_NHGFX = src/lib/nh-gfx
SRC_DIR_NHRENDERER = src/lib/nh-renderer
SRC_DIR_NHCSS = src/lib/nh-css
SRC_DIR_NHURL = src/lib/nh-url
SRC_DIR_NHMONITOR = src/lib/nh-monitor

SRC_DIR_VOLK= external/volk
SRC_DIR_FREETYPE_GL= external/freetype-gl

# List of source files for each library
SRC_FILES_NHAPI = \
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

SRC_FILES_NHCORE = \
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

SRC_FILES_NHWSI = \
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
    SRC_FILES_NHWSI += \
        Platforms/X11/Init.c \
        Platforms/X11/Window.c \
        Platforms/X11/WindowSettings.c
endif

ifeq ($(OS),macOS)
    SRC_FILES_NHWSI += \
        Platforms/Cocoa/Window.m \
        Platforms/Cocoa/WindowSettings.m
endif

SRC_FILES_NHHTML = \
    Parser/Parser.c \
    Parser/NamedCharacterReferences.c \
    Parser/Elements.c \
    Parser/Tokenizer.c \
    Parser/TreeConstruction.c \
    Parser/TreeConstructionHelper.c \
    Parser/Algorithms.c \
    Parser/AdoptionAgencyAlgorithm.c \
    Interfaces/Document.c \
    Interfaces/HTMLElement.c \
    Interfaces/HTMLImageElement.c \
    Main/DocumentContext.c \
    Common/Log.c \
    Common/Result.c \
    Common/IndexMap.c \
    Common/Terminate.c \
    Common/Initialize.c \
    Common/About.c \

SRC_FILES_NHDOM = \
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

SRC_FILES_NHNETWORK = \
    Common/Log.c \
    Common/Result.c \
    Common/About.c \
 
SRC_FILES_NHWEBIDL = \
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

SRC_FILES_NHECMASCRIPT = \
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

SRC_FILES_NHENCODING = \
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

SRC_FILES_NHGFX = \
    Vulkan/Host.c \
    Vulkan/GPU.c \
    Vulkan/Driver.c \
    Vulkan/Helper.c \
    Vulkan/Texture.c \
    Vulkan/Render.c \
    Vulkan/Surface.c \
    Vulkan/Vulkan.c \
    Vulkan/Viewport.c \
    OpenGL/Surface.c \
    OpenGL/SurfaceRequirements.c \
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

SRC_FILES_NHRENDERER = \
    Main/Renderer.c \
    Main/BoxTriangulation.c \
    Main/Vertices.c \
    Vulkan/Pipeline.c \
    Vulkan/Buffer.c \
    Vulkan/Uniform.c \
    Vulkan/Descriptor.c \
    Vulkan/Record.c \
    Vulkan/Data.c \
    Vulkan/Text.c \
    Common/Result.c \
    Common/Log.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/About.c \

SRC_FILES_NHCSS = \
    Parser/Tokenizer.c \
    Parser/Parser.c \
    Parser/SelectorParser.c \
    Parser/TokenParser.c \
    Parser/ComponentParser.c \
    Properties/Match.c \
    Properties/Values.c \
    Properties/Properties.c \
    Properties/Compute.c \
    Properties/Color.c \
    Properties/Filter.c \
    Properties/Cascade.c \
    Main/LayoutEngine.c \
    Main/SourceTree.c \
    Main/BoxTree.c \
    Main/FragmentTree.c \
    Main/FormattingContext.c \
    Main/Canvas.c \
    Layout/BoxModel.c \
    Layout/LayoutValues.c \
    Layout/Block.c \
    Layout/Inline.c \
    Layout/Depth.c \
    Layout/Float.c \
    Interfaces/Document.c \
    Interfaces/StyleSheet.c \
    Interfaces/StyleSheetList.c \
    Interfaces/RuleList.c \
    Interfaces/StyleRule.c \
    Interfaces/StyleDeclaration.c \
    Interfaces/CounterStyleRule.c \
    Interfaces/Rule.c \
    Common/Result.c \
    Common/Log.c \
    Common/Initialize.c \
    Common/IndexMap.c \
    Common/About.c \
 
SRC_FILES_NHURL = \
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

SRC_FILES_NHMONITOR = \
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
OBJ_FILES_NHAPI = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHAPI)/, $(SRC_FILES_NHAPI)))
OBJ_FILES_NHCORE = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHCORE)/, $(SRC_FILES_NHCORE)))
OBJ_FILES_NHWSI = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHWSI)/, $(SRC_FILES_NHWSI)))
OBJ_FILES_NHHTML = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHHTML)/, $(SRC_FILES_NHHTML)))
OBJ_FILES_NHDOM = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHDOM)/, $(SRC_FILES_NHDOM)))
OBJ_FILES_NHNETWORK = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHNETWORK)/, $(SRC_FILES_NHNETWORK)))
OBJ_FILES_NHWEBIDL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHWEBIDL)/, $(SRC_FILES_NHWEBIDL)))
OBJ_FILES_NHECMASCRIPT = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHECMASCRIPT)/, $(SRC_FILES_NHECMASCRIPT)))
OBJ_FILES_NHENCODING = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHENCODING)/, $(SRC_FILES_NHENCODING)))
OBJ_FILES_NHGFX = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHGFX)/, $(SRC_FILES_NHGFX)))
OBJ_FILES_NHRENDERER = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHRENDERER)/, $(SRC_FILES_NHRENDERER)))
OBJ_FILES_NHCSS = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHCSS)/, $(SRC_FILES_NHCSS)))
OBJ_FILES_NHURL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHURL)/, $(SRC_FILES_NHURL)))
OBJ_FILES_NHMONITOR = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHMONITOR)/, $(SRC_FILES_NHMONITOR)))

OBJ_FILES_VOLK = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_VOLK)/, $(SRC_FILES_VOLK)))
OBJ_FILES_FREETYPE_GL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_FREETYPE_GL)/, $(SRC_FILES_FREETYPE_GL)))

# Names of the shared libraries
LIB_NHAPI = lib/libnh-api.so
LIB_NHCORE = lib/libnh-core.so
LIB_NHWSI = lib/libnh-wsi.so
LIB_NHHTML = lib/libnh-html.so
LIB_NHDOM = lib/libnh-dom.so
LIB_NHNETWORK = lib/libnh-network.so
LIB_NHWEBIDL = lib/libnh-webidl.so
LIB_NHECMASCRIPT = lib/libnh-ecmascript.so
LIB_NHENCODING = lib/libnh-encoding.so
LIB_NHGFX = lib/libnh-gfx.so
LIB_NHRENDERER = lib/libnh-renderer.so
LIB_NHCSS = lib/libnh-css.so
LIB_NHURL = lib/libnh-url.so
LIB_NHMONITOR = lib/libnh-monitor.so

LIB_VOLK = external/volk/libvolk.a
LIB_FREETYPE_GL = external/freetype-gl/libfreetype-gl.so

# Keep this as default (first) target. 
all: $(LIB_NHAPI) $(LIB_NHCORE) $(LIB_NHWSI) $(LIB_NHHTML) $(LIB_NHDOM) $(LIB_NHNETWORK) $(LIB_NHWEBIDL) $(LIB_NHECMASCRIPT) $(LIB_NHENCODING) $(LIB_VOLK) $(LIB_FREETYPE_GL) $(LIB_NHGFX) $(LIB_NHRENDERER) $(LIB_NHURL) $(LIB_NHCSS) $(LIB_NHMONITOR)

# Build targets for each library
lib-nh-api: $(LIB_NHAPI)
lib-nh-core: $(LIB_NHCORE)
lib-nh-wsi: $(LIB_NHWSI)
lib-nh-html: $(LIB_NHHTML)
lib-nh-dom: $(LIB_NHDOM)
lib-nh-network: $(LIB_NHNETWORK)
lib-nh-webidl: $(LIB_NHWEBIDL)
lib-nh-ecmascript: $(LIB_NHECMASCRIPT)
lib-nh-encoding: $(LIB_NHENCODING)
lib-nh-gfx: $(LIB_VOLK) $(LIB_FREETYPE_GL) $(LIB_NHGFX)
lib-nh-renderer: $(LIB_NHRENDERER)
lib-nh-css: $(LIB_NHCSS)
lib-nh-url: $(LIB_NHURL)
lib-nh-monitor: $(LIB_NHMONITOR)

create_lib_dir:
	mkdir -p lib
download_ttyr: 
	git submodule update --init --checkout --force external/TTyr/

# Custom compiler flags
$(OBJ_FILES_NHCORE): CFLAGS += -lexternal
$(OBJ_FILES_NHHTML): CFLAGS += -Iexternal
$(OBJ_FILES_NHCSS): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_NHAPI): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2
$(OBJ_FILES_NHRENDERER): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_NHWSI): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2
$(OBJ_FILES_NHGFX): CFLAGS += -Iexternal -Iexternal/Vulkan-Headers/include -I/usr/include/freetype2 -I/usr/include/harfbuzz -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_NHRENDERER): CFLAGS += -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_VOLK): CFLAGS += -ldl -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface -DVOLK_VULKAN_H_PATH=\"../Vulkan-Headers/include/vulkan/vulkan.h\"
$(OBJ_FILES_FREETYPE_GL): CFLAGS += -I/usr/include/freetype2 -I/usr/include/harfbuzz -lfreetype -lharfbuzz
$(OBJ_FILES_NHMONITOR): CFLAGS += -Isrc/lib

ifeq ($(OS),Linux)
    LDFLAGS_NHAPI += -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
endif

ifeq ($(OS),Linux)
    $(OBJ_FILES_NHWSI): CFLAGS += -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
endif

# Rule to compile source files into object files
%.o: $(SRC_DIR_NHAPI)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHCORE)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHWSI)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHHTML)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHDOM)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHNETWORK)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHWEBIDL)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHECMASCRIPT)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHENCODING)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHGFX)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHRENDERER)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHCSS)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHURL)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHMONITOR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SRC_DIR_VOLK)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR_FREETYPE_GL)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Add rule for Objective-C files
%.o: $(SRC_DIR_NHWSI)/%.m
	$(OBJC) $(CFLAGS) $(OBJC_FLAGS) -c $< -o $@

# Rule to link object files into the shared libraries
$(LIB_NHAPI): create_lib_dir $(OBJ_FILES_NHAPI)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHAPI) $(LDFLAGS_NHAPI)
$(LIB_NHCORE): create_lib_dir $(OBJ_FILES_NHCORE)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHCORE) $(LDFLAGS_NHCORE)
$(LIB_NHWSI): create_lib_dir $(OBJ_FILES_NHWSI)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHWSI) $(LDFLAGS_NHWSI)
$(LIB_NHHTML): create_lib_dir $(OBJ_FILES_NHHTML)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHHTML) $(LDFLAGS_NHHTML)
$(LIB_NHDOM): create_lib_dir $(OBJ_FILES_NHDOM)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHDOM) $(LDFLAGS_NHDOM)
$(LIB_NHNETWORK): create_lib_dir $(OBJ_FILES_NHNETWORK)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHNETWORK) $(LDFLAGS_NHNETWORK)
$(LIB_NHWEBIDL): create_lib_dir $(OBJ_FILES_NHWEBIDL)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHWEBIDL) $(LDFLAGS_NHWEBIDL)
$(LIB_NHECMASCRIPT): create_lib_dir $(OBJ_FILES_NHECMASCRIPT)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHECMASCRIPT) $(LDFLAGS_NHECMASCRIPT)
$(LIB_NHENCODING): create_lib_dir $(OBJ_FILES_NHENCODING)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHENCODING) $(LDFLAGS_NHENCODING)
$(LIB_NHGFX): create_lib_dir $(OBJ_FILES_NHGFX)
	$(LD) $(CFLAGS) -Wl,-rpath,':$(CURDIR)/external/freetype-gl' -shared -o $@ $(OBJ_FILES_NHGFX) $(LDFLAGS_NHGFX)
$(LIB_NHRENDERER): create_lib_dir $(OBJ_FILES_NHRENDERER)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHRENDERER) $(LDFLAGS_NHRENDERER)
$(LIB_NHCSS): create_lib_dir $(OBJ_FILES_NHCSS)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHCSS) $(LDFLAGS_NHCSS)
$(LIB_NHURL): create_lib_dir $(OBJ_FILES_NHURL)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHURL) $(LDFLAGS_NHURL)
$(LIB_NHMONITOR): create_lib_dir download_ttyr $(OBJ_FILES_NHMONITOR)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_NHMONITOR) $(LDFLAGS_NHMONITOR)

$(LIB_VOLK): $(OBJ_FILES_VOLK)
	ar rcs $@ $^ 
$(LIB_FREETYPE_GL): $(OBJ_FILES_FREETYPE_GL)
	$(LD) $(CFLAGS) -shared -o $@ $(OBJ_FILES_FREETYPE_GL) $(LDFLAGS_FREETYPE_GL)

# Clean rule
clean:
	find . -name "*.o" -type f -delete
	rm -f $(LIB_NHAPI) 
	rm -f $(LIB_NHCORE) 
	rm -f $(LIB_NHWSI) 
	rm -f $(LIB_NHHTML)
	rm -f $(LIB_NHDOM)
	rm -f $(LIB_NHNETWORK)
	rm -f $(LIB_NHWEBIDL)
	rm -f $(LIB_NHECMASCRIPT)
	rm -f $(LIB_NHENCODING)
	rm -f $(LIB_NHGFX)
	rm -f $(LIB_NHRENDERER)
	rm -f $(LIB_NHCSS)
	rm -f $(LIB_NHURL)
	rm -f $(LIB_NHMONITOR)
	rm -f $(LIB_VOLK)
	rm -f $(LIB_FREETYPE_GL)
	rm -rf lib

.PHONY: all clean create_lib_dir download_ttyr
