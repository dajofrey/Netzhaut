```2020-09-12 13:43:03 | Netzhaut v0.0.1```

<div align="center"> 
<img src="data/logo/Netzhaut_Banner.png"/> 
</div> 

## Intro

Netzhaut runs web-technologies on the client-machine. You can use the library to write your own web-browser (example given) or anything else which makes use of web-technologies like HTML, CSS or JS. The library was designed with a focus on customization and integration among others.

The project is currently in an early stage, which means that there are a lot of features missing in order for Netzhaut to be sufficiently usable and some things are still experimental, so its **definitely not ready for users**.

This README shall only provide an intro and building instructions, for more information please visit the documentation.

#### Documentation

* [Dev Hub](https://dajofrey.github.io/Netzhaut/DevHub/html/Main.html)  
  Read about (current) development and learn how to participate.
* [User Hub](https://dajofrey.github.io/Netzhaut/UserHub/html/index.html)  
  Read explanations and detailed information about all interfaces.

Please submit an [issue](https://github.com/DajoFrey/Netzhaut/issues/new) if you want to report bugs or have suggestions/questions.

## Building

#### Linux

```bash
./build/linux
```

Third Party Prerequisites: [GCC], [Vulkan], [X11], [OpenSSL], [FreeType], [doxygen] (optional)  
Required System Commands : cp, mkdir, sudo, ar  

#### Windows
TBA  

#### NOTE
* Netzhaut uses its own Meta Build System.  
* The Meta Build System features verbose output by default.  
* Depending on the configuration it may ask your permission to copy files to specific system locations.  
* Building should require as few prerequisites as possible, so expect improvements in that regard in the future.  
* Read the help text down below for detailed info. 

[FreeType]: https://www.freetype.org/
[GCC]: https://gcc.gnu.org/
[CMake]: https://cmake.org/
[Make]: https://www.gnu.org/software/make/
[Vulkan]: https://www.khronos.org/vulkan/
[X11]: https://en.wikipedia.org/wiki/X_Window_System
[msbuild]: https://visualstudio.microsoft.com/de/downloads/ 
[doxygen]: http://www.doxygen.nl/
[Git]: https://git-scm.com/
[OpenSSL]: https://www.openssl.org/ 

#### HELP
```
By default (no arguments) Netzhaut Meta Build System will build Netzhaut as a shared library.
This behavior can be expanded by using the following commands:

--docs    Generate documentation.
--browser Build browser executable.
--install Install shared library. Install browser if --browser was specified.
--debug   Activate better debug capabilies.
--vulkan  Compile Vulkan shaders.
--quite   Suppress all output to the standard output channel.

The following commands suppress the building process:

-v | --version | version Print project version and other info.
-h | --help    | help    Print this help text.
```

