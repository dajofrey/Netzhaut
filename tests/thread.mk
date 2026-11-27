CC ?= cc
THREAD_SOURCES := tests/thread.c src/lib/nh-core/System/Thread.c \
    src/lib/nh-core/System/Memory.c src/lib/nh-core/Util/Time.c
THREAD_BIN := build/tests/thread
THREAD_CONFIG_BIN := build/tests/thread-config

.PHONY: test-thread test-cocoa-wait test-viewport-wake
test-thread: $(THREAD_BIN) $(THREAD_CONFIG_BIN)
	$(THREAD_BIN)
	$(THREAD_CONFIG_BIN)

test-cocoa-wait: build/tests/cocoa-wait
	build/tests/cocoa-wait

test-viewport-wake: build/tests/viewport-wake
	build/tests/viewport-wake

build/tests/viewport-wake: tests/viewport-wake.c src/lib/nh-gfx/Base/Viewport.c
	mkdir -p $(@D)
	$(CC) -std=gnu11 -g -O1 -ffunction-sections -Wno-typedef-redefinition \
	    tests/viewport-wake.c src/lib/nh-gfx/Base/Viewport.c \
	    -Wl,-dead_strip -o $@

build/tests/cocoa-wait: tests/cocoa-wait.m lib/libnh-wsi.dylib
	mkdir -p $(@D)
	$(CC) -std=gnu11 -g -O1 -Wall -Wextra -Werror -fobjc-arc \
	    tests/cocoa-wait.m -Llib -lnh-wsi -framework Cocoa -lpthread \
	    -Wl,-rpath,$(CURDIR)/lib -o $@

$(THREAD_BIN): $(THREAD_SOURCES) src/lib/nh-core/System/Thread.h
	mkdir -p $(@D)
	$(CC) -std=gnu99 -g -O1 -Wall -Wextra -Werror -Wno-typedef-redefinition \
	    $(THREAD_TEST_CFLAGS) $(THREAD_SOURCES) -lpthread -o $@

$(THREAD_CONFIG_BIN): tests/thread-config.c src/lib/nh-core/Common/Config.c src/lib/nh-core/Common/Config.h \
    src/lib/nh-core/System/Thread.h
	mkdir -p $(@D)
	$(CC) -std=gnu99 -g -O1 -Wall -Wextra -Werror -Wno-typedef-redefinition \
	    $(THREAD_TEST_CFLAGS) tests/thread-config.c -o $@
