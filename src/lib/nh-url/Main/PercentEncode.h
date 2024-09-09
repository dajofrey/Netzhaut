#ifndef NH_URL_PERCENT_ENCODE_H
#define NH_URL_PERCENT_ENCODE_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-url_enums
 *  @{
 */

    typedef enum NH_URL_PERCENT_ENCODE_SET {
        NH_URL_PERCENT_ENCODE_SET_C0_CONTROL,
        NH_URL_PERCENT_ENCODE_SET_FRAGMENT,
        NH_URL_PERCENT_ENCODE_SET_QUERY,
        NH_URL_PERCENT_ENCODE_SET_SPECIAL_QUERY,
        NH_URL_PERCENT_ENCODE_SET_PATH,
        NH_URL_PERCENT_ENCODE_SET_USERINFO,
        NH_URL_PERCENT_ENCODE_SET_COMPONENT,
    } NH_URL_PERCENT_ENCODE_SET;

/** @} */

/** @addtogroup lib_nh-url_functions
 *  @{
 */

    void nh_url_percentEncodeByte(
        NH_BYTE byte, nh_String *Output_p
    );
    
    nh_String nh_url_percentDecodeUTF8(
        NH_ENCODING_UTF32 *codepoints_p, int length
    );

    nh_String nh_url_percentEncodeAfterEncoding(
        NH_ENCODING_NAME encoding, nh_encoding_UTF32String Input, NH_URL_PERCENT_ENCODE_SET percentEncodeSet, 
        NH_BOOL spaceAsPlus
    );

    nh_String nh_url_percentEncodeAfterEncodingUTF8(
        nh_encoding_UTF32String Input, NH_URL_PERCENT_ENCODE_SET percentEncodeSet
    );

    nh_String nh_url_percentEncodeCodepointAfterEncodingUTF8(
        NH_ENCODING_UTF32 codepoint, NH_URL_PERCENT_ENCODE_SET percentEncodeSet
    );

    NH_BOOL nh_url_inPercentEncodeSet(
        NH_URL_PERCENT_ENCODE_SET encodeSet, NH_ENCODING_UTF32 codepoint
    );

/** @} */

#endif
