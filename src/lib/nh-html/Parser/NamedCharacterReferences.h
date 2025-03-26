#ifndef NH_HTML_NAMED_CHARACTER_REFERENCES_H
#define NH_HTML_NAMED_CHARACTER_REFERENCES_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

extern const char *NH_HTML_CHARACTER_REFERENCES_PP[];
extern const NH_ENCODING_UTF32 NH_HTML_CHARACTER_REFERENCE_CODEPOINTS_P[];

NH_ENCODING_UTF32 nh_html_matchCharacterReferencesEntity( 
    const char *text_p, int *matched_length
); 
void nh_html_freeCharacterReferencesTrie(
); 
NH_API_RESULT nh_html_initCharacterReferencesTrie(
);

#endif
