// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Node.h"
#include "NodeList.h"

#include "../Common/Macros.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Util/Array.h"

#include "../../nh-webidl/Runtime/Object.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// DECLARE =========================================================================================

#define NODE_TYPE Node_p->Attributes.pp[0]
#define NODE_NAME Node_p->Attributes.pp[1] 

typedef struct nh_dom_NodeInternal {
    nh_webidl_Object *Document_p;
    nh_List RegisteredObservers;
    nh_Array SpecifiedPropertyValues;
    nh_List ComputedPropertyValues;
    bool updateAnnotations;
} nh_dom_NodeInternal;

// INITIALIZE ======================================================================================

static NH_API_RESULT nh_dom_setNodeType(
    nh_webidl_Object *Node_p)
{
NH_DOM_BEGIN()

    NODE_TYPE = (void*)0;

    nh_webidl_Object *Parent_p = Node_p->Parent_p;
    while (Parent_p != NULL)
    { 
        if (strcmp(Parent_p->Interface_p->Specification_p->name_p, "DOM")) {
            Parent_p = Parent_p->Parent_p;
            continue;
        }
    
        if (!strcmp(Parent_p->Interface_p->name_p, "Element")) {
            NODE_TYPE = (void*)1;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "Attr")) {
            NODE_TYPE = (void*)2;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "Text")) {
            NODE_TYPE = (void*)3;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "CDATASection")) {
            NODE_TYPE = (void*)4;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "ProcessingInstruction")) {
            NODE_TYPE = (void*)7;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "Comment")) {
            NODE_TYPE = (void*)8;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "Document")) {
            NODE_TYPE = (void*)9;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "DocumentType")) {
            NODE_TYPE = (void*)10;
        }
        else if (!strcmp(Parent_p->Interface_p->name_p, "DocumentFragment")) {
            NODE_TYPE = (void*)11;
        }

        if (NODE_TYPE) {NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)}

        Parent_p = Parent_p->Parent_p;
    }

NH_DOM_DIAGNOSTIC_END(NH_API_ERROR_BAD_STATE)
}

// NOTE Keep public otherwise can't be loaded.
NH_API_RESULT nh_dom_initializeNode(
    nh_webidl_Object *Node_p)
{
NH_DOM_BEGIN()

    Node_p->internal_p = nh_core_allocate(sizeof(nh_dom_NodeInternal));
    NH_DOM_CHECK_MEM(Node_p->internal_p)

    ((nh_dom_NodeInternal*)Node_p->internal_p)->Document_p = NULL;
    ((nh_dom_NodeInternal*)Node_p->internal_p)->RegisteredObservers = nh_core_initList(8);

    NH_DOM_CHECK(nh_dom_setNodeType(Node_p))

    Node_p->Attributes.pp[7] = nh_dom_createNodeList();
    NH_DOM_CHECK_MEM(Node_p->Attributes.pp[7])

    ((nh_dom_NodeInternal*)Node_p->internal_p)->SpecifiedPropertyValues = nh_core_initArray(0, 0);
    ((nh_dom_NodeInternal*)Node_p->internal_p)->ComputedPropertyValues  = nh_core_initList(0);
    ((nh_dom_NodeInternal*)Node_p->internal_p)->updateAnnotations       = true;

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

// INTERNAL ========================================================================================

nh_dom_Node *nh_dom_getNode(
    nh_webidl_Object *Object_p)
{
NH_DOM_BEGIN()
NH_DOM_END((nh_dom_Node*)nh_webidl_getObject(Object_p, "DOM", "Node"))
}

NH_API_RESULT nh_dom_appendToNode(
    nh_dom_Node *Node_p, nh_webidl_Object *Child_p)
{
NH_DOM_BEGIN()

    NH_DOM_CHECK(nh_dom_appendToNodeList(((nh_webidl_Object*)Node_p)->Attributes.pp[7], Child_p))

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_dom_insertIntoNode(
    nh_dom_Node *Node_p, nh_webidl_Object *Child_p, NH_WEBIDL_UNSIGNED_LONG index)
{
NH_DOM_BEGIN()

    NH_DOM_CHECK(nh_dom_insertIntoNodeList(((nh_webidl_Object*)Node_p)->Attributes.pp[7], Child_p, index))

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_List *nh_dom_getNodeChildren(
    nh_dom_Node *Node_p)
{
NH_DOM_BEGIN()
NH_DOM_END(nh_dom_getNodeList(((nh_webidl_Object*)Node_p)->Attributes.pp[7]))
}

NH_API_RESULT nh_dom_setNodeDocument(
    nh_dom_Node *Node_p, nh_webidl_Object *NodeDocument_p)
{
NH_DOM_BEGIN()

    ((nh_dom_NodeInternal*)((nh_webidl_Object*)Node_p)->internal_p)->Document_p = NodeDocument_p;

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_webidl_Object *nh_dom_getNodeDocument(
    nh_dom_Node *Node_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_dom_NodeInternal*)((nh_webidl_Object*)Node_p)->internal_p)->Document_p)
}

NH_API_RESULT nh_dom_setSpecifiedPropertyValues(
    nh_dom_Node *Node_p, nh_Array SpecifiedPropertyValues)
{
NH_DOM_BEGIN()

    ((nh_dom_NodeInternal*)((nh_webidl_Object*)Node_p)->internal_p)->SpecifiedPropertyValues
        = SpecifiedPropertyValues;

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

NH_API_RESULT nh_dom_setComputedPropertyValues(
    nh_dom_Node *Node_p, nh_List ComputedPropertyValues)
{
NH_DOM_BEGIN()

    ((nh_dom_NodeInternal*)((nh_webidl_Object*)Node_p)->internal_p)->ComputedPropertyValues
        = ComputedPropertyValues;

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

nh_Array *nh_dom_getSpecifiedPropertyValues(
    nh_dom_Node *Node_p)
{
NH_DOM_BEGIN()
NH_DOM_END(&((nh_dom_NodeInternal*)((nh_webidl_Object*)Node_p)->internal_p)->SpecifiedPropertyValues)
}

nh_List *nh_dom_getComputedPropertyValues(
    nh_dom_Node *Node_p)
{
NH_DOM_BEGIN()
NH_DOM_END(&((nh_dom_NodeInternal*)((nh_webidl_Object*)Node_p)->internal_p)->ComputedPropertyValues)
}

bool nh_dom_getUpdateAnnotationsFlag(
    nh_dom_Node *Node_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_dom_NodeInternal*)((nh_webidl_Object*)Node_p)->internal_p)->updateAnnotations)
}

NH_API_RESULT nh_dom_setUpdateAnnotationsFlag(
    nh_dom_Node *Node_p, bool update)
{
NH_DOM_BEGIN()

    ((nh_dom_NodeInternal*)((nh_webidl_Object*)Node_p)->internal_p)->updateAnnotations = update;

NH_DOM_DIAGNOSTIC_END(NH_API_SUCCESS)
}

void nh_dom_setParentNode(
    nh_dom_Node *Node_p, nh_webidl_Object *Parent_p)
{
NH_DOM_BEGIN()

    if (nh_webidl_getObject(Parent_p, "DOM", "Element")) {
       ((nh_webidl_Object*)Node_p)->Attributes.pp[6] = Parent_p;
    }
    ((nh_webidl_Object*)Node_p)->Attributes.pp[5] = Parent_p;

NH_DOM_SILENT_END()
}

nh_webidl_Object *nh_dom_getParentNode(
    nh_dom_Node *Node_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Node_p)->Attributes.pp[5])
}

nh_webidl_Object *nh_dom_getParentElement(
    nh_dom_Node *Node_p)
{
NH_DOM_BEGIN()
NH_DOM_END(((nh_webidl_Object*)Node_p)->Attributes.pp[6])
}

