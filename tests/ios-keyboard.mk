IOS_SDK := $(shell xcrun --sdk iphonesimulator --show-sdk-path)
SIMULATOR ?= booted

.PHONY: test-ios-keyboard
test-ios-keyboard:
	mkdir -p build/tests
	xcrun --sdk iphonesimulator clang -std=gnu11 -fobjc-arc -target arm64-apple-ios15.0-simulator \
	    -isysroot "$(IOS_SDK)" -Isrc/lib -Iexternal -Wl,-dead_strip \
	    tests/ios-keyboard.m -framework UIKit -framework Foundation -framework QuartzCore -framework CoreGraphics \
	    -framework OpenGLES -o build/tests/ios-keyboard
	xcrun simctl spawn "$(SIMULATOR)" "$(CURDIR)/build/tests/ios-keyboard"
