# Netzhaut iOS static libraries (simulator + device)
#
# Usage:
#   make -f build/automation/lib-ios.mk
#   make -f build/automation/lib-ios.mk IOS_SDK=iphonesimulator
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

CFLAGS = -g -std=gnu99 -DNH_STATIC_LINK
CFLAGS += -isysroot $(IOS_SDK_PATH) -mios-version-min=$(IOS_MIN_VERSION)
CFLAGS += -I$(ROOT_DIR)/src/lib -I$(ROOT_DIR)/external
CFLAGS += -target $(IOS_TARGET)

OBJC_FLAGS = -fobjc-arc -fmodules
OBJC_FLAGS += -isysroot $(IOS_SDK_PATH) -mios-version-min=$(IOS_MIN_VERSION)
OBJC_FLAGS += -target $(IOS_TARGET)

AR = xcrun ar
LIB_DIR = $(ROOT_DIR)/build/ios/lib
OBJ_DIR = $(ROOT_DIR)/build/ios/obj

SRC_DIR_NH_CORE = src/lib/nh-core
SRC_DIR_NH_WSI = src/lib/nh-wsi
SRC_DIR_NH_ENCODING = src/lib/nh-encoding

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

SRC_FILES_NH_WSI_C = \
    Window/Window.c \
    Window/WindowSettings.c \
    Window/Event.c \
    Window/Listener.c \
    Common/Log.c \
    Common/Result.c \
    Common/Initialize.c \
    Common/Terminate.c \
    Common/Config.c \
    Common/About.c

SRC_FILES_NH_WSI_M = \
    Platforms/IOS/Init.m \
    Platforms/IOS/Window.m \
    Platforms/IOS/WindowSettings.m

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

OBJ_FILES_NH_CORE = $(patsubst %.c,$(OBJ_DIR)/nh-core/%.o,$(SRC_FILES_NH_CORE))
OBJ_FILES_NH_WSI = $(patsubst %.c,$(OBJ_DIR)/nh-wsi/%.o,$(SRC_FILES_NH_WSI_C))
OBJ_FILES_NH_WSI += $(patsubst %.m,$(OBJ_DIR)/nh-wsi/%.o,$(SRC_FILES_NH_WSI_M))
OBJ_FILES_NH_ENCODING = $(patsubst %.c,$(OBJ_DIR)/nh-encoding/%.o,$(SRC_FILES_NH_ENCODING))

LIB_NH_CORE = $(LIB_DIR)/libnh-core.a
LIB_NH_WSI = $(LIB_DIR)/libnh-wsi.a
LIB_NH_ENCODING = $(LIB_DIR)/libnh-encoding.a

all: $(LIB_NH_CORE) $(LIB_NH_WSI) $(LIB_NH_ENCODING)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

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

$(LIB_NH_CORE): $(LIB_DIR) $(OBJ_FILES_NH_CORE)
	$(AR) rcs $@ $(OBJ_FILES_NH_CORE)

$(LIB_NH_WSI): $(LIB_DIR) $(OBJ_FILES_NH_WSI)
	$(AR) rcs $@ $(OBJ_FILES_NH_WSI)

$(LIB_NH_ENCODING): $(LIB_DIR) $(OBJ_FILES_NH_ENCODING)
	$(AR) rcs $@ $(OBJ_FILES_NH_ENCODING)

clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR)

.PHONY: all clean
