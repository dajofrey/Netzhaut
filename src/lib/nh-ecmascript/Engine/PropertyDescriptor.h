#ifndef NH_ECMASCRIPT_PROPERTY_DESCRIPTOR_H
#define NH_ECMASCRIPT_PROPERTY_DESCRIPTOR_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Value.h"
#include "Object.h"

// STRUCTS =========================================================================================================================

typedef struct nh_ecmascript_Object nh_ecmascript_Object;

typedef struct nh_ecmascript_PropertyDescriptor {
    struct {
        // Presence flags
        unsigned int hasValue : 1;
        unsigned int hasWritable : 1;
        unsigned int hasGet : 1;
        unsigned int hasSet : 1;
        unsigned int hasEnumerable : 1;
        unsigned int hasConfigurable : 1;
        
        // The actual values for boolean attributes
        unsigned int writable : 1;
        unsigned int enumerable : 1;
        unsigned int configurable : 1;
    } flags;

    nh_ecmascript_Value Value;
    nh_ecmascript_Object *Get;
    nh_ecmascript_Object *Set;
} nh_ecmascript_PropertyDescriptor;

// FUNCTIONS =======================================================================================================================

nh_ecmascript_PropertyDescriptor nh_ecmascript_undefinedPropertyDescriptor(
);

//    nh_ecmascript_PropertyDescriptorWrapper nh_ecmascript_wrapPropertyDescriptor(
//        nh_ecmascript_PropertyDescriptor Descriptor
//    );
//    
//    nh_ecmascript_PropertyDescriptorWrapper nh_ecmascript_undefinedPropertyDescriptorWrapper(
//    );

//void nh_ecmascript_applyDescriptorToProperty(
//    nh_ecmascript_PropertyDescriptor *Desc_p,
//    nh_ecmascript_Property *Prop_p
//);

bool nh_ecmascript_isAccessorDescriptor(
    nh_ecmascript_PropertyDescriptor Descriptor 
);

bool nh_ecmascript_isDataDescriptor (
    nh_ecmascript_PropertyDescriptor Descriptor
);

bool nh_ecmascript_isGenericDescriptor (
    nh_ecmascript_PropertyDescriptor Descriptor
);

//    nh_ecmascript_fromPropertyDescriptor(
//        Desc
//    );
//

nh_ecmascript_Completion nh_ecmascript_toPropertyDescriptor(
    nh_ecmascript_Value ObjVal, 
    nh_ecmascript_PropertyDescriptor *Desc_out,
    nh_ecmascript_Realm *Realm_p
); 

nh_ecmascript_PropertyDescriptor nh_ecmascript_getPropertyDescriptor(
    nh_ecmascript_Value value,
    uint8_t attrs
);

//    nh_ecmascript_completePropertyDescriptor(
//        Desc
//    );

uint8_t nh_ecmascript_getAttributeFlags(
    nh_ecmascript_PropertyDescriptor *desc
);

/** @} */

#endif
