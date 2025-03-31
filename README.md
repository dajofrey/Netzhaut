# Netzhaut
 
Netzhaut is a **multi-purpose open-source Web-Browser-Engine** written from scratch in **C99**. 
Web-Browser-Engines are software systems which run web content on client machines. 
Popular examples are Chromium (Chrome), Gecko (Firefox) and WebKit (Safari).

The project is currently in the **EXPERIMENTAL** phase.

## Contents
  
 - [Build](#Build)
 - [Binaries](#Binaries)
 - [How-To](#How-To)

## Build

### 1. Get sources using [Git](https://git-scm.com/)
```bash 
git clone https://github.com/dajofrey/Netzhaut
cd Netzhaut && git submodule update --init --recursive
```

### 2. Compile using [Make](https://en.wikipedia.org/wiki/Make_\(software\))
```bash 
make -f build/automation/lib.mk all 
make -f build/automation/bin.mk all
```

## Binaries

### nh-css
Parses a file containing CSS rules and dumps the parsing result.
```bash
./nh-css <file-path> [Tokens | Rules | Objects]
```
`file-path` File-path to CSS file.  
`Tokens` If specified, prints CSS tokens.  
`Rules` If specified, prints CSS rules.  
`Objects` If specified, prints CSS objects.

### nh-html
Displays an HTML file.
```bash
./nh-html <file-path> [config-options]
```
`file-path` File-path to HTML file.  
`config-options` If specified, passes custom config-options to Netzhaut.

### nh-monitor
Launches a CLI interface for monitoring.
```bash
./nh-monitor <port>
```
`port` Monitors another Netzhaut process over localhost TCP via port.

## How-To

### Use nh-monitor
1. Run nh-monitor on port 50123 or any other open port.
```bash
./bin/nh-monitor 50123
```
2. Run nh-html with client-port specified and in block mode.
```bash
./bin/nh-html test.html "nh-monitor.client_port:50123;nh-monitor.block:1;nh-core.debug.monitor_on:1;"
```
