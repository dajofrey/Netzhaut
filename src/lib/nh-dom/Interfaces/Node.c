// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Node.h"
#include "NodeList.h"

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

typedef struct nh_dom_Internal {
    nh_ecmascript_Object *Document_p;
    nh_core_List RegisteredObservers;
    nh_core_Array SpecifiedPropertyValues;
    nh_core_List ComputedPropertyValues;
    bool updateAnnotations;
} nh_dom_Internal;

// INITIALIZE ======================================================================================

static NH_API_RESULT nh_dom_setNodeType(
    nh_ecmascript_Object *Node_p)
{
    nh_ecmascript_Object *Child_p = Node_p->Child_p;

    while (Child_p != NULL)
    { 
        if (strcmp(Child_p->Interface_p->Specification_p->name_p, "DOM")) {
            Child_p = Child_p->Child_p;
            continue;
        }
    
        if (!strcmp(Child_p->Interface_p->name_p, "Element")) {
            NODE_TYPE = (void*)1;
        }
        else if (!strcmp(Child_p->Interface_p->name_p, "Attr")) {
            NODE_TYPE = (void*)2;
        }
        else if (!strcmp(Child_p->Interface_p->name_p, "Text")) {
            NODE_TYPE = (void*)3;
        }
        else if (!strcmp(Child_p->Interface_p->name_p, "CDATASection")) {
            NODE_TYPE = (void*)4;
        }
        else if (!strcmp(Child_p->Interface_p->name_p, "ProcessingInstruction")) {
            NODE_TYPE = (void*)7;
        }
        else if (!strcmp(Child_p->Interface_p->name_p, "Comment")) {
            NODE_TYPE = (void*)8;
        }
        else if (!strcmp(Child_p->Interface_p->name_p, "Document")) {
            NODE_TYPE = (void*)9;
        }
        else if (!strcmp(Child_p->Interface_p->name_p, "DocumentType")) {
            NODE_TYPE = (void*)10;
        }
        else if (!strcmp(Child_p->Interface_p->name_p, "DocumentFragment")) {
            NODE_TYPE = (void*)11;
        }

        if (NODE_TYPE) {return NH_API_SUCCESS;}

        Child_p = Child_p->Child_p;
    }

    return NH_API_ERROR_BAD_STATE;
}

// NOTE Keep public otherwise can't be loaded.
NH_API_RESULT nh_dom_initializeNode(
    nh_ecmascript_Object *Node_p)
{
    Node_p->internal_p = nh_core_allocate(sizeof(nh_dom_Internal));
    NH_CORE_CHECK_MEM(Node_p->internal_p)

    ((nh_dom_Internal*)Node_p->internal_p)->Document_p = NULL;
    ((nh_dom_Internal*)Node_p->internal_p)->RegisteredObservers = nh_core_initList(8);

    NH_CORE_CHECK(nh_dom_setNodeType(Node_p))

    Node_p->Attributes.pp[7] = nh_dom_createNodeList();
    NH_CORE_CHECK_MEM(Node_p->Attributes.pp[7])

    ((nh_dom_Internal*)Node_p->internal_p)->SpecifiedPropertyValues = nh_core_initArray(0, 0);
    ((nh_dom_Internal*)Node_p->internal_p)->ComputedPropertyValues  = nh_core_initList(0);
    ((nh_dom_Internal*)Node_p->internal_p)->updateAnnotations       = true;

    return NH_API_SUCCESS;
}

// INTERNAL ========================================================================================

NH_API_RESULT nh_dom_appendToNode(
    nh_ecmascript_Object *Node_p, nh_ecmascript_Object *Child_p)
{
    return nh_dom_appendToNodeList(Node_p->Attributes.pp[7], Child_p);
}

NH_API_RESULT nh_dom_insertIntoNode(
    nh_ecmascript_Object *Node_p, nh_ecmascript_Object *Child_p, NH_WEBIDL_UNSIGNED_LONG index)
{
    return nh_dom_insertIntoNodeList(Node_p->Attributes.pp[7], Child_p, index);
}

nh_core_List *nh_dom_getNodeChildren(
    nh_ecmascript_Object *Node_p)
{
    return NH_WEBIDL_GET_DOM_NODEList(Node_p->Attributes.pp[7]);
}

NH_API_RESULT nh_dom_setNodeDocument(
    nh_ecmascript_Object *Node_p, nh_ecmascript_Object *NodeDocument_p)
{
    ((nh_dom_Internal*)Node_p->internal_p)->Document_p = NodeDocument_p;
    return NH_API_SUCCESS;
}

nh_ecmascript_Object *nh_dom_getNodeDocument(
    nh_ecmascript_Object *Node_p)
{
    return ((nh_dom_Internal*)Node_p->internal_p)->Document_p;
}

NH_API_RESULT nh_dom_setSpecifiedPropertyValues(
    nh_ecmascript_Object *Node_p, nh_core_Array SpecifiedPropertyValues)
{
    ((nh_dom_Internal*)Node_p->internal_p)->SpecifiedPropertyValues = SpecifiedPropertyValues;
    return NH_API_SUCCESS;
}

NH_API_RESULT nh_dom_setComputedPropertyValues(
    nh_ecmascript_Object *Node_p, nh_core_List ComputedPropertyValues)
{
    ((nh_dom_Internal*)Node_p->internal_p)->ComputedPropertyValues = ComputedPropertyValues;
    return NH_API_SUCCESS;
}

nh_core_Array *nh_dom_getSpecifiedPropertyValues(
    nh_ecmascript_Object *Node_p)
{
    return &((nh_dom_Internal*)Node_p->internal_p)->SpecifiedPropertyValues;
}

nh_core_List *nh_dom_getComputedPropertyValues(
    nh_ecmascript_Object *Node_p)
{
    return &((nh_dom_Internal*)Node_p->internal_p)->ComputedPropertyValues;
}

bool nh_dom_getUpdateAnnotationsFlag(
    nh_ecmascript_Object *Node_p)
{
    return ((nh_dom_Internal*)Node_p->internal_p)->updateAnnotations;
}

NH_API_RESULT nh_dom_setUpdateAnnotationsFlag(
    nh_ecmascript_Object *Node_p, bool update)
{
    ((nh_dom_Internal*)Node_p->internal_p)->updateAnnotations = update;
    return NH_API_SUCCESS;
}

void nh_dom_setParentNode(
    nh_ecmascript_Object *Node_p, nh_ecmascript_Object *Parent_p)
{
    if (nh_webidl_getObject(Parent_p, "DOM", "Element")) {
       Node_p->Attributes.pp[6] = Parent_p;
    }
    Node_p->Attributes.pp[5] = Parent_p;
}

nh_ecmascript_Object *nh_dom_getParentNode(
    nh_ecmascript_Object *Node_p)
{
    return Node_p->Attributes.pp[5];
}

nh_ecmascript_Object *nh_dom_getParentElement(
    nh_ecmascript_Object *Node_p)
{
    return ((nh_ecmascript_Object*)Node_p)->Attributes.pp[6];
}
