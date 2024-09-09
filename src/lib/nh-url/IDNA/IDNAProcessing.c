// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "IDNAProcessing.h"
#include "IDNAMappingTable.h"

#include "../Common/Macros.h"

#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-core/Util/Array.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// TO ASCII ========================================================================================

static NH_URL_RESULT nh_url_mapNewCodepoints(
    nh_encoding_UTF32String *Input_p, int i, nh_encoding_UTF32String ToBeMapped)
{
NH_URL_BEGIN()

    nh_encoding_removeUTF32(Input_p, i, 1);

    if (ToBeMapped.length > 0) {
        nh_encoding_insertUTF32(Input_p, i, ToBeMapped.p, ToBeMapped.length);
    }

NH_URL_END(NH_URL_SUCCESS)
}

// https://www.unicode.org/reports/tr46/#ToASCII
NH_URL_RESULT nh_url_unicodeToASCII(
    nh_encoding_UTF32String *DomainName_p, NH_BOOL transitionalProcessing)
{
NH_URL_BEGIN()

    for (int i = 0; i < DomainName_p->length; ++i) 
    {
        nh_url_IDNAMappingTableEntry *Entry_p = nh_url_getIDNAMappingTableEntry(DomainName_p->p[i]);
        NH_URL_CHECK_NULL(Entry_p)

        switch (Entry_p->status)
        {
            case NH_URL_IDNA_STATUS_UNDEFINED  : NH_URL_END(NH_URL_ERROR_BAD_STATE)
            case NH_URL_IDNA_STATUS_VALID : break;
            case NH_URL_IDNA_STATUS_DISALLOWED : break; // error 
            case NH_URL_IDNA_STATUS_DISALLOWED_STD3_VALID : break;
            case NH_URL_IDNA_STATUS_DISALLOWED_STD3_MAPPED : break;

            case NH_URL_IDNA_STATUS_IGNORED : 
                nh_url_mapNewCodepoints(DomainName_p, i--, nh_encoding_initUTF32(8));
                break;
            case NH_URL_IDNA_STATUS_MAPPED :
                nh_url_mapNewCodepoints(DomainName_p, i, Entry_p->Mapping);
                break;
            case NH_URL_IDNA_STATUS_DEVIATION : 
                if (transitionalProcessing) {
                    nh_url_mapNewCodepoints(DomainName_p, i, Entry_p->Mapping);
                }
                break;
        }
    }

    // TODO normalize
    // TODO break 
    // TODO convert/validate

NH_URL_END(NH_URL_SUCCESS)
}

