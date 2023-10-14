#ifndef NH_HTML_ELEMENTS_H
#define NH_HTML_ELEMENTS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nhhtml_enums
 *  @{
 */

    typedef enum NH_HTML_TAG {
        NH_HTML_TAG_A,
        NH_HTML_TAG_ABBR,
        NH_HTML_TAG_ADDRESS,
        NH_HTML_TAG_AREA,
        NH_HTML_TAG_ARTICLE,
        NH_HTML_TAG_ASIDE,
        NH_HTML_TAG_AUDIO,
        NH_HTML_TAG_B,
        NH_HTML_TAG_BASE,
        NH_HTML_TAG_BDI,
        NH_HTML_TAG_BDO,
        NH_HTML_TAG_BLOCKQUOTE,
        NH_HTML_TAG_BODY,
        NH_HTML_TAG_BR,
        NH_HTML_TAG_BUTTON,
        NH_HTML_TAG_CANVAS,
        NH_HTML_TAG_CAPTION,
        NH_HTML_TAG_CITE,
        NH_HTML_TAG_CODE,
        NH_HTML_TAG_COL,
        NH_HTML_TAG_COLGROUP,
        NH_HTML_TAG_DATA,
        NH_HTML_TAG_DATALIST,
        NH_HTML_TAG_DD,
        NH_HTML_TAG_DEL,
        NH_HTML_TAG_DETAILS,
        NH_HTML_TAG_DFN,
        NH_HTML_TAG_DIALOG,
        NH_HTML_TAG_DIV,
        NH_HTML_TAG_DL,
        NH_HTML_TAG_DT,
        NH_HTML_TAG_EM,
        NH_HTML_TAG_EMBED,
        NH_HTML_TAG_FIELDSET,
        NH_HTML_TAG_FIGCAPTION,
        NH_HTML_TAG_FIGURE,
        NH_HTML_TAG_FOOTER,
        NH_HTML_TAG_FORM,
        NH_HTML_TAG_H1,
        NH_HTML_TAG_H2,
        NH_HTML_TAG_H3,
        NH_HTML_TAG_H4,
        NH_HTML_TAG_H5,
        NH_HTML_TAG_H6,
        NH_HTML_TAG_HEAD,
        NH_HTML_TAG_HEADER,
        NH_HTML_TAG_HGROUP,
        NH_HTML_TAG_HR,
        NH_HTML_TAG_HTML,
        NH_HTML_TAG_I,
        NH_HTML_TAG_IFRAME,
        NH_HTML_TAG_IMG,
        NH_HTML_TAG_INPUT,
        NH_HTML_TAG_INS,
        NH_HTML_TAG_KBD,
        NH_HTML_TAG_LABEL,
        NH_HTML_TAG_LEGEND,
        NH_HTML_TAG_LI,
        NH_HTML_TAG_LINK,
        NH_HTML_TAG_MAIN,
        NH_HTML_TAG_MAP,
        NH_HTML_TAG_MARK,
        NH_HTML_TAG_MATH,
        NH_HTML_TAG_MENU,
        NH_HTML_TAG_META,
        NH_HTML_TAG_METER,
        NH_HTML_TAG_NAV,
        NH_HTML_TAG_NOSCRIPT,
        NH_HTML_TAG_OBJECT,
        NH_HTML_TAG_OL,
        NH_HTML_TAG_OPTGROUP,
        NH_HTML_TAG_OPTION,
        NH_HTML_TAG_OUTPUT,
        NH_HTML_TAG_P,
        NH_HTML_TAG_PARAM,
        NH_HTML_TAG_PICTURE,
        NH_HTML_TAG_PRE,
        NH_HTML_TAG_PROGRESS,
        NH_HTML_TAG_Q,
        NH_HTML_TAG_RP,
        NH_HTML_TAG_RT,
        NH_HTML_TAG_RUBY,
        NH_HTML_TAG_S,
        NH_HTML_TAG_SAMP,
        NH_HTML_TAG_SCRIPT,
        NH_HTML_TAG_SECTION,
        NH_HTML_TAG_SELECT,
        NH_HTML_TAG_SLOT,
        NH_HTML_TAG_SMALL,
        NH_HTML_TAG_SOURCE,
        NH_HTML_TAG_SPAN,
        NH_HTML_TAG_STRONG,
        NH_HTML_TAG_STYLE,
        NH_HTML_TAG_SUB,
        NH_HTML_TAG_SUMMARY,
        NH_HTML_TAG_SUP,
        NH_HTML_TAG_SVG,
        NH_HTML_TAG_TABLE,
        NH_HTML_TAG_TBODY,
        NH_HTML_TAG_TD,
        NH_HTML_TAG_TEMPLATE,
        NH_HTML_TAG_TEXTAREA,
        NH_HTML_TAG_TFOOT,
        NH_HTML_TAG_TH,
        NH_HTML_TAG_THEAD,
        NH_HTML_TAG_TIME,
        NH_HTML_TAG_TITLE,
        NH_HTML_TAG_TR,
        NH_HTML_TAG_TRACK,
        NH_HTML_TAG_U,
        NH_HTML_TAG_UL,
        NH_HTML_TAG_VAR,
        NH_HTML_TAG_VIDEO,
        NH_HTML_TAG_WBR,
        NH_HTML_TAG_APPLET,
        NH_HTML_TAG_BGSOUND,
        NH_HTML_TAG_BLINK,
        NH_HTML_TAG_ISINDEX,
        NH_HTML_TAG_KEYGEN,
        NH_HTML_TAG_MULTICOL,
        NH_HTML_TAG_NEXTID,
        NH_HTML_TAG_SPACER,
        NH_HTML_TAG_ACRONYM,
        NH_HTML_TAG_BASEFONT,
        NH_HTML_TAG_BIG,
        NH_HTML_TAG_CENTER,
        NH_HTML_TAG_NOBR,
        NH_HTML_TAG_NOEMBED,
        NH_HTML_TAG_NOFRAMES,
        NH_HTML_TAG_PLAINTEXT,
        NH_HTML_TAG_RB,
        NH_HTML_TAG_RTC,
        NH_HTML_TAG_STRIKE,
        NH_HTML_TAG_TT,
        NH_HTML_TAG_LISTING,
        NH_HTML_TAG_XMP,
        // legacy
        NH_HTML_TAG_FRAMESET,
        NH_HTML_TAG_DIR,
        NH_HTML_TAG_FONT,
        NH_HTML_TAG_MARQUEE,
        NH_HTML_TAG_FRAME,
    } NH_HTML_TAG;

/** @} */

/** @addtogroup lib_nhhtml_vars
 *  @{
 */

    extern const NH_BYTE *NH_HTML_TAG_NAMES_PP[];
    extern unsigned long NH_HTML_TAG_NAMES_PP_COUNT;

/** @} */

/** @addtogroup lib_nhhtml_functions
 *  @{
 */

    int nh_html_getTagIndex(
        NH_BYTE *tagname_p
    );

    nh_webidl_Interface *nh_html_getElementInterface(
        NH_BYTE *tagname_p
    );

/** @} */

#endif

