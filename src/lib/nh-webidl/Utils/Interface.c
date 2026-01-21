// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Interface.h"
#include "Builtin.h"
#include "Global.h"

#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CLASSES =========================================================================================

static const char *nh_webidl_getInterfaceMemberName(
    nh_webidl_ParseNode *Node_p)
{
    char *name_p = NULL;

    switch (Node_p->type)
    {
        case NH_WEBIDL_PARSE_NODE_CONST :
            name_p = ((nh_webidl_ParseNode*)Node_p->Children.pp[2])->Token_p->String.p;
            break;

        case NH_WEBIDL_PARSE_NODE_READ_ONLY_MEMBER :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_ATTRIBUTE :
        {
            nh_core_List AttributeNames = nh_core_initList(1);
            nh_webidl_getParseNodes(Node_p, NH_WEBIDL_PARSE_NODE_ATTRIBUTE_NAME, &AttributeNames);
            if (AttributeNames.size == 1) {
                if (((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)AttributeNames.pp[0])->Children.pp[0])->Children.size == 0) {
                    name_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)AttributeNames.pp[0])->Children.pp[0])->Token_p->String.p;
                } 
            }
            nh_core_freeList(&AttributeNames, false);
            break;
        }
        case NH_WEBIDL_PARSE_NODE_OPERATION :
        {
            nh_core_List OperationNames = nh_core_initList(1);
            nh_webidl_getParseNodes(Node_p, NH_WEBIDL_PARSE_NODE_OPERATION_NAME, &OperationNames);
            if (OperationNames.size == 1) {
                if (((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)OperationNames.pp[0])->Children.pp[0])->Children.size == 0) {
                    name_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)OperationNames.pp[0])->Children.pp[0])->Token_p->String.p;
                } 
            }
            nh_core_freeList(&OperationNames, false);
            break;
        }
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_MAPLIKE :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_SETLIKE :
            break;
    }

    return name_p;
}

static NH_API_RESULT nh_webidl_getInterfaceMembers(
    nh_webidl_Interface *Interface_p, nh_webidl_ParseNode *InterfaceRest_p, bool partial)
{
    nh_core_List InterfaceMembers = nh_core_initList(8);

    if (!partial) {
        nh_webidl_getParseNodes(InterfaceRest_p, NH_WEBIDL_PARSE_NODE_CLASS_MEMBER, &InterfaceMembers);
        for (int i = 0; i < InterfaceMembers.size; ++i) {
            nh_webidl_InterfaceMember *InterfaceMember_p = (nh_webidl_InterfaceMember*)nh_core_incrementArray(&Interface_p->Members);
            if (((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0])->type == NH_WEBIDL_PARSE_NODE_PARTIAL_CLASS_MEMBER) {
                InterfaceMember_p->Node_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0])->Children.pp[0];
            }
            else {InterfaceMember_p->Node_p = ((nh_webidl_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0];} // constructor
            InterfaceMember_p->name_p = nh_webidl_getInterfaceMemberName(InterfaceMember_p->Node_p);
        }
    }
    else {
        nh_webidl_getParseNodes(InterfaceRest_p, NH_WEBIDL_PARSE_NODE_PARTIAL_CLASS_MEMBER, &InterfaceMembers);
        for (int i = 0; i < InterfaceMembers.size; ++i) {
            nh_webidl_InterfaceMember *InterfaceMember_p = (nh_webidl_InterfaceMember*)nh_core_incrementArray(&Interface_p->Members);
            InterfaceMember_p->Node_p = ((nh_webidl_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0];
            InterfaceMember_p->name_p = nh_webidl_getInterfaceMemberName(InterfaceMember_p->Node_p);
        }
    }
  
    nh_core_freeList(&InterfaceMembers, false);

    return NH_API_SUCCESS;
}

static NH_API_RESULT nh_webidl_getInterfaceInheritance(
    nh_webidl_Interface *Interface_p, nh_webidl_ParseNode *InterfaceRest_p)
{
    nh_core_List Inheritance = nh_core_initList(1);
    nh_webidl_getParseNodes(InterfaceRest_p, NH_WEBIDL_PARSE_NODE_INHERITANCE, &Inheritance);

    if (Inheritance.size == 1 && ((nh_webidl_ParseNode*)Inheritance.pp[0])->Children.size > 0) 
    {
        Interface_p->Inheritance_p = (nh_webidl_InterfaceInheritance*)nh_core_allocate(sizeof(nh_webidl_InterfaceInheritance));
        NH_CORE_CHECK_MEM(Interface_p->Inheritance_p)
        Interface_p->Inheritance_p->interface_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)Inheritance.pp[0])->Children.pp[1])->Token_p->String.p;
        Interface_p->Inheritance_p->specification_p = NULL;
        if (((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)Inheritance.pp[0])->Children.pp[2])->Children.size == 2) {
            Interface_p->Inheritance_p->specification_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)Inheritance.pp[0])->Children.pp[2])->Children.pp[1])->Token_p->String.p;
        }
    } else {
        Interface_p->Inheritance_p = NULL;
    }

    nh_core_freeList(&Inheritance, false);

    return NH_API_SUCCESS;
}

static nh_webidl_Interface nh_webidl_createInterface(
    nh_webidl_Specification *Specification_p, nh_webidl_ParseNode *InterfaceRest_p, bool partial)
{
    nh_webidl_Interface Interface;
    Interface.partial = partial;
    Interface.Members = nh_core_initArray(sizeof(nh_webidl_InterfaceMember), 8);
    Interface.name_p  = ((nh_webidl_ParseNode*)InterfaceRest_p->Children.pp[0])->Token_p->String.p;
    Interface.Specification_p = Specification_p;

    nh_webidl_getInterfaceMembers(&Interface, InterfaceRest_p, partial);
    nh_webidl_getInterfaceInheritance(&Interface, InterfaceRest_p);

    return Interface;
}

NH_API_RESULT nh_webidl_createInterfaces(
    nh_webidl_Fragment *Fragment_p)
{
    Fragment_p->Interfaces = nh_core_initArray(sizeof(nh_webidl_Interface), 8);

    nh_core_List InterfaceRests = nh_core_initList(8);
    nh_core_List PartialInterfaceRests = nh_core_initList(8);

    nh_webidl_getParseNodes(Fragment_p->ParseResult.Root_p, NH_WEBIDL_PARSE_NODE_CLASS_REST, &InterfaceRests);
    nh_webidl_getParseNodes(Fragment_p->ParseResult.Root_p, NH_WEBIDL_PARSE_NODE_PARTIAL_CLASS_REST, &PartialInterfaceRests);

    for (int i = 0; i < InterfaceRests.size; ++i) {
        nh_webidl_Interface *Interface_p = (nh_webidl_Interface*)nh_core_incrementArray(&Fragment_p->Interfaces);
        NH_CORE_CHECK_MEM(Interface_p)
        *Interface_p = nh_webidl_createInterface(Fragment_p->Specification_p, InterfaceRests.pp[i], false);
    }

    for (int i = 0; i < PartialInterfaceRests.size; ++i) {
        nh_webidl_Interface *Interface_p = (nh_webidl_Interface*)nh_core_incrementArray(&Fragment_p->Interfaces);
        NH_CORE_CHECK_MEM(Interface_p)
        *Interface_p = nh_webidl_createInterface(Fragment_p->Specification_p, PartialInterfaceRests.pp[i], true);
    }

    nh_core_freeList(&InterfaceRests, false);
    nh_core_freeList(&PartialInterfaceRests, false);

    return NH_API_SUCCESS;
}

nh_webidl_Interface *nh_webidl_getInterfaceFromFragment(
    nh_webidl_Fragment *Fragment_p, char *className_p)
{
    nh_webidl_Interface *Interface_p = NULL;
    for (int i = 0; i < Fragment_p->Interfaces.length; ++i) {
        Interface_p = &((nh_webidl_Interface*)Fragment_p->Interfaces.p)[i];
        if (!strcmp(Interface_p->name_p, className_p)) {break;}
        Interface_p = NULL;
    }

    return Interface_p;
}
