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

#include "../../nh-core/System/Memory.h"
#include "../../nh-core/Loader/Loader.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// DECLARE =========================================================================================

static nh_webidl_Object *nh_webidl_createObjectFromInterfaceWithChild(
    nh_webidl_Interface *Interface_p, nh_webidl_Object *Child_p
);

typedef int (*init_f)(nh_webidl_Object *Object_p);

// CREATE OBJECT ===================================================================================

static bool nh_webidl_isAttribute(
    nh_webidl_ParseNode *Node_p)
{
    switch (Node_p->type)
    {
        case NH_WEBIDL_PARSE_NODE_READ_ONLY_MEMBER     :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_ATTRIBUTE :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_MAPLIKE   :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_SETLIKE   :
            return true;
    }

    return false;
}

static nh_webidl_Object *nh_webidl_createObjectWithChild(
    char *specification_p, char *interface_p, nh_webidl_Object *Child_p)
{
    NH_CORE_CHECK_NULL_2(NULL, specification_p)
    NH_CORE_CHECK_NULL_2(NULL, interface_p)

    nh_webidl_Interface *Interface_p = nh_webidl_getInterface(specification_p, interface_p);
    NH_CORE_CHECK_NULL_2(NULL, Interface_p)

    if (Interface_p->partial) {return NULL;}

    return nh_webidl_createObjectFromInterfaceWithChild(Interface_p, Child_p);
}

static nh_webidl_Object *nh_webidl_createObjectFromInterfaceWithChild(
    nh_webidl_Interface *Interface_p, nh_webidl_Object *Child_p)
{
    nh_webidl_Object *Object_p = (nh_webidl_Object*)nh_core_allocate(sizeof(nh_webidl_Object));
    NH_CORE_CHECK_MEM_2(NULL, Object_p)
 
    Object_p->Interface_p = Interface_p;
    Object_p->Child_p     = Child_p;
    Object_p->Parent_p    = NULL;
    Object_p->internal_p  = NULL;
    Object_p->Parts       = nh_core_initList(1);

    int attributes = 0;

    for (int i = 0; i < Interface_p->Members.length; ++i) {
        nh_webidl_InterfaceMember *InterfaceMember_p = &((nh_webidl_InterfaceMember*)Interface_p->Members.p)[i];
        if (nh_webidl_isAttribute(InterfaceMember_p->Node_p)) {attributes++;}
    }

    Object_p->Attributes = nh_core_initList(attributes);

    for (int i = 0; i < attributes; ++i) {nh_core_appendToList(&Object_p->Attributes, NULL);}

    char lowerName_p[255] = {'\0'};
    sprintf(lowerName_p, "%s", Object_p->Interface_p->Specification_p->name_p);
    for (int i = 0; i < strlen(lowerName_p); ++i) {lowerName_p[i] = tolower(lowerName_p[i]);}

    char funcName_p[255] = {'\0'};
    sprintf(funcName_p, "nh_%s_initialize%s", lowerName_p, Object_p->Interface_p->name_p);

    char libName_p[255] = {'\0'};
    sprintf(libName_p, "nh-%s", lowerName_p);

    int _module = nh_core_getModuleIndex(libName_p);
    if (_module < 0) {return NULL;}

    // CRITICAL DESIGN DECISION: In the next line nh_core_loadExisitingSymbol() is used, which means that 
    // modules are expected to be loaded manually. This avoids unnecessary dependencies.
    init_f initNewObject_f = nh_core_loadExistingSymbol(_module, 0, funcName_p);
    if (initNewObject_f) {initNewObject_f(Object_p);} 
    else {
        char message_p[255];
        sprintf(message_p, "Object initializer %s couldn't be found.", funcName_p);
        nh_core_debug(message_p);
    }

    if (Interface_p->Inheritance_p != NULL) 
    {
        Object_p->Parent_p = nh_webidl_createObjectWithChild(
            Interface_p->Inheritance_p->specification_p ? (char*)Interface_p->Inheritance_p->specification_p : (char*)Interface_p->Specification_p->name_p, 
            (char*)Interface_p->Inheritance_p->interface_p, 
            Object_p
        );
        NH_CORE_CHECK_MEM_2(NULL, Object_p->Parent_p)
    }

    return Object_p;
}

nh_webidl_Object *nh_webidl_createObjectFromInterface(
    nh_webidl_Interface *Interface_p)
{
    return nh_webidl_createObjectFromInterfaceWithChild(Interface_p, NULL);
}

nh_webidl_Object *nh_webidl_createObject(
    char *specification_p, char *interface_p)
{
    return nh_webidl_createObjectWithChild(specification_p, interface_p, NULL);
}

// CREATE COMPOSITE OBJECT =========================================================================

nh_webidl_Object *nh_webidl_createCompositeObject(
    char *specification_p, char *interface_p, ...)
{
    NH_CORE_CHECK_NULL_2(NULL, specification_p)
    NH_CORE_CHECK_NULL_2(NULL, interface_p)

    nh_webidl_Interface *Interface_p = nh_webidl_getInterface(specification_p, interface_p);
    NH_CORE_CHECK_NULL_2(NULL, Interface_p)

    if (!Interface_p->partial) {return NULL;}

    nh_core_List Interfaces = nh_webidl_getCompositeInterfaces(Interface_p);
    if (Interfaces.size <= 1) {return NULL;}

    Interface_p = NULL;
    for (int i = 0; i < Interfaces.size; ++i) {
        Interface_p = Interfaces.pp[i];
        if (!Interface_p->partial) {break;}
        Interface_p = NULL;
    }
    NH_CORE_CHECK_NULL_2(NULL, Interface_p)

    nh_webidl_Object *Object_p = nh_webidl_createObjectFromInterface(Interface_p);
    NH_CORE_CHECK_NULL_2(NULL, Object_p)

    for (int i = 0; i < Interfaces.size; ++i) 
    {
        Interface_p = Interfaces.pp[i];
        if (!Interface_p->partial) {continue;}

        nh_webidl_Object *Part_p = nh_webidl_createObjectFromInterface(Interface_p);
        NH_CORE_CHECK_NULL_2(NULL, Part_p)
    
        nh_core_appendToList(&Object_p->Parts, Part_p);
    }

    nh_core_freeList(&Interfaces, false);

    return Object_p;
}

// GET =============================================================================================

static nh_webidl_Object *nh_webidl_getPart(
    nh_webidl_Object *Object_p, char *specification_p, char *interface_p) 
{
    NH_CORE_CHECK_NULL_2(NULL, Object_p)
    NH_CORE_CHECK_NULL_2(NULL, specification_p)

    nh_webidl_Object *Part_p = NULL;

    for (int i = 0; i < Object_p->Parts.size; ++i) 
    {
        Part_p = Object_p->Parts.pp[i];

        if (!strcmp(Part_p->Interface_p->Specification_p->name_p, specification_p)
        &&  !strcmp(Part_p->Interface_p->name_p, interface_p)) {
            return Part_p;
        }
    }

    return NULL;
}

nh_webidl_Object *nh_webidl_getObject(
    nh_webidl_Object *Object_p, char *specification_p, char *interface_p) 
{
    nh_webidl_Object *Tmp_p = Object_p;

    while (Tmp_p != NULL) 
    {
        if (!strcmp(Tmp_p->Interface_p->Specification_p->name_p, specification_p)
        &&  !strcmp(Tmp_p->Interface_p->name_p, interface_p)) {
            return Tmp_p;
        }

        if (Tmp_p->Parts.size > 0) {
            nh_webidl_Object *Part_p = nh_webidl_getPart(Tmp_p, specification_p, interface_p);
            if (Part_p) {return Part_p;}
        }

        Tmp_p = Tmp_p->Parent_p;
    }

    Tmp_p = Object_p;

    while (Tmp_p != NULL) 
    {
        if (!strcmp(Tmp_p->Interface_p->Specification_p->name_p, specification_p)
        &&  !strcmp(Tmp_p->Interface_p->name_p, interface_p)) {
            return Tmp_p;
        }

        if (Tmp_p->Parts.size > 0) {
            nh_webidl_Object *Part_p = nh_webidl_getPart(Tmp_p, specification_p, interface_p);
            if (Part_p) {return Part_p;}
        }

        Tmp_p = Tmp_p->Child_p;
    }

    return NULL;
}

void *nh_webidl_getAttribute(
    nh_webidl_Object *Object_p, char *attribute_p)
{
    while (Object_p != NULL) 
    {
        unsigned int attributeIndex = 0;

        for (int i = 0; i < Object_p->Interface_p->Members.length; ++i) 
        {
            nh_webidl_InterfaceMember *Member_p = &((nh_webidl_InterfaceMember*)Object_p->Interface_p->Members.p)[i];
            if (Member_p->name_p != NULL && !strcmp(Member_p->name_p, attribute_p)) {
                return Object_p->Attributes.pp[attributeIndex];
            }
            if (nh_webidl_isAttribute(Member_p->Node_p)) {attributeIndex++;}
        }

        Object_p = Object_p->Parent_p;
    }

    return NULL;
}

// DEBUG ===========================================================================================

static void nh_webidl_stringifyObjectForDebuggingRecursively(
    nh_webidl_Object *Object_p, nh_core_String *String_p) 
{
    nh_webidl_Object *LastChild_p = NULL;
    for (nh_webidl_Object *Child_p = Object_p->Child_p; Child_p; Child_p = Child_p->Child_p) {
        LastChild_p = Child_p;
    }
    for (; LastChild_p != NULL && LastChild_p != Object_p; LastChild_p = LastChild_p->Parent_p) {
        nh_core_appendToString(String_p, LastChild_p->Interface_p->name_p, strlen(LastChild_p->Interface_p->name_p));
        nh_core_appendToString(String_p, "->", 2);
    }
 
    nh_core_appendToString(String_p, "[(", 2);
    nh_core_appendToString(String_p, Object_p->Interface_p->Specification_p->name_p, strlen(Object_p->Interface_p->Specification_p->name_p));
    nh_core_appendToString(String_p, ")", 1);
    nh_core_appendToString(String_p, Object_p->Interface_p->name_p, strlen(Object_p->Interface_p->name_p));
    nh_core_appendToString(String_p, "]", 1);
    for (nh_webidl_Object *Parent_p = Object_p->Parent_p; Parent_p; Parent_p = Parent_p->Parent_p) {
        nh_core_appendToString(String_p, "->", 2);
        nh_core_appendToString(String_p, Parent_p->Interface_p->name_p, strlen(Parent_p->Interface_p->name_p));
    }
    for (int i = 0; i < Object_p->Parts.size; ++i) {
        nh_core_appendToString(String_p, ",", 1);
        nh_webidl_stringifyObjectForDebuggingRecursively(Object_p->Parts.pp[i], String_p);
    }
    return; 
}

nh_core_String nh_webidl_stringifyObjectForDebugging(
    nh_webidl_Object *Object_p) 
{
    nh_core_String String = nh_core_initString(64); 
    nh_webidl_stringifyObjectForDebuggingRecursively(Object_p, &String);
    return String; 
}

