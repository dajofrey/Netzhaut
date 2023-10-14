// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "IDNAMappingTable.h"

#include "../Common/Data/IdnaMappingTable.txt.inc"
#include "../Common/Macros.h"

#include "../../nhencoding/Encodings/UTF32.h"
#include "../../nhcore/Util/Array.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// DATA ============================================================================================

nh_url_IDNAMappingTable NH_URL_IDNA_MAPPING_TABLE;

// DECLARE =========================================================================================

typedef struct nh_url_IDNAMappingTableParser {
    nh_url_IDNAMappingTableEntry *Entry_p;
    NH_BYTE *p;
    int expect;
} nh_url_IDNAMappingTableParser;

// PARSE ===========================================================================================

static NH_URL_RESULT nh_url_parseCodepoints(
    nh_url_IDNAMappingTableParser *Parser_p)
{
NH_URL_BEGIN()

    NH_BYTE *p = Parser_p->p;
    NH_BOOL seekFirst = NH_TRUE, seekSecond = NH_FALSE;

    while (*p != ';' && *p != ' ') {
        if (seekFirst && nh_encoding_isASCIIHexDigit(*p)) {
            Parser_p->Entry_p->begin = (int)strtol(p, NULL, 16);
            seekFirst = NH_FALSE;
        }
        if (seekSecond && *p != '.') {
            Parser_p->Entry_p->end = (int)strtol(p, NULL, 16);
            break;
        }
        if (*p == '.') {
            seekSecond = NH_TRUE;
        }
        p++;
    }

NH_URL_END(NH_URL_SUCCESS)
}

static NH_URL_RESULT nh_url_parseStatus(
    nh_url_IDNAMappingTableParser *Parser_p)
{
NH_URL_BEGIN()

    switch (Parser_p->p[1])
    {
        case 'v' :
            Parser_p->Entry_p->status = NH_URL_IDNA_STATUS_VALID;
            break;
        case 'i' :
            Parser_p->Entry_p->status = NH_URL_IDNA_STATUS_IGNORED;
            break;
        case 'm' :
            Parser_p->Entry_p->status = NH_URL_IDNA_STATUS_MAPPED;
            break;
        case 'd' :
            if (Parser_p->p[1] == 'e') {
                Parser_p->Entry_p->status = NH_URL_IDNA_STATUS_DEVIATION;
            }
            else if (Parser_p->p[1] == 'i') {
                if (Parser_p->p[10] == '_' && Parser_p->p[16] == 'v') {
                    Parser_p->Entry_p->status = NH_URL_IDNA_STATUS_DISALLOWED_STD3_VALID;
                }
                else if (Parser_p->p[10] == '_' && Parser_p->p[16] == 'm') {
                    Parser_p->Entry_p->status = NH_URL_IDNA_STATUS_DISALLOWED_STD3_MAPPED;
                }
                else {
                    Parser_p->Entry_p->status = NH_URL_IDNA_STATUS_DISALLOWED;
                }
            }
            else {NH_URL_END(NH_URL_ERROR_BAD_STATE)}
            break;
        default : NH_URL_END(NH_URL_ERROR_BAD_STATE)
    }

NH_URL_END(NH_URL_SUCCESS)
}

static NH_URL_RESULT nh_url_parseMapping(
    nh_url_IDNAMappingTableParser *Parser_p)
{
NH_URL_BEGIN()

    NH_BYTE *p = Parser_p->p;

    while (*p != ';' && *p != '#' && *p != '\n') {
        if (nh_encoding_isASCIIHexDigit(*p)) {
            nh_encoding_appendUTF32Codepoint(&Parser_p->Entry_p->Mapping, (NH_ENCODING_UTF32)strtol(p, NULL, 16));
            while (nh_encoding_isASCIIHexDigit(*p)) {++p;}
        }
        p++;
    }

NH_URL_END(NH_URL_SUCCESS)
}

static NH_URL_RESULT nh_url_parseIDNA2008Status(
    nh_url_IDNAMappingTableParser *Parser_p)
{
NH_URL_BEGIN()
NH_URL_END(NH_URL_SUCCESS)
}

// PARSE ===========================================================================================

static nh_url_IDNAMappingTableParser nh_url_initIDNAMappingTableParser()
{
NH_URL_BEGIN()

    nh_url_IDNAMappingTableParser Parser;
    Parser.Entry_p = NULL;
    Parser.expect = 0;
    Parser.p = idna_mapping_table_txt_inc;

NH_URL_END(Parser)
}

static NH_URL_RESULT nh_url_parseNextField(
    nh_url_IDNAMappingTableParser *Parser_p)
{
NH_URL_BEGIN()

    while (*(Parser_p->p) == '#') {
        while (*(Parser_p->p) != '\n') {++(Parser_p->p);}
        ++(Parser_p->p);
    }

    switch (Parser_p->expect) 
    {
        case 0 : nh_url_parseCodepoints(Parser_p); break;;
        case 1 : nh_url_parseStatus(Parser_p); break;
        case 2 : nh_url_parseMapping(Parser_p); break;
        case 3 : nh_url_parseIDNA2008Status(Parser_p); break;
    }

    NH_BYTE *p = Parser_p->p;
    while (*p != 0 && *p != ';' && *p != '\n') {p++;}

    if (*p == '\n') {Parser_p->expect = 0;}
    else {Parser_p->expect++;}

    if (*p) {p++;}

    Parser_p->p = p;

NH_URL_END(NH_URL_SUCCESS)
}

// https://www.unicode.org/reports/tr46/#IDNA_Mapping_Table
NH_URL_RESULT nh_url_parseIDNAMappingTable()
{
NH_URL_BEGIN()

    NH_URL_IDNA_MAPPING_TABLE.Entries = nh_core_initArray(sizeof(nh_url_IDNAMappingTableEntry), 9000);

    nh_url_IDNAMappingTableParser Parser = nh_url_initIDNAMappingTableParser();

    while (*Parser.p)
    {
        if (Parser.expect == 0) {
            Parser.Entry_p = nh_core_incrementArray(&NH_URL_IDNA_MAPPING_TABLE.Entries);
            NH_URL_CHECK_MEM(Parser.Entry_p)
            Parser.Entry_p->begin = 0;
            Parser.Entry_p->end = 0;
            Parser.Entry_p->status = NH_URL_IDNA_STATUS_UNDEFINED;
            Parser.Entry_p->Mapping = nh_encoding_initUTF32(2);
            Parser.Entry_p->status2008 = 0;
        }
        NH_URL_CHECK(nh_url_parseNextField(&Parser))
    }

NH_URL_END(NH_URL_SUCCESS)
}

// GET =============================================================================================

nh_url_IDNAMappingTableEntry *nh_url_getIDNAMappingTableEntry(
    NH_ENCODING_UTF32 codepoint)
{
NH_URL_BEGIN()

    for (int i = 0; i < NH_URL_IDNA_MAPPING_TABLE.Entries.length; ++i) {
        nh_url_IDNAMappingTableEntry *Entry_p = 
            &((nh_url_IDNAMappingTableEntry*)NH_URL_IDNA_MAPPING_TABLE.Entries.p)[i];
        if (codepoint == Entry_p->begin) {
            NH_URL_END(Entry_p)
        }
        else if (Entry_p->end && codepoint >= Entry_p->begin && codepoint <= Entry_p->end) {
            NH_URL_END(Entry_p)
        }
    }

NH_URL_END(NULL)
}

