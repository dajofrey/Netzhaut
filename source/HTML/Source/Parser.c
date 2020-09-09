// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Parser.h"
#include "../Header/Macros.h"

#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/String.h"
#include "../../Core/Header/URI.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DEFINE ==========================================================================================

#define DONT_IGNORE(c) (c != ' ' && c != '\n' && c != '\t' && c != '\v' && c != '\r' && c != '\0')
#define IS_VOID_TAG(t) (t == NH_HTML_TAG_AREA  || t == NH_HTML_TAG_BASE  || t == NH_HTML_TAG_BR     || \
                        t == NH_HTML_TAG_COL   || t == NH_HTML_TAG_EMBED || t == NH_HTML_TAG_HR     || \
                        t == NH_HTML_TAG_IMG   || t == NH_HTML_TAG_INPUT || t == NH_HTML_TAG_LINK   || \
                        t == NH_HTML_TAG_META  || t == NH_HTML_TAG_PARAM || t == NH_HTML_TAG_SOURCE || \
                        t == NH_HTML_TAG_TRACK || t == NH_HTML_TAG_WBR)
#define OMITTED_ENDTAG_ALLOWED(prev, next) ( \
   (prev == NH_HTML_TAG_P && (next == NH_HTML_TAG_ADDRESS || next == NH_HTML_TAG_ARTICLE || next == NH_HTML_TAG_ASIDE || next == NH_HTML_TAG_BLOCKQUOTE || next == NH_HTML_TAG_DETAILS || next == NH_HTML_TAG_DIV || next == NH_HTML_TAG_DL || next == NH_HTML_TAG_FIELDSET || next == NH_HTML_TAG_FIGCAPTION || next == NH_HTML_TAG_FIGURE || next == NH_HTML_TAG_FOOTER || next == NH_HTML_TAG_FORM || next == NH_HTML_TAG_H1 || next == NH_HTML_TAG_H2 || next == NH_HTML_TAG_H3 || next == NH_HTML_TAG_H4 || next == NH_HTML_TAG_H5 || next == NH_HTML_TAG_H6 || next == NH_HTML_TAG_HEADER || next == NH_HTML_TAG_HGROUP || next == NH_HTML_TAG_HR || next == NH_HTML_TAG_MAIN || next == NH_HTML_TAG_MENU || next == NH_HTML_TAG_NAV || next == NH_HTML_TAG_OL || next == NH_HTML_TAG_P || next == NH_HTML_TAG_PRE || next == NH_HTML_TAG_SECTION || next == NH_HTML_TAG_TABLE || next == NH_HTML_TAG_UL))\
|| (prev == NH_HTML_TAG_LI && next == NH_HTML_TAG_LI))
#define OMITTED_ENDTAG_AT_END_OF_CONTENT_ALLOWED(last, parent) ( \
   (last == NH_HTML_TAG_P && (parent != NH_HTML_TAG_A && parent != NH_HTML_TAG_AUDIO && parent != NH_HTML_TAG_DEL && parent != NH_HTML_TAG_INS && parent != NH_HTML_TAG_MAP && parent != NH_HTML_TAG_NOSCRIPT && parent != NH_HTML_TAG_VIDEO)) \
|| (last == NH_HTML_TAG_LI) \
)
#define IS_PHRASING_CONTENT(tag) ( \
    tag == NH_HTML_TAG_ABBR || tag == NH_HTML_TAG_AUDIO || tag == NH_HTML_TAG_B || tag == NH_HTML_TAG_BDO || tag == NH_HTML_TAG_BR || tag == NH_HTML_TAG_BUTTON || tag == NH_HTML_TAG_CANVAS || tag == NH_HTML_TAG_CITE || tag == NH_HTML_TAG_CODE || tag == NH_HTML_TAG_DATA || tag == NH_HTML_TAG_DATALIST || tag == NH_HTML_TAG_DFN || tag == NH_HTML_TAG_EM || tag == NH_HTML_TAG_EMBED || tag == NH_HTML_TAG_I || tag == NH_HTML_TAG_IFRAME || tag == NH_HTML_TAG_IMG || tag == NH_HTML_TAG_INPUT || tag == NH_HTML_TAG_KBD || tag == NH_HTML_TAG_LABEL || tag == NH_HTML_TAG_MARK || tag == NH_HTML_TAG_MATH || tag == NH_HTML_TAG_METER || tag == NH_HTML_TAG_NOSCRIPT || tag == NH_HTML_TAG_OBJECT || tag == NH_HTML_TAG_OUTPUT || tag == NH_HTML_TAG_PICTURE || tag == NH_HTML_TAG_PROGRESS || tag == NH_HTML_TAG_Q || tag == NH_HTML_TAG_RUBY || tag == NH_HTML_TAG_SAMP || tag == NH_HTML_TAG_SCRIPT || tag == NH_HTML_TAG_SELECT || tag == NH_HTML_TAG_SMALL || tag == NH_HTML_TAG_SPAN || tag == NH_HTML_TAG_STRONG || tag == NH_HTML_TAG_SUB || tag == NH_HTML_TAG_SUP || tag == NH_HTML_TAG_SVG || tag == NH_HTML_TAG_TEXTAREA || tag == NH_HTML_TAG_TIME || tag == NH_HTML_TAG_VAR || tag == NH_HTML_TAG_VIDEO || tag == NH_HTML_TAG_WBR \
)

// DECLARE =========================================================================================

static NH_RESULT Nh_HTML_parseElement(
    char *current_p, Nh_HTML_Node *Node_p, Nh_HTML_Node *Parent_p, NH_HTML_TAG parent, char **end_pp
);
static NH_RESULT Nh_HTML_parseAttributes(
    char *current_p, Nh_HTML_Node *Node_p
);
static NH_RESULT Nh_HTML_parseContent(
    char *current_p, Nh_HTML_Node *Node_p, NH_HTML_TAG parent
);
static NH_RESULT Nh_HTML_allocateAttributes(
    Nh_List *Attributes_p, Nh_String **Name_pp, Nh_String **Value_pp
);

static NH_HTML_TAG Nh_HTML_getTag(
    char *current_p
);

static char *Nh_HTML_seekNext(
    char *current_p, int offset
);
static char *Nh_HTML_seekEndOfElement(
    char *current_p, NH_HTML_TAG parent
);
static char *Nh_HTML_seekEndOfNormalDocType(
    char *begin_p
);

static NH_HTML_TAG Nh_HTML_getClosestMatchingTag(
    char *current_p
);

// PARSER ==========================================================================================

NH_RESULT Nh_HTML_parseDocument(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    char *data_p = Nh_getData(Document_p->URI, NULL);
    if (data_p == NULL) {NH_END(NH_ERROR_BAD_STATE)}

    char *begin_p = Nh_HTML_seekEndOfNormalDocType(data_p);
    if (begin_p == NULL) {begin_p = Nh_HTML_seekNext(data_p, 0);}
    else {begin_p = Nh_HTML_seekNext(begin_p, 0);}
    if (begin_p == NULL) {NH_END(NH_HTML_ERROR_DOCUMENT_EMPTY)}

    char *end_p = NULL;

    Document_p->Tree.Root_p = Nh_allocate(sizeof(Nh_HTML_Node));
    NH_CHECK_MEM(Document_p->Tree.Root_p)
    NH_CHECK(Nh_HTML_parseElement(begin_p, Document_p->Tree.Root_p, NULL, -1, &end_p))

    Nh_freeData(Document_p->URI);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_parseElement(
    char *current_p, Nh_HTML_Node *Node_p, Nh_HTML_Node *Parent_p, NH_HTML_TAG parent, char **end_pp)
{
NH_BEGIN()

    char replace;
    *end_pp = Nh_HTML_seekEndOfElement(current_p, parent);
    NH_CHECK_NULL(*end_pp)
    replace = **end_pp;
    
    **end_pp = '\0';

    NH_HTML_TAG tag = Nh_HTML_getTag(current_p);

    NH_CHECK(Nh_HTML_initNode(Node_p, Parent_p, tag))

    if (!IS_VOID_TAG(tag)) {
        NH_CHECK(Nh_HTML_parseAttributes(current_p, Node_p))
        NH_CHECK(Nh_HTML_parseContent(current_p, Node_p, tag))
    }

    **end_pp = replace;

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_parseAttributes(
    char *current_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    bool inAttributeValue = false, inAttributeName = false, inText = false;

    for (int i = 1; current_p[i] != ' '; current_p = &current_p[i]) 
    {
        if (current_p[i] == '"')            {inText = !inText;}
        if (current_p[i] == '>' && !inText) {NH_END(NH_SUCCESS)}
    }

    current_p = &current_p[1];
    inText = false;

    Nh_List Attributes;
    NH_INIT_LIST(Attributes)

    Nh_String *Name_p  = Nh_allocateString(NULL);
    Nh_String *Value_p = Nh_allocateString(NULL);

    NH_CHECK_MEM(Name_p, Value_p)

    for (int i = 0; i < strlen(current_p); ++i) 
    {
        if (current_p[i] == '"') {inText = !inText;}

        if (!inText) 
        {
            if (!inAttributeName && inAttributeValue) {NH_CHECK(Nh_HTML_allocateAttributes(&Attributes, &Name_p, &Value_p))} // end of attribute=value reached 
            if (inAttributeName && current_p[i] == '>' && Nh_getChars(Name_p) != NULL) {
                NH_CHECK(Nh_HTML_allocateAttributes(&Attributes, &Name_p, &Value_p)) // corner case (attribute without value at the end)
            }

            if (current_p[i] != ' ') { // must be new attribute
                inAttributeName = true; 
                inAttributeValue = false;
            }
        } 

        if (!inText && inAttributeName && current_p[i] == ' ' && Nh_getChars(Name_p) != NULL) {
            NH_CHECK(Nh_HTML_allocateAttributes(&Attributes, &Name_p, &Value_p)) // handle attribute without value
        }
        if (!inText && inAttributeName && current_p[i] == '=') {inAttributeName = false; inAttributeValue = true;}

        if (inAttributeName  && current_p[i] != '"' && current_p[i] != ' ' && current_p[i] != '=') {NH_CHECK(Nh_appendCharToString(Name_p, current_p[i]))}
        if (inAttributeValue && current_p[i] != '"') 
        {
            if (!inText && current_p[i] == '=') {continue;} 
            Nh_appendCharToString(Value_p, current_p[i]);
        }

        if (!inText && current_p[i] == '>') {break;}
    }

    Node_p->Attributes = Attributes;

    Nh_freeString(Name_p, true);
    Nh_freeString(Value_p, true);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_parseContent(
    char *current_p, Nh_HTML_Node *Node_p, NH_HTML_TAG parent)
{
NH_BEGIN()

    Nh_String *String_p = Nh_allocateString(NULL);

    bool inAttributeValue = false;
    int startTagLength;
    for (startTagLength = 0; startTagLength < strlen(current_p); ++startTagLength) {
        if (current_p[startTagLength] == '"') {inAttributeValue = !inAttributeValue;}
        if (!inAttributeValue && current_p[startTagLength] == '>') {break;}
    }
    current_p = &current_p[startTagLength + 1];

    while (current_p != NULL)
    {
        current_p = Nh_HTML_seekNext(current_p, 0);
        int whiteSpaceCount = 0;

        for (int i = 0; current_p != NULL && (*current_p != '<' || current_p[1] == ' ' || current_p[1] == '\n') && *current_p != '\0'; current_p = current_p + 1) 
        {
            if (*current_p != ' ') {whiteSpaceCount = 0;}
            if (*current_p == ' ') {whiteSpaceCount++;}
            if (whiteSpaceCount <= 1 && *current_p != '\r' && *current_p != '\n') {
                NH_CHECK(Nh_appendCharToString(String_p, *current_p))
            }
        }

        if (Nh_getChars(String_p) != NULL && strlen(Nh_getChars(String_p)) > 0) 
        {
            Nh_HTML_Node *Child_p = Nh_allocate(sizeof(Nh_HTML_Node));
            NH_CHECK_MEM(Child_p)
            Nh_HTML_initNode(Child_p, Node_p, NH_HTML_TAG_TEXT);
            Child_p->text_p = Nh_allocateChars(Nh_getChars(String_p));
            NH_CHECK(Nh_addListItem(&Node_p->Children.Unformatted, Child_p))
            Nh_clearString(String_p);
        }

        NH_HTML_TAG tag = Nh_HTML_getTag(current_p);

        if (tag != -1 && current_p[0] == '<' && current_p[1] != '/' && current_p[1] != ' ') 
        {
            Nh_HTML_Node *Child_p = Nh_allocate(sizeof(Nh_HTML_Node));
            NH_CHECK_MEM(Child_p)
            char *end_p = NULL;
            NH_CHECK(Nh_HTML_parseElement(current_p, Child_p, Node_p, parent, &end_p))
            NH_CHECK(Nh_addListItem(&Node_p->Children.Unformatted, Child_p))
            current_p = end_p;
        } 
        else {break;}
    }

    Nh_freeString(String_p, true);

NH_END(NH_SUCCESS)
}

static NH_RESULT Nh_HTML_allocateAttributes(
    Nh_List *Attributes_p, Nh_String **Name_pp, Nh_String **Value_pp)
{
NH_BEGIN()

    char *attributeName_p = Nh_getChars(*Name_pp);
    for (int i = 0; i < strlen(attributeName_p); ++i) {attributeName_p[i] = tolower(attributeName_p[i]);}

    Nh_HashValue *HashValue_p;
    if (hashmap_get(NH_HASHMAPS.HTML.Attributes, attributeName_p, (void**)(&HashValue_p)) != MAP_OK) {
        NH_END(NH_HTML_ERROR_ATTRIBUTE_UNKNOWN)
    }

    NH_CHECK(Nh_HTML_addAttribute(Attributes_p, HashValue_p->number, Nh_getChars(*Value_pp)))

    Nh_freeString(*Name_pp, true);
    Nh_freeString(*Value_pp, false);

    *Name_pp  = Nh_allocateString(NULL);
    *Value_pp = Nh_allocateString(NULL);

    NH_CHECK_MEM(*Name_pp, *Value_pp)
 
NH_END(NH_SUCCESS)
}

// GET =============================================================================================

static NH_HTML_TAG Nh_HTML_getTag(
    char *current_p)
{
NH_BEGIN()
    
    if (current_p == NULL) {NH_END(-1)}
    if (current_p[0] == '<') {current_p = current_p + 1;}    
    if (current_p[0] == '/') {current_p = current_p + 1;}

    int length = 0;
    for (length = 0; length < strlen(current_p) && (current_p[length] != ' ' && current_p[length] != '>'); ++length) {} 
    char replace = current_p[length];
    current_p[length] = '\0';

    char tagName_p[64] = {'\0'};
    strcpy(tagName_p, current_p);
    for (int i = 0; i < strlen(tagName_p); ++i) {tagName_p[i] = tolower(tagName_p[i]);}
    current_p[length] = replace;

    Nh_HashValue *HashValue_p;
    if (hashmap_get(NH_HASHMAPS.HTML.Tags, tagName_p, (void**)(&HashValue_p)) != MAP_OK) {
        NH_END(Nh_HTML_getClosestMatchingTag(current_p))
    }

NH_END(HashValue_p->number)
}

static NH_HTML_TAG Nh_HTML_getClosestMatchingTag(
    char *current_p)
{
NH_BEGIN()

    if (current_p[0] == '<') {current_p = &current_p[1];}

    int length;
    for (length = 0; length < strlen(current_p) && (current_p[length] != ' ' && current_p[length] != '>'); ++length) {} 

    char replace = current_p[length];
    current_p[length] = '\0';

    char tagName_p[64] = {'\0'};
    strcpy(tagName_p, current_p);
    current_p[length] = replace;

    int values_p[1000] = {-1}; // TODO should be safe but who knows
    for (int i = 0; i < NH_HTML_TAGS_PP_COUNT; ++i) {
        values_p[i] = Nh_levenshteinDistance((char*)NH_HTML_TAGS_PP[i], tagName_p, NH_FALSE); 
    }

    int lowest = 100, index = -1;
    for (int i = 0; i < NH_HTML_TAGS_PP_COUNT; ++i) {
        if (values_p[i] < lowest) {index = i; lowest = values_p[i];}
    }

NH_END(index)
}

// SEEK ============================================================================================

static char *Nh_HTML_seekNext(
    char *current_p, int offset)
{
NH_BEGIN()

    if (current_p == NULL) {NH_END(NULL)}

    bool comment = false;

    for (int i = offset; i < strlen(current_p); ++i) 
    {
        if (comment  && i + 2 < strlen(current_p) && current_p[i]     == '-'
                                                  && current_p[i + 1] == '-'
                                                  && current_p[i + 2] == '>'
           ) {comment = false; i += 2;}

        if (!comment && i + 3 < strlen(current_p) && current_p[i]     == '<'
                                                  && current_p[i + 1] == '!'
                                                  && current_p[i + 2] == '-'
                                                  && current_p[i + 3] == '-'
           ) {comment = true; i += 3;}

        if (!comment && DONT_IGNORE(current_p[i])) {NH_END(&current_p[i])}
    }

NH_END(NULL)
}

static char *Nh_HTML_seekEndOfElement(
    char *current_p, NH_HTML_TAG parent)
{
NH_BEGIN()

    NH_HTML_TAG tag = Nh_HTML_getTag(current_p);
    if (tag == -1) {NH_END(NULL)}

    if (IS_VOID_TAG(tag)) {
        while (current_p[0] != '>') {current_p = current_p + 1;}
        NH_END(&current_p[1])
    }

    char *tmp_p = NULL;
    int item = 0;
    bool inAttributeValue = false, 
         inContent        = false, 
         inEndTag         = false, 
         inStartTag       = true; 

    for (int i = 0; current_p != NULL && i < strlen(current_p); ++i) 
    {
        if (inStartTag && current_p[i] == '"') {inAttributeValue = !inAttributeValue;}
        if (inStartTag && !inAttributeValue && current_p[i] == '>') {
            inContent = true; inStartTag = false;
        }
        if (!inContent && current_p[i] == '/' && current_p[i + 1] == '>') {
            NH_END(&current_p[i + 2])
        }
        if ( inContent && current_p[i] == '<' && current_p[i + 1] == '/') 
        {
            int endTag = Nh_HTML_getTag(&current_p[i]);
            if (endTag == tag) {inEndTag = true; inContent = false;}
            else {NH_END(tmp_p != NULL ? tmp_p : &current_p[i - 1])}
        }
        if ( inContent && current_p[i] == '<' && current_p[i + 1] != '/' && current_p[i + 1] != ' ') 
        {
            NH_HTML_TAG next = Nh_HTML_getTag(&current_p[i]);
            if (!IS_PHRASING_CONTENT(next) && item == 0) {tmp_p = &current_p[i]; item++;}
            current_p = Nh_HTML_seekEndOfElement(&current_p[i], tag);
            i = -1;
        }
        if (inEndTag && current_p[i] == '>') {
            NH_END(&current_p[i + 1])
        } 
        if (strlen(current_p) == i + 1) {
            NH_END(tmp_p)
        }
    }

NH_END(NULL)
}

static char *Nh_HTML_seekEndOfNormalDocType(
    char *begin_p)
{
NH_BEGIN()

    char *current_p = Nh_HTML_seekNext(begin_p, 0);
    if (current_p == NULL || strlen(current_p) < 9) {NH_END(NULL)}
    
    bool hasDocType =  current_p[0] == '<' 
                   &&  current_p[1] == '!' 
                   && (current_p[2] == 'D' || current_p[2] == 'd')
                   && (current_p[3] == 'O' || current_p[3] == 'o')
                   && (current_p[4] == 'C' || current_p[4] == 'c')
                   && (current_p[5] == 'T' || current_p[5] == 't')
                   && (current_p[6] == 'Y' || current_p[6] == 'y')
                   && (current_p[7] == 'P' || current_p[7] == 'p')
                   && (current_p[8] == 'E' || current_p[8] == 'e');

    if (hasDocType)
    {
        current_p = Nh_HTML_seekNext(current_p, 9);
        if (current_p == NULL || strlen(current_p) < 4) {NH_END(NULL)}

        hasDocType = (current_p[0] == 'H' || current_p[0] == 'h') 
                  && (current_p[1] == 'T' || current_p[1] == 't')
                  && (current_p[2] == 'M' || current_p[2] == 'm')
                  && (current_p[3] == 'L' || current_p[3] == 'l');
    }

    if (hasDocType)
    {
         current_p = Nh_HTML_seekNext(current_p, 4);
         if (current_p == NULL || strlen(current_p) < 2) {NH_END(NULL)}

         if (current_p[0] == '>') {NH_END(&current_p[1]);}
    }

NH_END(NULL)
}

