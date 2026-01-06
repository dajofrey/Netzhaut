#ifndef NH_ECMASCRIPT_INTRINSICS_PARSER_H
#define NH_ECMASCRIPT_INTRINSICS_PARSER_H

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "Tokenizer.h"

#include "../Common/Includes.h"
#include "../Engine/ObjectType.h"

#include "../../nh-core/Util/List.h"
#include "../../nh-core/Util/String.h"

// ENUMS =================================================

typedef enum NH_ECMASCRIPT_INTRINSIC_VALUE_E {
    NH_ECMASCRIPT_INTRINSIC_VALUE_STRING,
    NH_ECMASCRIPT_INTRINSIC_VALUE_NUMBER,
    NH_ECMASCRIPT_INTRINSIC_VALUE_IDENTIFIER,
    NH_ECMASCRIPT_INTRINSIC_VALUE_INTRINSIC_REF, // %Name%
    NH_ECMASCRIPT_INTRINSIC_VALUE_NULL,
    NH_ECMASCRIPT_INTRINSIC_VALUE_OBJECT,        // nested object literal
    NH_ECMASCRIPT_INTRINSIC_VALUE_ATTRIBUTES_SET // set of identifiers: { writable, configurable }
} NH_ECMASCRIPT_INTRINSIC_VALUE_E;

typedef enum NH_ECMASCRIPT_INTERNAL_SLOT_E {
    NH_ECMASCRIPT_INTERNAL_SLOT_NULL,
    NH_ECMASCRIPT_INTERNAL_SLOT_CURRENT_REALM,
    NH_ECMASCRIPT_INTERNAL_SLOT_INTRINSIC_REF,
    NH_ECMASCRIPT_INTERNAL_SLOT_BOOLEAN,
    NH_ECMASCRIPT_INTERNAL_SLOT_NUMBER,
    NH_ECMASCRIPT_INTERNAL_SLOT_UNDEFINED,
    NH_ECMASCRIPT_INTERNAL_SLOT_PRIVATE_ELEMENTS,
} NH_ECMASCRIPT_INTERNAL_SLOT_E;

// STRUCTS ====================================================

typedef struct nh_ecmascript_IntrinsicTemplateValue {
    NH_ECMASCRIPT_INTRINSIC_VALUE_E kind;
    char *raw; // original lexeme (for simple kinds)
    // for objects / attributes:
    struct nh_ecmascript_IntrinsicTemplateProperty **props;
    int props_count;
    char **attr_names;
    int attr_count;
} nh_ecmascript_IntrinsicTemplateValue;

typedef struct nh_ecmascript_IntrinsicTemplateProperty {
    char *name;
    nh_ecmascript_IntrinsicTemplateValue *value;
} nh_ecmascript_IntrinsicTemplateProperty;

typedef struct nh_ecmascript_InternalSlotTemplate {
    NH_ECMASCRIPT_INTERNAL_SLOT_E type;
    union { 
        bool boolean;
        double number;
    };
} nh_ecmascript_InternalSlotTemplate;

typedef struct nh_ecmascript_IntrinsicTemplate {
    char *name; // intrinsic identifier (without %)
    NH_ECMASCRIPT_OBJECT_E kind; // value of type: constructor / ordinaryObject / ...
    char *prototype_p; // percent-wrapped target name (without %)
    char *instancePrototype;
    bool isCallable;
    bool isConstructor;
    bool isExotic;
    nh_core_List InternalSlotTemplates;
    nh_ecmascript_IntrinsicTemplateProperty **properties; // named properties under properties { ... }
    int propertiesCount;
} nh_ecmascript_IntrinsicTemplate;

// FUNCTIONS ===========================================================

void nh_ecmascript_freeIntrinsicTemplate(
    nh_ecmascript_IntrinsicTemplate *IntrinsicTemplate_p
);

nh_ecmascript_IntrinsicTemplate *nh_ecmascript_parseIntrinsicTemplate(
    nh_core_Array tokens
);

nh_core_List nh_ecmascript_parseIntrinsicTemplates(
);

#endif
