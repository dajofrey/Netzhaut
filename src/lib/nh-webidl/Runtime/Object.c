// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Object.h"
#include "Runtime.h"
#include "Type.h"
#include "Definitions.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Loader/Loader.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

static nh_webidl_Object *nh_webidl_createObjectFromInterfaceWithParent(
    nh_webidl_Interface *Interface_p, nh_webidl_Object *Parent_p
);

typedef int (*init_f)(nh_webidl_Object *Object_p);

// CREATE OBJECT ===================================================================================

static NH_BOOL nh_webidl_isAttribute(
    nh_webidl_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    switch (Node_p->type)
    {
        case NH_WEBIDL_PARSE_NODE_READ_ONLY_MEMBER     :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_ATTRIBUTE :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_MAPLIKE   :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_SETLIKE   :
            NH_WEBIDL_END(NH_TRUE)
    }

NH_WEBIDL_END(NH_FALSE)
}

static nh_webidl_Object *nh_webidl_createObjectWithParent(
    NH_BYTE *specification_p, NH_BYTE *interface_p, nh_webidl_Object *Parent_p)
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK_NULL_2(NULL, specification_p)
    NH_WEBIDL_CHECK_NULL_2(NULL, interface_p)

    nh_webidl_Interface *Interface_p = nh_webidl_getInterface(specification_p, interface_p);
    NH_WEBIDL_CHECK_NULL_2(NULL, Interface_p)

    if (Interface_p->partial) {NH_WEBIDL_END(NULL)}

NH_WEBIDL_END(nh_webidl_createObjectFromInterfaceWithParent(Interface_p, Parent_p))
}

static nh_webidl_Object *nh_webidl_createObjectFromInterfaceWithParent(
    nh_webidl_Interface *Interface_p, nh_webidl_Object *Parent_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Object *Object_p = nh_core_allocate(sizeof(nh_webidl_Object));
    NH_WEBIDL_CHECK_MEM_2(NULL, Object_p)
 
    Object_p->Interface_p = Interface_p;
    Object_p->Child_p     = NULL;
    Object_p->Parent_p    = Parent_p;
    Object_p->internal_p  = NULL;
    Object_p->Parts       = nh_core_initList(1);

    int attributes = 0;

    for (int i = 0; i < Interface_p->Members.length; ++i) {
        nh_webidl_InterfaceMember *InterfaceMember_p = &((nh_webidl_InterfaceMember*)Interface_p->Members.p)[i];
        if (nh_webidl_isAttribute(InterfaceMember_p->Node_p)) {attributes++;}
    }

    Object_p->Attributes = nh_core_initList(attributes);

    for (int i = 0; i < attributes; ++i) {nh_core_appendToList(&Object_p->Attributes, NULL);}

    NH_BYTE lowerName_p[255] = {'\0'};
    sprintf(lowerName_p, "%s", Object_p->Interface_p->Specification_p->name_p);
    for (int i = 0; i < strlen(lowerName_p); ++i) {lowerName_p[i] = tolower(lowerName_p[i]);}

    NH_BYTE funcName_p[255] = {'\0'};
    sprintf(funcName_p, "nh_%s_initialize%s", lowerName_p, Object_p->Interface_p->name_p);

    NH_BYTE libName_p[255] = {'\0'};
    sprintf(libName_p, "nh-%s", lowerName_p);

    int _module = nh_core_getModuleIndex(libName_p);
    if (_module < 0) {NH_WEBIDL_END(NULL)}

    // CRITICAL DESIGN DECISION: In the next line nh_core_loadExisitingSymbol() is used, which means that 
    // modules are expected to be loaded manually. This avoids unnecessary dependencies.
    init_f initNewObject_f = nh_core_loadExistingSymbol(_module, 0, funcName_p);
    if (initNewObject_f) {NH_WEBIDL_CHECK_2(NULL, initNewObject_f(Object_p))} 
    else {printf("Object initializer %s couldn't be found.\n", funcName_p);}

    if (Interface_p->Inheritance_p != NULL) 
    {
        Object_p->Child_p = nh_webidl_createObjectWithParent(
            Interface_p->Inheritance_p->specification_p ? (NH_BYTE*)Interface_p->Inheritance_p->specification_p : (NH_BYTE*)Interface_p->Specification_p->name_p, 
            (NH_BYTE*)Interface_p->Inheritance_p->interface_p, 
            Object_p
        );
        NH_WEBIDL_CHECK_MEM_2(NULL, Object_p->Child_p)
    }

NH_WEBIDL_END(Object_p)
}

nh_webidl_Object *nh_webidl_createObjectFromInterface(
    nh_webidl_Interface *Interface_p)
{
NH_WEBIDL_BEGIN()
NH_WEBIDL_END(nh_webidl_createObjectFromInterfaceWithParent(Interface_p, NULL))
}

nh_webidl_Object *nh_webidl_createObject(
    NH_BYTE *specification_p, NH_BYTE *interface_p)
{
NH_WEBIDL_BEGIN()
NH_WEBIDL_END(nh_webidl_createObjectWithParent(specification_p, interface_p, NULL))
}

// CREATE COMPOSITE OBJECT =========================================================================

nh_webidl_Object *nh_webidl_createCompositeObject(
    NH_BYTE *specification_p, NH_BYTE *interface_p, ...)
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK_NULL_2(NULL, specification_p)
    NH_WEBIDL_CHECK_NULL_2(NULL, interface_p)

    nh_webidl_Interface *Interface_p = nh_webidl_getInterface(specification_p, interface_p);
    NH_WEBIDL_CHECK_NULL_2(NULL, Interface_p)

    if (!Interface_p->partial) {NH_WEBIDL_END(NULL)}

    nh_List Interfaces = nh_webidl_getCompositeInterfaces(Interface_p);
    if (Interfaces.size <= 1) {NH_WEBIDL_END(NULL)}

    Interface_p = NULL;
    for (int i = 0; i < Interfaces.size; ++i) {
        Interface_p = Interfaces.pp[i];
        if (!Interface_p->partial) {break;}
        Interface_p = NULL;
    }
    NH_WEBIDL_CHECK_NULL_2(NULL, Interface_p)

    nh_webidl_Object *Object_p = nh_webidl_createObjectFromInterface(Interface_p);
    NH_WEBIDL_CHECK_NULL_2(NULL, Object_p)

    for (int i = 0; i < Interfaces.size; ++i) 
    {
        Interface_p = Interfaces.pp[i];
        if (!Interface_p->partial) {continue;}

        nh_webidl_Object *Part_p = nh_webidl_createObjectFromInterface(Interface_p);
        NH_WEBIDL_CHECK_NULL_2(NULL, Part_p)
    
        nh_core_appendToList(&Object_p->Parts, Part_p);
    }

    nh_core_freeList(&Interfaces, NH_FALSE);

NH_WEBIDL_END(Object_p)
}

// GET =============================================================================================

static nh_webidl_Object *nh_webidl_getPart(
    nh_webidl_Object *Object_p, NH_BYTE *specification_p, NH_BYTE *interface_p) 
{
NH_WEBIDL_BEGIN()

    NH_WEBIDL_CHECK_NULL_2(NULL, Object_p)
    NH_WEBIDL_CHECK_NULL_2(NULL, specification_p)

    nh_webidl_Object *Part_p = NULL;

    for (int i = 0; i < Object_p->Parts.size; ++i) 
    {
        Part_p = Object_p->Parts.pp[i];

        if (!strcmp(Part_p->Interface_p->Specification_p->name_p, specification_p)
        &&  !strcmp(Part_p->Interface_p->name_p, interface_p)) {
            NH_WEBIDL_END(Part_p)
        }
    }

NH_WEBIDL_END(NULL)
}

nh_webidl_Object *nh_webidl_getObject(
    nh_webidl_Object *Object_p, NH_BYTE *specification_p, NH_BYTE *interface_p) 
{
NH_WEBIDL_BEGIN()

    while (Object_p != NULL) 
    {
        if (!strcmp(Object_p->Interface_p->Specification_p->name_p, specification_p)
        &&  !strcmp(Object_p->Interface_p->name_p, interface_p)) {
            NH_WEBIDL_END(Object_p) 
        }

        if (Object_p->Parts.size > 0) {
            nh_webidl_Object *Part_p = nh_webidl_getPart(Object_p, specification_p, interface_p);
            if (Part_p) {NH_WEBIDL_END(Part_p)}
        }

        Object_p = Object_p->Child_p;
    }

NH_WEBIDL_END(NULL)
}

void *nh_webidl_getAttribute(
    nh_webidl_Object *Object_p, NH_BYTE *attribute_p)
{
NH_WEBIDL_BEGIN()

    while (Object_p != NULL) 
    {
        unsigned int attributeIndex = 0;

        for (int i = 0; i < Object_p->Interface_p->Members.length; ++i) 
        {
            nh_webidl_InterfaceMember *Member_p = &((nh_webidl_InterfaceMember*)Object_p->Interface_p->Members.p)[i];
            if (Member_p->name_p != NULL && !strcmp(Member_p->name_p, attribute_p)) {
                NH_WEBIDL_END(Object_p->Attributes.pp[attributeIndex])
            }
            if (nh_webidl_isAttribute(Member_p->Node_p)) {attributeIndex++;}
        }

        Object_p = Object_p->Child_p;
    }

NH_WEBIDL_END(NULL)
}

