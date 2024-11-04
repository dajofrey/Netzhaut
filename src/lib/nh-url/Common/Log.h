#ifndef NH_URL_COMMON_LOG_H
#define NH_URL_COMMON_LOG_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Includes.h"
#include "../IDNA/IDNAMappingTable.h"

NH_API_RESULT nh_url_logIDNAMappingTable(
    nh_url_IDNAMappingTable *MappingTable_p
);

#endif // NH_URL_COMMON_LOG_H 
