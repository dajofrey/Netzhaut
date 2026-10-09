# Netzhaut Android static libraries
#
# Usage:
#   ANDROID_NDK_HOME=/path/to/ndk make -f build/automation/lib-android.mk
#   ANDROID_NDK_HOME=/path/to/ndk make -f build/automation/lib-android.mk ANDROID_ABI=x86_64
#
# The host app must also build android_native_app_glue.c and link with
# -landroid -llog -lEGL -lGLESv3.

ROOT_DIR := $(abspath $(CURDIR))
ANDROID_NDK ?= $(if $(ANDROID_NDK_HOME),$(ANDROID_NDK_HOME),$(ANDROID_NDK_ROOT))
ANDROID_ABI ?= arm64-v8a
ANDROID_API ?= 24

ifeq ($(strip $(ANDROID_NDK)),)
    ifneq ($(MAKECMDGOALS),clean)
        $(error Set ANDROID_NDK_HOME or ANDROID_NDK_ROOT to your Android NDK path)
    endif
endif

ifeq ($(ANDROID_HOST_TAG),)
    ifeq ($(shell uname -s),Darwin)
        ANDROID_HOST_TAG := darwin-x86_64
    else
        ANDROID_HOST_TAG := linux-x86_64
    endif
endif

ifeq ($(ANDROID_ABI),arm64-v8a)
    ANDROID_TARGET := aarch64-linux-android
    ANDROID_CC_NAME := aarch64-linux-android$(ANDROID_API)-clang
else ifeq ($(ANDROID_ABI),armeabi-v7a)
    ANDROID_TARGET := armv7a-linux-androideabi
    ANDROID_CC_NAME := armv7a-linux-androideabi$(ANDROID_API)-clang
else ifeq ($(ANDROID_ABI),x86_64)
    ANDROID_TARGET := x86_64-linux-android
    ANDROID_CC_NAME := x86_64-linux-android$(ANDROID_API)-clang
else ifeq ($(ANDROID_ABI),x86)
    ANDROID_TARGET := i686-linux-android
    ANDROID_CC_NAME := i686-linux-android$(ANDROID_API)-clang
else
    $(error Unsupported ANDROID_ABI '$(ANDROID_ABI)')
endif

TOOLCHAIN_BIN := $(ANDROID_NDK)/toolchains/llvm/prebuilt/$(ANDROID_HOST_TAG)/bin
ANDROID_CC ?= $(TOOLCHAIN_BIN)/$(ANDROID_CC_NAME)
ANDROID_AR ?= $(TOOLCHAIN_BIN)/llvm-ar

EXT_IOS_DIR := $(ROOT_DIR)/external/ios
FREETYPE_DIR := $(EXT_IOS_DIR)/freetype
HARFBUZZ_DIR := $(EXT_IOS_DIR)/harfbuzz

CFLAGS := -g -std=gnu99 -DNH_STATIC_LINK
CFLAGS += -I$(ROOT_DIR)/src/lib -I$(ROOT_DIR)/external
CFLAGS += -I$(FREETYPE_DIR)/include
CFLAGS += -I$(HARFBUZZ_DIR)/include/harfbuzz
CFLAGS += -I$(ANDROID_NDK)/sources/android/native_app_glue

LIB_DIR := $(ROOT_DIR)/build/android/$(ANDROID_ABI)/lib
OBJ_DIR := $(ROOT_DIR)/build/android/$(ANDROID_ABI)/obj

SRC_DIR_NH_API := src/lib/nh-api
SRC_DIR_NH_CORE := src/lib/nh-core
SRC_DIR_NH_WSI := src/lib/nh-wsi
SRC_DIR_NH_ENCODING := src/lib/nh-encoding
SRC_DIR_NH_GFX := src/lib/nh-gfx

SRC_FILES_NH_API := \
    nh-api.c \
    nh-core.c \
    nh-wsi.c \
    nh-gfx.c \
    nh-encoding.c

SRC_FILES_NH_CORE := \
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

SRC_FILES_NH_WSI := \
    Window/Window.c \
    Window/WindowSettings.c \
    Window/Event.c \
    Window/Listener.c \
    Common/Log.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Config.c \
    Common/About.c \
    Platforms/Android/Init.c \
    Platforms/Android/Window.c

SRC_FILES_NH_ENCODING := \
    Base/Encodings.c \
    Base/UnicodeDataHelper.c \
    Base/UnicodeData.gen.c \
    Base/String.c \
    Encodings/UTF8.c \
    Encodings/UTF32.c \
    Common/IndexMap.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Log.c \
    Common/About.c

SRC_FILES_NH_GFX := \
    OpenGL/Surface.c \
    OpenGL/CommandBuffer.c \
    OpenGL/Data.c \
    OpenGL/API.c \
    OpenGL/Commands.c \
    OpenGL/Viewport.c \
    OpenGL/Render.c \
    OpenGL/OpenGL.c \
    OpenGL/ContextAndroid.c \
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

SRC_FILES_FTGL := \
    texture-atlas.c \
    texture-font.c \
    vector.c \
    utf8-utils.c \
    distance-field.c \
    edtaa3func.c \
    platform.c

OBJ_FILES_NH_API := $(patsubst %.c,$(OBJ_DIR)/nh-api/%.o,$(SRC_FILES_NH_API))
OBJ_FILES_NH_CORE := $(patsubst %.c,$(OBJ_DIR)/nh-core/%.o,$(SRC_FILES_NH_CORE))
OBJ_FILES_NH_WSI := $(patsubst %.c,$(OBJ_DIR)/nh-wsi/%.o,$(SRC_FILES_NH_WSI))
OBJ_FILES_NH_ENCODING := $(patsubst %.c,$(OBJ_DIR)/nh-encoding/%.o,$(SRC_FILES_NH_ENCODING))
OBJ_FILES_NH_GFX := $(patsubst %.c,$(OBJ_DIR)/nh-gfx/%.o,$(SRC_FILES_NH_GFX))
OBJ_FILES_FTGL := $(patsubst %.c,$(OBJ_DIR)/freetype-gl/%.o,$(SRC_FILES_FTGL))

LIB_NH_API := $(LIB_DIR)/libnh-api.a
LIB_NH_CORE := $(LIB_DIR)/libnh-core.a
LIB_NH_WSI := $(LIB_DIR)/libnh-wsi.a
LIB_NH_ENCODING := $(LIB_DIR)/libnh-encoding.a
LIB_NH_GFX := $(LIB_DIR)/libnh-gfx.a

all: $(LIB_NH_API) $(LIB_NH_CORE) $(LIB_NH_WSI) $(LIB_NH_ENCODING) $(LIB_NH_GFX)

$(LIB_DIR):
	mkdir -p $@

$(OBJ_DIR)/nh-api/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_API)/%.c
	@mkdir -p $(dir $@)
	$(ANDROID_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/nh-core/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_CORE)/%.c
	@mkdir -p $(dir $@)
	$(ANDROID_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/nh-wsi/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_WSI)/%.c
	@mkdir -p $(dir $@)
	$(ANDROID_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/nh-encoding/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_ENCODING)/%.c
	@mkdir -p $(dir $@)
	$(ANDROID_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/nh-gfx/%.o: $(ROOT_DIR)/$(SRC_DIR_NH_GFX)/%.c
	@mkdir -p $(dir $@)
	$(ANDROID_CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/freetype-gl/%.o: $(ROOT_DIR)/external/freetype-gl/%.c
	@mkdir -p $(dir $@)
	$(ANDROID_CC) $(CFLAGS) -c $< -o $@

$(LIB_NH_API): $(LIB_DIR) $(OBJ_FILES_NH_API)
	$(ANDROID_AR) rcs $@ $(OBJ_FILES_NH_API)
$(LIB_NH_CORE): $(LIB_DIR) $(OBJ_FILES_NH_CORE)
	$(ANDROID_AR) rcs $@ $(OBJ_FILES_NH_CORE)
$(LIB_NH_WSI): $(LIB_DIR) $(OBJ_FILES_NH_WSI)
	$(ANDROID_AR) rcs $@ $(OBJ_FILES_NH_WSI)
$(LIB_NH_ENCODING): $(LIB_DIR) $(OBJ_FILES_NH_ENCODING)
	$(ANDROID_AR) rcs $@ $(OBJ_FILES_NH_ENCODING)
$(LIB_NH_GFX): $(LIB_DIR) $(OBJ_FILES_NH_GFX) $(OBJ_FILES_FTGL)
	$(ANDROID_AR) rcs $@ $(OBJ_FILES_NH_GFX) $(OBJ_FILES_FTGL)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)

.PHONY: all clean
