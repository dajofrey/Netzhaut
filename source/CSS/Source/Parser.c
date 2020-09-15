// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Parser.h"
#include "../Header/Sheet.h"
#include "../Header/Properties.h"
#include "../Header/Macros.h"

#include "../../Core/Header/List.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/Memory.h"
#include "../../HTML/Header/Attribute.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DECLARE ========================================================================================

static inline NH_RESULT parseStyleRuleSet(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *Rule_p, int i
);
static inline NH_RESULT parseDeclaration(
    char *declBegin_p, NH_CSS_Declaration *Declaration_p
);
static inline NH_RESULT parseSelectorList(
    char *begin_p, NH_CSS_StyleRuleSet *RuleSet_p
);
static inline NH_RESULT parseProperty(
    char *declBegin_p, NH_CSS_Declaration *Declaration_p
);
static inline NH_RESULT parsePropertyValues(
    char *valueBegin_p, NH_CSS_Declaration *Declaration_p
);

static inline char *next(
    char *str_p
);
static inline void getSelectorList(
    char *begin_p, char *selector_p, size_t selectorSize
);
static inline char *jumpOverSelectorList(
    char *begin_p
);

static inline char *getStyleRuleSetBegin(
    Nh_CSS_Sheet *Sheet_p, int number, int *count_p
);
static inline char *getDeclarationBegin(
    char *begin_p, int number, int *count_p
);
static inline char *getPropertyValueBegin(
    char *valueBegin_p, int number, int *count_p
);

// PARSE ===========================================================================================

NH_RESULT Nh_CSS_parseSheet(
    Nh_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Sheet_p)

    int styleRuleSetCount = 0;
    getStyleRuleSetBegin(Sheet_p, -1, &styleRuleSetCount);
    
    if (styleRuleSetCount < 0) {NH_END(NH_ERROR_BAD_STATE)}

    if (styleRuleSetCount > 0) 
    {
        Sheet_p->RuleSets_p = Nh_allocate(sizeof(NH_CSS_RuleSet) * styleRuleSetCount);
        NH_CHECK_MEM(Sheet_p->RuleSets_p)

        for (int i = 0; i < styleRuleSetCount; ++i) {
            Sheet_p->RuleSets_p[i].type = NH_CSS_RULE_SET_STYLE;
            Sheet_p->RuleSets_p[i].Data_p = Nh_allocate(sizeof(NH_CSS_StyleRuleSet));
            NH_CHECK_MEM(Sheet_p->RuleSets_p[i].Data_p)
        }
    }

    for (int i = 1; i <= styleRuleSetCount; ++i) {
        NH_CHECK(parseStyleRuleSet(Sheet_p, Sheet_p->RuleSets_p[i - 1].Data_p, i - 1))
    }

    Sheet_p->ruleSetCount = styleRuleSetCount;

NH_END(NH_SUCCESS)
}

static inline NH_RESULT parseStyleRuleSet(
    Nh_CSS_Sheet *Sheet_p, NH_CSS_StyleRuleSet *RuleSet_p, int i)
{
NH_BEGIN()

    char *begin_p = getStyleRuleSetBegin(Sheet_p, i, NULL);
    if (begin_p == NULL) {NH_END(NH_SUCCESS)}

    parseSelectorList(begin_p, RuleSet_p);

    begin_p = jumpOverSelectorList(begin_p);

    getDeclarationBegin(begin_p, 0, &RuleSet_p->declarationCount);

    RuleSet_p->Declarations_p = Nh_allocate(sizeof(NH_CSS_Declaration) * RuleSet_p->declarationCount);
    NH_CHECK_MEM(RuleSet_p->Declarations_p)

    for (int j = 1; j <= RuleSet_p->declarationCount; ++j) 
    {
        char *declBegin_p = getDeclarationBegin(begin_p, j, NULL);
        NH_CHECK_NULL(declBegin_p)

        parseDeclaration(declBegin_p, &RuleSet_p->Declarations_p[j - 1]);
    }

NH_END(NH_SUCCESS)
}

static inline NH_RESULT parseSelectorList(
    char *begin_p, NH_CSS_StyleRuleSet *RuleSet_p)
{
NH_BEGIN()

    char selectorList_p[255] = {'\0'};
    getSelectorList(begin_p, selectorList_p, 255);

    RuleSet_p->selectorCount = 1;
    
    for (int i = 0; i < strlen(selectorList_p); ++i) {
        if (selectorList_p[i] == ',') {RuleSet_p->selectorCount++;}
    }

    RuleSet_p->selectors_pp = Nh_allocate(sizeof(char*) * RuleSet_p->selectorCount);
    NH_CHECK_MEM(RuleSet_p->selectors_pp)

    int count = 0, index1 = 0, index2 = 0;
    char tmp_p[128] = {'\0'};

    for (int i = 0; i < strlen(selectorList_p); ++i) 
    {
        count++;
        tmp_p[index1++] = selectorList_p[i];
        
        if (selectorList_p[i] == ',' || i == strlen(selectorList_p) - 1) 
        {
            if (tmp_p[index1 - 1] == ',') {tmp_p[index1 - 1] = '\0';}

            RuleSet_p->selectors_pp[index2] = Nh_allocate(sizeof(char) * (count + 1));
            NH_CHECK_MEM(RuleSet_p->selectors_pp[index2])
            
            strcpy(RuleSet_p->selectors_pp[index2++], tmp_p);

            for (int j = 0; j < index1; ++j) {tmp_p[j] = '\0';}
            index1 = 0;
        }
    }

NH_END(NH_SUCCESS)
}

static inline NH_RESULT parseDeclaration(
    char *declBegin_p, NH_CSS_Declaration *Declaration_p)
{
NH_BEGIN()

    NH_CHECK(parseProperty(declBegin_p, Declaration_p))

    int count = 0;
    for (count = 0; declBegin_p[count] != ':'; ++count);
    
    char *valueBegin_p = declBegin_p + count + 1;
    valueBegin_p = next(valueBegin_p);

    NH_CHECK(parsePropertyValues(valueBegin_p, Declaration_p))

NH_END(NH_SUCCESS)
}

static inline NH_RESULT parseProperty(
    char *declBegin_p, NH_CSS_Declaration *Declaration_p)
{
NH_BEGIN()

    int count = 0;
    for (int i = 0; declBegin_p[i] != ':'; ++i) {if (declBegin_p[i] != ' ' && declBegin_p[i] != '\n') {count++;}}

    Declaration_p->property_p = Nh_allocate(sizeof(char) * count + 1);
    NH_CHECK_MEM(Declaration_p->property_p)

    count = 0;
    for (int i = 0; declBegin_p[i] != ':'; ++i) {if (declBegin_p[i] != ' ' && declBegin_p[i] != '\n') {Declaration_p->property_p[count++] = declBegin_p[i];}}
    Declaration_p->property_p[count] = '\0';

NH_END(NH_SUCCESS)
}

static inline NH_RESULT parsePropertyValues(
    char *valuesBegin_p, NH_CSS_Declaration *Declaration_p)
{
NH_BEGIN()

    bool inFunction = false;
    bool inBetween = false;

    Declaration_p->valueCount = 0;
    getPropertyValueBegin(valuesBegin_p, 0, &Declaration_p->valueCount);

    Declaration_p->values_pp = Nh_allocate(sizeof(char*) * Declaration_p->valueCount);
    NH_CHECK_MEM(Declaration_p->values_pp)

    for (int i = 1; i <= Declaration_p->valueCount; ++i) 
    {
        char *valueBegin_p = getPropertyValueBegin(valuesBegin_p, i, NULL);

        int charCount = 0;
        bool inFunction = false;
        for (charCount = 0; inFunction || (valueBegin_p[charCount] != ' ' && valueBegin_p[charCount] != '\n' && valueBegin_p[charCount] != ';'); ++charCount) {
            if (valueBegin_p[charCount] == '(') {inFunction = true;}
            if (valueBegin_p[charCount] == ')') {inFunction = false;}
        }

        Declaration_p->values_pp[i - 1] = Nh_allocate(sizeof(char) * (charCount + 1));
        NH_CHECK_MEM(Declaration_p->values_pp[i - 1])

        charCount = 0;
        for (charCount = 0; inFunction || (valueBegin_p[charCount] != ' ' && valueBegin_p[charCount] != '\n' && valueBegin_p[charCount] != ';'); ++charCount) {
            if (valueBegin_p[charCount] == '(') {inFunction = true;}
            if (valueBegin_p[charCount] == ')') {inFunction = false;}
            Declaration_p->values_pp[i - 1][charCount] = valueBegin_p[charCount];
        }
        Declaration_p->values_pp[i - 1][charCount] = '\0';
    }

NH_END(NH_SUCCESS)
}

static inline char *getStyleRuleSetBegin(
    Nh_CSS_Sheet *Sheet_p, int number, int *count_p)
{
NH_BEGIN()

    char *source_p = Nh_getData(Sheet_p->URI, NULL);
    char *begin_p = Nh_getData(Sheet_p->URI, NULL);
    bool styleRule = false, ignore = false;
    int styleRuleSetCount = 0;
    
    while (source_p != NULL) 
    {
        source_p = next(source_p);
        if (source_p == NULL) {break;}

        if (styleRule && *source_p == '}') {
            styleRuleSetCount++; 
            styleRule = false; 
            if (number > -1 && styleRuleSetCount - 1 == number) {NH_END(next(begin_p))}
        }
        
        if (*source_p == '}') {ignore = false; begin_p = source_p + 1;}
        if (!ignore    && *source_p == '{') {styleRule = true;}
        if (!styleRule && *source_p == '@') {ignore = true;}

        if (strlen(source_p) > 1) {source_p = source_p + 1;}
        else {break;} 
    }

    if (count_p != NULL) {*count_p = styleRuleSetCount;}

NH_END(NULL)
}

static inline char *next(
    char *str_p)
{
NH_BEGIN()

    int comment = 0;

    for (int i = 0; i < strlen(str_p); ++i)
    {
        if (comment == 3 && str_p[i] != '/') {comment = 2;}
        if (comment == 3 && str_p[i] == '/') {comment = 0;}
        
        if (comment == 2 && str_p[i] == '*') {comment = 3;}

        if (comment == 1 && str_p[i] == '*') {comment = 2;}
        if (comment == 1 && str_p[i] != '*') {comment = 0;}

        if (comment == 0 && str_p[i] == '/') {comment = 1;}
        if (comment == 0 && str_p[i] != ' ' && str_p[i] != '\n' && str_p[i] != '\t' && str_p[i] != '\v') {NH_END(str_p + i)}
    }

NH_END(NULL)
}

static inline void getSelectorList(
    char *begin_p, char *selector_p, size_t selectorSize)
{
NH_BEGIN()

    int count = 0;
    for (int j = 0; j < selectorSize; ++j) {
        if (begin_p[j] != '{') {
            if (begin_p[j] != ' ') {selector_p[count++] = begin_p[j];} 
        } else {break;}
    }

NH_SILENT_END()
}

static inline char *jumpOverSelectorList(
    char *begin_p)
{
NH_BEGIN()

    bool rule = false;

    for (int i = 0; i < strlen(begin_p); ++i) 
    {
        if (rule) {NH_END(next(begin_p + i))}
        if (begin_p[i] == '{') {rule = true;}
    }

NH_END(NULL)
}

static inline char *getDeclarationBegin(
    char *begin_p, int number, int *count_p)
{
NH_BEGIN()

    char *declBegin_p = begin_p;
    int declCount = 0;
    bool inDecl = false;

    for (int i = 0; i < strlen(begin_p); ++i) 
    {
        if (begin_p[i] == ':') {inDecl = true;}
        
        if (inDecl && begin_p[i] == ';') {
            inDecl = false; 
            declCount++; 
            if (number > 0 && declCount == number) {NH_END(next(declBegin_p))}
            declBegin_p = begin_p + i + 1;
        }
        
        if (begin_p[i] == '}') {break;}
    }

    if (count_p != NULL) {*count_p = declCount;}

NH_END(NULL)
}

static inline char *getPropertyValueBegin(
    char *valueBegin_p, int number, int *count_p)
{
NH_BEGIN()

    char *begin_p = valueBegin_p;

    bool inFunction = false;
    bool inBetween = false;

    int count = 0;
    for (int i = 0; strlen(valueBegin_p); ++i) 
    {
        if (valueBegin_p[i] == '(')               {inFunction = true;}
        if (valueBegin_p[i] == ')' && inFunction) {inFunction = false;}

        if (!inFunction &&  inBetween && (valueBegin_p[i] != ' ' || valueBegin_p[i] != '\n')) {begin_p = valueBegin_p + i; inBetween = false;}
        if (!inFunction && !inBetween && (valueBegin_p[i] == ' ' || valueBegin_p[i] == ';' )) {
            count++; 
            if (number > 0 && count == number) {NH_END(next(begin_p))}
            inBetween = true;
        }

        if (!inFunction && valueBegin_p[i] == ';') {break;}
    }

    if (count_p != NULL) {*count_p = count;}

NH_END(NULL)
}

