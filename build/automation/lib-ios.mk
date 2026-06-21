# Netzhaut iOS static libraries (simulator + device)
#
# Usage:
#   make -f build/automation/lib-ios.mk deps
#   make -f build/automation/lib-ios.mk
#   make -f build/automation/lib-ios.mk IOS_SDK=iphoneos

ROOT_DIR := $(abspath $(CURDIR))
IOS_SDK ?= iphonesimulator
IOS_MIN_VERSION ?= 15.0

ifeq ($(IOS_SDK),iphoneos)
    IOS_ARCH := arm64
    IOS_TARGET_SUFFIX := ios
else
    IOS_ARCH := arm64
    IOS_TARGET_SUFFIX := ios-simulator
endif

IOS_SDK_PATH := $(shell xcrun --sdk $(IOS_SDK) --show-sdk-path)
IOS_CC := xcrun --sdk $(IOS_SDK) clang
IOS_TARGET := $(IOS_ARCH)-apple-$(IOS_TARGET_SUFFIX)$(IOS_MIN_VERSION)

# --- External Dependencies ---
EXT_IOS_DIR := $(ROOT_DIR)/external/ios
FREETYPE_DIR := $(EXT_IOS_DIR)/freetype
HARFBUZZ_DIR := $(EXT_IOS_DIR)/harfbuzz

# --- C FLAGS ---
CFLAGS = -g -std=gnu99 -DNH_STATIC_LINK
CFLAGS += -isysroot $(IOS_SDK_PATH) -mios-version-min=$(IOS_MIN_VERSION)
CFLAGS += -I$(ROOT_DIR)/src/lib -I$(ROOT_DIR)/external
CFLAGS += -target $(IOS_TARGET)
CFLAGS += -F$(IOS_SDK_PATH)/System/Library/Frameworks 

# Typography Includes (FreeType & HarfBuzz)
CFLAGS += -I$(FREETYPE_DIR)/include
CFLAGS += -I$(HARFBUZZ_DIR)/include/harfbuzz

# OpenGL ES specifics for iOS
CFLAGS += -DGL_SILENCE_DEPRECATION
CFLAGS += -I$(IOS_SDK_PATH)/System/Library/Frameworks/OpenGLES.framework/Headers

# --- OBJC FLAGS ---
OBJC_FLAGS = -fobjc-arc -fmodules
OBJC_FLAGS += -isysroot $(IOS_SDK_PATH) -mios-version-min=$(IOS_MIN_VERSION)
OBJC_FLAGS += -target $(IOS_TARGET)
OBJC_FLAGS += -F$(IOS_SDK_PATH)/System/Library/Frameworks
OBJC_FLAGS += -I$(FREETYPE_DIR)/include
OBJC_FLAGS += -I$(HARFBUZZ_DIR)/include/harfbuzz

AR = xcrun ar
LIB_DIR = $(ROOT_DIR)/build/ios/lib
OBJ_DIR = $(ROOT_DIR)/build/ios/obj

SRC_DIR_NH_CORE = src/lib/nh-core
SRC_DIR_NH_WSI = src/lib/nh-wsi
SRC_DIR_NH_ENCODING = src/lib/nh-encoding
SRC_DIR_NH_GFX = src/lib/nh-gfx

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
    Util/Queue.c \
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
    External/c_hashmap/hashmap.c

SRC_FILES_NH_WSI = \
    Window/Window.c \
    Window/WindowSettings.c \
    Window/Event.c \
    Window/Listener.c \
    Platforms/IOS/Init.m \
    Platforms/IOS/Window.m \
    Platforms/IOS/WindowSettings.m \
    Common/Log.c \
    Common/Result.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Config.c \
    Common/About.c

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
    Common/About.c

SRC_FILES_NH_GFX = \
    OpenGL/Surface.c \
    OpenGL/CommandBuffer.c \
    OpenGL/Data.c \
    OpenGL/API.c \
    OpenGL/Commands.c \
    OpenGL/Viewport.c \
    OpenGL/Render.c \
    OpenGL/OpenGL.c \
    OpenGL/ContextIOS.m \
    OpenGL/RenderIOS.m \
    Metal/Device.m \
    Metal/Metal.c \
    Metal/Render.m \
    Metal/SurfaceIOS.m \
    Metal/Viewport.m \
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
    Common/IndexMap.c

# --- Object File Path Corrections ---
OBJ_FILES_NH_CORE = $(patsubst %.c,$(OBJ_DIR)/nh-core/%.o,$(SRC_FILES_NH_CORE))
OBJ_FILES_NH_ENCODING = $(patsubst %.c,$(OBJ_DIR)/nh-encoding/%.o,$(SRC_FILES_NH_ENCODING))

# Properly filter and route both .c and .m files to the build directory
OBJ_FILES_NH_WSI = $(patsubst %.c,$(OBJ_DIR)/nh-wsi/%.o,$(filter %.c,$(SRC_FILES_NH_WSI))) \
                   $(patsubst %.m,$(OBJ_DIR)/nh-wsi/%.o,$(filter %.m,$(SRC_FILES_NH_WSI)))
OBJ_FILES_NH_GFX = $(patsubst %.c,$(OBJ_DIR)/nh-gfx/%.o,$(filter %.c,$(SRC_FILES_NH_GFX))) \
                   $(patsubst %.m,$(OBJ_DIR)/nh-gfx/%.o,$(filter %.m,$(SRC_FILES_NH_GFX)))

LIB_NH_CORE = $(LIB_DIR)/libnh-core.a
LIB_NH_WSI = $(LIB_DIR)/libnh-wsi.a
LIB_NH_ENCODING = $(LIB_DIR)/libnh-encoding.a
LIB_NH_GFX = $(LIB_DIR)/libnh-gfx.a

all: $(LIB_NH_CORE) $(LIB_NH_WSI) $(LIB_NH_ENCODING) $(LIB_NH_GFX)

# --- Dependency Builder ---
deps:
	@scripts/build-ios-deps.sh $(IOS_SDK)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

# --- Compilation Rules ---
$(OBJ_DIR)/nh-core/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_CORE)/%.c
	@mkdir -p $(dir $@)
	$(IOS_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/nh-encoding/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_ENCODING)/%.c
	@mkdir -p $(dir $@)
	$(IOS_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/nh-wsi/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_WSI)/%.c
	@mkdir -p $(dir $@)
	$(IOS_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/nh-wsi/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_WSI)/%.m
	@mkdir -p $(dir $@)
	$(IOS_CC) $(CFLAGS) $(OBJC_FLAGS) -c $< -o $@
$(OBJ_DIR)/nh-gfx/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_GFX)/%.c
	@mkdir -p $(dir $@)
	$(IOS_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/nh-gfx/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_GFX)/%.m
	@mkdir -p $(dir $@)
	$(IOS_CC) $(CFLAGS) $(OBJC_FLAGS) -c $< -o $@

# --- Archiving Rules ---
$(LIB_NH_CORE): $(LIB_DIR) $(OBJ_FILES_NH_CORE)
	$(AR) rcs $@ $(OBJ_FILES_NH_CORE)
$(LIB_NH_WSI): $(LIB_DIR) $(OBJ_FILES_NH_WSI)
	$(AR) rcs $@ $(OBJ_FILES_NH_WSI)
$(LIB_NH_ENCODING): $(LIB_DIR) $(OBJ_FILES_NH_ENCODING)
	$(AR) rcs $@ $(OBJ_FILES_NH_ENCODING)
$(LIB_NH_GFX): $(LIB_DIR) $(OBJ_FILES_NH_GFX)
	$(AR) rcs $@ $(OBJ_FILES_NH_GFX)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)

clean-deps:
	rm -rf $(EXT_IOS_DIR)

.PHONY: all clean deps clean-deps
