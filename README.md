# Netzhaut
 
Netzhaut is a multi-purpose open-source **Web-Browser-Engine** written from scratch in modern **C**. 
A web-browser-engine is software which runs web-content on client machines. 
Popular examples are Chromium (Chrome, Google), Gecko (Firefox, Google) and WebKit (Safari, Apple).

Static libraries:

make -f build/automation/lib-ios.mk
Xcode app (builds libs automatically, then links the app):

./build/ios/run-simulator.sh
# or open build/ios/Netzhaut.xcodeproj in Xcode and run NetzhautApp
The app (src/ios/NetzhautApp/) uses UIApplicationMain, initializes Netzhaut in AppDelegate, creates a window via nh_api_createWindow, and drives the engine with a CADisplayLink.


## Contents
  
 - [Build](#Build)
 - [Binaries](#Binaries)
 - [Design](#Design)
 - [FAQ](#FAQ)

## Build

### 0. Check OS support
* Linux ✅  
* MacOs ❌
* Windows ❌  

### 1. Get source-code
#### using [Git](https://git-scm.com/)
```bash 
git clone https://github.com/dajofrey/Netzhaut
cd Netzhaut && git submodule update --init --recursive
```

### 2 Prepare build

MacOS dependencies:
```bash 
brew install freetype harfbuzz openssl
```

#### Android static libraries

Install the Android SDK and NDK, then set `ANDROID_NDK_HOME` to the installed
NDK directory. For example:

```bash
export ANDROID_SDK_ROOT="$HOME/Library/Android/sdk"
export ANDROID_NDK_HOME="$ANDROID_SDK_ROOT/ndk/<ndk-version>"
```

Build the five static libraries (`nh-api`, `nh-core`, `nh-wsi`,
`nh-encoding`, and `nh-gfx`) with the NDK toolchain:

```bash
make -f build/automation/lib-android.mk
```

The default target is `arm64-v8a` with Android API level 24. Select another
supported ABI or API level by passing make variables:

```bash
make -f build/automation/lib-android.mk ANDROID_ABI=arm64-v8a ANDROID_API=24
make -f build/automation/lib-android.mk ANDROID_ABI=x86_64 ANDROID_API=24
```

Supported ABIs are `arm64-v8a`, `armeabi-v7a`, `x86_64`, and `x86`. Each
ABI is written to its own output directory:
`build/android/<abi>/lib/libnh-*.a`. To remove the selected ABI's objects and
archives, run:

```bash
make -f build/automation/lib-android.mk clean
```

This Makefile compiles Netzhaut's archives; it does not build FreeType or
HarfBuzz for Android. The Android app must provide Android-built FreeType and
HarfBuzz when linking `libnh-gfx.a`, compile `android_native_app_glue.c`, and
link the Android system libraries used by the backends (`-landroid -llog
-lEGL -lGLESv3`). Configure `nh-gfx.api` as `opengl` when using the Android
EGL backend.

### 3. Compile source-code into libraries and binaries 

#### using [Make](https://en.wikipedia.org/wiki/Make_\(software\))
```bash 
# Full compile
make -f build/automation/lib.mk all 
make -f build/automation/bin.mk all

# Selective compile
make -f build/automation/lib.mk lib-nh-api lib-nh-*
make -f build/automation/bin.mk bin-nh-wsi bin-nh-*
```

## Binaries

### nh-css
```bash
./nh-css <file-path> [Tokens | Rules | Objects]
```
Parses a file containing CSS rules and dumps the parsing result.  

`file-path` File-path to CSS file.  
`Tokens` If specified, prints CSS tokens.  
`Rules` If specified, prints CSS rules.  
`Objects` If specified, prints CSS objects.

### nh-html
```bash
./nh-html <file-path> [config-options]
```  
Displays an HTML file.  

`file-path` File-path to HTML file.  
`config-options` If specified, passes custom config-options to Netzhaut.

### nh-ecmascript
```bash
./nh-ecmascript <file-path> [config-options]
```  
Runs a ECMAScript script. 

`file-path` File-path to ECMAScript file.  
`config-options` If specified, passes custom config-options to Netzhaut.

### nh-webidl
```bash
./nh-webidl <file-path> [config-options]
```  
Runs a ECMAScript script with WebIDL interfaces. 

`file-path` File-path to ECMAScript file.  
`config-options` If specified, passes custom config-options to Netzhaut.

### nh-monitor
```bash
./nh-monitor <port>
```  
Launches a CLI interface for monitoring.  

`port` Monitors another Netzhaut process over localhost TCP via port.

### nh-wsi
```bash
./nh-wsi
```
Launches empty window for testing purposes.

## Design

### nh-ecmascript

Host/ECMAScript sends jobs to Runtime workload (1 runtime workload)  
Runtime enques jobs in Agent  
Agent Cluster (x cluster workloads) executes jobs from Agent  

#### Add a new intrinsic method
1. add it to .is
2. run generator
3. implement function
4. compile

## FAQ

### How-to use nh-monitor
1. Run nh-monitor on port 50123 or any other open port.  
```bash
./bin/nh-monitor 50123
```
2. Run nh-html with client-port specified and in block mode.  
```bash
./bin/nh-html test.html "nh-monitor.client_port:50123;nh-monitor.block:1;nh-core.debug.monitor_on:1;"
```
