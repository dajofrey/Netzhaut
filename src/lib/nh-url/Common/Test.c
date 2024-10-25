// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Test.h"
#include "Macros.h"

#include "../Main/Helper.h"
#include "../Main/HostParser.h"
#include "../Main/URLParser.h"

#include "../../nh-encoding/Encodings/UTF8.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

// TEST =============================================================================================

static void nh_url_printHost(
    nh_url_Host *Host_p, char *output_p)
{
    sprintf(output_p+strlen(output_p), "HOST("); 

    switch (Host_p->type)
    {
        case NH_URL_HOST_IPV6_ADDRESS :
            sprintf(output_p+strlen(output_p), "IPV6(");
            for (int i = 0; i < 8; ++i) {
                sprintf(output_p+strlen(output_p), "%d", Host_p->IPv6.pieces_p[i]);
                if (i+1<8) {output_p[strlen(output_p)] = ',';}
            }
            output_p[strlen(output_p)] = ')';
            break;
        case NH_URL_HOST_IPV4_ADDRESS:
            sprintf(output_p+strlen(output_p), "IPV4(");
            sprintf(output_p+strlen(output_p), "%" PRIu32 ")", Host_p->IPv4.address);
            break;
        case NH_URL_HOST_OPAQUE:
        case NH_URL_HOST_DOMAIN :
            sprintf(output_p+strlen(output_p), "OPAQUE(");
            sprintf(output_p+strlen(output_p), "%s)", Host_p->DomainOrOpaque.String.p);
            break;
    }

    sprintf(output_p+strlen(output_p), ")"); 
}

//int nh_url_testHostParser(
//    int arguments, nh_make_TestArgument *Arguments_p, char *output_p, int maxOutputLength)
//{
//    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(Arguments_p->p, strlen(Arguments_p->p), NULL);
//
//    nh_url_Host Host;
//    nh_url_parseHost(String, !strcmp(Arguments_p[1].p, "isNotSpecial") ? true : false, &Host);
//
//    nh_url_printHost(&Host, output_p);
//
//    return 0;
//}
//
//int nh_url_testURLParser(
//    int arguments, nh_make_TestArgument *Arguments_p, char *output_p, int maxOutputLength)
//{
//    nh_encoding_UTF32String String = nh_encoding_decodeUTF8(Arguments_p->p, strlen(Arguments_p->p), NULL);
//
//    nh_url_URL URL;
//    NH_CORE_CHECK_NULL(nh_url_parseBasicURL(String, NULL, &URL, NH_URL_PARSER_STATE_UNDEFINED))
//
//    sprintf(output_p, "URL("); 
//
//    if (URL.Scheme.length > 0) {
//        nh_encoding_UTF8String Scheme = nh_encoding_encodeUTF8(URL.Scheme.p, URL.Scheme.length);
//        sprintf(output_p+strlen(output_p), "scheme:\"%s\"", Scheme.p); 
//        nh_encoding_freeUTF8(&Scheme);
//    }
//
//    if (URL.Host_p) {
//        if (strlen(output_p) > 4) {
//            sprintf(output_p+strlen(output_p), ", "); 
//        }
//        sprintf(output_p+strlen(output_p), "host:");
//        nh_url_printHost(URL.Host_p, output_p);
//    }
//
//    if (URL.Path.length > 0) {
//        if (strlen(output_p) > 4) {
//            sprintf(output_p+strlen(output_p), ", "); 
//        }
//        sprintf(output_p+strlen(output_p), "path:\""); 
//        for (int i = 0; i < URL.Path.length; ++i) {
//            sprintf(output_p+strlen(output_p), i+1 < URL.Path.length ? "%s/" : "%s", ((nh_core_String*)URL.Path.p)[i].p); 
//        }
//        sprintf(output_p+strlen(output_p), "\"");
//    }
//
//    if (URL.Fragment_p) {
//        if (strlen(output_p) > 4) {
//            sprintf(output_p+strlen(output_p), ", "); 
//        }
//        sprintf(output_p+strlen(output_p), "fragment:\"%s\"", URL.Fragment_p->p); 
//    }
//
//    sprintf(output_p+strlen(output_p), ")"); 
//
//    return 0;
//}

