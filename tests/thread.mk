CC ?= cc
THREAD_SOURCES := tests/thread.c src/lib/nh-core/System/Thread.c \
    src/lib/nh-core/System/Memory.c src/lib/nh-core/Util/Time.c
THREAD_BIN := build/tests/thread
THREAD_CONFIG_BIN := build/tests/thread-config

.PHONY: test-thread
test-thread: $(THREAD_BIN) $(THREAD_CONFIG_BIN)
	$(THREAD_BIN)
	$(THREAD_CONFIG_BIN)

$(THREAD_BIN): $(THREAD_SOURCES) src/lib/nh-core/System/Thread.h
	mkdir -p $(@D)
	$(CC) -std=gnu99 -g -O1 -Wall -Wextra -Werror -Wno-typedef-redefinition \
	    $(THREAD_TEST_CFLAGS) $(THREAD_SOURCES) -lpthread -o $@

$(THREAD_CONFIG_BIN): tests/thread-config.c src/lib/nh-core/Common/Config.c src/lib/nh-core/Common/Config.h \
    src/lib/nh-core/System/Thread.h
	mkdir -p $(@D)
	$(CC) -std=gnu99 -g -O1 -Wall -Wextra -Werror -Wno-typedef-redefinition \
	    $(THREAD_TEST_CFLAGS) tests/thread-config.c -o $@
