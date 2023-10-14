#ifndef NH_URL_IDNA_MAPPING_TABLE_H
#define NH_URL_IDNA_MAPPING_TABLE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhurl_enums
 *  @{
 */

    typedef enum NH_URL_IDNA_STATUS {
        NH_URL_IDNA_STATUS_UNDEFINED,
        NH_URL_IDNA_STATUS_VALID, 
        NH_URL_IDNA_STATUS_IGNORED, 
        NH_URL_IDNA_STATUS_MAPPED, 
        NH_URL_IDNA_STATUS_DEVIATION, 
        NH_URL_IDNA_STATUS_DISALLOWED, 
        NH_URL_IDNA_STATUS_DISALLOWED_STD3_VALID,
        NH_URL_IDNA_STATUS_DISALLOWED_STD3_MAPPED,
    } NH_URL_IDNA_STATUS;

    typedef enum NH_URL_IDNA_2008_STATUS {
        NH_URL_IDNA_2008_STATUS_NONE, 
        NH_URL_IDNA_2008_STATUS_NV8, 
        NH_URL_IDNA_2008_STATUS_XV8, 
    } NH_URL_IDNA_2008_STATUS;

/** @} */

/** @addtogroup lib_nhurl_structs
 *  @{
 */

    typedef struct nh_url_IDNAMappingTableEntry {
        NH_ENCODING_UTF32 begin, end;
        NH_URL_IDNA_STATUS status;
        nh_encoding_UTF32String Mapping;
        NH_URL_IDNA_2008_STATUS status2008;
    } nh_url_IDNAMappingTableEntry;

    typedef struct nh_url_IDNAMappingTable {
        nh_Array Entries; 
    } nh_url_IDNAMappingTable;

/** @} */

/** @addtogroup lib_nhurl_vars
 *  @{
 */

    extern nh_url_IDNAMappingTable NH_URL_IDNA_MAPPING_TABLE;

/** @} */

/** @addtogroup lib_nhurl_functions
 *  @{
 */

    NH_URL_RESULT nh_url_parseIDNAMappingTable(
    );

    nh_url_IDNAMappingTableEntry *nh_url_getIDNAMappingTableEntry(
        NH_ENCODING_UTF32 codepoint
    );

/** @} */

#endif
