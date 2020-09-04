// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Sheet.h"
#include "../Header/Properties.h"
#include "../Header/Macros.h"

#include "../Header/Sheets/Misc.h"
#include "../Header/Sheets/SelectOption.h"
#include "../Header/Sheets/Header.h"
#include "../Header/Sheets/List.h"

#include "../Header/Icons/Expand.h"
#include "../Header/Icons/Disc.h"
#include "../Header/Icons/Circle.h"
#include "../Header/Icons/Square.h"

#include "../../Core/Header/List.h"
#include "../../Core/Header/String.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/HashMap.h"
#include "../../Core/Header/Memory.h"
#include "../../Core/Header/File.h"

#include "../../HTML/Header/Attribute.h"

#include NH_DEBUG
#include NH_DEFAULT_CHECK
#include NH_CSS_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

// DECLARE ========================================================================================

static inline NH_RESULT Nh_CSS_addDefaultSheets(
    Nh_HTML_Document *Document_p
);
static inline NH_RESULT Nh_CSS_addSheetsFromStyleElements(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
);
static inline NH_RESULT Nh_CSS_addSheetsFromLinks(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
);
static inline NH_RESULT Nh_CSS_addSheetsFromAttributes(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p
);

// INIT ============================================================================================

void Nh_CSS_initSheet(
    NH_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

    Sheet_p->ruleSetCount = 0;
    Sheet_p->RuleSets_p   = NULL;
    Sheet_p->Apply_p      = NULL;

NH_SILENT_END()
}

// GET =============================================================================================

NH_CSS_Sheet *Nh_CSS_getSheet(
    Nh_List *Sheets_p, int index)
{
NH_BEGIN()
NH_END(Nh_getListItem(Sheets_p, index))
}

char *Nh_CSS_getDefaultSheet(
    NH_INTERNAL_FILE file)
{
NH_BEGIN()

    switch (file)
    {
        case NH_INTERNAL_FILE_MISC          : NH_END(MiscCSS) break; 
        case NH_INTERNAL_FILE_SELECT_OPTION : NH_END(SelectOptionCSS) break;
        case NH_INTERNAL_FILE_HEADER        : NH_END(HeaderCSS) break;
        case NH_INTERNAL_FILE_LIST          : NH_END(ListCSS) break;
    }

NH_END(NULL)
}

char *Nh_CSS_getDefaultIcon(
    NH_INTERNAL_FILE file, long *size_p)
{
NH_BEGIN()

    if (size_p != NULL)
    {
        switch (file)
        {
            case NH_INTERNAL_FILE_EXPAND : *size_p =  ExpandPNG_len; break;
            case NH_INTERNAL_FILE_DISC   : *size_p =  DiscPNG_len; break;
            case NH_INTERNAL_FILE_CIRCLE : *size_p =  CirclePNG_len; break;
            case NH_INTERNAL_FILE_SQUARE : *size_p =  SquarePNG_len; break;
        }
    }

    switch (file)
    {
        case NH_INTERNAL_FILE_EXPAND : NH_END(ExpandPNG) break;
        case NH_INTERNAL_FILE_DISC   : NH_END(DiscPNG) break;
        case NH_INTERNAL_FILE_CIRCLE : NH_END(CirclePNG) break;
        case NH_INTERNAL_FILE_SQUARE : NH_END(SquarePNG) break;
    }

NH_END(NULL)
}

// ADD =============================================================================================

NH_RESULT Nh_CSS_addSheets(
    Nh_HTML_Document *Document_p, bool addDefaultSheets)
{
NH_BEGIN()

    if (addDefaultSheets) {NH_CHECK(Nh_CSS_addDefaultSheets(Document_p))}

    NH_CHECK(Nh_CSS_addSheetsFromStyleElements(Document_p, Document_p->Tree.Root_p))
    NH_CHECK(Nh_CSS_addSheetsFromLinks(Document_p, Document_p->Tree.Root_p))
    NH_CHECK(Nh_CSS_addSheetsFromAttributes(Document_p, Document_p->Tree.Root_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_CSS_addSheet(
    Nh_List *Sheets_p, NH_CSS_Sheet *Prototype_p)
{
NH_BEGIN()

    NH_CHECK_NULL(Sheets_p, Prototype_p)

    NH_CSS_Sheet *Sheet_p = Nh_allocate(sizeof(NH_CSS_Sheet));
    NH_CHECK_MEM(Sheet_p)
    
    Sheet_p->RuleSets_p   = NULL;
    Sheet_p->ruleSetCount = Prototype_p->ruleSetCount;
    Sheet_p->Apply_p      = Prototype_p->Apply_p;
    Sheet_p->URI          = Prototype_p->URI;

    if (Sheet_p->ruleSetCount > 0) {Sheet_p->RuleSets_p = Prototype_p->RuleSets_p;}
    
    NH_CHECK(Nh_addListItem(Sheets_p, Sheet_p))

NH_END(NH_SUCCESS)
}

// ADD SHEETS ======================================================================================

static inline NH_RESULT Nh_CSS_addDefaultSheets(
    Nh_HTML_Document *Document_p)
{
NH_BEGIN()

    NH_CSS_Sheet Sheet;
    Nh_CSS_initSheet(&Sheet);

    Sheet.URI = Nh_createInternalURN("css:misc");
    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))

    Sheet.URI = Nh_createInternalURN("css:selectoption"); 
    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))

    Sheet.URI = Nh_createInternalURN("css:header");
    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))

    Sheet.URI = Nh_createInternalURN("css:list");
    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_addSheetsFromStyleElements(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_CSS_Sheet Sheet;
    Nh_CSS_initSheet(&Sheet);

    if (Node_p->tag == NH_HTML_TAG_STYLE && Node_p->Children.Unformatted.count > 0) 
    {
        Nh_HTML_Node *Sheet_p = Nh_getListItem(&Node_p->Children.Unformatted, 0);
        if (Sheet_p->text_p != NULL) {
            Sheet.URI = Nh_createInternalURL(Sheet_p, NH_INTERNAL_URL_HTML_NODE_TEXT);
            NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))
        }
    }

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
        NH_CHECK(Nh_CSS_addSheetsFromStyleElements(Document_p, Nh_getListItem(&Node_p->Children.Unformatted, i)))
    }

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_addSheetsFromLinks(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_CSS_Sheet Sheet;
    Nh_CSS_initSheet(&Sheet);

    if (Node_p->tag == NH_HTML_TAG_LINK) 
    {
        bool stylesheet = false;
        
        for (int i = 0; i < Node_p->Attributes.count; ++i) {
            if (Nh_HTML_getAttribute(&Node_p->Attributes, i)->type == NH_HTML_ATTRIBUTE_REL) {
                stylesheet = !strcmp(Nh_HTML_getAttribute(&Node_p->Attributes, i)->value_p, "stylesheet");
            }
        }
        
        if (stylesheet) 
        {
            for (int i = 0; i < Node_p->Attributes.count; ++i) 
            {
                if (Nh_HTML_getAttribute(&Node_p->Attributes, i)->type == NH_HTML_ATTRIBUTE_HREF) 
                {
                    Sheet.URI = Nh_createURI(Nh_HTML_getAttribute(&Node_p->Attributes, i)->value_p, NULL, &Document_p->URI);
                    NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))
                }
            }
        }
    }

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
        NH_CHECK(Nh_CSS_addSheetsFromLinks(Document_p, Nh_getListItem(&Node_p->Children.Unformatted, i)))
    }

NH_END(NH_SUCCESS)
}

static inline NH_RESULT Nh_CSS_addSheetsFromAttributes(
    Nh_HTML_Document *Document_p, Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    NH_CSS_Sheet Sheet;
    Nh_CSS_initSheet(&Sheet);

    for (int i = 0; i < Node_p->Attributes.count; ++i) 
    {
        if (Nh_HTML_getAttribute(&Node_p->Attributes, i)->type == NH_HTML_ATTRIBUTE_STYLE) 
        {
            Sheet.URI = Nh_createInternalURL(Nh_HTML_getAttribute(&Node_p->Attributes, i), NH_INTERNAL_URL_HTML_NODE_ATTRIBUTE_VALUE);
            Sheet.Apply_p = Node_p;

            NH_CHECK(Nh_CSS_addSheet(&Document_p->Sheets, &Sheet))
        }
    }

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
        NH_CHECK(Nh_CSS_addSheetsFromAttributes(Document_p, Nh_getListItem(&Node_p->Children.Unformatted, i)))
    }

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_CSS_destroySheetList(
    Nh_List *Sheets_p)
{
NH_BEGIN()

    if (Sheets_p == NULL) {NH_SILENT_END()}

    for (int i = 0; i < Sheets_p->count; ++i) 
    {
        NH_CSS_Sheet *Sheet_p = Nh_CSS_getSheet(Sheets_p, i);
        if (Sheet_p != NULL) {Nh_CSS_destroySheet(Sheet_p);}
    }
    
    Nh_destroyList(Sheets_p, true);
    
NH_SILENT_END()
}

void Nh_CSS_destroySheet(
    NH_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

    Nh_freeURI(Sheet_p->URI);

    for (int i = 0; i < Sheet_p->ruleSetCount; ++i) 
    {
        if (Sheet_p->RuleSets_p[i].type == NH_CSS_RULE_SET_STYLE) 
        {
            NH_CSS_StyleRuleSet *RuleSet_p = Sheet_p->RuleSets_p[i].Data_p;
            
            for (int j = 0; j < RuleSet_p->selectorCount; ++j) {Nh_free(RuleSet_p->selectors_pp[j]);}
            Nh_free(RuleSet_p->selectors_pp);
            
            for (int j = 0; j < RuleSet_p->declarationCount; ++j) 
            {
                NH_CSS_Declaration *Declaration_p = &RuleSet_p->Declarations_p[j];
                for (int k = 0; k < Declaration_p->valueCount; ++k) {Nh_free(Declaration_p->values_pp[k]);}
                Nh_free(Declaration_p->values_pp);
                Nh_free(Declaration_p->property_p);
            }
            Nh_free(RuleSet_p->Declarations_p);
            Nh_free(RuleSet_p);
        }
    }

    Nh_free(Sheet_p->RuleSets_p);

NH_SILENT_END()
}

// STRINGIFY =======================================================================================

char *Nh_CSS_stringifySheet(
    NH_CSS_Sheet *Sheet_p)
{
NH_BEGIN()

#include NH_CUSTOM_CHECK

    Nh_String *String_p = Nh_allocateString(NULL);

    NH_CHECK(NULL, Nh_appendToString(String_p, "{\n"))
//    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "  \e[1;32mSheet\e[0m: %s\n", Sheet_p->path_p))
    NH_CHECK(NULL, Nh_appendFormatToString(String_p, "  Total Rule-Set count: %d\n\n", Sheet_p->ruleSetCount))

    for (int i = 0; i < Sheet_p->ruleSetCount; ++i) 
    {
        if (Sheet_p->RuleSets_p[i].type == NH_CSS_RULE_SET_STYLE)
        {
            NH_CSS_StyleRuleSet *RuleSet_p = Sheet_p->RuleSets_p[i].Data_p;
 
            NH_CHECK(NULL, Nh_appendFormatToString(String_p, "  \e[1;32mStyle Rule-Set Nr. %d\e[0m\n", i))              
            NH_CHECK(NULL, Nh_appendToString(String_p, "    Selectors:"))

            for (int j = 0; j < RuleSet_p->selectorCount; ++j) {
                if (j == 0) {NH_CHECK(NULL, Nh_appendFormatToString(String_p, " \e[1;34m%s\e[0m\n", RuleSet_p->selectors_pp[j]))}
                else        {NH_CHECK(NULL, Nh_appendFormatToString(String_p, "               \e[1;34m%s\e[0m\n", RuleSet_p->selectors_pp[j]))}
            }

            NH_CHECK(NULL, Nh_appendToString(String_p, "    Declarations:\n"))

            int max = 0;
            for (int j = 0; j < RuleSet_p->declarationCount; ++j) {
                if (max < strlen(RuleSet_p->Declarations_p[j].property_p)) {max = strlen(RuleSet_p->Declarations_p[j].property_p);}
            }

            for (int j = 0; j < RuleSet_p->declarationCount; ++j)
            {
                NH_CHECK(NULL, Nh_appendFormatToString(String_p, "      Property: \e[1;36m%s\e[0m", RuleSet_p->Declarations_p[j].property_p))

                for (int k = 0; k < max - strlen(RuleSet_p->Declarations_p[j].property_p); ++k) {NH_CHECK(NULL, Nh_appendToString(String_p, " "))}
                NH_CHECK(NULL, Nh_appendToString(String_p, " : "))

                for (int k = 0; k < RuleSet_p->Declarations_p[j].valueCount; ++k) 
                {
                    if (k == 0) {NH_CHECK(NULL, Nh_appendFormatToString(String_p, "%s\n", RuleSet_p->Declarations_p[j].values_pp[k]))}
                    else {
                        for (int l = 0; l < max + 16; ++l) {
                             NH_CHECK(NULL, Nh_appendToString(String_p, " "))
                        } 
                        NH_CHECK(NULL, Nh_appendFormatToString(String_p, " : %s\n", RuleSet_p->Declarations_p[j].values_pp[k]))
                    }
                }
            }

            if (i + 1 < Sheet_p->ruleSetCount) {NH_CHECK(NULL, Nh_appendToString(String_p, "\n"))}
        }
    }

    NH_CHECK(NULL, Nh_appendToString(String_p, "}\n"))

    char *string_p = Nh_getChars(String_p);
    Nh_freeString(String_p, false);

#include NH_DEFAULT_CHECK

NH_END(string_p)
}

