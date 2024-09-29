#ifndef NH_ECMASCRIPT_PROPERTIES_H
#define NH_ECMASCRIPT_PROPERTIES_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "PropertyDescriptor.h"
#include "Type.h"

#include "../../nh-core/Util/List.h"

#endif

/** @addtogroup lib_nh-ecmascript_enums
 *  @{
 */

    typedef enum NH_ECMASCRIPT_PROPERTY {
        NH_ECMASCRIPT_PROPERTY_DATA,
        NH_ECMASCRIPT_PROPERTY_ACCESSOR,
    } NH_ECMASCRIPT_PROPERTY;

/** @} */

/** @addtogroup lib_nh-ecmascript_typedefs
 *  @{
 */

    #define nh_ecmascript_Properties nh_List

/** @} */

/** @addtogroup lib_nh-ecmascript_structs
 *  @{
 */

    // https://www.262.ecma-international.org/11.0/index.html#table-2
    typedef struct nh_ecmascript_DataProperty {
        nh_ecmascript_Any Value;
        NH_ECMASCRIPT_BOOLEAN writable;
    } nh_ecmascript_DataProperty;

    // https://www.262.ecma-international.org/11.0/index.html#table-3
    typedef struct nh_ecmascript_AccessorProperty {
        nh_ecmascript_Any Get;
        nh_ecmascript_Any Set;
    } nh_ecmascript_AccessorProperty;

    typedef union nh_ecmascript_PropertyFields {
        nh_ecmascript_DataProperty Data;
        nh_ecmascript_AccessorProperty Accessor;
    } nh_ecmascript_PropertyFields;

    typedef struct nh_ecmascript_Property {
        NH_ECMASCRIPT_PROPERTY type;
        nh_ecmascript_Any Key;
        nh_ecmascript_PropertyFields Fields;
        NH_ECMASCRIPT_BOOLEAN enumerable;
        NH_ECMASCRIPT_BOOLEAN configurable;
    } nh_ecmascript_Property;

/** @} */

/** @addtogroup lib_nh-ecmascript_functions
 *  @{
 */

    nh_ecmascript_Properties nh_ecmascript_initProperties(
        int chunkSize 
    );

    bool nh_ecmascript_equalPropertyKeys(
        nh_ecmascript_Any Key1, nh_ecmascript_Any Key2
    );

    nh_ecmascript_Property *nh_ecmascript_newProperty(
        nh_ecmascript_Properties *Properties_p, nh_ecmascript_Any Key, nh_ecmascript_PropertyDescriptor Descriptor 
    );

/** @} */

#endif
