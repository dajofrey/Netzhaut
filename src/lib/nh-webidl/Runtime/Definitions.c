// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Definitions.h"
#include "Builtin.h"

#include "../Common/Macros.h"

#include "../../nh-core/System/Memory.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// CLASSES =========================================================================================

static const NH_BYTE *nh_webidl_getInterfaceMemberName(
    nh_webidl_ParseNode *Node_p)
{
NH_WEBIDL_BEGIN()

    NH_BYTE *name_p = NULL;

    switch (Node_p->type)
    {
        case NH_WEBIDL_PARSE_NODE_CONST :
            name_p = ((nh_webidl_ParseNode*)Node_p->Children.pp[2])->Token_p->String.p;
            break;

        case NH_WEBIDL_PARSE_NODE_READ_ONLY_MEMBER :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_ATTRIBUTE :
        {
            nh_List AttributeNames = nh_core_initList(1);
            nh_webidl_getParseNodes(Node_p, NH_WEBIDL_PARSE_NODE_ATTRIBUTE_NAME, &AttributeNames);
            if (AttributeNames.size == 1) {
                if (((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)AttributeNames.pp[0])->Children.pp[0])->Children.size == 0) {
                    name_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)AttributeNames.pp[0])->Children.pp[0])->Token_p->String.p;
                } 
            }
            nh_core_freeList(&AttributeNames, NH_FALSE);
            break;
        }
        case NH_WEBIDL_PARSE_NODE_OPERATION :
        {
            nh_List OperationNames = nh_core_initList(1);
            nh_webidl_getParseNodes(Node_p, NH_WEBIDL_PARSE_NODE_OPERATION_NAME, &OperationNames);
            if (OperationNames.size == 1) {
                if (((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)OperationNames.pp[0])->Children.pp[0])->Children.size == 0) {
                    name_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)OperationNames.pp[0])->Children.pp[0])->Token_p->String.p;
                } 
            }
            nh_core_freeList(&OperationNames, NH_FALSE);
            break;
        }
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_MAPLIKE :
        case NH_WEBIDL_PARSE_NODE_READ_WRITE_SETLIKE :
            break;
    }

NH_WEBIDL_END(name_p)
}

static NH_WEBIDL_RESULT nh_webidl_getInterfaceMembers(
    nh_webidl_Interface *Interface_p, nh_webidl_ParseNode *InterfaceRest_p, NH_BOOL partial)
{
NH_WEBIDL_BEGIN()

    nh_List InterfaceMembers = nh_core_initList(8);

    if (!partial) {
        nh_webidl_getParseNodes(InterfaceRest_p, NH_WEBIDL_PARSE_NODE_CLASS_MEMBER, &InterfaceMembers);
        for (int i = 0; i < InterfaceMembers.size; ++i) {
            nh_webidl_InterfaceMember *InterfaceMember_p = nh_core_incrementArray(&Interface_p->Members);
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
            nh_webidl_InterfaceMember *InterfaceMember_p = nh_core_incrementArray(&Interface_p->Members);
            InterfaceMember_p->Node_p = ((nh_webidl_ParseNode*)InterfaceMembers.pp[i])->Children.pp[0];
            InterfaceMember_p->name_p = nh_webidl_getInterfaceMemberName(InterfaceMember_p->Node_p);
        }
    }
  
    nh_core_freeList(&InterfaceMembers, NH_FALSE);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

static NH_WEBIDL_RESULT nh_webidl_getInterfaceInheritance(
    nh_webidl_Interface *Interface_p, nh_webidl_ParseNode *InterfaceRest_p)
{
NH_WEBIDL_BEGIN()

    nh_List Inheritance = nh_core_initList(1);
    nh_webidl_getParseNodes(InterfaceRest_p, NH_WEBIDL_PARSE_NODE_INHERITANCE, &Inheritance);

    if (Inheritance.size == 1 && ((nh_webidl_ParseNode*)Inheritance.pp[0])->Children.size > 0) 
    {
        Interface_p->Inheritance_p = nh_core_allocate(sizeof(nh_webidl_InterfaceInheritance));
        NH_WEBIDL_CHECK_MEM(Interface_p->Inheritance_p)
        Interface_p->Inheritance_p->interface_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)Inheritance.pp[0])->Children.pp[1])->Token_p->String.p;
        Interface_p->Inheritance_p->specification_p = NULL;
        if (((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)Inheritance.pp[0])->Children.pp[2])->Children.size == 2) {
            Interface_p->Inheritance_p->specification_p = ((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)((nh_webidl_ParseNode*)Inheritance.pp[0])->Children.pp[2])->Children.pp[1])->Token_p->String.p;
        }
    }

    nh_core_freeList(&Inheritance, NH_FALSE);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

static nh_webidl_Interface nh_webidl_createInterface(
    nh_webidl_Specification *Specification_p, nh_webidl_ParseNode *InterfaceRest_p, NH_BOOL partial)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Interface Interface;
    Interface.partial = partial;
    Interface.Members = nh_core_initArray(sizeof(nh_webidl_InterfaceMember), 8);
    Interface.name_p  = ((nh_webidl_ParseNode*)InterfaceRest_p->Children.pp[0])->Token_p->String.p;
    Interface.Specification_p = Specification_p;
    Interface.Inheritance_p   = NULL;

    nh_webidl_getInterfaceMembers(&Interface, InterfaceRest_p, partial);
    nh_webidl_getInterfaceInheritance(&Interface, InterfaceRest_p);

NH_WEBIDL_END(Interface)
}

NH_WEBIDL_RESULT nh_webidl_createInterfaces(
    nh_webidl_Fragment *Fragment_p)
{
NH_WEBIDL_BEGIN()

    Fragment_p->Interfaces = nh_core_initArray(sizeof(nh_webidl_Interface), 8);

    nh_List InterfaceRests = nh_core_initList(8);
    nh_List PartialInterfaceRests = nh_core_initList(8);

    nh_webidl_getParseNodes(Fragment_p->ParseResult.Root_p, NH_WEBIDL_PARSE_NODE_CLASS_REST, &InterfaceRests);
    nh_webidl_getParseNodes(Fragment_p->ParseResult.Root_p, NH_WEBIDL_PARSE_NODE_PARTIAL_CLASS_REST, &PartialInterfaceRests);

    for (int i = 0; i < InterfaceRests.size; ++i) {
        nh_webidl_Interface *Interface_p = nh_core_incrementArray(&Fragment_p->Interfaces);
        NH_WEBIDL_CHECK_MEM(Interface_p)
        *Interface_p = nh_webidl_createInterface(Fragment_p->Specification_p, InterfaceRests.pp[i], NH_FALSE);
    }

    for (int i = 0; i < PartialInterfaceRests.size; ++i) {
        nh_webidl_Interface *Interface_p = nh_core_incrementArray(&Fragment_p->Interfaces);
        NH_WEBIDL_CHECK_MEM(Interface_p)
        *Interface_p = nh_webidl_createInterface(Fragment_p->Specification_p, PartialInterfaceRests.pp[i], NH_TRUE);
    }

    nh_core_freeList(&InterfaceRests, NH_FALSE);
    nh_core_freeList(&PartialInterfaceRests, NH_FALSE);

NH_WEBIDL_DIAGNOSTIC_END(NH_WEBIDL_SUCCESS)
}

nh_webidl_Interface *nh_webidl_getInterfaceFromFragment(
    nh_webidl_Fragment *Fragment_p, NH_BYTE *className_p)
{
NH_WEBIDL_BEGIN()

    nh_webidl_Interface *Interface_p = NULL;
    for (int i = 0; i < Fragment_p->Interfaces.length; ++i) {
        Interface_p = &((nh_webidl_Interface*)Fragment_p->Interfaces.p)[i];
        if (!strcmp(Interface_p->name_p, className_p)) {break;}
        Interface_p = NULL;
    }

NH_WEBIDL_END(Interface_p)
}

