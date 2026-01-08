// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES =======================================================================================

#include "Shape.h"
#include "Realm.h"
#include "../Intrinsics/Intrinsics.h"

#include "../../nh-core/System/Memory.h"

#include <string.h>
#include <stdlib.h>

// FUNCTIONS ======================================================================================

nh_ecmascript_Shape* nh_ecmascript_createRootShape() 
{
    nh_ecmascript_Shape *Root_p = (nh_ecmascript_Shape*)nh_core_allocate(sizeof(nh_ecmascript_Shape));
    Root_p->parent = NULL;
    Root_p->property_key = NULL;
    Root_p->property_index = -1;
    Root_p->Transitions = nh_core_createHashMap();
    return Root_p;
}

nh_ecmascript_Shape* nh_ecmascript_transitionShape(
    nh_ecmascript_Shape *parent, 
    char *key, 
    uint8_t attrs, 
    bool is_accessor) 
{
    // 1. Safety check for parent
    if (!parent) return NULL; 

    // 2. Lookup existing transition
    char lookup_key[256];
    snprintf(lookup_key, sizeof(lookup_key), "%s:%d:%d", key, attrs, is_accessor);

    nh_ecmascript_Shape *existing = nh_core_getFromHashMap(&parent->Transitions, lookup_key);
    if (existing) return existing;

    // 3. Create new Shape
    nh_ecmascript_Shape *new_shape = calloc(1, sizeof(nh_ecmascript_Shape));
    new_shape->parent = parent;
    new_shape->property_key = strdup(key);
    new_shape->attributes = attrs;
    new_shape->is_accessor = is_accessor;
    
    // FIX: Indexing should be the count of properties BEFORE this one was added
    new_shape->property_index = parent->property_count;

    // FIX: Total count is the parent's count + 1
    new_shape->property_count = parent->property_count + 1;

    // 4. Initialize child transitions table
    new_shape->Transitions = nh_core_createHashMap();

    // 5. Store in parent's transition table
    nh_core_addToHashMap(&parent->Transitions, strdup(lookup_key), new_shape);

    return new_shape;
}

/**
 * The Resolver's helper: 
 * Takes the list of Properties from your .is template and bakes a Shape.
 */
nh_ecmascript_Shape* nh_ecmascript_buildShapeFromTemplate(
    nh_ecmascript_Shape *root, 
    nh_core_List *property_templates) 
{
    nh_ecmascript_Shape *current = root;

//    for (int i = 0; i < property_templates->size; i++) {
//        nh_ecmascript_KeyValuePair *pair = nh_core_getFromList(property_templates, i);
//        
//        // Use our resolver helpers from earlier
//        uint8_t attrs = nh_ecmascript_resolveAttributes(pair->value);
//        bool is_accessor = (nh_ecmascript_computePropertyType(pair->value) == NH_PROP_TYPE_ACCESSOR);
//
//        current = nh_ecmascript_transitionShape(current, pair->key, attrs, is_accessor);
//    }

    return current;
}

/**
 * Searches the Shape lineage for a property name and returns its index.
 * @param Shape_p The current shape of the object.
 * @param P The property name string.
 * @return The index in the Properties_p array, or -1 if not found.
 */
int nh_ecmascript_getIndexFromShape(
    nh_ecmascript_Shape *Shape_p,
    const char *p)
{
    nh_ecmascript_Shape *current = Shape_p;

    // Walk up the tree until we hit the Root (where parent is NULL)
    while (current != NULL && current->property_key != NULL) {
        // Compare the property name of this specific "Station"
        if (strcmp(current->property_key, p) == 0) {
            return current->property_index;
        }
        
        // Move to the previous state
        current = current->parent;
    }

    return -1; // Property not found in this Shape's history
}
