# Define the compiler and compile flags
CC = gcc
CFLAGS = -std=gnu99 -Wl,-rpath,$(CURDIR)/lib,-rpath,$(CURDIR)/external/TTyr/lib

# Define the linker and linker flags
LD = gcc
LDFLAGS_NH_HTML = -Llib -lnh-api
LDFLAGS_NH_CSS = -Llib -lnh-api
LDFLAGS_NH_MONITOR = -Llib -lnh-api -Lexternal/TTyr/lib -lttyr-api
LDFLAGS_NH_ECMASCRIPT = -Llib -lnh-api

# Define the source file directory for each binary
SRC_DIR_NH_HTML = src/bin/nh-html
SRC_DIR_NH_CSS = src/bin/nh-css
SRC_DIR_NH_MONITOR = src/bin/nh-monitor
SRC_DIR_NH_ECMASCRIPT = src/bin/nh-ecmascript

# List of source files for each binary
SRC_FILES_NH_HTML = Main.c
SRC_FILES_NH_CSS = Main.c
SRC_FILES_NH_MONITOR = Main.c
SRC_FILES_NH_ECMASCRIPT = Main.c

# Object files derived from source files for each binary 
OBJ_FILES_NH_HTML = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_HTML)/, $(SRC_FILES_NH_HTML)))
OBJ_FILES_NH_CSS = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_CSS)/, $(SRC_FILES_NH_CSS)))
OBJ_FILES_NH_MONITOR = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_MONITOR)/, $(SRC_FILES_NH_MONITOR)))
OBJ_FILES_NH_ECMASCRIPT = $(patsubst %.c, %.o, $(addprefix $(SRC_DIR_NH_ECMASCRIPT)/, $(SRC_FILES_NH_ECMASCRIPT)))

# Names of the shared libraries
BIN_NH_HTML = bin/nh-html
BIN_NH_CSS = bin/nh-css
BIN_NH_MONITOR = bin/nh-monitor
BIN_NH_ECMASCRIPT = bin/nh-ecmascript

# Keep this as first (default) target.
all: $(BIN_NH_HTML) $(BIN_NH_CSS) $(BIN_NH_MONITOR) $(BIN_NH_ECMASCRIPT)

build_ttyr:
	(cd external/TTyr && make -f build/automation/Makefile lib NETZHAUT_PATH=$(CURDIR))
create_bin_dir:
	mkdir -p bin

# Custom compiler flags
$(OBJ_FILES_NH_HTML): CFLAGS += $(CCFLAGS) -Isrc/lib -Iexternal -Iexternal/Vulkan-Headers/include -DINCLUDE_VOLK -DVK_VERSION_1_2 -DVK_USE_PLATFORM_XLIB_KHR -DVK_KHR_xlib_surface
$(OBJ_FILES_NH_CSS): CFLAGS += $(CCFLAGS) -Isrc/lib -Iexternal
$(OBJ_FILES_NH_MONITOR): CFLAGS += $(CCFLAGS) -Iexternal/TTyr/src/lib -Isrc/lib -Iexternal
$(OBJ_FILES_NH_ECMASCRIPT): CFLAGS += $(CCFLAGS) -Isrc/lib -Iexternal

# Rule to compile source files into object files
%.o: $(SRC_DIR_NH_HTML)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR_NH_CSS)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR_NH_MONITOR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
%.o: $(SRC_DIR_NH_ECMASCRIPT)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<


# Rule to link object files into the shared libraries
$(BIN_NH_HTML): create_bin_dir $(OBJ_FILES_NH_HTML)
	$(LD) $(CFLAGS) -o $@ $(OBJ_FILES_NH_HTML) $(LDFLAGS_NH_HTML)
$(BIN_NH_CSS): create_bin_dir $(OBJ_FILES_NH_CSS)
	$(LD) $(CFLAGS) -o $@ $(OBJ_FILES_NH_CSS) $(LDFLAGS_NH_CSS)
$(BIN_NH_MONITOR): create_bin_dir build_ttyr $(OBJ_FILES_NH_MONITOR)
	$(LD) $(CFLAGS) -o $@ $(OBJ_FILES_NH_MONITOR) $(LDFLAGS_NH_MONITOR)
$(BIN_NH_ECMASCRIPT): create_bin_dir $(OBJ_FILES_NH_ECMASCRIPT)
	$(LD) $(CFLAGS) -o $@ $(OBJ_FILES_NH_ECMASCRIPT) $(LDFLAGS_NH_ECMASCRIPT)

# Clean rule
clean:
	rm -f $(OBJ_FILES_NH_HTML) $(BIN_NH_HTML)
	rm -f $(OBJ_FILES_NH_CSS) $(BIN_NH_CSS)
	rm -f $(OBJ_FILES_NH_MONITOR) $(BIN_NH_MONITOR)
	rm -f $(OBJ_FILES_NH_ECMASCRIPT) $(BIN_NH_ECMASCRIPT)
	rm -rf bin 
	(cd external/TTyr && make -f build/automation/Makefile clean)

.PHONY: all clean create_bin_dir build_ttyr
