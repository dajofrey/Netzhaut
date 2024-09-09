# Define the compiler and compile flags
CC = gcc
CFLAGS = -std=gnu99 -Wl,-rpath,$(CURDIR)/lib,-rpath,$(CURDIR)/external/TTyr/lib

# Check flags
MONITOR_FLAG := $(filter -DMONITOR_CLI -DMONITOR_SA,$(CCFLAGS))

# Define the linker and linker flags
LD = gcc
LDFLAGS_NHHTML = -Llib -lnh-api

# Define the source file directory for each binary
SRC_DIR_NHHTML = src/bin/nh-html

# List of source files for each binary
SRC_FILES_NHHTML = Main.c

# Object files derived from source files for each binary 
OBJ_FILES_NHHTML = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHHTML)/, $(SRC_FILES_NHHTML)))

# Names of the shared libraries
BIN_NHHTML = bin/nh-html

ifneq ($(strip $(MONITOR_FLAG)),)
    $(info MONITOR flag was provided)
    BUILD_TTYR_TARGET := build_ttyr
    LDFLAGS_NHHTML += -Lexternal/TTyr/lib -lttyr-api
else
    $(info MONITOR flag was not provided)
    BUILD_TTYR_TARGET := # don't build TTyr
endif

build_ttyr:
	(cd external/TTyr && make -f build/automation/Makefile lib NETZHAUT_PATH=$(CURDIR))
create_bin_dir:
	mkdir -p bin

# Build targets for each library
all: $(BUILD_TTYR_TARGET) $(BIN_NHHTML)

# Custom compiler flags
$(OBJ_FILES_NHHTML): CFLAGS += $(CCFLAGS) -Iexternal/TTyr/src/lib -Isrc/lib -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface

# Rule to compile source files into object files
%.o: $(SRC_DIR_NHHTML)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Rule to link object files into the shared libraries
$(BIN_NHHTML): create_bin_dir $(OBJ_FILES_NHHTML)
	$(LD) $(CFLAGS) -o $@ $(OBJ_FILES_NHHTML) $(LDFLAGS_NHHTML)

# Clean rule
clean:
	rm -f $(OBJ_FILES_NHHTML) $(BIN_NHHTML)
	rm -rf bin 
	(cd external/TTyr && make -f build/automation/Makefile clean)

.PHONY: all lib clean
