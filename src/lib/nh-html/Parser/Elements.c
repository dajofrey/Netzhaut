// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Elements.h"

#include "../Common/IndexMap.h"
#include "../Common/Macros.h"

#include "../../nh-webidl/Runtime/Runtime.h"

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// TAG NAMES =======================================================================================

const char *NH_HTML_TAG_NAMES_PP[] =
{
    "a",
    "abbr",
    "address",
    "area",
    "article",
    "aside",
    "audio",
    "b",
    "base",
    "bdi",
    "bdo",
    "blockquote",
    "body",
    "br",
    "button",
    "canvas",
    "caption",
    "cite",
    "code",
    "col",
    "colgroup",
    "data",
    "datalist",
    "dd",
    "del",
    "details",
    "dfn",
    "dialog",
    "div",
    "dl",
    "dt",
    "em",
    "embed",
    "fieldset",
    "figcaption",
    "figure",
    "footer",
    "form",
    "h1",
    "h2",
    "h3",
    "h4",
    "h5",
    "h6",
    "head",
    "header",
    "hgroup",
    "hr",
    "html",
    "i",
    "iframe",
    "img",
    "input",
    "ins",
    "kbd",
    "label",
    "legend",
    "li",
    "link",
    "main",
    "map",
    "mark",
    "math",
    "menu",
    "meta",
    "meter",
    "nav",
    "noscript",
    "object",
    "ol",
    "optgroup",
    "option",
    "output",
    "p",
    "param",
    "picture",
    "pre",
    "progress",
    "q",
    "rp",
    "rt",
    "ruby",
    "s",
    "samp",
    "script",
    "section",
    "select",
    "slot",
    "small",
    "source",
    "span",
    "strong",
    "style",
    "sub",
    "summary",
    "sup",
    "svg",
    "table",
    "tbody",
    "td",
    "template",
    "textarea",
    "tfoot",
    "th",
    "thead",
    "time",
    "title",
    "tr",
    "track",
    "u",
    "ul",
    "var",
    "video",
    "wbr",
    "applet",
    "bgsound",
    "blink",
    "isindex",
    "keygen",
    "multicol",
    "nextid",
    "spacer",
    "acronym",
    "basefont",
    "big",
    "center",
    "nobr",
    "noembed",
    "noframes",
    "plaintext",
    "rb",
    "rtc",
    "strike",
    "tt",
    "listing",
    "xmp",
    // legacy
    "frameset",
    "dir",
    "font",
    "marquee",
    "frame",
};

unsigned long NH_HTML_TAG_NAMES_PP_COUNT = sizeof(NH_HTML_TAG_NAMES_PP) / sizeof(NH_HTML_TAG_NAMES_PP[0]);

// GET INDEX =======================================================================================

int nh_html_getTagIndex(
    char *tagname_p)
{
NH_HTML_BEGIN()

    char lower_p[255] = {0};
    memset(lower_p, 255, 0); 
    for (int i = 0; i < strlen(tagname_p); ++i) {lower_p[i] = tolower(tagname_p[i]);}

    void *value_p = nh_core_getFromHashMap(&NH_HTML_INDEXMAP.Tags, lower_p);
    if (!value_p) {NH_HTML_END(-1)}

NH_HTML_END(*((unsigned int*)value_p))
}

// GET CORRESPONDING CLASS =========================================================================

// https://html.spec.whatwg.org/multipage/dom.html#elements-in-the-dom
nh_webidl_Interface *nh_html_getElementInterface(
    char *tagname_p)
{
NH_HTML_BEGIN()

    int index = nh_html_getTagIndex(tagname_p);

    if (index == -1) {
        NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLUnknownElement"))
    }

    switch (index)
    {
        case NH_HTML_TAG_APPLET    :
        case NH_HTML_TAG_BGSOUND   :
        case NH_HTML_TAG_BLINK     :
        case NH_HTML_TAG_ISINDEX   :
        case NH_HTML_TAG_KEYGEN    :
        case NH_HTML_TAG_MULTICOL  :
        case NH_HTML_TAG_NEXTID    :
        case NH_HTML_TAG_SPACER    : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLUnknownElement"))
        case NH_HTML_TAG_ACRONYM   :
        case NH_HTML_TAG_BASEFONT  :
        case NH_HTML_TAG_BIG       :
        case NH_HTML_TAG_CENTER    :
        case NH_HTML_TAG_NOBR      :
        case NH_HTML_TAG_NOEMBED   :
        case NH_HTML_TAG_NOFRAMES  :
        case NH_HTML_TAG_PLAINTEXT :
        case NH_HTML_TAG_RB        :
        case NH_HTML_TAG_RTC       :
        case NH_HTML_TAG_STRIKE    :
        case NH_HTML_TAG_TT        : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLElement"))
        case NH_HTML_TAG_LISTING   :
        case NH_HTML_TAG_XMP       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLPreElement"))
    }

    switch (index)
    {
        case NH_HTML_TAG_A          : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLAnchorElement"))
        case NH_HTML_TAG_AREA       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLAreaElement"))
        case NH_HTML_TAG_AUDIO      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLAudioElement"))
        case NH_HTML_TAG_BASE       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLBaseElement"))
        case NH_HTML_TAG_BLOCKQUOTE : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLQuoteElement"))
        case NH_HTML_TAG_BODY       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLBodyElement"))
        case NH_HTML_TAG_BR         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLBRElement"))
        case NH_HTML_TAG_BUTTON     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLButtonElement"))
        case NH_HTML_TAG_CANVAS     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLCanvasElement"))
        case NH_HTML_TAG_CAPTION    : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTableCaptionElement"))
        case NH_HTML_TAG_COL        :
        case NH_HTML_TAG_COLGROUP   : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTableColElement"))
        case NH_HTML_TAG_DATA       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLDataElement"))
        case NH_HTML_TAG_DATALIST   : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLDataListElement"))
        case NH_HTML_TAG_DEL        : 
        case NH_HTML_TAG_INS        : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLModElement"))
        case NH_HTML_TAG_DETAILS    : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLDetailsElement"))
        case NH_HTML_TAG_DIALOG     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLDialogElement"))
        case NH_HTML_TAG_DIV        : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLDivElement"))
        case NH_HTML_TAG_DL         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLDListElement"))
        case NH_HTML_TAG_EMBED      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLEmbedElement"))
        case NH_HTML_TAG_FIELDSET   : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLFieldSetElement"))
        case NH_HTML_TAG_FORM       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLFormElement"))
        case NH_HTML_TAG_H1         :
        case NH_HTML_TAG_H2         :
        case NH_HTML_TAG_H3         :
        case NH_HTML_TAG_H4         :
        case NH_HTML_TAG_H5         :
        case NH_HTML_TAG_H6         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLHeadingElement"))
        case NH_HTML_TAG_HEAD       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLHeadElement"))
        case NH_HTML_TAG_HR         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLHRElement"))
        case NH_HTML_TAG_HTML       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLHtmlElement"))
        case NH_HTML_TAG_IFRAME     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLIFrameElement"))
        case NH_HTML_TAG_IMG        : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLImageElement"))
        case NH_HTML_TAG_INPUT      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLInputElement"))
        case NH_HTML_TAG_LABEL      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLLabelElement"))
        case NH_HTML_TAG_LEGEND     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLLegendElement"))
        case NH_HTML_TAG_LI         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLLIElement"))
        case NH_HTML_TAG_LINK       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLLinkElement"))
        case NH_HTML_TAG_MAP        : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLMapElement"))
        case NH_HTML_TAG_MATH       : NH_HTML_END(nh_webidl_getInterface("DOM", "Element"))
        case NH_HTML_TAG_MENU       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLMenuElement"))
        case NH_HTML_TAG_META       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLMetaElement"))
        case NH_HTML_TAG_METER      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLMeterElement"))
        case NH_HTML_TAG_OBJECT     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLObjectElement"))
        case NH_HTML_TAG_OL         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLOListElement"))
        case NH_HTML_TAG_OPTGROUP   : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLOptGroupElement"))
        case NH_HTML_TAG_OPTION     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLOptionElement"))
        case NH_HTML_TAG_OUTPUT     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLOutputElement"))
        case NH_HTML_TAG_P          : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLParagraphElement"))
        case NH_HTML_TAG_PARAM      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLParamElement"))
        case NH_HTML_TAG_PICTURE    : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLPictureElement"))
        case NH_HTML_TAG_PRE        : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLPreElement"))
        case NH_HTML_TAG_PROGRESS   : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLProgressElement"))
        case NH_HTML_TAG_Q          : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLQuoteElement"))
        case NH_HTML_TAG_SCRIPT     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLScriptElement"))
        case NH_HTML_TAG_SELECT     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLSelectElement"))
        case NH_HTML_TAG_SLOT       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLSlotElement"))
        case NH_HTML_TAG_SOURCE     : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLSourceElement"))
        case NH_HTML_TAG_SPAN       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLSpanElement"))
        case NH_HTML_TAG_STYLE      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLStyleElement"))
        case NH_HTML_TAG_SVG        : NH_HTML_END(nh_webidl_getInterface("SVG", "SVGSVGElement"))
        case NH_HTML_TAG_TABLE      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTableElement"))
        case NH_HTML_TAG_TD         :
        case NH_HTML_TAG_TH         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTableCellElement"))
        case NH_HTML_TAG_TEMPLATE   : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTemplateElement"))
        case NH_HTML_TAG_TEXTAREA   : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTextAreaElement"))
        case NH_HTML_TAG_TBODY      :
        case NH_HTML_TAG_TFOOT      :
        case NH_HTML_TAG_THEAD      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTableSectionElement"))
        case NH_HTML_TAG_TIME       : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTimeElement"))
        case NH_HTML_TAG_TITLE      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTitleElement"))
        case NH_HTML_TAG_TR         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTableRowElement"))
        case NH_HTML_TAG_TRACK      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLTrackElement"))
        case NH_HTML_TAG_UL         : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLUListElement"))
        case NH_HTML_TAG_VIDEO      : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLVideoElement"))
        case NH_HTML_TAG_ABBR       :
        case NH_HTML_TAG_ADDRESS    :
        case NH_HTML_TAG_ARTICLE    :
        case NH_HTML_TAG_ASIDE      :
        case NH_HTML_TAG_B          :
        case NH_HTML_TAG_BDI        :
        case NH_HTML_TAG_BDO        :
        case NH_HTML_TAG_CITE       :
        case NH_HTML_TAG_CODE       :
        case NH_HTML_TAG_DD         :
        case NH_HTML_TAG_DFN        :
        case NH_HTML_TAG_DT         :
        case NH_HTML_TAG_EM         :
        case NH_HTML_TAG_FIGCAPTION :
        case NH_HTML_TAG_FIGURE     :
        case NH_HTML_TAG_FOOTER     :
        case NH_HTML_TAG_HEADER     :
        case NH_HTML_TAG_HGROUP     :
        case NH_HTML_TAG_I          :
        case NH_HTML_TAG_KBD        :
        case NH_HTML_TAG_MAIN       :
        case NH_HTML_TAG_MARK       :
        case NH_HTML_TAG_NAV        :
        case NH_HTML_TAG_NOSCRIPT   :
        case NH_HTML_TAG_RP         :
        case NH_HTML_TAG_RT         :
        case NH_HTML_TAG_RUBY       :
        case NH_HTML_TAG_S          :
        case NH_HTML_TAG_SAMP       :
        case NH_HTML_TAG_SECTION    :
        case NH_HTML_TAG_SMALL      :
        case NH_HTML_TAG_STRONG     :
        case NH_HTML_TAG_SUB        :
        case NH_HTML_TAG_SUMMARY    :
        case NH_HTML_TAG_SUP        :
        case NH_HTML_TAG_U          :
        case NH_HTML_TAG_VAR        :
        case NH_HTML_TAG_WBR        : NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLElement"))
    }

NH_HTML_END(nh_webidl_getInterface("HTML", "HTMLUnknownElement"))
}

