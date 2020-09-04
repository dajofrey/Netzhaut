/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_HTML_UTILITY_MACROS_H
#define NH_HTML_UTILITY_MACROS_H

#define NH_HTML_IS_DOCUMENT(path)       \
(                                       \
    strlen(path) >= 5                   \
    &&  path_p[strlen(path) - 1] == 'l' \
    &&  path_p[strlen(path) - 2] == 'm' \
    &&  path_p[strlen(path) - 3] == 't' \
    &&  path_p[strlen(path) - 4] == 'h' \
    &&  path_p[strlen(path) - 5] == '.' \
)

#define NH_HTML_METADATA(tag)       \
(                                   \
    tag == NH_HTML_TAG_BASE     ||  \
    tag == NH_HTML_TAG_SCRIPT   ||  \
    tag == NH_HTML_TAG_LINK     ||  \
    tag == NH_HTML_TAG_STYLE    ||  \
    tag == NH_HTML_TAG_META     ||  \
    tag == NH_HTML_TAG_TITLE    ||  \
    tag == NH_HTML_TAG_NOSCRIPT ||  \
    tag == NH_HTML_TAG_HEAD         \
)

#define NH_HTML_SHOW_IMAGE(Node_p) \
    (Node_p->Computed.Attributes.Texture_p != NULL && Node_p->tag == NH_HTML_TAG_IMG)

#endif
