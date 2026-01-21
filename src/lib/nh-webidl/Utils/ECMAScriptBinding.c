// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "ECMAScriptBinding.h"
#include "Global.h"

#include "../../nh-core/System/Memory.h"
#include "../../nh-ecmascript/Intrinsics/Intrinsics.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// FUNCTIONS ========================================================================================

static nh_ecmascript_Object *nh_webidl_createAttributeGetter(
    char *interfaceName, 
    char *attrName, 
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. Create a brand new Function Object
    // It inherits from %Function.prototype%
    void *nativeSteps_p = nh_ecmascript_getNativeSteps(interfaceName, attrName);
    nh_ecmascript_Object *F = nh_ecmascript_createBuiltinFunction(nativeSteps_p, attrName, Realm_p);

    // 3. Set Spec-Mandated Properties on the function itself
    // Spec: Getters have a .length of 0
    nh_ecmascript_defineDataProperty(F, "length", nh_ecmascript_makeNumber(0), 0x04);
    
    // Spec: The .name is "get " + the attribute name
    char nameBuf[256];
    snprintf(nameBuf, sizeof(nameBuf), "get %s", attrName);
    nh_ecmascript_defineDataProperty(F, "name", nh_ecmascript_makeString(nameBuf), 0x04);

    return F;
}

static void nh_webidl_bindMember(
    nh_webidl_Interface *I,
    nh_webidl_InterfaceMember *M, 
    nh_ecmascript_Object *Obj,
    nh_ecmascript_Realm *R) 
{
    nh_webidl_ParseNode *node = M->Node_p;

    if (node->type == NH_WEBIDL_PARSE_NODE_OPERATION) {
        // Create a Built-in Function for the operation
        void *f = nh_ecmascript_getNativeSteps(I->name_p, M->name_p);
        nh_ecmascript_Object *F = nh_ecmascript_createBuiltinFunction(f, M->name_p, R);

        // Define on Prototype: { [[Writable]]: true, [[Enumerable]]: true, [[Configurable]]: true }
        nh_ecmascript_defineDataProperty(Obj, M->name_p, nh_ecmascript_makeObject(F), 0x07);
    } 
    else if (node->type == NH_WEBIDL_PARSE_NODE_READ_WRITE_ATTRIBUTE || 
             node->type == NH_WEBIDL_PARSE_NODE_READ_ONLY_MEMBER) {

        nh_ecmascript_Object *Get_p = nh_webidl_createAttributeGetter(I->name_p, M->name_p, R);
        nh_ecmascript_Object *Set_p = node->type == NH_WEBIDL_PARSE_NODE_READ_WRITE_ATTRIBUTE ?
            nh_webidl_createAttributeGetter(I->name_p, M->name_p, R) : NULL;

        nh_ecmascript_defineAccessorProperty(Obj, M->name_p, Get_p, Set_p, 0x07);
    }
}

/**
 * @brief Resolves a Constructor object by name from the WebIDL registry.
 */
static nh_ecmascript_Object *nh_webidl_lookupInterfaceConstructor(
    const char *name_p, 
    nh_ecmascript_Realm *Realm_p) 
{
    if (name_p == NULL) return NULL;

    char key[256];
    snprintf(key, sizeof(key), "%sConstructor", name_p);
    nh_ecmascript_Object *Constructor_p = nh_core_getFromHashMap(
        &Realm_p->WebIDL, 
        key
    );

    // 3. Return the found constructor
    // If not found, it returns NULL (which is correct for error handling 
    // in Pass 2 if an IDL references a non-existent interface).
    return Constructor_p;
}

/**
 * @brief Resolves a prototype object by name for WebIDL inheritance.
 */
static nh_ecmascript_Object *nh_webidl_lookupInterfacePrototype(
    char *name_p, 
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. Safety check: If no name is provided, or name is "Object", 
    // the spec implies inheritance from the base Object prototype.
    if (name_p == NULL || strcmp(name_p, "Object") == 0) {
        return nh_core_getFromHashMap(&Realm_p->Intrinsics, "ObjectPrototype");
    }

    char key[256];
    snprintf(key, sizeof(key), "%sPrototype", name_p);
    nh_ecmascript_Object *Proto_p = nh_core_getFromHashMap(&Realm_p->WebIDL, key);

    // 3. Return the found prototype
    if (Proto_p != NULL) {
        return Proto_p;
    }

    // 4. Fallback/Error: 
    // If the interface is unknown, spec-compliance usually suggests
    // falling back to ObjectPrototype to keep the chain valid, 
    // though in a debug build you'd want a warning here.
    return nh_core_getFromHashMap(&Realm_p->Intrinsics, "ObjectPrototype");
}

/**
 * @brief Hydrates the pre-allocated Interface Object and Prototype Object.
 * This should be called in "Pass 2" of your initialization.
 */
static void nh_webidl_defineInterface(
    nh_webidl_Interface *IDLInterface_p,
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. Resolve Parent Prototype
    // Use the lookup helper to find the parent (e.g., "EventTargetPrototype")
    nh_ecmascript_Object *ParentProto = nh_core_getFromHashMap(&Realm_p->Intrinsics, "ObjectPrototype");
    if (IDLInterface_p->Inheritance_p) {
        ParentProto = nh_webidl_lookupInterfacePrototype(
            IDLInterface_p->Inheritance_p->interface_p, 
            Realm_p
        );
    }

    // 2. Retrieve the pre-allocated Shells from Pass 1
    char protoKey[256], constrKey[256];
    snprintf(protoKey, sizeof(protoKey), "%sPrototype", IDLInterface_p->name_p);
    snprintf(constrKey, sizeof(constrKey), "%sConstructor", IDLInterface_p->name_p);

    nh_ecmascript_Object *Proto_p = nh_core_getFromHashMap(&Realm_p->WebIDL, protoKey);
    nh_ecmascript_Object *Constructor_p = nh_core_getFromHashMap(&Realm_p->WebIDL, constrKey);

    // 3. Link the Prototype Chain
    // Prototype inherits from ParentProto, Constructor inherits from %Function.prototype%
//    Proto_p->Slots[NH_ECMASCRIPT_SLOT_PROTOTYPE] = (Realm_p->FunctionPrototype)
//        ? nh_ecmascript_makeObject(ParentProto)
//        : nh_ecmascript_makeNull();


    // Constructor.prototype = Proto
    // Attributes: { [[Writable]]: false, [[Enumerable]]: false, [[Configurable]]: false }
    nh_ecmascript_defineDataProperty(
        Constructor_p, 
        "prototype", 
        nh_ecmascript_makeObject(Proto_p), 
        0 // No attribute bits set
    );
    
    // Proto.constructor = Constructor
    // Attributes: { [[Writable]]: false, [[Enumerable]]: false, [[Configurable]]: true }
    // Note: As you noted, 'constructor' is usually configurable to allow user re-definition.
    nh_ecmascript_defineDataProperty(
        Proto_p,
        "constructor", 
        nh_ecmascript_makeObject(Constructor_p),
        NH_ECMASCRIPT_ATTR_CONFIGURABLE
    );

//    // 5. Set [Symbol.toStringTag] on the Prototype
//    nh_ecmascript_PropertyDescriptor tagDesc = {
//        .Value = nh_ecmascript_makeString(IDLInterface_p->name_p),
//        .flags = { .hasValue = 1, .hasConfigurable = 1, .configurable = 1 }
//    };
//    nh_ecmascript_Value toStringTag = nh_ecmascript_getWellKnownSymbol(Realm_p, "toStringTag");
//    Proto->Methods_p->defineOwnProperty(Proto, toStringTag.data.symbol_p->key, &tagDesc);

    // 6. Hydrate Members (Operations, Attributes, Constants)
    for (int i = 0; i < IDLInterface_p->Members.length; i++) {
        nh_webidl_InterfaceMember *member = &((nh_webidl_InterfaceMember*)IDLInterface_p->Members.p)[i];
        
//        // This function will decide if the member goes on Proto or Constructor
//        if (member->is_static) {
//            // Static members go on the Constructor (e.g., Notification.permission)
//            nh_webidl_bindMember(IDLInterface_p, member, Constructor, Realm_p);
//        } 
//        else if (member->type == NH_WEBIDL_MEMBER_CONSTANT) {
//            // Constants (like Node.ELEMENT_NODE) must be on BOTH per spec
//            nh_webidl_bindMember(IDLInterface_p, member, Proto, Realm_p);
//            nh_webidl_bindMember(IDLInterface_p, member, Constructor, Realm_p);
//        } 
//        else {
//            // Everything else (standard methods/attributes) goes on the Prototype
//            nh_webidl_bindMember(IDLInterface_p, member, Proto, Realm_p);
//        }
    }
}

/**
 * @brief Creates a Prototype Object for ANY WebIDL interface.
 */
static nh_ecmascript_Object *nh_webidl_configureInterfacePrototype(
    nh_webidl_Interface *IDLInterface_p, 
    nh_ecmascript_Realm *Realm_p) 
{
    // 1. Resolve inheritance dynamically
    // If IDL says "interface A : B", we find B's prototype. Defaults to %Object.prototype%
    nh_ecmascript_Object *ParentProto = nh_core_getFromHashMap(&Realm_p->Intrinsics, "ObjectPrototype");
    if (IDLInterface_p->Inheritance_p) {
        ParentProto = nh_webidl_lookupInterfacePrototype(
            IDLInterface_p->Inheritance_p->interface_p, 
            Realm_p
        );
    }

    // 2. Get pre-allocated Prototype Object
    nh_ecmascript_Object *Proto_p = nh_webidl_lookupInterfacePrototype(IDLInterface_p->name_p, Realm_p);

    // FIX: Physically link the prototype chain!
    // This allows window.addEventListener to work by delegation.
    Proto_p->Slots[NH_ECMASCRIPT_SLOT_PROTOTYPE] = (ParentProto)
        ? nh_ecmascript_makeObject(ParentProto)
        : nh_ecmascript_makeNull();

    // 3. Hydrate with Members
    // This loop handles Operations and Attributes for any interface
    for (int i = 0; i < IDLInterface_p->Members.length; i++) {
        nh_webidl_InterfaceMember *member = 
            &((nh_webidl_InterfaceMember*)IDLInterface_p->Members.p)[i];
        
        // Pass the specific Interface name (e.g., "Document") so the 
        // native resolver find the correct C functions (nh_ecmascript_document_...)
        nh_webidl_bindMember(
            IDLInterface_p,
            member, 
            Proto_p, 
            Realm_p
        );
    }

//    // 4. Set [Symbol.toStringTag] (Spec requirement)
//    // This makes Object.prototype.toString.call(obj) return "[object InterfaceName]"
//    nh_ecmascript_PropertyDescriptor tagDesc = {
//        .Value = nh_ecmascript_makeString(IDLInterface_p->name_p),
//        .flags = { .hasValue = 1, .hasConfigurable = 1, .configurable = 1 }
//    };
//    
//    nh_ecmascript_Value toStringTag = nh_ecmascript_getWellKnownSymbol(Realm_p, "toStringTag");
//    Proto_p->Methods_p->defineOwnProperty(Proto_p, toStringTag.data.symbol_p->key, &tagDesc);

    return Proto_p;
}

static NH_API_RESULT nh_webidl_installInterfacesFromList(
    nh_core_List *IDLInterfaces_p,
    nh_ecmascript_Realm *Realm_p)
{
    // Pass 1: Allocate all objects
    for (int i = 0; i < IDLInterfaces_p->size; i++) {
        nh_webidl_Interface *idl = IDLInterfaces_p->pp[i];
        nh_ecmascript_Object *Prototype_p = nh_ecmascript_ordinaryObjectCreate(NULL, Realm_p); // No parent yet
        nh_ecmascript_Object *Constructor_p = nh_ecmascript_ordinaryFunctionCreate(NULL, NULL, NULL, Realm_p); // No parent yet
 
        char key[255] = {0};
        snprintf(key, sizeof(key), "%sPrototype", idl->name_p);
        nh_core_addToHashMap(&Realm_p->WebIDL, key, Prototype_p);

        snprintf(key, sizeof(key), "%sConstructor", idl->name_p);
        nh_core_addToHashMap(&Realm_p->WebIDL, key, Constructor_p);
    }

    // 1. First pass: Configure all Prototype objects
    for (int i = 0; i < IDLInterfaces_p->size; i++) {
        nh_webidl_configureInterfacePrototype(IDLInterfaces_p->pp[i], Realm_p);
    }

    // 2. Second pass: Create Constructors (Interface Objects) and link them
    // This must be a second pass because Prototype.constructor needs 
    // to point to a valid Constructor object.
    for (int i = 0; i < IDLInterfaces_p->size; i++) {
        nh_webidl_defineInterface(IDLInterfaces_p->pp[i], Realm_p);
        
        // If it's the Global interface (Window), fix the Global Object's prototype
//        if (nh_webidl_hasExtendedAttribute(idl, "Global")) {
//            Realm_p->GlobalObject->Prototype_p = Proto;
//            // Re-sync the shape if necessary
//        }
    }

    return NH_API_SUCCESS;
}

NH_API_RESULT nh_webidl_installInterfaces(
    nh_ecmascript_Realm *Realm_p)
{
    return nh_webidl_installInterfacesFromList(&NH_WEBIDL_GLOBAL.Specifications, Realm_p);
}
