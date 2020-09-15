#ifndef NH_CSS_SHEET_H
#define NH_CSS_SHEET_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../API/Header/Netzhaut.h"
#include "../../Core/Header/List.h"
#include "../../Core/Header/URI.h"

typedef struct Nh_HTML_Document Nh_HTML_Document;
typedef struct Nh_HTML_Node Nh_HTML_Node;

#endif

/** @addtogroup CSSEnums Enums
 *  \ingroup CSS
 *  @{
 */

    typedef enum NH_CSS_RULE_SET {
        NH_CSS_RULE_SET_STYLE,
        NH_CSS_RULE_SET_IMPORT,
        NH_CSS_RULE_SET_FONT_FACE,
        NH_CSS_RULE_SET_KEY_FRAMES,
        NH_CSS_RULE_SET_MEDIA,
        NH_CSS_RULE_SET_SUPPORTS,
        NH_CSS_RULE_SET_UNKNOWN,
    } NH_CSS_RULE_SET;

/** @} */

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_RuleSet {
        NH_CSS_RULE_SET type;
        void *Data_p;
    } NH_CSS_RuleSet;

    typedef struct Nh_CSS_Sheet {
        Nh_URI URI;
        int ruleSetCount;
        NH_CSS_RuleSet *RuleSets_p;
        Nh_HTML_Node *Apply_p;
    } Nh_CSS_Sheet;
    
    typedef struct NH_CSS_Declaration {
        char *property_p;
        int valueCount;
        char **values_pp;
    } NH_CSS_Declaration;
    
    typedef struct NH_CSS_StyleRuleSet {
        int selectorCount;
        char **selectors_pp;
        int declarationCount;
        NH_CSS_Declaration *Declarations_p;
    } NH_CSS_StyleRuleSet;

/** @} */

/** @addtogroup CSSFunctions Functions
 *  \ingroup CSS
 *  @{
 */

    NH_RESULT Nh_CSS_addSheets(
        Nh_HTML_Document *Document_p, bool addDefaultSheets
    );

    NH_RESULT Nh_CSS_addSheet(
        Nh_List *Sheets_p, Nh_CSS_Sheet *Sheet_p
    );
    
    Nh_CSS_Sheet *Nh_CSS_getSheet(
        Nh_List *Sheets_p, int index
    );

    void Nh_CSS_destroySheetList(
        Nh_List *Sheets_p
    );

    void Nh_CSS_destroySheet(
        Nh_CSS_Sheet *Sheet_p
    );

    void Nh_CSS_initSheet(
        Nh_CSS_Sheet *Sheet_p
    );

    char *Nh_CSS_stringifySheet(
        Nh_CSS_Sheet *Sheet_p
    );

/** @} */

#endif 
