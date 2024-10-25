# Netzhaut
 
Netzhaut is a **multi-purpose open-source Web-Browser-Engine** written from scratch in **C99**. 
Web-Browser-Engines are software systems which run web content on client machines. 
Popular examples are Chromium (Chrome), Gecko (Firefox) and WebKit (Safari).

The project is currently in the **EXPERIMENTAL** phase and thus not usable. 

The documentation for this project is fully contained in this README.

## Contents
  
 - [Build](#Build)

## Build


### Get sources using [Git](https://git-scm.com/)
```bash 
git clone https://github.com/dajofrey/Netzhaut
cd Netzhaut && git submodule update --init
```

### Compile using [Make](https://en.wikipedia.org/wiki/Make_\(software\))

#### Build (default)
```bash 
make -f build/automation/lib.mk all 
make -f build/automation/bin.mk all
```

#### Build with monitor
```bash
make -f build/automation/lib.mk all MONITOR=1
make -f build/automation/bin.mk all CCFLAGS="-DMONITOR"
```
