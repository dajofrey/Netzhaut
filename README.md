# Netzhaut
 
Netzhaut is a **multi-purpose open-source web-browser-engine** written from scratch in **C99**. Web-Browser-Engines are software systems which run web content on client machines. Popular examples are Chromium (Chrome), Gecko (Firefox) and WebKit (Safari). 

The project is currently in the **experimental** phase and thus not usable. 

The documentation for this project is fully contained in this README.

## Contents
  
 - [Build](#Build)
 - [Architecture](#Architecture) 
 - [Design](#Design) 
 - [API](#API) 

## Build


### Get sources: Git
```bash 
git clone https://github.com/dajofrey/Netzhaut
cd Netzhaut && git submodule update --init --recursive 
```

### Compile: Make

Workflow:
```bash 
# build libraries
make -f build/automation/lib.mk all 

# build binaries
make -f build/automation/bin.mk all
```

CCFLAGS:
```bash
MONITOR # Enable monitor (binaries).
```
CCFLAGS Example:
```bash
make -f build/automation/bin.mk all CCFLAGS="-DMONITOR"
```

## Architecture

### Contents
  
 - [Layout](#Layout) 
 - [System](#System) 
 - [Engine](#Engine) 

### Layout
TODO

### System 
TODO

### Engine
TODO

## Design
TODO

## API
TODO
