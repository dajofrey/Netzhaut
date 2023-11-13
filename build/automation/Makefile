# Define the compiler and compile flags
CC = gcc
CFLAGS = -fPIC -std=gnu99 -Wl,-rpath=/usr/local/lib

# Define the linker and linker flags
LD = gcc

LDFLAGS_NHAPI = -ldl
LDFLAGS_NHCORE = -lm -ldl -Lexternal/c_hashmap -l:libc_hashmap.a
LDFLAGS_NHWSI = -lX11 -lX11-xcb -lXcursor -lxkbcommon -lxkbcommon-x11
LDFLAGS_NHHTML =
LDFLAGS_NHDOM =
LDFLAGS_NHTTY = -lutil -Lexternal/st-0.8.5/ -l:libst.so
LDFLAGS_NHTERMINAL =
LDFLAGS_NHNETWORK = -lssl
LDFLAGS_NHWEBIDL =
LDFLAGS_NHECMASCRIPT =
LDFLAGS_NHENCODING =
LDFLAGS_NHGFX = -lX11 -lXrender -lXext -lfreetype -lharfbuzz -Lexternal/volk-master/ -l:libvolk.a -Lexternal/freetype-gl -lfreetype-gl -lGL
LDFLAGS_NHRENDERER =
LDFLAGS_NHCSS =
LDFLAGS_NHURL =

LDFLAGS_NHTTY_BIN = -L$(CURDIR)/lib -lnhapi
LDFLAGS_NHTERMINAL_BIN = -L$(CURDIR)/lib -lnhapi

# Define the source file directory for each library
SRC_DIR_NHAPI = src/lib/nhapi
SRC_DIR_NHCORE = src/lib/nhcore
SRC_DIR_NHWSI = src/lib/nhwsi
SRC_DIR_NHHTML = src/lib/nhhtml
SRC_DIR_NHDOM = src/lib/nhdom
SRC_DIR_NHTTY = src/lib/nhtty
SRC_DIR_NHTERMINAL = src/lib/nhterminal
SRC_DIR_NHNETWORK = src/lib/nhnetwork
SRC_DIR_NHWEBIDL = src/lib/nhwebidl
SRC_DIR_NHECMASCRIPT = src/lib/nhecmascript
SRC_DIR_NHENCODING = src/lib/nhencoding
SRC_DIR_NHGFX = src/lib/nhgfx
SRC_DIR_NHRENDERER = src/lib/nhrenderer
SRC_DIR_NHCSS = src/lib/nhcss
SRC_DIR_NHURL = src/lib/nhurl

SRC_DIR_NHTTY_BIN = src/bin/nhtty
SRC_DIR_NHTERMINAL_BIN = src/bin/nhterminal

# List of source files for each library
SRC_FILES_NHAPI = \
    nhapi.c \
    nhcore.c \
    nhtty.c \
    nhwsi.c \
    nhgfx.c \
    nhhtml.c \
    nhcss.c \
    nhterminal.c \
    nhrenderer.c \
    nhecmascript.c \
    nhencoding.c \

SRC_FILES_NHCORE = \
    Loader/Library.c \
    Loader/Reload.c \
    Loader/Repository.c \
    Loader/Loader.c \
    System/System.c \
    System/Thread.c \
    System/Channel.c \
    System/Logger.c \
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

SRC_FILES_NHWSI = \
    Window/Window.c \
    Window/WindowSettings.c \
    Window/Event.c \
    Window/Listener.c \
    Platforms/X11/Init.c \
    Platforms/X11/Window.c \
    Platforms/X11/WindowSettings.c \
    Common/Log.c \
    Common/Result.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Config.c \
    Common/About.c \
    Common/IndexMap.c

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

SRC_FILES_NHTTY = \
    TTY/TTY.c \
    TTY/Draw.c \
    TTY/Program.c \
    TTY/Macro.c \
    TTY/SideBar.c \
    TTY/TopBar.c \
    TTY/TopBarMessage.c \
    TTY/Tiling.c \
    TTY/StandardIO.c \
    TTY/View.c \
    TTY/Micro.c \
    TTY/ContextMenu.c \
    Editor/TreeListing.c \
    Editor/Editor.c \
    Editor/FileEditor.c \
    Editor/File.c \
    Editor/TextFile.c \
    Editor/TextFileInput.c \
    Editor/SyntaxHighlights.c \
    Logger/Logger.c \
    Shell/Shell.c \
    Shell/Socket.c \
    Common/Result.c \
    Common/About.c \
    Common/Log.c \
    Common/Config.c \
    Common/IndexMap.c \
    Common/Initialize.c \
    Common/Terminate.c \

SRC_FILES_NHTERMINAL = \
    Terminal/Terminal.c \
    Terminal/Grid.c \
    Terminal/Graphics.c \
    Terminal/Vertices.c \
    Vulkan/Pipeline.c \
    Vulkan/Text.c \
    Vulkan/Render.c \
    OpenGL/Foreground.c \
    OpenGL/Background.c \
    OpenGL/Boxes.c \
    OpenGL/Render.c \
    Common/Result.c \
    Common/About.c \
    Common/Log.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Config.c \
    Common/IndexMap.c \
 
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
    Parser/Match.c \
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
	
SRC_FILES_NHTTY_BIN = Main.c
SRC_FILES_NHTERMINAL_BIN = Main.c

# Object files derived from source files for each library
OBJ_FILES_NHAPI = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHAPI)/, $(SRC_FILES_NHAPI)))
OBJ_FILES_NHCORE = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHCORE)/, $(SRC_FILES_NHCORE)))
OBJ_FILES_NHWSI = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHWSI)/, $(SRC_FILES_NHWSI)))
OBJ_FILES_NHHTML = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHHTML)/, $(SRC_FILES_NHHTML)))
OBJ_FILES_NHDOM = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHDOM)/, $(SRC_FILES_NHDOM)))
OBJ_FILES_NHTTY = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHTTY)/, $(SRC_FILES_NHTTY)))
OBJ_FILES_NHTERMINAL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHTERMINAL)/, $(SRC_FILES_NHTERMINAL)))
OBJ_FILES_NHNETWORK = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHNETWORK)/, $(SRC_FILES_NHNETWORK)))
OBJ_FILES_NHWEBIDL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHWEBIDL)/, $(SRC_FILES_NHWEBIDL)))
OBJ_FILES_NHECMASCRIPT = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHECMASCRIPT)/, $(SRC_FILES_NHECMASCRIPT)))
OBJ_FILES_NHENCODING = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHENCODING)/, $(SRC_FILES_NHENCODING)))
OBJ_FILES_NHGFX = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHGFX)/, $(SRC_FILES_NHGFX)))
OBJ_FILES_NHRENDERER = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHRENDERER)/, $(SRC_FILES_NHRENDERER)))
OBJ_FILES_NHCSS = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHCSS)/, $(SRC_FILES_NHCSS)))
OBJ_FILES_NHURL = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHURL)/, $(SRC_FILES_NHURL)))

OBJ_FILES_NHTTY_BIN = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHTTY_BIN)/, $(SRC_FILES_NHTTY_BIN)))
OBJ_FILES_NHTERMINAL_BIN = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHTERMINAL_BIN)/, $(SRC_FILES_NHTERMINAL_BIN)))

# Name of the shared libraries and binaries
LIB_NHAPI = lib/libnhapi.so
LIB_NHCORE = lib/libnhcore.so
LIB_NHWSI = lib/libnhwsi.so
LIB_NHHTML = lib/libnhhtml.so
LIB_NHDOM = lib/libnhdom.so
LIB_NHTTY = lib/libnhtty.so
LIB_NHTERMINAL = lib/libnhterminal.so
LIB_NHNETWORK = lib/libnhnetwork.so
LIB_NHWEBIDL = lib/libnhwebidl.so
LIB_NHECMASCRIPT = lib/libnhecmascript.so
LIB_NHENCODING = lib/libnhencoding.so
LIB_NHGFX = lib/libnhgfx.so
LIB_NHRENDERER = lib/libnhrenderer.so
LIB_NHCSS = lib/libnhcss.so
LIB_NHURL = lib/libnhurl.so

BIN_NHTTY = bin/nhtty
BIN_NHTERMINAL = bin/nhterminal

# Build targets for each library
all: $(LIB_NHAPI) $(LIB_NHCORE) $(LIB_NHWSI) $(LIB_NHHTML) $(LIB_NHDOM) $(LIB_NHTTY) $(BIN_NHTTY) $(LIB_NHTERMINAL) $(BIN_NHTERMINAL) $(LIB_NHNETWORK) $(LIB_NHWEBIDL) $(LIB_NHECMASCRIPT) $(LIB_NHENCODING) $(LIB_NHGFX) $(LIB_NHRENDERER) $(LIB_NHURL)
lib: $(LIB_NHAPI) $(LIB_NHCORE) $(LIB_NHWSI) $(LIB_NHHTML) $(LIB_NHDOM) $(LIB_NHTTY) $(LIB_NHTERMINAL) $(LIB_NHNETWORK) $(LIB_NHWEBIDL) $(LIB_NHECMASCRIPT) $(LIB_NHENCODING) $(LIB_NHGFX) $(LIB_NHRENDERER) $(LIB_NHURL)
bin: $(BIN_NHTTY) $(BIN_NHTERMINAL)

# Custom compiler flags
$(OBJ_FILES_NHGFX): CFLAGS += -Wl,-rpath=external/freetype-gl -I/usr/include/freetype2 -I/usr/include/harfbuzz -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_NHTTY): CFLAGS += -Wl,-rpath=external/st-0.8.5
$(OBJ_FILES_NHTERMINAL): CFLAGS += -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_NHRENDERER): CFLAGS += -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface

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
%.o: $(SRC_DIR_NHTTY)/%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o: $(SRC_DIR_NHTERMINAL)/%.c
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

%.o: $(SRC_DIR_NHTTY_BIN)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR_NHTERMINAL_BIN)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to link object files into the shared libraries
$(LIB_NHAPI): $(OBJ_FILES_NHAPI)
	$(LD) -shared -o $@ $(OBJ_FILES_NHAPI) $(LDFLAGS_NHAPI)
$(LIB_NHCORE): prepare-nhcore $(OBJ_FILES_NHCORE)
	$(LD) -shared -o $@ $(OBJ_FILES_NHCORE) $(LDFLAGS_NHCORE)
$(LIB_NHWSI): $(OBJ_FILES_NHWSI)
	$(LD) -shared -o $@ $(OBJ_FILES_NHWSI) $(LDFLAGS_NHWSI)
$(LIB_NHHTML): $(OBJ_FILES_NHHTML)
	$(LD) -shared -o $@ $(OBJ_FILES_NHHTML) $(LDFLAGS_NHHTML)
$(LIB_NHDOM): $(OBJ_FILES_NHDOM)
	$(LD) -shared -o $@ $(OBJ_FILES_NHDOM) $(LDFLAGS_NHDOM)
$(LIB_NHTTY): $(OBJ_FILES_NHTTY)
	$(LD) -shared -o $@ $(OBJ_FILES_NHTTY) $(LDFLAGS_NHTTY)
$(LIB_NHTERMINAL): $(OBJ_FILES_NHTERMINAL)
	$(LD) -shared -o $@ $(OBJ_FILES_NHTERMINAL) $(LDFLAGS_NHTERMINAL)
$(LIB_NHNETWORK): $(OBJ_FILES_NHNETWORK)
	$(LD) -shared -o $@ $(OBJ_FILES_NHNETWORK) $(LDFLAGS_NHNETWORK)
$(LIB_NHWEBIDL): prepare-nhwebidl $(OBJ_FILES_NHWEBIDL)
	$(LD) -shared -o $@ $(OBJ_FILES_NHWEBIDL) $(LDFLAGS_NHWEBIDL)
$(LIB_NHECMASCRIPT): $(OBJ_FILES_NHECMASCRIPT)
	$(LD) -shared -o $@ $(OBJ_FILES_NHECMASCRIPT) $(LDFLAGS_NHECMASCRIPT)
$(LIB_NHENCODING): $(OBJ_FILES_NHENCODING)
	$(LD) -shared -o $@ $(OBJ_FILES_NHENCODING) $(LDFLAGS_NHENCODING)
$(LIB_NHGFX): prepare-nhgfx $(OBJ_FILES_NHGFX)
	$(LD) -shared -o $@ $(OBJ_FILES_NHGFX) $(LDFLAGS_NHGFX)
$(LIB_NHRENDERER): $(OBJ_FILES_NHRENDERER)
	$(LD) -shared -o $@ $(OBJ_FILES_NHRENDERER) $(LDFLAGS_NHRENDERER)
$(LIB_NHCSS): prepare-nhcss $(OBJ_FILES_NHCSS)
	$(LD) -shared -o $@ $(OBJ_FILES_NHCSS) $(LDFLAGS_NHCSS)
$(LIB_NHURL): prepare-nhurl $(OBJ_FILES_NHURL)
	$(LD) -shared -o $@ $(OBJ_FILES_NHURL) $(LDFLAGS_NHURL)

$(BIN_NHTTY): $(OBJ_FILES_NHTTY_BIN)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS_NHTTY_BIN)
$(BIN_NHTERMINAL): $(OBJ_FILES_NHTERMINAL_BIN)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS_NHTERMINAL_BIN)

# Rule for build-preparation scripts
prepare-nhcore:
	sh build/scripts/prepare-nhcore.sh
prepare-nhgfx:
	sh build/scripts/prepare-nhgfx.sh
prepare-nhcss:
	sh build/scripts/prepare-nhcss.sh
prepare-nhurl:
	sh build/scripts/prepare-nhurl.sh
prepare-nhwebidl:
	sh build/scripts/prepare-nhwebidl.sh

# Clean rule
clean:
	rm -f $(OBJ_FILES_NHAPI) $(LIB_NHAPI) 
	sh build/scripts/prepare-nhcore.sh clean
	rm -f $(OBJ_FILES_NHCORE) $(LIB_NHCORE) 
	rm -f $(OBJ_FILES_NHWSI) $(LIB_NHWSI) 
	rm -f $(OBJ_FILES_NHHTML) $(LIB_NHHTML)
	rm -f $(OBJ_FILES_NHDOM) $(LIB_NHDOM)
	rm -f $(OBJ_FILES_NHTTY) $(LIB_NHTTY)
	rm -f $(OBJ_FILES_NHTERMINAL) $(LIB_NHTERMINAL)
	rm -f $(OBJ_FILES_NHNETWORK) $(LIB_NHNETWORK)
	rm -f $(OBJ_FILES_NHWEBIDL) $(LIB_NHWEBIDL)
	rm -f $(OBJ_FILES_NHECMASCRIPT) $(LIB_NHECMASCRIPT)
	rm -f $(OBJ_FILES_NHENCODING) $(LIB_NHENCODING)
	rm -f $(OBJ_FILES_NHGFX) $(LIB_NHGFX)
	sh build/scripts/prepare-nhgfx.sh clean
	rm -f $(OBJ_FILES_NHRENDERER) $(LIB_NHRENDERER)
	rm -f $(OBJ_FILES_NHCSS) $(LIB_NHCSS)
	sh build/scripts/prepare-nhcss.sh clean
	rm -f $(OBJ_FILES_NHURL) $(LIB_NHURL)
	sh build/scripts/prepare-nhurl.sh clean
	rm -f $(OBJ_FILES_NHTTY_BIN) $(BIN_NHTTY)
	rm -f $(OBJ_FILES_NHTERMINAL_BIN) $(BIN_NHTERMINAL)

.PHONY: all lib bin clean prepare-nhcore prepare-nhgfx prepare-nhcss prepare-nhurl prepare-nhwebidl
