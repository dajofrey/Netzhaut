// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "HostParser.h"
#include "Helper.h"
#include "PercentEncode.h"

#include "../IDNA/IDNAProcessing.h"
#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/String.h"
#include "../../nh-core/Util/Array.h"

#include "../../nh-encoding/Encodings/UTF32.h"
#include "../../nh-encoding/Encodings/UTF8.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// IPV6 PARSER =====================================================================================

/**
 * Parses a IPv6-address.
 *
 * The implementation is based on WHATWG's URL Standard: 
 *   https://url.spec.whatwg.org/#concept-ipv6-parser
 *
 * A valid IPv6-address string is defined in the "Text Representation of Addresses" chapter of 
 * IP Version 6 Addressing Architecture. [RFC4291]: 
 *   https://datatracker.ietf.org/doc/html/rfc4291#section-2.2
 */
static NH_URL_RESULT nh_url_parseIPv6(
    nh_encoding_UTF32String Input, nh_url_Host *Host_p)
{
NH_URL_BEGIN()

    memset(Host_p, 0, sizeof(nh_url_Host));
    Host_p->type = NH_URL_HOST_IPV6_ADDRESS;

    int pieceIndex = 0;
    int compress = -1;
    NH_ENCODING_UTF32 *pointer = Input.p;

    if (*pointer == 0x3A) {
        if (*(pointer+1) != 0x3A) {
            // val err
            NH_URL_END(NH_URL_ERROR_BAD_STATE)
        }
        pointer += 2;
        pieceIndex++;
        compress = pieceIndex;
    }

    while (*pointer) 
    {
        if (pieceIndex == 8) {
            // val err
            NH_URL_END(NH_URL_ERROR_BAD_STATE)
        }

        if (*pointer == 0x3A) {
            if (compress != -1) {
                // val err
                NH_URL_END(NH_URL_ERROR_BAD_STATE)
            }
            pointer++;
            pieceIndex++;
            compress = pieceIndex;
            continue;
        }

        int value = 0, length = 0;

        while (length < 4 && nh_encoding_isASCIIHexDigit(*pointer)) {
            value = value * 0x10 + nh_encoding_hexDigitToNumber(*pointer);
            pointer++;
            length++;
        }

        if (*pointer == 0x2E) {
            if (length == 0) {
                // val err
                NH_URL_END(NH_URL_ERROR_BAD_STATE)
            }
            pointer -= length;
            if (pieceIndex > 6) {
                // val err
                NH_URL_END(NH_URL_ERROR_BAD_STATE)
            }
            int numbersSeen = 0;
            while (*pointer)
            {
                int ipv4Piece = -1;
                if (numbersSeen > 0) {
                    if (*pointer == 0x2E && numbersSeen < 4) {
                        pointer++;
                    }
                    else {
                        // val err
                        NH_URL_END(NH_URL_ERROR_BAD_STATE)
                    }
                }
                if (!nh_encoding_isASCIIDigit(*pointer)) {
                    // val err
                    NH_URL_END(NH_URL_ERROR_BAD_STATE)
                }
                while (nh_encoding_isASCIIDigit(*pointer)) {
                    int number = nh_encoding_hexDigitToNumber(*pointer);
                    if (ipv4Piece == -1) {
                        ipv4Piece = number;
                    }
                    else if (ipv4Piece == 0) {
                        // val err
                        NH_URL_END(NH_URL_ERROR_BAD_STATE)
                    }
                    else {
                        ipv4Piece = ipv4Piece * 10 + number;
                    }
                    if (ipv4Piece > 255) {
                        // val err
                        NH_URL_END(NH_URL_ERROR_BAD_STATE)
                    }
                    pointer++;
                }
                Host_p->IPv6.pieces_p[pieceIndex] = Host_p->IPv6.pieces_p[pieceIndex] * 0x100 + ipv4Piece;
                numbersSeen++;
                if (numbersSeen == 2 || numbersSeen == 4) {
                    pieceIndex++;
                }
            }
            if (numbersSeen != 4) {
                // val err
                NH_URL_END(NH_URL_ERROR_BAD_STATE)
            }
            break;
        }
        else if (*pointer == 0x3A) {
            pointer++;
            if (*pointer == 0) {
                // val err
                NH_URL_END(NH_URL_ERROR_BAD_STATE)
            }
        }
        else if (*pointer) {
            // val err
            NH_URL_END(NH_URL_ERROR_BAD_STATE)
        }
        Host_p->IPv6.pieces_p[pieceIndex] = value;
        pieceIndex++;
    }

    if (compress != -1) {
        int swaps = pieceIndex - compress;
        int pieceIndex = 7;
        while (pieceIndex && swaps > 0) {
            NH_URL_IPV6_PIECE piece = Host_p->IPv6.pieces_p[pieceIndex];
            Host_p->IPv6.pieces_p[pieceIndex] = Host_p->IPv6.pieces_p[compress+swaps-1];
            Host_p->IPv6.pieces_p[compress+swaps-1] = piece;
            pieceIndex--;
            swaps--;
        }
    }
    else if (compress == -1 && pieceIndex != 8) {
        // val err
        NH_URL_END(NH_URL_ERROR_BAD_STATE)
    }

NH_URL_END(NH_URL_SUCCESS)
}

// IPV4 PARSER =====================================================================================

static NH_URL_RESULT nh_url_parseIPv4Number(
    nh_encoding_UTF32String *Input_p, long *number_p)
{
NH_URL_BEGIN()

    int r = 10;
    int offset = 0;

    if (Input_p->length > 1) {
        if (Input_p->p[0] == '0' && (Input_p->p[1] == 'x' || Input_p->p[1] == 'X')) {
            // val err
            offset = 2;
            r = 16;
        }
        else if (Input_p->p[0] == '0') {
            // val err
            offset = 1;
            r = 8;
        }
    }

    if (Input_p->length - offset <= 0) {
        *number_p = 0;
        NH_URL_END(NH_URL_SUCCESS)
    }

    for (int i = offset; i < Input_p->length; ++i) {
        if (r <= 10) {
            if (!nh_encoding_isASCIIDigit(Input_p->p[i])) {
                NH_URL_END(NH_URL_ERROR_BAD_STATE)
            }
        }
        else {
            if (!nh_encoding_isASCIIHexDigit(Input_p->p[i])) {
                NH_URL_END(NH_URL_ERROR_BAD_STATE)
            }
        }
    }

    *number_p = nh_encoding_digitsToNumber(&Input_p->p[offset], Input_p->length - offset, r);

NH_URL_END(NH_URL_SUCCESS)
}

/**
 * Parses a IPv4-address.
 *
 * The implementation is based on WHATWG's URL Standard: 
 *   https://url.spec.whatwg.org/#concept-ipv4-parser
 */
static NH_URL_RESULT nh_url_parseIPv4(
    nh_encoding_UTF32String *Input_p, nh_url_Host *Host_p)
{
NH_URL_BEGIN()

    NH_URL_RESULT result = NH_URL_SUCCESS;
    nh_List Parts = nh_encoding_splitUTF32(Input_p, 0x2E);
    nh_Array Numbers = nh_core_initArray(sizeof(long), 4);

    if (Parts.size > 0 && ((nh_encoding_UTF32String*)Parts.pp[Parts.size-1])->length == 0) {
        // val err
        if (Parts.size > 1) {
            nh_core_removeFromList(&Parts, NH_TRUE, Parts.size-1);
        }
    }

    if (Parts.size > 4) {
        result = NH_URL_ERROR_BAD_STATE;
        goto NH_URL_PARSE_IPV4_CLEAN_UP;
    }

    for (int i = 0; i < Parts.size; ++i) {
        nh_encoding_UTF32String *Part_p = Parts.pp[i];
        if (Part_p->length == 0) {
            result = NH_URL_ERROR_BAD_STATE;
            goto NH_URL_PARSE_IPV4_CLEAN_UP;
        }
        long number = 0;
        if (nh_url_parseIPv4Number(Part_p, &number) != NH_URL_SUCCESS) {
            result = NH_URL_ERROR_BAD_STATE;
            goto NH_URL_PARSE_IPV4_CLEAN_UP;
        }
        long *number_p = nh_core_incrementArray(&Numbers);
        *number_p = number;
    }

    for (int i = 0; i < Numbers.length; ++i) {
        long *number_p = ((long*)Numbers.p)+i;
        if (*number_p > 255) {
            // val err
        }
    }

    Host_p->type = NH_URL_HOST_IPV4_ADDRESS;
    Host_p->IPv4.address = (NH_URL_IPV4_ADDRESS)((long*)Numbers.p)[Numbers.length-1];

    for (int i = 0; i < Numbers.length - 1; ++i) {
        Host_p->IPv4.address += (NH_URL_IPV4_ADDRESS)((long*)Numbers.p)[i] * pow(256, 3-i);
    }

NH_URL_PARSE_IPV4_CLEAN_UP:

    for (int i = 0; i < Parts.size; ++i) {
        nh_encoding_freeUTF32(Parts.pp[i]);
    }

    nh_core_freeList(&Parts, NH_TRUE);
    nh_core_freeArray(&Numbers);

NH_URL_END(result)
}

// OPAQUE HOST PARSER ==============================================================================

static NH_URL_RESULT nh_url_parseOpaqueHost(
    nh_encoding_UTF32String Input, nh_url_Host *Host_p)
{
NH_URL_BEGIN()

    for (int i = 0; i < Input.length; ++i) {
        if (Input.p[i] != 0x25 && nh_url_isForbiddenHostCodepoint(Input.p[i])) {
            // val err
            NH_URL_END(NH_URL_ERROR_BAD_STATE)
        }
        if (Input.p[i] != 0x25 && !nh_url_isURLCodepoint(Input.p[i])) {
            // val err
            NH_URL_END(NH_URL_ERROR_BAD_STATE)
        }
        if (Input.p[i] == 0x25 && (i+2 >= Input.length || !nh_encoding_isASCIIHexDigit(Input.p[i+1]) || !nh_encoding_isASCIIHexDigit(Input.p[i+2]))) {
            // val err
            NH_URL_END(NH_URL_ERROR_BAD_STATE)
        } 
    }

    Host_p->type = NH_URL_HOST_OPAQUE;
    Host_p->DomainOrOpaque.String = nh_url_percentEncodeAfterEncodingUTF8(Input, NH_URL_PERCENT_ENCODE_SET_C0_CONTROL);

NH_URL_END(NH_URL_SUCCESS)
}

// HOST PARSER =====================================================================================

static NH_URL_RESULT nh_url_domainToASCII(
    nh_encoding_UTF32String *Domain_p, NH_BOOL beStrict)
{
NH_URL_BEGIN()

    NH_URL_CHECK(nh_url_unicodeToASCII(Domain_p, NH_FALSE))

    if (Domain_p->length == 0) {NH_URL_END(NH_URL_ERROR_BAD_STATE)}

NH_URL_END(NH_URL_SUCCESS)
}

// https://url.spec.whatwg.org/#host-parsing
NH_URL_RESULT nh_url_parseHost(
    nh_encoding_UTF32String Input, NH_BOOL isNotSpecial, nh_url_Host *Host_p)
{
NH_URL_BEGIN()

    if (Input.length && Input.p[0] == 0x5B) 
    {
        if (Input.p[Input.length - 1] != 0x5D) {
            // val err
            NH_URL_END(NH_URL_ERROR_BAD_STATE)
        }

        nh_encoding_UTF32String Tmp = nh_encoding_initUTF32(Input.length-1);
        nh_encoding_appendUTF32(&Tmp, Input.p+1, Input.length-2);

        nh_url_parseIPv6(Tmp, Host_p);
        nh_encoding_freeUTF32(&Tmp);

        NH_URL_END(NH_URL_SUCCESS)
    }

    if (isNotSpecial) {
        NH_URL_END(nh_url_parseOpaqueHost(Input, Host_p))
    }

    nh_String String = nh_url_percentDecodeUTF8(Input.p, Input.length);

    nh_encoding_UTF32String Domain = nh_encoding_decodeUTF8(String.p, String.length, NULL);
    nh_core_freeString(&String);

    NH_URL_CHECK(nh_url_domainToASCII(&Domain, NH_FALSE))

    for (int i = 0; i < Domain.length; ++i) {
        if (nh_url_isForbiddenHostCodepoint(Domain.p[i])) {
            nh_encoding_freeUTF32(&Domain);
            NH_URL_END(NH_URL_ERROR_BAD_STATE)
        }
    }

    if (nh_url_parseIPv4(&Domain, Host_p) == NH_URL_SUCCESS) {
        nh_encoding_freeUTF32(&Domain);
        NH_URL_END(NH_URL_SUCCESS)
    }

    Host_p->type = NH_URL_HOST_DOMAIN;
    Host_p->DomainOrOpaque.String = nh_encoding_encodeUTF8(Domain.p, Domain.length); 
    nh_encoding_freeUTF32(&Domain);

NH_URL_END(NH_URL_SUCCESS)
}

