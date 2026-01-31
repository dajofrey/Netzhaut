// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "TreeConstructionHelper.h"
#include "AdoptionAgencyAlgorithm.h"
#include "Algorithms.h"

#include "../../nh-dom/Interfaces/Attr.h"
#include "../../nh-dom/Interfaces/Element.h"
#include "../../nh-dom/Interfaces/Node.h"
#include "../../nh-dom/Interfaces/NodeList.h"
#include "../../nh-dom/Interfaces/Comment.h"
#include "../../nh-dom/Interfaces/Document.h"
#include "../../nh-dom/Interfaces/DocumentType.h"
#include "../../nh-dom/Interfaces/Text.h"

#include "../../nh-core/Util/Array.h"
#include "../../nh-webidl/Runtime/Object.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// ADOPTION AGENCY ALGORITHM =======================================================================

bool inB = false;

typedef struct nh_html_AdoptionAgencyBookmark {
    int left;
    int right;
} nh_html_AdoptionAgencyBookmark;

static void nh_html_runAdoptionAgencyAlgorithmInnerLoop(
    nh_html_Parser *Parser_p, nh_ecmascript_Object *FormattingElement_p, nh_ecmascript_Object *FurthestBlock_p, 
    nh_html_AdoptionAgencyBookmark *Bookmark_p)
{
    nh_ecmascript_Object *Node_p = FurthestBlock_p;
    nh_ecmascript_Object *LastNode_p = Node_p;

    int innerLoopCounter = 0;

    while (++innerLoopCounter)
    {
        // TODO
        exit(0);
    }

    return;
}

static void nh_html_runAdoptionAgencyAlgorithmOuterLoop(
    nh_html_Parser *Parser_p, nh_encoding_UTF8String *Subject_p)
{
    int counter = 0;

    while (true) 
    {
        if (counter >= 8) {break;}
        counter++;
    
        nh_ecmascript_Object *FormattingElement_p = NULL;
        for (int i = Parser_p->ActiveFormattingElements.size - 1; i >= 0; --i) 
        {
            FormattingElement_p = Parser_p->ActiveFormattingElements.pp[i];
            if (!FormattingElement_p) {break;} // check marker
            nh_ecmascript_Object *Element_p = NH_WEBIDL_GET_DOM_ELEMENT(FormattingElement_p);
            if (Element_p && !strcmp(nh_dom_getLocalName(Element_p)->p, Subject_p->p)) {
                break;
            }
            FormattingElement_p = NULL;
        }
   
        if (!FormattingElement_p) {
            // TODO 
            break;
        }

        if (!nh_core_inList(&Parser_p->OpenElements, FormattingElement_p)) {
            // parse error
            nh_core_removeFromList2(&Parser_p->ActiveFormattingElements, false, FormattingElement_p);
            break; 
        }

        if (!nh_html_hasElementInScope(Parser_p, Subject_p->p)) {
            // parse error
            break;
        }
    
        if (nh_html_getCurrentNode(Parser_p) != FormattingElement_p) {
            // parse error
        }
    
        nh_ecmascript_Object *FurthestBlock_p = NULL;
        for (unsigned long i = nh_core_getListIndex(&Parser_p->OpenElements, FormattingElement_p) + 1; i < Parser_p->OpenElements.size; ++i) {
            nh_ecmascript_Object *Tmp_p = Parser_p->OpenElements.pp[i];       
            if (nh_html_inSpecialCategory(Tmp_p)) {
                FurthestBlock_p = Tmp_p;
            }
        }
    
        if (!FurthestBlock_p) {
            for (unsigned long i = nh_core_getListIndex(&Parser_p->OpenElements, FormattingElement_p) - 1; i >= 0; ++i) {
                nh_ecmascript_Object *Pop_p = nh_html_popCurrentNode(Parser_p);
                if (Pop_p == FormattingElement_p) {
                    break;
                }
            }
            nh_core_removeFromList2(&Parser_p->ActiveFormattingElements, false, FormattingElement_p);
            break;
        }
    
        nh_ecmascript_Object *CommonAncestor_p = Parser_p->OpenElements.pp[nh_core_getListIndex(&Parser_p->OpenElements, FurthestBlock_p) - 1]; 
    
        nh_html_AdoptionAgencyBookmark Bookmark;
        Bookmark.left = 0;
        Bookmark.right = 0;
        bool countRight = true;
        for (int i = Parser_p->ActiveFormattingElements.size - 1; i >= 0; --i) 
        {
            nh_ecmascript_Object *Tmp_p = Parser_p->ActiveFormattingElements.pp[i];
            if (Tmp_p == FormattingElement_p) {countRight = false;}
            else if (countRight) {Bookmark.right++;}
            else {Bookmark.left++;}
        }
    
        nh_html_runAdoptionAgencyAlgorithmInnerLoop(Parser_p, FormattingElement_p, FurthestBlock_p, &Bookmark);

        // TODO

        exit(0);
    }

    return;
}

void nh_html_runAdoptionAgencyAlgorithm(
    nh_html_Parser *Parser_p, nh_html_Token *Token_p)
{
    nh_encoding_UTF8String *Subject_p = &Token_p->StartOrEndTag.TagName;
    nh_ecmascript_Object *CurrentNode_p = nh_html_getCurrentNode(Parser_p);
    nh_ecmascript_Object *Element_p = NH_WEBIDL_GET_DOM_ELEMENT(CurrentNode_p);


    if (!strcmp(Subject_p->p, "b")) {inB = true;}


    if (Element_p && !strcmp(nh_dom_getLocalName(Element_p)->p, Subject_p->p)) {
        if (!nh_core_inList(&Parser_p->ActiveFormattingElements, CurrentNode_p)) {
            nh_html_popCurrentNode(Parser_p);
            return;
        }
    }

    nh_html_runAdoptionAgencyAlgorithmOuterLoop(Parser_p, Subject_p);

    return;
}

