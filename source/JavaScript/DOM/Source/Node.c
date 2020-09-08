// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Node.h"
#include "../Header/HTMLElement.h"
#include "../Header/Macros.h"

#include "../../../HTML/Header/Text.h"
#include "../../../Graphics/Header/Text.h"

#include "../../../Core/Header/Memory.h"
#include "../../../Core/Header/Config.h"
#include "../../../Core/Header/HashMap.h"
#include "../../../Core/Header/String.h"

#include <ctype.h>

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result Nh_JS_Node_Node(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_baseURI(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_baseURIObject(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_childNodes(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_firstChild(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 0) {
        Nh_JS_Object *Object_p = Nh_JS_getObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT);
        if (Object_p != NULL) {
            Nh_JS_HTMLElement *Element_p = Object_p->data_p;
            if (Element_p->Children.count <= 0) {NH_END(Nh_JS_getNULLResult())}
            else {NH_END(Nh_JS_getResult(NH_JS_TYPE_OBJECT, false, Nh_getListItem(&Element_p->Children, 0)))}
        }
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_isConnected(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_lastChild(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_nextSibling(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_nodeName(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_nodeType(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_nodeValue(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_ownerDocument(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_parentNode(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_parentElement(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount == 0) // get
    {
        if (Nh_JS_getInheritingObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT) != NULL) 
        {
            Nh_JS_HTMLElement *Element_p = Nh_JS_getInheritingObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT)->data_p;
            Nh_JS_Object *Parent_p = Nh_JS_getInheritedObject(Element_p->Parent_p, NH_JS_OBJECT_HTML_ELEMENT);
            if (Parent_p != NULL) {NH_END(Nh_JS_getResult(NH_JS_TYPE_OBJECT, false, Parent_p))}
        }
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_previousSibling(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_textContent(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    Nh_Tab *Tab_p = Script_p->Run.Tab_p;

    if (aCount == 0) // get
    {
        if (Nh_JS_getInheritingObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT) != NULL) 
        {
            Nh_JS_HTMLElement *Element_p = Nh_JS_getInheritingObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT)->data_p;
            NH_END(Nh_JS_getResult(NH_JS_TYPE_STRING, true, Nh_HTML_getText(Element_p->Node_p, NH_TRUE)))
        }
    } 
    else if (aCount == 1 && Arguments_p[0].type == NH_JS_TYPE_STRING) // set
    {
        if (Nh_JS_getInheritingObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT) != NULL) 
        {
            Nh_JS_HTMLElement *Element_p = Nh_JS_getInheritingObject(Function_p->Inherit_p, NH_JS_OBJECT_HTML_ELEMENT)->data_p;

            Nh_HTML_Node *Text_p = Nh_allocate(sizeof(Nh_HTML_Node));
            NH_CHECK_MEM(Nh_JS_getNULLResult(), Text_p)

            NH_CHECK(Nh_JS_getNULLResult(), Nh_HTML_initNode(Text_p, Element_p->Node_p, NH_HTML_TAG_TEXT))
            Text_p->text_p = Nh_allocateChars(Arguments_p[0].data_p);

            NH_CHECK(Nh_JS_getNULLResult(), Nh_HTML_replaceChildren(Tab_p, Element_p->Node_p, Text_p))
        }
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_appendChild(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_cloneNode(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_compareDocumentPosition(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_contains(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_getBoxQuads(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_getRootNode(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_hasChildNodes(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_insertBefore(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_isDefaultNamespace(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_isEqualNode(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_isSameNode(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_lookupPrefix(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_lookupNamespaceURI(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_normalize(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_removeChild(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result Nh_JS_Node_replaceChild(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

