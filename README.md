# Netzhaut
 
Netzhaut is a multi-purpose open-source **Web-Browser-Engine** written from scratch in modern **C**. 
A web-browser-engine is software which runs web-content on client machines. 
Popular examples are Chromium (Chrome, Google), Gecko (Firefox, Google) and WebKit (Safari, Apple).

## Contents
  
 - [Build](#Build)
 - [Binaries](#Binaries)
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
