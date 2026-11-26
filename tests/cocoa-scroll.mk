.PHONY: test-cocoa-scroll
test-cocoa-scroll:
	mkdir -p build/tests
	clang -std=gnu11 -fobjc-arc -fmodules -Isrc/lib -Iexternal \
	    -Wl,-undefined,dynamic_lookup tests/cocoa-scroll.m \
	    -framework Cocoa -framework QuartzCore -framework Carbon -o build/tests/cocoa-scroll
	build/tests/cocoa-scroll
