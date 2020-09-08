// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Document.h"
#include "../Header/Parser.h"
#include "../Header/Text.h"
#include "../Header/Macros.h"
#include "../Header/Log.h"

#include "../../Core/Header/Memory.h"
#include "../../Core/Header/Tab.h"
#include "../../Core/Header/List.h"
#include "../../Core/Header/HashMap.h"

#include "../../CSS/Header/Input.h"
#include "../../CSS/Header/Associate.h"
#include "../../CSS/Header/Log.h"
#include "../../CSS/Header/Properties.h"
#include "../../CSS/Header/Arrange.h"

#include "../../JavaScript/Header/Input.h"
#include "../../JavaScript/Header/Debug.h"
#include "../../JavaScript/Header/Loader.h"
#include "../../JavaScript/DOM/Header/Document.h"

#include "../../Graphics/Header/Text.h"
#include "../../Graphics/Header/Node.h"

#include NH_DEBUG
#include NH_UTILS
#include NH_DEFAULT_CHECK
#include NH_HTML_UTILS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// DATA ============================================================================================

const char *NH_HTML_TAGS_PP[] = 
{
    "a",           
    "abbr",       
    "acronym",    
    "address",    
    "applet",     
    "area",       
    "article",    
    "aside",      
    "audio",      
    "b",          
    "base",       
    "basefont",   
    "bdi",        
    "bdo",        
    "big",        
    "blockquote", 
    "body",       
    "br",         
    "button",     
    "canvas",     
    "caption",    
    "center",     
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
    "dir",        
    "div",        
    "dl",         
    "dt",         
    "em",         
    "embed",      
    "fieldset",   
    "figcaption", 
    "figure",     
    "font",       
    "footer",     
    "form",       
    "frame",      
    "frameset",   
    "h1",         
    "h2",         
    "h3",         
    "h4",         
    "h5",         
    "h6",         
    "head",       
    "header",      
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
    "meta",       
    "meter",      
    "nav",        
    "noframes",   
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
    "small",      
    "source",     
    "span",       
    "strike",     
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
    "tt",         
    "u",          
    "ul",    
    "var",         
    "video",      
    "wbr",        
    "menu",
    "hgroup",
    "math",
    "text",
    "undefined",
};

size_t NH_HTML_TAGS_PP_COUNT = sizeof(NH_HTML_TAGS_PP) / sizeof(NH_HTML_TAGS_PP[0]);

// INIT ============================================================================================

NH_RESULT Nh_HTML_initNode(
    Nh_HTML_Node *Node_p, Nh_HTML_Node *Parent_p, NH_HTML_TAG tag)
{
NH_BEGIN()

    if (tag == -1) {NH_END(NH_ERROR_BAD_STATE)}

    Node_p->tag = tag;

    NH_INIT_LIST(Node_p->Properties)
    NH_INIT_LIST(Node_p->Attributes)
    NH_INIT_LIST(Node_p->Children.Unformatted)
    NH_INIT_LIST(Node_p->Children.Formatted)

    NH_INIT_LIST(Node_p->Vulkan.Buffers)
    NH_INIT_LIST(Node_p->Vulkan.Uniforms)
    NH_INIT_LIST(Node_p->Vulkan.Descriptors)

    Node_p->Parent_p = Parent_p;
    Node_p->text_p   = NULL;

    Node_p->Computed.Margin = Nh_CSS_initBox();
    Nh_HTML_initText(&Node_p->Computed.Text);

NH_END(NH_SUCCESS)
}

// COMPUTE =========================================================================================

NH_RESULT Nh_HTML_computeNode(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL text) 
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_computeAttributes(Tab_p, Node_p))
    NH_CHECK(Nh_CSS_computeProperties(Tab_p, Node_p, NH_TRUE))

    if (text && Node_p->tag == NH_HTML_TAG_TEXT) {NH_CHECK(Nh_HTML_createNormalizedText(Tab_p, Node_p))}

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_recomputeNode(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, NH_BOOL text) 
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_computeAttributes(Tab_p, Node_p))
    NH_CHECK(Nh_CSS_computeProperties(Tab_p, Node_p, NH_FALSE))

    if (text && Node_p->tag == NH_HTML_TAG_TEXT) {NH_CHECK(Nh_HTML_recreateNormalizedText(Tab_p, Node_p))}

    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_TRUE))
    NH_CHECK(Nh_HTML_recomputeFormattedTree(Tab_p))

NH_END(NH_SUCCESS)
}

// MANIPULATE ======================================================================================

NH_RESULT Nh_HTML_replaceChild(
    Nh_Tab *Tab_p, Nh_HTML_Node *Replace_p, Nh_HTML_Node *Replacement_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_destroyFormattedTree(&Tab_p->Document.Tree, &Tab_p->Window_p->GPU))

    NH_CHECK(Nh_replaceListItem(&Replace_p->Parent_p->Children.Unformatted, Replace_p, Replacement_p))
    Nh_HTML_destroyNode(Replace_p, NH_TRUE);

    NH_CHECK(Nh_CSS_associateSheets(Tab_p, Replacement_p))
    NH_CHECK(Nh_HTML_computeNode(Tab_p, Replacement_p, NH_TRUE))

    Nh_HTML_recreateFlatTree(&Tab_p->Document.Tree, Tab_p->Document.Tree.Root_p, NH_TRUE);

    if (Tab_p->Document.Scripts.count > 0) {
        NH_CHECK(Nh_JS_updateDocumentObject(Nh_getListItem(&Tab_p->Document.Scripts, 0)))
    }

    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_TRUE))
    NH_CHECK(Nh_HTML_computeFormattedTree(Tab_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_replaceChildren(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Node *Replace_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_destroyFormattedTree(&Tab_p->Document.Tree, &Tab_p->Window_p->GPU))

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
        Nh_HTML_destroyNode(Nh_getListItem(&Node_p->Children.Unformatted, i), NH_TRUE);
    }

    Nh_destroyList(&Node_p->Children.Unformatted, true);
    NH_CHECK(Nh_addListItem(&Node_p->Children.Unformatted, Replace_p))

    NH_CHECK(Nh_CSS_associateSheets(Tab_p, Replace_p))
    NH_CHECK(Nh_HTML_computeNode(Tab_p, Replace_p, NH_TRUE))

    Nh_HTML_recreateFlatTree(&Tab_p->Document.Tree, Tab_p->Document.Tree.Root_p, NH_TRUE);

    if (Tab_p->Document.Scripts.count > 0) {
        NH_CHECK(Nh_JS_updateDocumentObject(Nh_getListItem(&Tab_p->Document.Scripts, 0)))
    }

    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_TRUE))
    NH_CHECK(Nh_HTML_computeFormattedTree(Tab_p))

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_HTML_prependChild(
    Nh_Tab *Tab_p, Nh_HTML_Node *Node_p, Nh_HTML_Node *Prepend_p)
{
NH_BEGIN()

    NH_CHECK(Nh_HTML_destroyFormattedTree(&Tab_p->Document.Tree, &Tab_p->Window_p->GPU))

    NH_CHECK(Nh_prependListItem(&Node_p->Children.Unformatted, Prepend_p))

    NH_CHECK(Nh_CSS_associateSheets(Tab_p, Prepend_p))
    NH_CHECK(Nh_HTML_computeNode(Tab_p, Prepend_p, NH_TRUE))

    Nh_HTML_recreateFlatTree(&Tab_p->Document.Tree, Tab_p->Document.Tree.Root_p, NH_TRUE);

    if (Tab_p->Document.Scripts.count > 0) {
        NH_CHECK(Nh_JS_updateDocumentObject(Nh_getListItem(&Tab_p->Document.Scripts, 0)))
    }

    NH_CHECK(Nh_CSS_arrange(Tab_p, NH_TRUE))
    NH_CHECK(Nh_HTML_computeFormattedTree(Tab_p))

NH_END(NH_SUCCESS)
}

// DESTROY =========================================================================================

void Nh_HTML_destroyNode(
    Nh_HTML_Node *Node_p, NH_BOOL destroyTextData)
{
NH_BEGIN()

    Nh_HTML_destroyAttributes(&Node_p->Attributes);
    Nh_CSS_destroyProperties(&Node_p->Computed.Properties);
    Nh_CSS_destroyGenericProperties(&Node_p->Properties);

    Nh_free(Node_p->text_p);
    if (destroyTextData) {Nh_HTML_destroyText(Node_p);}

    for (int i = 0; i < Node_p->Children.Unformatted.count; ++i) {
        Nh_HTML_destroyNode(Nh_getListItem(&Node_p->Children.Unformatted, i), destroyTextData);
    }

    Nh_destroyList(&Node_p->Children.Unformatted, true);

NH_SILENT_END()
}

// GET =============================================================================================

Nh_HTML_Node *Nh_HTML_getNode(
    Nh_Tab *Tab_p, NH_HTML_TAG tag, NH_BOOL unformatted)
{
NH_BEGIN()

    Nh_List *List_p = unformatted ? &Tab_p->Document.Tree.Flat.Unformatted : &Tab_p->Document.Tree.Flat.Formatted;

    for (int i = 0; i < List_p->count; ++i) {
        Nh_HTML_Node *Node_p = Nh_getListItem(List_p, i);
        if (Node_p->tag == tag) {NH_END(Node_p)}
    }
 
NH_END(NULL)
}

NH_BOOL Nh_HTML_isMetaNode(
    Nh_HTML_Node *Node_p)
{
NH_BEGIN()

    while (Node_p != NULL) {
        if (NH_HTML_METADATA(Node_p->tag)) {NH_END(NH_TRUE)}
        Node_p = Node_p->Parent_p;
    }

NH_END(NH_FALSE)
}

