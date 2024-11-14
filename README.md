# Netzhaut
 
Netzhaut is a **multi-purpose open-source Web-Browser-Engine** written from scratch in **C99**. 
Web-Browser-Engines are software systems which run web content on client machines. 
Popular examples are Chromium (Chrome), Gecko (Firefox) and WebKit (Safari).

The project is currently in the **EXPERIMENTAL** phase.

## Contents
  
 - [Build](#Build)
 - [Binaries](#Binaries)
 - [Config](#Config)

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
./nh-monitor [port]
```
`port` If specified, monitors another Netzhaut process over localhost TCP.
