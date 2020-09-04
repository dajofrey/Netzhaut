// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Docs.h"
#include "../Header/Macros.h"
#include "../Header/Utils.h"
#include "../Header/Main.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_UTILS

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

// DATA ============================================================================================

const char *README_p = 
"\
<div align=\"center\"> \n\
<img src=\"data/logo/Netzhaut_Banner.png\"/> \n\
</div> \n\
\n\
## Intro\n\
\n\
Netzhaut runs web-technologies on the client-machine. You can use the library to write your own web-browser (example given) or anything else which makes use of web-technologies like HTML, CSS or JS. The library was designed with a focus on customization and integration among others.\n\
\n\
The project is currently in an early stage, which means that there are a lot of features missing in order for Netzhaut to be sufficiently usable and some things are still experimental, so its **definitely not ready for users**.\n\
\n\
This README shall only provide an intro and building instructions, for more information please visit the documentation.\n\
\n\
#### Documentation\n\
\n\
* [Dev Hub](https://dajofrey.github.io/Netzhaut/DevHub/html/Main.html)  \n\
  Read about (current) development and learn how to participate.\n\
* [User Hub](https://dajofrey.github.io/Netzhaut/UserHub/html/index.html)  \n\
  Read explanations and detailed information about all interfaces.\n\
\n\
Please submit an [issue](https://github.com/DajoFrey/Netzhaut/issues/new) if you want to report bugs or have suggestions/questions.\n\
\n\
## Building\n\
\n\
#### Linux\n\
\n\
```bash\n\
./build/linux\n\
```\n\
\n\
Third Party Prerequisites: [GCC], [Vulkan], [X11], [OpenSSL], [FreeType], [doxygen] (optional)  \n\
Required System Commands : cp, mkdir, sudo, ar  \n\
\n\
#### Windows\n\
TBA  \n\
\n\
#### NOTE\n\
* Netzhaut uses its own Meta Build System.  \n\
* The Meta Build System features verbose output by default.  \n\
* Depending on the configuration it may ask your permission to copy files to specific system locations.  \n\
* Building should require as few prerequisites as possible, so expect improvements in that regard in the future.  \n\
* Read the help text down below for detailed info. \n\
\n\
[FreeType]: https://www.freetype.org/\n\
[GCC]: https://gcc.gnu.org/\n\
[CMake]: https://cmake.org/\n\
[Make]: https://www.gnu.org/software/make/\n\
[Vulkan]: https://www.khronos.org/vulkan/\n\
[X11]: https://en.wikipedia.org/wiki/X_Window_System\n\
[msbuild]: https://visualstudio.microsoft.com/de/downloads/ \n\
[doxygen]: http://www.doxygen.nl/\n\
[Git]: https://git-scm.com/\n\
[OpenSSL]: https://www.openssl.org/ \n\
";

const char *HTMLHeaderPart1_p = 
"\
<!-- HTML header for doxygen 1.8.15-->\n\
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"https://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
<head>\n\
<meta http-equiv=\"Content-Type\" content=\"text/xhtml;charset=UTF-8\"/>\n\
<meta http-equiv=\"X-UA-Compatible\" content=\"IE=9\"/>\n\
<meta name=\"generator\" content=\"Doxygen $doxygenversion\"/>\n\
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"/>\n\
<!--BEGIN PROJECT_NAME--><title>$projectname: $title</title><!--END PROJECT_NAME-->\n\
<!--BEGIN !PROJECT_NAME--><title>$title</title><!--END !PROJECT_NAME-->\n\
<link href=\"$relpath^tabs.css\" rel=\"stylesheet\" type=\"text/css\"/>\n\
<script type=\"text/javascript\" src=\"$relpath^jquery.js\"></script>\n\
<script type=\"text/javascript\" src=\"$relpath^dynsections.js\"></script>\n\
$treeview\n\
$search\n\
$mathjax\n\
<link href=\"$relpath^$stylesheet\" rel=\"stylesheet\" type=\"text/css\" />\n\
$extrastylesheet\n\
</head>\n\
<body>\n\
<div id=\"top\"><!-- do not remove this div, it is closed by doxygen! -->\n\
\n\
<!--BEGIN TITLEAREA-->\n\
<div id=\"titlearea\">\n\
<table cellspacing=\"0\" cellpadding=\"0\">\n\
 <tbody>\n\
 <tr style=\"height: 56px;\">\n\
  <!--BEGIN PROJECT_LOGO-->\n\
  <td id=\"projectlogo\"><img alt=\"Logo\" src=\"$relpath^$projectlogo\"/></td>\n\
  <!--END PROJECT_LOGO-->\n\
  <!--BEGIN PROJECT_NAME-->\n\
  <td id=\"projectalign\" style=\"padding-left: 0.5em;\">\n\
   <div id=\"projectname\">$projectname\n\
   <!--BEGIN PROJECT_NUMBER-->&#160;<span id=\"projectnumber\">$projectnumber</span><!--END PROJECT_NUMBER-->\n\
   </div>\n\
   <!--BEGIN PROJECT_BRIEF--><div id=\"projectbrief\">$projectbrief</div><!--END PROJECT_BRIEF-->\n\
  </td>\n\
  <!--END PROJECT_NAME-->\n\
  <!--BEGIN !PROJECT_NAME-->\n\
   <!--BEGIN PROJECT_BRIEF-->\n\
    <td style=\"padding-left: 0.5em;\">\n\
    <div id=\"projectbrief\">$projectbrief</div>\n\
    </td>\n\
   <!--END PROJECT_BRIEF-->\n\
  <!--END !PROJECT_NAME-->\n\
<td style=\"padding-left: 10px;\"/>\n\
<td style=\"background-color: inherit; color: inherit;\">\
";

const char *HTMLHeaderPart2_p = 
"\
</td>\n\
  <!--BEGIN DISABLE_INDEX-->\n\
   <!--BEGIN SEARCHENGINE-->\n\
   <td>$searchbox</td>\n\
   <!--END SEARCHENGINE-->\n\
  <!--END DISABLE_INDEX-->\n\
 </tr>\n\
 </tbody>\n\
</table>\n\
</div>\n\
<!--END TITLEAREA-->\n\
<!-- end header part -->\n\
";

// TIME ============================================================================================

static void Nh_Bld_getTime(
    char set_p[26])
{
NH_BEGIN()

    time_t timer;
    struct tm* tm_info;

    timer = time(NULL);
    tm_info = localtime(&timer);

    strftime(set_p, 26, "%Y-%m-%d %H:%M:%S", tm_info);

NH_SILENT_END()
}

// GENERATE ========================================================================================

static NH_RESULT Nh_Bld_generateVersionDependentDocs()
{
NH_BEGIN()

    char out1_p[8192] = {'\0'};
    char out2_p[8192] = {'\0'};

    char time_p[26] = {'\0'};
    Nh_Bld_getTime(time_p);

    if (!strcmp(NH_PRE_RELEASE_VERSION, "none")) {
        sprintf(out1_p, "```%s | %s %s```\n\n%s\n#### HELP\n```\n%s```\n", time_p, NH_NAME, NH_VERSION, README_p, Nh_Bld_getHelp());
        sprintf(out2_p, "%s%s<br>%s %s", HTMLHeaderPart1_p, time_p, NH_VERSION, HTMLHeaderPart2_p);
    }
    else {
        sprintf(out1_p, "```%s | %s %s-%s```\n\n%s\n#### HELP\n```\n%s```\n", time_p, NH_NAME, NH_VERSION, NH_PRE_RELEASE_VERSION, README_p, Nh_Bld_getHelp());
        sprintf(out2_p, "%s%s<br>%s-%s%s", HTMLHeaderPart1_p, time_p, NH_VERSION, NH_PRE_RELEASE_VERSION, HTMLHeaderPart2_p);
    }

#include NH_CUSTOM_CHECK

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", topDir_p, "/README.md");

    FILE *f = fopen(path_p, "w");
    NH_CHECK_NULL(NH_BUILD_ERROR_CANT_OPEN_README, f)    
    
    fprintf(f, "%s\n", out1_p);
    fclose(f);

    sprintf(path_p, "%s%s", topDir_p, "/docs/theme/header.html");

    f = fopen(path_p, "w");
    NH_CHECK_NULL(NH_BUILD_ERROR_CANT_OPEN_HEADER, f)    
    
    fprintf(f, "%s\n", out2_p);
    fclose(f);

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_Bld_generateDocsUsingDoxygen()
{
NH_BEGIN()

    if (!Nh_Bld_canRunCommand("doxygen")) {NH_END(NH_BUILD_ERROR_DOXYGEN_NOT_FOUND)}

#include NH_CUSTOM_CHECK

    char wrkdir_p[2048] = {'\0'};
    NH_CHECK_NULL(NH_BUILD_ERROR_GET_WORK_DIRECTORY, getcwd(wrkdir_p, 2048))

    char topDir_p[2048] = {'\0'};
    NH_CHECK(NH_BUILD_ERROR_GET_TOP_DIRECTORY, Nh_Bld_getTopDirectory(topDir_p, 2048))

#include NH_DEFAULT_CHECK

    char path_p[2048] = {'\0'};
    sprintf(path_p, "%s%s", topDir_p, "/docs/DevHub");
    chdir(path_p);

    int status = system("doxygen Doxyfile");
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_DOXYGEN_EXECUTION_FAILED)}

    sprintf(path_p, "%s%s", topDir_p, "/docs/UserHub");
    chdir(path_p);

    status = system("doxygen Doxyfile");
    if (WEXITSTATUS(status) || WIFSIGNALED(status)) {NH_END(NH_BUILD_ERROR_DOXYGEN_EXECUTION_FAILED)}

    chdir(wrkdir_p);

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_Bld_generateDocs()
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    NH_CHECK(NH_BUILD_ERROR_GENERATE_VERSION_DEPENDENT_DOCS, Nh_Bld_generateVersionDependentDocs())
    NH_CHECK(NH_BUILD_ERROR_GENERATE_DOCS_USING_DOXYGEN, Nh_Bld_generateDocsUsingDoxygen())

#include NH_DEFAULT_CHECK

NH_END(NH_SUCCESS)
}

