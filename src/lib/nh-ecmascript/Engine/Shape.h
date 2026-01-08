#ifndef NH_ECMASCRIPT_ENGINE_SHAPE_H
#define NH_ECMASCRIPT_ENGINE_SHAPE_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/HashMap.h"

// TYPES ============================================================

typedef struct nh_ecmascript_Shape {
    struct nh_ecmascript_Shape *parent;    // The shape we came from
    char *property_key;                    // The name of the property added
    int32_t property_index;               // Where this property lives in the object's value array
    int property_count;
    int8_t attributes;                    // writable, enumerable, configurable
    bool is_accessor;                      // If true, the value at property_index is a getter/setter pair

    // Optional: Transition table (for when code adds a new property at runtime)
    // For intrinsics, we can pre-calculate these.
    nh_core_HashMap Transitions; 
} nh_ecmascript_Shape;

// FUNCTIONS =======================================================

nh_ecmascript_Shape* nh_ecmascript_createRootShape(
);

nh_ecmascript_Shape* nh_ecmascript_transitionShape(
    nh_ecmascript_Shape *parent, 
    char *key, 
    uint8_t attrs, 
    bool is_accessor
);

nh_ecmascript_Shape* nh_ecmascript_buildShapeFromTemplate(
    nh_ecmascript_Shape *root,
    nh_core_List *property_templates
);

int nh_ecmascript_getIndexFromShape(
    nh_ecmascript_Shape *Shape_p,
    const char *p
);

#endif
