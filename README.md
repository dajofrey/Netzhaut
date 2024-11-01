# Netzhaut
 
Netzhaut is a **multi-purpose open-source Web-Browser-Engine** written from scratch in **C99**. 
Web-Browser-Engines are software systems which run web content on client machines. 
Popular examples are Chromium (Chrome), Gecko (Firefox) and WebKit (Safari).

The project is currently in the **EXPERIMENTAL** phase and thus not usable. 

The documentation for this project is fully contained in this README.

## Contents
  
 - [Build](#Build)
 - [Binaries](#Binaries)

## Build

### 1. Get sources using [Git](https://git-scm.com/)
```bash 
git clone https://github.com/dajofrey/Netzhaut
cd Netzhaut && git submodule update --init --recursive
```
If you want to use the CLI monitor, do
```
git submodule update --init --checkout --force external/TTyr/
```

### 2. Compile using [Make](https://en.wikipedia.org/wiki/Make_\(software\))

#### Build (default)
```bash 
make -f build/automation/lib.mk all 
make -f build/automation/bin.mk all
```

#### Build with CLI monitor
This requires the TTyr submodule.
```bash
make -f build/automation/lib.mk all MONITOR=1
make -f build/automation/bin.mk all CCFLAGS="-DMONITOR"
```

## Binaries

The resulting binaries after build can be found at `Netzhaut/bin`.

### nh-css
Parses a file containing CSS rules and dumps the parsing result.
```base
./nh-css <file-path> [Tokens | Rules | Objects]
```
