#ifndef NH_ECMASCRIPT_PROPERTY_DESCRIPTOR_H
#define NH_ECMASCRIPT_PROPERTY_DESCRIPTOR_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Type.h"

#endif

/** @addtogroup lib_nhecmascript_structs
 *  @{
 */

    typedef struct nh_ecmascript_DataPropertyDescriptor {
        nh_ecmascript_Any Value;
        NH_ECMASCRIPT_BOOLEAN writable;
    } nh_ecmascript_DataPropertyDescriptor;

    typedef struct nh_ecmascript_AccessorPropertyDescriptor {
        nh_ecmascript_Any Get;
        nh_ecmascript_Any Set;
    } nh_ecmascript_AccessorPropertyDescriptor;

    typedef union nh_ecmascript_PropertyDescriptorFields {
        nh_ecmascript_DataPropertyDescriptor Data;
        nh_ecmascript_AccessorPropertyDescriptor Accessor;
    } nh_ecmascript_PropertyDescriptorFields;

    typedef struct nh_ecmascript_PropertyDescriptor {
        int type;
        nh_ecmascript_PropertyDescriptorFields Fields;
        NH_ECMASCRIPT_BOOLEAN enumerable;
        NH_ECMASCRIPT_BOOLEAN configurable;
    } nh_ecmascript_PropertyDescriptor;

/** @} */

/** @addtogroup lib_nhecmascript_functions
 *  @{
 */

    nh_ecmascript_PropertyDescriptor nh_ecmascript_undefinedPropertyDescriptor(
    );

//    nh_ecmascript_PropertyDescriptorWrapper nh_ecmascript_wrapPropertyDescriptor(
//        nh_ecmascript_PropertyDescriptor Descriptor
//    );
//    
//    nh_ecmascript_PropertyDescriptorWrapper nh_ecmascript_undefinedPropertyDescriptorWrapper(
//    );

    // https://tc39.es/ecma262/#sec-isaccessordescriptor
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isAccessorDescriptor(
        nh_ecmascript_PropertyDescriptor Descriptor 
    );

    // https://tc39.es/ecma262/#sec-isdatadescriptor
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isDataDescriptor (
        nh_ecmascript_PropertyDescriptor Descriptor
    );

    // https://tc39.es/ecma262/#sec-isgenericdescriptor
    NH_ECMASCRIPT_BOOLEAN nh_ecmascript_isGenericDescriptor (
        nh_ecmascript_PropertyDescriptor Descriptor
    );

//    nh_ecmascript_fromPropertyDescriptor(
//        Desc
//    );
//
//    nh_ecmascript_toPropertyDescriptor(
//        Obj
//    );
//
//    nh_ecmascript_completePropertyDescriptor(
//        Desc
//    );

/** @} */

#endif
