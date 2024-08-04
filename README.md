# Netzhaut
 
Netzhaut is a **multi-purpose open-source WEB-BROWSER-ENGINE** written from scratch in **C99**. Web-Browser-Engines are software systems which run web content on client machines. Popular examples are Chromium (Chrome), Gecko (Firefox) and WebKit (Safari). 

The project is currently in the **EXPERIMENTAL** phase and thus not usable. 

The documentation for this project is fully contained in this README.

## Contents
  
 - [Build](#Build)
 - [Architecture](#Architecture) 
 - [Design](#Design) 
 - [API](#API) 

## Build


### Get sources using [Git](https://git-scm.com/)
```bash 
git clone https://github.com/dajofrey/Netzhaut
cd Netzhaut && git submodule update --init --recursive 
```

### Compile using [Make](https://en.wikipedia.org/wiki/Make_\(software\))

#### Workflow:
```bash 
# build libraries
make -f build/automation/lib.mk all 

# build binaries
make -f build/automation/bin.mk all
```

#### CCFLAGS for bin.mk:
```bash
MONITOR_CLI # Enable cli monitor.
MONITOR_SA # Enable standalone monitor.
```

#### CCFLAGS Usage-Example:
```bash
make -f build/automation/bin.mk all CCFLAGS="-DMONITOR_CLI"
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
