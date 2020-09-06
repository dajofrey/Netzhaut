// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Associate.h"
#include "../Header/Properties.h"
#include "../Header/Macros.h"

#include "../../Core/Header/List.h"
#include "../../Core/Header/HashMap.h"

#include "../../HTML/Header/Attribute.h"
#include "../../HTML/Header/Document.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DECLARE ========================================================================================

static bool Nh_CSS_isAttributeSelector(
    const char *selector_p
);

/**
 * Checks if the attribute selector matches a specific node attribute. The implementation currently 
 * also checks for matching node properties.
 */
static bool Nh_CSS_attributeSelectorHit(
    const char *selector_p, Nh_HTML_Node *Node_p, map_t Attributes, map_t Properties
);

static bool Nh_CSS_attributeHit(
    Nh_HTML_Node *Node_p, NH_HTML_ATTRIBUTE type, char *value_p
);
static bool Nh_CSS_propertyHit(
    Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type, char *value_p
);

// HIT =============================================================================================

bool Nh_CSS_selectorHit(
    Nh_HTML_Node *Node_p, Nh_HashMaps *Maps_p, char *selectorString_p, NH_CSS_Pseudo *Pseudo_p,
    NH_CSS_SELECTOR *selector_p, bool handleDependentSelectors)
{
NH_BEGIN()

    bool positive = false;
    NH_CSS_SELECTOR selector = NH_CSS_SELECTOR_UNDEFINED;
    if (Pseudo_p != NULL) {Pseudo_p->_class = Pseudo_p->parentClass = -1;}

    if (!handleDependentSelectors)
    {
             if (strstr(selectorString_p, ">"))                              {selector = NH_CSS_SELECTOR_CHILD_COMBINATOR;}
        else if (strstr(selectorString_p, ":"))                              {selector = NH_CSS_SELECTOR_PSEUDO_CLASS;} 
        else if (strstr(selectorString_p, "::"))                             {selector = NH_CSS_SELECTOR_PSEUDO_ELEMENT;}
        else if (strstr(selectorString_p, "*"))                              {selector = NH_CSS_SELECTOR_UNIVERSAL;}
        else if (selectorString_p[0] == '#')                                 {selector = NH_CSS_SELECTOR_ID;}
        else if (selectorString_p[0] == '.')                                 {selector = NH_CSS_SELECTOR_CLASS;}
        else if (!strcmp(selectorString_p, NH_HTML_TAGS_PP[Node_p->tag])) {selector = NH_CSS_SELECTOR_TYPE;}
    }
    else {
             if (strstr(selectorString_p, ">"))                {selector = NH_CSS_SELECTOR_CHILD_COMBINATOR;}
        else if (Nh_CSS_isAttributeSelector(selectorString_p)) {selector = NH_CSS_SELECTOR_ATTRIBUTE;}
    }

    switch (selector)
    {
        case NH_CSS_SELECTOR_UNIVERSAL :
        case NH_CSS_SELECTOR_TYPE : 
        {
            positive = true;
            break;
        }
        case NH_CSS_SELECTOR_ID :
        {    
            for (int i = 0; i < Node_p->Attributes.count; ++i) {
                if (Nh_HTML_getAttribute(&Node_p->Attributes, i)->type == NH_HTML_ATTRIBUTE_ID) {
                    positive = !strcmp(selectorString_p + 1, Nh_HTML_getAttribute(&Node_p->Attributes, i)->value_p);
                }
            }
            break;
        }
        case NH_CSS_SELECTOR_CLASS :
        {    
            for (int i = 0; i < Node_p->Attributes.count; ++i) {
                if (Nh_HTML_getAttribute(&Node_p->Attributes, i)->type == NH_HTML_ATTRIBUTE_CLASS) {
                    positive = !strcmp(selectorString_p + 1, Nh_HTML_getAttribute(&Node_p->Attributes, i)->value_p);
                }
            }
            break;
        }
        case NH_CSS_SELECTOR_ATTRIBUTE :
        {
            positive = Nh_CSS_attributeSelectorHit(selectorString_p, Node_p, Maps_p->HTML.Attributes, Maps_p->CSS.Properties);
            break;
        }
        case NH_CSS_SELECTOR_CHILD_COMBINATOR :
        {
            if (Node_p->Parent_p != NULL) 
            {
                char leftSelect_p[256] = {'\0'};
                char *rightSelect_p = strstr(selectorString_p, ">") + 1;
                strcpy(leftSelect_p, selectorString_p);
                leftSelect_p[strlen(selectorString_p) - (strlen(rightSelect_p) + 1)] = '\0';

                NH_CSS_Pseudo LeftPseudo, RightPseudo;

                for (int i = 0; i < Node_p->Parent_p->Children.Unformatted.count; ++i) 
                {
                    positive = 
                        Nh_CSS_selectorHit(Node_p->Parent_p, Maps_p, leftSelect_p, &LeftPseudo, selector_p, handleDependentSelectors)
                    &&  Nh_CSS_selectorHit(Node_p, Maps_p, rightSelect_p, &RightPseudo, selector_p, handleDependentSelectors);

                    if (handleDependentSelectors && !positive) {
                        positive = (Nh_CSS_selectorHit(Node_p->Parent_p, Maps_p, leftSelect_p, &LeftPseudo, selector_p, !handleDependentSelectors) 
                                &&  Nh_CSS_selectorHit(Node_p, Maps_p, rightSelect_p, &RightPseudo, selector_p, handleDependentSelectors)) 
                                || (Nh_CSS_selectorHit(Node_p->Parent_p, Maps_p, leftSelect_p, &LeftPseudo, selector_p, handleDependentSelectors) 
                                &&  Nh_CSS_selectorHit(Node_p, Maps_p, rightSelect_p, &RightPseudo, selector_p, !handleDependentSelectors));
                    }
                }

                if (Pseudo_p != NULL) {
                    Pseudo_p->_class = RightPseudo._class;
                    Pseudo_p->parentClass = LeftPseudo._class;
                }
            }
            break;
        }
        case NH_CSS_SELECTOR_PSEUDO_CLASS :
        {
            char pseudoClassString_p[255] = {'\0'};
            strcpy(pseudoClassString_p, strstr(selectorString_p, ":") + 1);

            Nh_HashValue *HashValue_p;
            if (hashmap_get(Maps_p->CSS.PseudoClasses, pseudoClassString_p, (void**)(&HashValue_p)) != MAP_OK) {NH_END(false)}
            if (Pseudo_p != NULL) {Pseudo_p->_class = HashValue_p->number;}

            char leftSelect_p[255] = {'\0'};
            strcpy(leftSelect_p, selectorString_p);
            leftSelect_p[strlen(selectorString_p) - (strlen(pseudoClassString_p) + 1)] = '\0';

            positive = Nh_CSS_selectorHit(Node_p, Maps_p, leftSelect_p, NULL, selector_p, handleDependentSelectors);
            if (positive) {break;}
        }

        case NH_CSS_SELECTOR_PSEUDO_ELEMENT :
        {
            int offset = 0;
            char pseudoElementString_p[255] = {'\0'};
            strcpy(pseudoElementString_p, strstr(selectorString_p, ":") + 1);

            if (pseudoElementString_p[0] == ':') {offset = 1;}

            Nh_HashValue *HashValue_p;
            if (hashmap_get(Maps_p->CSS.PseudoElements, pseudoElementString_p + offset, (void**)(&HashValue_p)) != MAP_OK) {NH_END(false)}
            if (Pseudo_p != NULL) {Pseudo_p->element = HashValue_p->number;}

            char leftSelect_p[255] = {'\0'};
            strcpy(leftSelect_p, selectorString_p);
            leftSelect_p[strlen(selectorString_p) - (strlen(pseudoElementString_p) + 1 + offset)] = '\0';

            positive = Nh_CSS_selectorHit(Node_p, Maps_p, leftSelect_p, NULL, selector_p, handleDependentSelectors);
            break;
        }
    }

    if (selector_p != NULL) {*selector_p = selector;}

NH_END(positive)
}

// HELPER ==========================================================================================

static bool Nh_CSS_isAttributeSelector(
    const char *selector_p)
{
NH_BEGIN()

    int c = 0;
    for (int i = 0; i < strlen(selector_p); ++i) {
        if (selector_p[i] == '[') {c = 1;}
        if (c == 1 && selector_p[i] == ']') {c = 2;}
    }

NH_END(c == 2)
}

static bool Nh_CSS_attributeSelectorHit(
    const char *selector_p, Nh_HTML_Node *Node_p, map_t Attributes, map_t Properties)
{
NH_BEGIN()

    char element_p[256] = {'\0'};
    int count = 0;
    
    for (int i = 0; i < strlen(selector_p); ++i) {if (selector_p[i] == '[') {break;}++count;}
    memcpy((void*)element_p, (void*)selector_p, sizeof(char) * count);
    
    if (!strcmp(element_p, NH_HTML_TAGS_PP[Node_p->tag]))
    {
        char attr_p[256] = {'\0'}, val_p[256] = {'\0'};
        bool attr = false, val = false;
        
        count = 0;
        for (int i = 0; i < strlen(selector_p); ++i) 
        {
            if (selector_p[i] == ']')        {break;}
            if (val && selector_p[i] != '"') {val_p[count++] = selector_p[i];}
            if (selector_p[i] == '=')        {attr = false; val = true; count = 0;}
            if (attr)                        {attr_p[count++] = selector_p[i];}
            if (selector_p[i] == '[')        {attr = true;}
        }

        Nh_HashValue *HashValue_p;
        bool validAttribute = false, validProperty = false;

        if (hashmap_get(Attributes, attr_p, (void**)(&HashValue_p)) != MAP_OK) {
            if (hashmap_get(Properties, attr_p, (void**)(&HashValue_p)) != MAP_OK) {NH_END(false)}
            else {validProperty = true;}
        } 
        else {validAttribute = true;}

        if (validAttribute) {NH_END(Nh_CSS_attributeHit(Node_p, HashValue_p->number, val_p[0] != '\0' ? val_p : NULL))}
        if (validProperty)  {NH_END(Nh_CSS_propertyHit(Node_p, HashValue_p->number, val_p[0] != '\0' ? val_p : NULL))}
    }

NH_END(false)
}

static bool Nh_CSS_attributeHit(
    Nh_HTML_Node *Node_p, NH_HTML_ATTRIBUTE type, char *value_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Attributes.count; ++i) {
        if (Nh_HTML_getAttribute(&Node_p->Attributes, i)->type == type) {
            if (value_p != NULL) {NH_END(!strcmp(value_p, Nh_HTML_getAttribute(&Node_p->Attributes, i)->value_p))}
            else {NH_END(true)}
        }
    }

NH_END(false)
}

static bool Nh_CSS_propertyHit(
    Nh_HTML_Node *Node_p, NH_CSS_PROPERTY type, char *value_p)
{
NH_BEGIN()

    for (int i = 0; i < Node_p->Properties.count; ++i) {
        if (Nh_CSS_getProperty(&Node_p->Properties, i)->type == type) {
            if (value_p == NULL) {NH_END(true)}
            for (int j = 0; j < Nh_CSS_getProperty(&Node_p->Properties, i)->valueCount; ++j) {
                if (!strcmp(value_p, Nh_CSS_getProperty(&Node_p->Properties, i)->values_pp[j])) {NH_END(true)}
            }
        }
    }

NH_END(false)
}

