// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "FontFamily.h"

#include "../Common/Log.h"

#include "../../nh-core/Util/File.h"
#include "../../nh-core/Util/String.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/File.h"

#include <ctype.h>
#include <string.h>
#include <stdio.h>

#ifdef __unix__ 	
    #include <dirent.h>
#elif defined(_WIN32) || defined(WIN32)
#endif

// GENERIC FAMILY NAMES ============================================================================

char *NH_GFX_GENERIC_FONT_FAMILY_NAMES_PP[] = {
    "SERIF",
    "SANS_SERIF",
    "CURSIVE",
    "FANTASY",
    "MONOSPACE",
    "SYSTEM_UI",
    "EMOJI",
    "MATH",
    "FANGSONG",
    "UI_SERIF",
    "UI_SANS_SERIF",
    "UI_MONOSPACE",
    "UI_ROUNDED",
};

// GENERIC FAMILIES ================================================================================

static char *sansSerif_pp[] = {
    "agency fb",
    "akzidenz-grotesk",
    "amplitude",
    "andalé sans",
    "antique olive",
    "arial",
    "arial rounded",
    "austria",
    "itc avant garde gothic",
    "avenir",
    "bahnschrift",
    "bank gothic",
    "bell centennial",
    "bell gothic",
    "benguiat gothic",
    "bobcaygeon",
    "breeze sans",
    "calibri",
    "candara",
    "cantarell",
    "century gothic",
    "charcoal",
    "chicago",
    "adobe clean",
    "clear sans",
    "clearview",
    "comic neue",
    "comic sans",
    "consolas",
    "corbel",
    "ff dax",
    "dejavu sans",
    "drogowskaz",
    "droid sans",
    "dubai",
    "eras",
    "eurostile",
    "fira sans",
    "folio",
    "franklin gothic",
    "frutiger",
    "futura",
    "geneva",
    "gill sans",
    "gotham",
    "haettenschweiler",
    "handel gothic",
    "helvetica",
    "highway gothic",
    "ibm plex sans",
    "impact",
    "interstate",
    "ge inspira",
    "johnston",
    "kabel",
    "klavika",
    "lato",
    "liberation sans",
    "libre franklin",
    "linux biolinum",
    "lucida grande",
    "lucida sans",
    "fs me",
    "ff meta",
    "microsoft sans serif",
    "montserrat",
    "myriad",
    "national trust",
    "news gothic",
    "neuzeit s",
    "nokia pure",
    "noto sans",
    "open sans",
    "optima",
    "overpass",
    "oxygen",
    "proxima nova",
    "radis sans",
    "rail alphabet",
    "roboto",
    "san francisco",
    "ff scala sans",
    "segoe ui",
    "selawik",
    "semplicità",
    "seravek",
    "skia",
    "oneplus slate",
    "source sans pro",
    "sweden sans",
    "syntax",
    "tahoma",
    "thesis sans",
    "tiresias",
    "trade gothic",
    "transport",
    "tratex",
    "trebuchet ms",
    "ubuntu",
    "unica",
    "univers",
    "universalis",
    "vag rounded",
    "vera sans",
    "verdana",
};

static char *serif_pp[] = {
    "adobe jenson",
    "albertus",
    "aldus",
    "alexandria",
    "american typewriter",
    "amiri",
    "archer",
    "arno",
    "aster",
    "athelas",
    "baskerville",
    "bauer bodoni",
    "bell",
    "bembo",
    "benguiat",
    "bernhard modern",
    "bodoni",
    "bookman",
    "bookerly",
    "bulmer",
    "caledonia",
    "calisto",
    "cambria",
    "caslon",
    "centaur",
    "century schoolbook",
    "chaparral",
    "bitstream charter",
    "cheltenham",
    "city",
    "clarendon",
    "cochin",
    "computer modern",
    "constantia",
    "constructium",
    "copperplate gothic",
    "dejavu serif",
    "didot",
    "droid serif",
    "emerson",
    "ff scala",
    "footlight",
    "friz quadrata",
    "itc galliard",
    "garamond",
    "gentium",
    "georgia",
    "goudy old style",
    "granjon",
    "hoefler text",
    "iowan old style",
    "janson",
    "joanna",
    "junicode",
    "korinna",
    "liberation serif",
    "linux libertine",
    "literata",
    "literaturnaya",
    "lucida bright",
    "minion",
    "mrs eaves",
    "ms serif",
    "new york",
    "nilland",
    "palatino",
    "perpetua",
    "plantin",
    "ibm plex serif",
    "rawlinson roadway",
    "requiem",
    "roboto slab",
    "rockwell",
    "rotis",
    "sabon",
    "serifa",
    "source serif pro",
    "souvenir",
    "theano didot",
    "times new roman",
    "trajan",
    "trinité",
    "trump mediaeval",
    "utopia",
    "windsor",
    "zilla slab",
};
  
static char *emoji_pp[] = {
    "noto color emoji",
};

static char *monospace_pp[] = {
    "andalé mono",
    "cascadia code",
    "consolas",
    "courier",
    "cutive mono",
    "dejavu sans mono",
    "droid sans mono",
    "everson mono",
    "fira mono",
    "fixed",
    "fixedsys",
    "freemono",
    "go mono",
    "hyperfont",
    "ibm plex mono",
    "inconsolata",
    "iosevka",
    "letter gothic",
    "liberation mono",
    "lucida console",
    "menlo",
    "monaco",
    "monofur",
    "monospace",
    "nimbus mono l",
    "noto mono",
    "ocr-a",
    "ocr-b",
    "overpass mono",
    "oxygen mono",
    "pragmatapro",
    "prestige elite",
    "profont",
    "pt mono",
    "roboto mono",
    "source code pro",
    "terminus",
    "tex gyre cursor",
    "um typewriter",
};

// FUNCTIONS =======================================================================================

NH_API_RESULT nh_gfx_parseFontFamily(
    nh_gfx_FontFamily *FontFamily_p, char *name_p)
{
    FontFamily_p->name_p = nh_core_allocateBytes(name_p);
    NH_CORE_CHECK_MEM(FontFamily_p->name_p)
    for (int i = 0; i < strlen(FontFamily_p->name_p); ++i) {
        FontFamily_p->name_p[i] = tolower(FontFamily_p->name_p[i]);
    }
   
    memset(FontFamily_p->generic_p, false, sizeof(bool)*NH_GFX_GENERIC_FONT_FAMILY_COUNT);

    for (int i = 0; i < sizeof(serif_pp)/sizeof(serif_pp[0]); ++i) {
        if (strstr(FontFamily_p->name_p, serif_pp[i])) {
            FontFamily_p->generic_p[NH_GFX_GENERIC_FONT_FAMILY_SERIF] = true;
            break;
        }
    }

    for (int i = 0; i < sizeof(sansSerif_pp)/sizeof(sansSerif_pp[0]); ++i) {
        if (strstr(FontFamily_p->name_p, sansSerif_pp[i])) {
            FontFamily_p->generic_p[NH_GFX_GENERIC_FONT_FAMILY_SANS_SERIF] = true;
            break;
        }
    }

    for (int i = 0; i < sizeof(emoji_pp)/sizeof(emoji_pp[0]); ++i) {
        if (strstr(FontFamily_p->name_p, emoji_pp[i])) {
            FontFamily_p->generic_p[NH_GFX_GENERIC_FONT_FAMILY_EMOJI] = true;
            break;
        }
    }

    for (int i = 0; i < sizeof(monospace_pp)/sizeof(monospace_pp[0]); ++i) {
        if (strstr(FontFamily_p->name_p, monospace_pp[i])) {
            FontFamily_p->generic_p[NH_GFX_GENERIC_FONT_FAMILY_MONOSPACE] = true;
            break;
        }
    }

    return NH_API_SUCCESS;
}

void nh_gfx_freeFontFamily(
    nh_gfx_FontFamily *Family_p)
{
    nh_core_free(Family_p->name_p);
    return;
}

nh_gfx_FontFamily nh_gfx_initFontFamily(
    char *name_p)
{
    nh_gfx_FontFamily Family;
    Family.name_p = name_p;
    memset(Family.generic_p, false, sizeof(bool)*NH_GFX_GENERIC_FONT_FAMILY_COUNT);
    return Family;
}

