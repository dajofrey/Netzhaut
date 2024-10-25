// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Encodings.h"
#include "../Common/IndexMap.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// LABELS ==========================================================================================

// https://encoding.spec.whatwg.org/#names-and-labels
const char *NH_ENCODING_LABELS_PP[] = 
{
    "unicode-1-1-utf-8",
    "unicode11utf8",
    "unicode20utf8",
    "utf-8",
    "utf8",
    "x-unicode20utf8",
    "866",
    "cp866",
    "csibm866",
    "ibm866",
    "csisolatin2",
    "iso-8859-2",
    "iso-ir-101",
    "iso8859-2",
    "iso88592",
    "iso_8859-2",
    "iso_8859-2:1987",
    "l2",
    "latin2",
    "csisolatin3",
    "iso-8859-3",
    "iso-ir-109",
    "iso8859-3",
    "iso88593",
    "iso_8859-3",
    "iso_8859-3:1988",
    "l3",
    "latin3",
    "csisolatin4",
    "iso-8859-4",
    "iso-ir-110",
    "iso8859-4",
    "iso88594",
    "iso_8859-4",
    "iso_8859-4:1988",
    "l4",
    "latin4",
    "csisolatincyrillic",
    "cyrillic",
    "iso-8859-5",
    "iso-ir-144",
    "iso8859-5",
    "iso88595",
    "iso_8859-5",
    "iso_8859-5:1988",
    "arabic",
    "asmo-708",
    "csiso88596e",
    "csiso88596i",
    "csisolatinarabic",
    "ecma-114",
    "iso-8859-6",
    "iso-8859-6-e",
    "iso-8859-6-i",
    "iso-ir-127",
    "iso8859-6",
    "iso88596",
    "iso_8859-6",
    "iso_8859-6:1987",
    "csisolatingreek",
    "ecma-118",
    "elot_928",
    "greek",
    "greek8",
    "iso-8859-7",
    "iso-ir-126",
    "iso8859-7",
    "iso88597",
    "iso_8859-7",
    "iso_8859-7:1987",
    "sun_eu_greek",
    "csiso88598e",
    "csisolatinhebrew",
    "hebrew",
    "iso-8859-8",
    "iso-8859-8-e",
    "iso-ir-138",
    "iso8859-8",
    "iso88598",
    "iso_8859-8",
    "iso_8859-8:1988",
    "visual",
    "csiso88598i",
    "iso-8859-8-i",
    "logical",
    "csisolatin6",
    "iso-8859-10",
    "iso-ir-157",
    "iso8859-10",
    "iso885910",
    "l6",
    "latin6",
    "iso-8859-13",
    "iso8859-13",
    "iso885913",
    "iso-8859-14",
    "iso8859-14",
    "iso885914",
    "csisolatin9",
    "iso-8859-15",
    "iso8859-15",
    "iso885915",
    "iso_8859-15",
    "l9",
    "iso-8859-16",
    "cskoi8r",
    "koi",
    "koi8",
    "koi8-r",
    "koi8_r",
    "koi8-ru",
    "koi8-u",
    "csmacintosh",
    "mac",
    "macintosh",
    "x-mac-roman",
    "dos-874",
    "iso-8859-11",
    "iso8859-11",
    "iso885911",
    "tis-620",
    "windows-874",
    "cp1250",
    "windows-1250",
    "x-cp1250",
    "cp1251",
    "windows-1251",
    "x-cp1251",
    "ansi_x3.4-1968",
    "ascii",
    "cp1252",
    "cp819",
    "csisolatin1",
    "ibm819",
    "iso-8859-1",
    "iso-ir-100",
    "iso8859-1",
    "iso88591",
    "iso_8859-1",
    "iso_8859-1:1987",
    "l1",
    "latin1",
    "us-ascii",
    "windows-1252",
    "x-cp1252",
    "cp1253",
    "windows-1253",
    "x-cp1253",
    "cp1254",
    "csisolatin5",
    "iso-8859-9",
    "iso-ir-148",
    "iso8859-9",
    "iso88599",
    "iso_8859-9",
    "iso_8859-9:1989",
    "l5",
    "latin5",
    "windows-1254",
    "x-cp1254",
    "cp1255",
    "windows-1255",
    "x-cp1255",
    "cp1256",
    "windows-1256",
    "x-cp1256",
    "cp1257",
    "windows-1257",
    "x-cp1257",
    "cp1258",
    "windows-1258",
    "x-cp1258",
    "x-mac-cyrillic",
    "x-mac-ukrainian",
    "chinese",
    "csgb2312",
    "csiso58gb231280",
    "gb2312",
    "gb_2312",
    "gb_2312-80",
    "gbk",
    "iso-ir-58",
    "x-gbk",
    "gb18030",
    "big5",
    "big5-hkscs",
    "cn-big5",
    "csbig5",
    "x-x-big5",
    "cseucpkdfmtjapanese",
    "euc-jp",
    "x-euc-jp",
    "csiso2022jp",
    "iso-2022-jp",
    "csshiftjis",
    "ms932",
    "ms_kanji",
    "shift-jis",
    "shift_jis",
    "sjis",
    "windows-31j",
    "x-sjis",
    "cseuckr",
    "csksc56011987",
    "euc-kr",
    "iso-ir-149",
    "korean",
    "ks_c_5601-1987",
    "ks_c_5601-1989",
    "ksc5601",
    "ksc_5601",
    "windows-949",
    "csiso2022kr",
    "hz-gb-2312",
    "iso-2022-cn",
    "iso-2022-cn-ext",
    "iso-2022-kr",
    "replacement",
    "unicodefffe",
    "utf-16be",
    "csunicode",
    "iso-10646-ucs-2",
    "ucs-2",
    "unicode",
    "unicodefeff",
    "utf-16",
    "utf-16le",
    "x-user-defined",
};

int NH_ENCODING_LABELS_PP_COUNT = sizeof(NH_ENCODING_LABELS_PP)/sizeof(NH_ENCODING_LABELS_PP[0]);

// GET =============================================================================================

// https://encoding.spec.whatwg.org/#concept-encoding-get
NH_ENCODING_NAME nh_encoding_getEncoding(
    char *label_p)
{
    char lowerlabel_p[255] = {'\0'};
    strcpy(lowerlabel_p, label_p);

    for (int i = 0; i < strlen(lowerlabel_p); ++i) {
        lowerlabel_p[i] = tolower(lowerlabel_p[i]);
    }

    NH_ENCODING_NAME name = NH_ENCODING_NAME_UNDEFINED;

    unsigned int *labelIndex_p = nh_core_getFromHashMap(&NH_ENCODING_INDEXMAP.Labels, lowerlabel_p);
    if (!labelIndex_p) {return name;}

    switch (*labelIndex_p)
    {
        case NH_ENCODING_LABEL_UNICODE_1_1_UTF_8   :
        case NH_ENCODING_LABEL_UNICODE11UTF8       :
        case NH_ENCODING_LABEL_UNICODE20UTF8       :
        case NH_ENCODING_LABEL_UTF_8               :
        case NH_ENCODING_LABEL_UTF8                :
        case NH_ENCODING_LABEL_X_UNICODE20UTF8     : name = NH_ENCODING_NAME_UTF_8; break;
        case NH_ENCODING_LABEL_866                 :
        case NH_ENCODING_LABEL_CP866               :
        case NH_ENCODING_LABEL_CSIBM866            :
        case NH_ENCODING_LABEL_IBM866              : name = NH_ENCODING_NAME_IBM866; break;
        case NH_ENCODING_LABEL_CSISOLATIN2         :
        case NH_ENCODING_LABEL_ISO_8859_2          :
        case NH_ENCODING_LABEL_ISO_IR_101          :
        case NH_ENCODING_LABEL_ISO8859_2           :
        case NH_ENCODING_LABEL_ISO88592            :
        case NH_ENCODING_LABEL_ISO_8859_2_         :
        case NH_ENCODING_LABEL_ISO_8859_2_1987     :
        case NH_ENCODING_LABEL_L2                  :
        case NH_ENCODING_LABEL_LATIN2              : name = NH_ENCODING_NAME_ISO_8859_2; break;
        case NH_ENCODING_LABEL_CSISOLATIN3         :
        case NH_ENCODING_LABEL_ISO_8859_3          :
        case NH_ENCODING_LABEL_ISO_IR_109          :
        case NH_ENCODING_LABEL_ISO8859_3           :
        case NH_ENCODING_LABEL_ISO88593            :
        case NH_ENCODING_LABEL_ISO_8859_3_         :
        case NH_ENCODING_LABEL_ISO_8859_3_1988     :
        case NH_ENCODING_LABEL_L3                  :
        case NH_ENCODING_LABEL_LATIN3              : name = NH_ENCODING_NAME_ISO_8859_3; break;
        case NH_ENCODING_LABEL_CSISOLATIN4         :
        case NH_ENCODING_LABEL_ISO_8859_4          :
        case NH_ENCODING_LABEL_ISO_IR_110          :
        case NH_ENCODING_LABEL_ISO8859_4           :
        case NH_ENCODING_LABEL_ISO88594            :
        case NH_ENCODING_LABEL_ISO_8859_4_         :
        case NH_ENCODING_LABEL_ISO_8859_4_1988     :
        case NH_ENCODING_LABEL_L4                  :
        case NH_ENCODING_LABEL_LATIN4              : name = NH_ENCODING_NAME_ISO_8859_4; break;
        case NH_ENCODING_LABEL_CSISOLATINCYRILLIC  :
        case NH_ENCODING_LABEL_CYRILLIC            :
        case NH_ENCODING_LABEL_ISO_8859_5          :
        case NH_ENCODING_LABEL_ISO_IR_144          :
        case NH_ENCODING_LABEL_ISO8859_5           :
        case NH_ENCODING_LABEL_ISO88595            :
        case NH_ENCODING_LABEL_ISO_8859_5_         :
        case NH_ENCODING_LABEL_ISO_8859_5_1988     : name = NH_ENCODING_NAME_ISO_8859_5; break;
        case NH_ENCODING_LABEL_ARABIC              :
        case NH_ENCODING_LABEL_ASMO_708            :
        case NH_ENCODING_LABEL_CSISO88596E         :
        case NH_ENCODING_LABEL_CSISO88596I         :
        case NH_ENCODING_LABEL_CSISOLATINARABIC    :
        case NH_ENCODING_LABEL_ECMA_114            :
        case NH_ENCODING_LABEL_ISO_8859_6          :
        case NH_ENCODING_LABEL_ISO_8859_6_E        :
        case NH_ENCODING_LABEL_ISO_8859_6_I        :
        case NH_ENCODING_LABEL_ISO_IR_127          :
        case NH_ENCODING_LABEL_ISO8859_6           :
        case NH_ENCODING_LABEL_ISO88596            :
        case NH_ENCODING_LABEL_ISO_8859_6_         :
        case NH_ENCODING_LABEL_ISO_8859_6_1987     : name = NH_ENCODING_NAME_ISO_8859_6; break;
        case NH_ENCODING_LABEL_CSISOLATINGREEK     :
        case NH_ENCODING_LABEL_ECMA_118            :
        case NH_ENCODING_LABEL_ELOT_928            :
        case NH_ENCODING_LABEL_GREEK               :
        case NH_ENCODING_LABEL_GREEK8              :
        case NH_ENCODING_LABEL_ISO_8859_7          :
        case NH_ENCODING_LABEL_ISO_IR_126          :
        case NH_ENCODING_LABEL_ISO8859_7           :
        case NH_ENCODING_LABEL_ISO88597            :
        case NH_ENCODING_LABEL_ISO_8859_7_         :
        case NH_ENCODING_LABEL_ISO_8859_7_1987     :
        case NH_ENCODING_LABEL_SUN_EU_GREEK        : name = NH_ENCODING_NAME_ISO_8859_7; break;
        case NH_ENCODING_LABEL_CSISO88598E         :
        case NH_ENCODING_LABEL_CSISOLATINHEBREW    :
        case NH_ENCODING_LABEL_HEBREW              :
        case NH_ENCODING_LABEL_ISO_8859_8          :
        case NH_ENCODING_LABEL_ISO_8859_8_E        :
        case NH_ENCODING_LABEL_ISO_IR_138          :
        case NH_ENCODING_LABEL_ISO8859_8           :
        case NH_ENCODING_LABEL_ISO88598            :
        case NH_ENCODING_LABEL_ISO_8859_8_         :
        case NH_ENCODING_LABEL_ISO_8859_8_1988     :
        case NH_ENCODING_LABEL_VISUAL              : name = NH_ENCODING_NAME_ISO_8859_8; break;
        case NH_ENCODING_LABEL_CSISO88598I         :
        case NH_ENCODING_LABEL_ISO_8859_8_I        :
        case NH_ENCODING_LABEL_LOGICAL             : name = NH_ENCODING_NAME_ISO_8859_8_I; break;
        case NH_ENCODING_LABEL_CSISOLATIN6         :
        case NH_ENCODING_LABEL_ISO_8859_10         :
        case NH_ENCODING_LABEL_ISO_IR_157          :
        case NH_ENCODING_LABEL_ISO8859_10          :
        case NH_ENCODING_LABEL_ISO885910           :
        case NH_ENCODING_LABEL_L6                  :
        case NH_ENCODING_LABEL_LATIN6              : name = NH_ENCODING_NAME_ISO_8859_10; break;
        case NH_ENCODING_LABEL_ISO_8859_13         :
        case NH_ENCODING_LABEL_ISO8859_13          :
        case NH_ENCODING_LABEL_ISO885913           : name = NH_ENCODING_NAME_ISO_8859_13; break;
        case NH_ENCODING_LABEL_ISO_8859_14         :
        case NH_ENCODING_LABEL_ISO8859_14          :
        case NH_ENCODING_LABEL_ISO885914           : name = NH_ENCODING_NAME_ISO_8859_14; break;
        case NH_ENCODING_LABEL_CSISOLATIN9         :
        case NH_ENCODING_LABEL_ISO_8859_15         :
        case NH_ENCODING_LABEL_ISO8859_15          :
        case NH_ENCODING_LABEL_ISO885915           :
        case NH_ENCODING_LABEL_ISO_8859_15_        :
        case NH_ENCODING_LABEL_L9                  : name = NH_ENCODING_NAME_ISO_8859_15; break;
        case NH_ENCODING_LABEL_ISO_8859_16         : name = NH_ENCODING_NAME_ISO_8859_16; break;
        case NH_ENCODING_LABEL_CSKOI8R             :
        case NH_ENCODING_LABEL_KOI                 :
        case NH_ENCODING_LABEL_KOI8                :
        case NH_ENCODING_LABEL_KOI8_R              :
        case NH_ENCODING_LABEL_KOI8_R_             : name = NH_ENCODING_NAME_KOI8_R; break;
        case NH_ENCODING_LABEL_KOI8_RU             :
        case NH_ENCODING_LABEL_KOI8_U              : name = NH_ENCODING_NAME_KOI8_U; break;
        case NH_ENCODING_LABEL_CSMACINTOSH         :
        case NH_ENCODING_LABEL_MAC                 :
        case NH_ENCODING_LABEL_MACINTOSH           :
        case NH_ENCODING_LABEL_X_MAC_ROMAN         : name = NH_ENCODING_NAME_MACINTOSH; break;
        case NH_ENCODING_LABEL_DOS_874             :
        case NH_ENCODING_LABEL_ISO_8859_11         :
        case NH_ENCODING_LABEL_ISO8859_11          :
        case NH_ENCODING_LABEL_ISO885911           :
        case NH_ENCODING_LABEL_TIS_620             :
        case NH_ENCODING_LABEL_WINDOWS_874         : name = NH_ENCODING_NAME_WINDOWS_874; break;
        case NH_ENCODING_LABEL_CP1250              :
        case NH_ENCODING_LABEL_WINDOWS_1250        :
        case NH_ENCODING_LABEL_X_CP1250            : name = NH_ENCODING_NAME_WINDOWS_1250; break;
        case NH_ENCODING_LABEL_CP1251              :
        case NH_ENCODING_LABEL_WINDOWS_1251        :
        case NH_ENCODING_LABEL_X_CP1251            : name = NH_ENCODING_NAME_WINDOWS_1251; break;
        case NH_ENCODING_LABEL_ANSI_X3_4_1968      :
        case NH_ENCODING_LABEL_ASCII               :
        case NH_ENCODING_LABEL_CP1252              :
        case NH_ENCODING_LABEL_CP819               :
        case NH_ENCODING_LABEL_CSISOLATIN1         :
        case NH_ENCODING_LABEL_IBM819              :
        case NH_ENCODING_LABEL_ISO_8859_1          :
        case NH_ENCODING_LABEL_ISO_IR_100          :
        case NH_ENCODING_LABEL_ISO8859_1           :
        case NH_ENCODING_LABEL_ISO88591            :
        case NH_ENCODING_LABEL_ISO_8859_1_         :
        case NH_ENCODING_LABEL_ISO_8859_1_1987     :
        case NH_ENCODING_LABEL_L1                  :
        case NH_ENCODING_LABEL_LATIN1              :
        case NH_ENCODING_LABEL_US_ASCII            :
        case NH_ENCODING_LABEL_WINDOWS_1252        :
        case NH_ENCODING_LABEL_X_CP1252            : name = NH_ENCODING_NAME_WINDOWS_1252; break;
        case NH_ENCODING_LABEL_CP1253              :
        case NH_ENCODING_LABEL_WINDOWS_1253        :
        case NH_ENCODING_LABEL_X_CP1253            : name = NH_ENCODING_NAME_WINDOWS_1253; break;
        case NH_ENCODING_LABEL_CP1254              :
        case NH_ENCODING_LABEL_CSISOLATIN5         :
        case NH_ENCODING_LABEL_ISO_8859_9          :
        case NH_ENCODING_LABEL_ISO_IR_148          :
        case NH_ENCODING_LABEL_ISO8859_9           :
        case NH_ENCODING_LABEL_ISO88599            :
        case NH_ENCODING_LABEL_ISO_8859_9_         :
        case NH_ENCODING_LABEL_ISO_8859_9_1989     :
        case NH_ENCODING_LABEL_L5                  :
        case NH_ENCODING_LABEL_LATIN5              :
        case NH_ENCODING_LABEL_WINDOWS_1254        :
        case NH_ENCODING_LABEL_X_CP1254            : name = NH_ENCODING_NAME_WINDOWS_1254; break;
        case NH_ENCODING_LABEL_CP1255              :
        case NH_ENCODING_LABEL_WINDOWS_1255        :
        case NH_ENCODING_LABEL_X_CP1255            : name = NH_ENCODING_NAME_WINDOWS_1255; break;
        case NH_ENCODING_LABEL_CP1256              :
        case NH_ENCODING_LABEL_WINDOWS_1256        :
        case NH_ENCODING_LABEL_X_CP1256            : name = NH_ENCODING_NAME_WINDOWS_1256; break;
        case NH_ENCODING_LABEL_CP1257              :
        case NH_ENCODING_LABEL_WINDOWS_1257        :
        case NH_ENCODING_LABEL_X_CP1257            : name = NH_ENCODING_NAME_WINDOWS_1257; break;
        case NH_ENCODING_LABEL_CP1258              :
        case NH_ENCODING_LABEL_WINDOWS_1258        :
        case NH_ENCODING_LABEL_X_CP1258            : name = NH_ENCODING_NAME_WINDOWS_1258; break;
        case NH_ENCODING_LABEL_X_MAC_CYRILLIC      :
        case NH_ENCODING_LABEL_X_MAC_UKRAINIAN     : name = NH_ENCODING_NAME_X_MAC_CYRILLIC; break;
        case NH_ENCODING_LABEL_CHINESE             :
        case NH_ENCODING_LABEL_CSGB2312            :
        case NH_ENCODING_LABEL_CSISO58GB231280     :
        case NH_ENCODING_LABEL_GB2312              :
        case NH_ENCODING_LABEL_GB_2312             :
        case NH_ENCODING_LABEL_GB_2312_80          :
        case NH_ENCODING_LABEL_GBK                 :
        case NH_ENCODING_LABEL_ISO_IR_58           :
        case NH_ENCODING_LABEL_X_GBK               : name = NH_ENCODING_NAME_GBK; break;
        case NH_ENCODING_LABEL_GB18030             : name = NH_ENCODING_NAME_GB18030; break;
        case NH_ENCODING_LABEL_BIG5                :
        case NH_ENCODING_LABEL_BIG5_HKSCS          :
        case NH_ENCODING_LABEL_CN_BIG5             :
        case NH_ENCODING_LABEL_CSBIG5              :
        case NH_ENCODING_LABEL_X_X_BIG5            : name = NH_ENCODING_NAME_BIG5; break;
        case NH_ENCODING_LABEL_CSEUCPKDFMTJAPANESE :
        case NH_ENCODING_LABEL_EUC_JP              :
        case NH_ENCODING_LABEL_X_EUC_JP            : name = NH_ENCODING_NAME_EUC_JP; break;
        case NH_ENCODING_LABEL_CSISO2022JP         :
        case NH_ENCODING_LABEL_ISO_2022_JP         : name = NH_ENCODING_NAME_ISO_2022_JP; break;
        case NH_ENCODING_LABEL_CSSHIFTJIS          :
        case NH_ENCODING_LABEL_MS932               :
        case NH_ENCODING_LABEL_MS_KANJI            :
        case NH_ENCODING_LABEL_SHIFT_JIS           :
        case NH_ENCODING_LABEL_SHIFT_JIS_          :
        case NH_ENCODING_LABEL_SJIS                :
        case NH_ENCODING_LABEL_WINDOWS_31J         :
        case NH_ENCODING_LABEL_X_SJIS              : name = NH_ENCODING_NAME_SHIFT_JIS; break;
        case NH_ENCODING_LABEL_CSEUCKR             :
        case NH_ENCODING_LABEL_CSKSC56011987       :
        case NH_ENCODING_LABEL_EUC_KR              :
        case NH_ENCODING_LABEL_ISO_IR_149          :
        case NH_ENCODING_LABEL_KOREAN              :
        case NH_ENCODING_LABEL_KS_C_5601_1987      :
        case NH_ENCODING_LABEL_KS_C_5601_1989      :
        case NH_ENCODING_LABEL_KSC5601             :
        case NH_ENCODING_LABEL_KSC_5601            :
        case NH_ENCODING_LABEL_WINDOWS_949         : name = NH_ENCODING_NAME_EUC_KR; break;
        case NH_ENCODING_LABEL_CSISO2022KR         :
        case NH_ENCODING_LABEL_HZ_GB_2312          :
        case NH_ENCODING_LABEL_ISO_2022_CN         :
        case NH_ENCODING_LABEL_ISO_2022_CN_EXT     :
        case NH_ENCODING_LABEL_ISO_2022_KR         :
        case NH_ENCODING_LABEL_REPLACEMENT         : name = NH_ENCODING_NAME_REPLACEMENT; break;
        case NH_ENCODING_LABEL_UNICODEFFFE         :
        case NH_ENCODING_LABEL_UTF_16BE            : name = NH_ENCODING_NAME_UTF_16BE; break;
        case NH_ENCODING_LABEL_CSUNICODE           :
        case NH_ENCODING_LABEL_ISO_10646_UCS_2     :
        case NH_ENCODING_LABEL_UCS_2               :
        case NH_ENCODING_LABEL_UNICODE             :
        case NH_ENCODING_LABEL_UNICODEFEFF         :
        case NH_ENCODING_LABEL_UTF_16              :
        case NH_ENCODING_LABEL_UTF_16LE            : name = NH_ENCODING_NAME_UTF_16LE; break;
        case NH_ENCODING_LABEL_X_USER_DEFINED      : name = NH_ENCODING_NAME_X_USER_DEFINED; break;
    }

    return name;
}

// https://encoding.spec.whatwg.org/#output-encodings
NH_ENCODING_NAME nh_encoding_getOutputEncoding(
    NH_ENCODING_NAME encoding)
{
    if (encoding == NH_ENCODING_NAME_REPLACEMENT || encoding == NH_ENCODING_NAME_UTF_16BE || encoding == NH_ENCODING_NAME_UTF_16LE) {
        return NH_ENCODING_NAME_UTF_8;
    }

    return encoding;
}

