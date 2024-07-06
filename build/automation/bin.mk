# Define the compiler and compile flags
CC = gcc
CFLAGS = -std=gnu99 -Wl,-rpath,$(CURDIR)/lib,-rpath,$(CURDIR)/external/TTyr/lib

# Define the linker and linker flags
LD = gcc
LDFLAGS_NHHTML = -Llib -lnhapi -Lexternal/TTyr/lib -lttyr-api

# Define the source file directory for each binary
SRC_DIR_NHHTML = src/bin/nhhtml

# List of source files for each binary
SRC_FILES_NHHTML = Main.c

# Object files derived from source files for each binary 
OBJ_FILES_NHHTML = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NHHTML)/, $(SRC_FILES_NHHTML)))

# Names of the shared libraries
BIN_NHHTML = bin/nhhtml

build_ttyr:
	(cd external/TTyr && make -f build/automation/Makefile lib NETZHAUT_PATH=$(CURDIR))
create_bin_dir:
	mkdir -p bin

# Build targets for each library
all: build_ttyr $(BIN_NHHTML)

# Custom compiler flags
$(OBJ_FILES_NHHTML): CFLAGS += $(CCFLAGS) -Isrc/lib -Iexternal/TTyr/src/lib -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface

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
