// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Elements.h"

#include "../Common/IndexMap.h"
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
    char lower_p[255] = {0};
    memset(lower_p, 255, 0); 
    for (int i = 0; i < strlen(tagname_p); ++i) {lower_p[i] = tolower(tagname_p[i]);}

    void *value_p = nh_core_getFromHashMap(&NH_HTML_INDEXMAP.Tags, lower_p);
    if (!value_p) {return -1;}

    return *((unsigned int*)value_p);
}

// GET CORRESPONDING CLASS =========================================================================

// https://html.spec.whatwg.org/multipage/dom.html#elements-in-the-dom
nh_webidl_Interface *nh_html_getElementInterface(
    char *tagname_p)
{
    int index = nh_html_getTagIndex(tagname_p);

    if (index == -1) {
        return nh_webidl_getInterface("HTML", "HTMLUnknownElement");
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
        case NH_HTML_TAG_SPACER    : return nh_webidl_getInterface("HTML", "HTMLUnknownElement");
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
        case NH_HTML_TAG_TT        : return nh_webidl_getInterface("HTML", "HTMLElement");
        case NH_HTML_TAG_LISTING   :
        case NH_HTML_TAG_XMP       : return nh_webidl_getInterface("HTML", "HTMLPreElement");
    }

    switch (index)
    {
        case NH_HTML_TAG_A          : return nh_webidl_getInterface("HTML", "HTMLAnchorElement");
        case NH_HTML_TAG_AREA       : return nh_webidl_getInterface("HTML", "HTMLAreaElement");
        case NH_HTML_TAG_AUDIO      : return nh_webidl_getInterface("HTML", "HTMLAudioElement");
        case NH_HTML_TAG_BASE       : return nh_webidl_getInterface("HTML", "HTMLBaseElement");
        case NH_HTML_TAG_BLOCKQUOTE : return nh_webidl_getInterface("HTML", "HTMLQuoteElement");
        case NH_HTML_TAG_BODY       : return nh_webidl_getInterface("HTML", "HTMLBodyElement");
        case NH_HTML_TAG_BR         : return nh_webidl_getInterface("HTML", "HTMLBRElement");
        case NH_HTML_TAG_BUTTON     : return nh_webidl_getInterface("HTML", "HTMLButtonElement");
        case NH_HTML_TAG_CANVAS     : return nh_webidl_getInterface("HTML", "HTMLCanvasElement");
        case NH_HTML_TAG_CAPTION    : return nh_webidl_getInterface("HTML", "HTMLTableCaptionElement");
        case NH_HTML_TAG_COL        :
        case NH_HTML_TAG_COLGROUP   : return nh_webidl_getInterface("HTML", "HTMLTableColElement");
        case NH_HTML_TAG_DATA       : return nh_webidl_getInterface("HTML", "HTMLDataElement");
        case NH_HTML_TAG_DATALIST   : return nh_webidl_getInterface("HTML", "HTMLDataListElement");
        case NH_HTML_TAG_DEL        : 
        case NH_HTML_TAG_INS        : return nh_webidl_getInterface("HTML", "HTMLModElement");
        case NH_HTML_TAG_DETAILS    : return nh_webidl_getInterface("HTML", "HTMLDetailsElement");
        case NH_HTML_TAG_DIALOG     : return nh_webidl_getInterface("HTML", "HTMLDialogElement");
        case NH_HTML_TAG_DIV        : return nh_webidl_getInterface("HTML", "HTMLDivElement");
        case NH_HTML_TAG_DL         : return nh_webidl_getInterface("HTML", "HTMLDListElement");
        case NH_HTML_TAG_EMBED      : return nh_webidl_getInterface("HTML", "HTMLEmbedElement");
        case NH_HTML_TAG_FIELDSET   : return nh_webidl_getInterface("HTML", "HTMLFieldSetElement");
        case NH_HTML_TAG_FORM       : return nh_webidl_getInterface("HTML", "HTMLFormElement");
        case NH_HTML_TAG_H1         :
        case NH_HTML_TAG_H2         :
        case NH_HTML_TAG_H3         :
        case NH_HTML_TAG_H4         :
        case NH_HTML_TAG_H5         :
        case NH_HTML_TAG_H6         : return nh_webidl_getInterface("HTML", "HTMLHeadingElement");
        case NH_HTML_TAG_HEAD       : return nh_webidl_getInterface("HTML", "HTMLHeadElement");
        case NH_HTML_TAG_HR         : return nh_webidl_getInterface("HTML", "HTMLHRElement");
        case NH_HTML_TAG_HTML       : return nh_webidl_getInterface("HTML", "HTMLHtmlElement");
        case NH_HTML_TAG_IFRAME     : return nh_webidl_getInterface("HTML", "HTMLIFrameElement");
        case NH_HTML_TAG_IMG        : return nh_webidl_getInterface("HTML", "HTMLImageElement");
        case NH_HTML_TAG_INPUT      : return nh_webidl_getInterface("HTML", "HTMLInputElement");
        case NH_HTML_TAG_LABEL      : return nh_webidl_getInterface("HTML", "HTMLLabelElement");
        case NH_HTML_TAG_LEGEND     : return nh_webidl_getInterface("HTML", "HTMLLegendElement");
        case NH_HTML_TAG_LI         : return nh_webidl_getInterface("HTML", "HTMLLIElement");
        case NH_HTML_TAG_LINK       : return nh_webidl_getInterface("HTML", "HTMLLinkElement");
        case NH_HTML_TAG_MAP        : return nh_webidl_getInterface("HTML", "HTMLMapElement");
        case NH_HTML_TAG_MATH       : return nh_webidl_getInterface("DOM", "Element");
        case NH_HTML_TAG_MENU       : return nh_webidl_getInterface("HTML", "HTMLMenuElement");
        case NH_HTML_TAG_META       : return nh_webidl_getInterface("HTML", "HTMLMetaElement");
        case NH_HTML_TAG_METER      : return nh_webidl_getInterface("HTML", "HTMLMeterElement");
        case NH_HTML_TAG_OBJECT     : return nh_webidl_getInterface("HTML", "HTMLObjectElement");
        case NH_HTML_TAG_OL         : return nh_webidl_getInterface("HTML", "HTMLOListElement");
        case NH_HTML_TAG_OPTGROUP   : return nh_webidl_getInterface("HTML", "HTMLOptGroupElement");
        case NH_HTML_TAG_OPTION     : return nh_webidl_getInterface("HTML", "HTMLOptionElement");
        case NH_HTML_TAG_OUTPUT     : return nh_webidl_getInterface("HTML", "HTMLOutputElement");
        case NH_HTML_TAG_P          : return nh_webidl_getInterface("HTML", "HTMLParagraphElement");
        case NH_HTML_TAG_PARAM      : return nh_webidl_getInterface("HTML", "HTMLParamElement");
        case NH_HTML_TAG_PICTURE    : return nh_webidl_getInterface("HTML", "HTMLPictureElement");
        case NH_HTML_TAG_PRE        : return nh_webidl_getInterface("HTML", "HTMLPreElement");
        case NH_HTML_TAG_PROGRESS   : return nh_webidl_getInterface("HTML", "HTMLProgressElement");
        case NH_HTML_TAG_Q          : return nh_webidl_getInterface("HTML", "HTMLQuoteElement");
        case NH_HTML_TAG_SCRIPT     : return nh_webidl_getInterface("HTML", "HTMLScriptElement");
        case NH_HTML_TAG_SELECT     : return nh_webidl_getInterface("HTML", "HTMLSelectElement");
        case NH_HTML_TAG_SLOT       : return nh_webidl_getInterface("HTML", "HTMLSlotElement");
        case NH_HTML_TAG_SOURCE     : return nh_webidl_getInterface("HTML", "HTMLSourceElement");
        case NH_HTML_TAG_SPAN       : return nh_webidl_getInterface("HTML", "HTMLSpanElement");
        case NH_HTML_TAG_STYLE      : return nh_webidl_getInterface("HTML", "HTMLStyleElement");
        case NH_HTML_TAG_SVG        : return nh_webidl_getInterface("SVG", "SVGSVGElement");
        case NH_HTML_TAG_TABLE      : return nh_webidl_getInterface("HTML", "HTMLTableElement");
        case NH_HTML_TAG_TD         :
        case NH_HTML_TAG_TH         : return nh_webidl_getInterface("HTML", "HTMLTableCellElement");
        case NH_HTML_TAG_TEMPLATE   : return nh_webidl_getInterface("HTML", "HTMLTemplateElement");
        case NH_HTML_TAG_TEXTAREA   : return nh_webidl_getInterface("HTML", "HTMLTextAreaElement");
        case NH_HTML_TAG_TBODY      :
        case NH_HTML_TAG_TFOOT      :
        case NH_HTML_TAG_THEAD      : return nh_webidl_getInterface("HTML", "HTMLTableSectionElement");
        case NH_HTML_TAG_TIME       : return nh_webidl_getInterface("HTML", "HTMLTimeElement");
        case NH_HTML_TAG_TITLE      : return nh_webidl_getInterface("HTML", "HTMLTitleElement");
        case NH_HTML_TAG_TR         : return nh_webidl_getInterface("HTML", "HTMLTableRowElement");
        case NH_HTML_TAG_TRACK      : return nh_webidl_getInterface("HTML", "HTMLTrackElement");
        case NH_HTML_TAG_UL         : return nh_webidl_getInterface("HTML", "HTMLUListElement");
        case NH_HTML_TAG_VIDEO      : return nh_webidl_getInterface("HTML", "HTMLVideoElement");
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
        case NH_HTML_TAG_WBR        : return nh_webidl_getInterface("HTML", "HTMLElement");
    }

    return nh_webidl_getInterface("HTML", "HTMLUnknownElement");
}

