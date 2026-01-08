// Object() as a function
nh_ecmascript_Completion nh_ecmascript_object_objectConstructorCall(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    nh_ecmascript_Value Value = (argumentCount > 0) ? Arguments_p[0] : nh_ecmascript_makeUndefined();
    
    if (Value.tag == NH_ECMASCRIPT_VALUE_UNDEFINED || Value.tag == NH_ECMASCRIPT_VALUE_NULL) {
        return nh_ecmascript_normalCompletion(nh_ecmascript_makeObject(nh_ecmascript_ordinaryObjectCreate(
            nh_core_getFromHashMap(&Realm_p->Intrinsics, "ObjectPrototype"), NULL, -1)));
    }
    
    return nh_ecmascript_toObject(Value, Realm_p);
}

// new Object() as a constructor
nh_ecmascript_Completion nh_ecmascript_object_objectConstructorConstruct(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    // Essentially the same as Object(...) call but handles subclassing (NewTarget)
    // For a base engine, this just creates a new ordinary object.
    return nh_ecmascript_objectConstructorCall(Object_p, Arguments_p, argumentCount, Realm_p);
}

nh_ecmascript_Completion nh_ecmascript_object_assign(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    // 1. Let to be ? ToObject(target).
    if (argumentCount < 1) return nh_ecmascript_throwTypeError("Target required", Realm_p);
    nh_ecmascript_Completion toComp = nh_ecmascript_toObject(Arguments_p[0], Realm_p);
    if (toComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return toComp;
    nh_ecmascript_Object *to = toComp.value.p.object;

    // 2. If only one argument, return target
    if (argumentCount == 1) return toComp;

    // 3. For each source in Arguments_p[1...]
    for (size_t i = 1; i < argumentCount; i++) {
        nh_ecmascript_Value nextSource = Arguments_p[i];
        
        // 4. If source is null or undefined, skip it
        if (nextSource.tag == NH_ECMASCRIPT_VALUE_UNDEFINED || nextSource.tag == NH_ECMASCRIPT_VALUE_NULL) {
            continue;
        }

        // 5. Let from be ! ToObject(nextSource).
        nh_ecmascript_Object *from = nextSource.p.object; // Simplified: usually requires ToObject

        // 6. Get keys (Simplified: iterating your HashMap)
        nh_core_HashMapIterator it = nh_core_getHashMapIterator(&from->Properties);
        while (nh_core_hashMapNext(&it)) {
            const char *key = it.key;
            nh_ecmascript_Property *prop = it.value;

            // 7. Only copy enumerable properties
            if (prop->enumerable) {
                // In a real engine, we call [[Get]] then [[Set]]
                nh_ecmascript_createDataProperty(to, key, prop->value, true, true, true);
            }
        }
    }

    return toComp;
}

nh_ecmascript_Completion nh_ecmascript_object_create(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 1) return nh_ecmascript_throwTypeError("Proto must be an object or null", Realm_p);
    nh_ecmascript_Value protoVal = Arguments_p[0];

    // 1. Validate proto type
    if (protoVal.tag != NH_ECMASCRIPT_VALUE_OBJECT && protoVal.tag != NH_ECMASCRIPT_VALUE_NULL) {
        return nh_ecmascript_throwTypeError("Object prototype may only be an Object or null", Realm_p);
    }

    // 2. Create the new object
    nh_ecmascript_Object *proto = (protoVal.tag == NH_ECMASCRIPT_VALUE_OBJECT) ? protoVal.p.object : NULL;
    nh_ecmascript_Object *newObj = nh_ecmascript_ordinaryObjectCreate(proto, NULL, -1);
    
    // 3. Handle properties (second argument) - skipping for brevity, but this is where DefineProperties goes
    if (argumentCount > 1 && Arguments_p[1].tag != NH_ECMASCRIPT_VALUE_UNDEFINED) {
        // Here you would call nh_ecmascript_object_defineProperties logic
    }

    return nh_ecmascript_normalCompletion(nh_ecmascript_makeObject(newObj));
}

nh_ecmascript_Completion nh_ecmascript_object_defineProperties(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_object_defineProperty(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 3) return nh_ecmascript_throwTypeError("Not enough arguments", Realm_p);

    nh_ecmascript_Value Target = Arguments_p[0];
    nh_ecmascript_Value Key    = Arguments_p[1];
    nh_ecmascript_Value Desc   = Arguments_p[2];

    // 1. ToObject validation for target
    if (Target.tag != NH_ECMASCRIPT_VALUE_OBJECT) {
        return nh_ecmascript_throwTypeError("Object.defineProperty called on non-object", Realm_p);
    }

    // 2. Convert JS object to C PropertyDescriptor
    nh_ecmascript_Completion descComp = nh_ecmascript_toPropertyDescriptor(Desc, Realm_p);
    if (descComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return descComp;
    
    nh_ecmascript_PropertyDescriptor *desc_p = descComp.Value.p.internalPointer;

    // 3. Convert Key to Property Key (String or Symbol)
    nh_ecmascript_Completion keyComp = nh_ecmascript_toPropertyKey(Key, Realm_p);
    if (keyComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return keyComp;
    const char *keyStr = keyComp.value.p.string;

    // 4. Perform the Internal Method [[DefineOwnProperty]]
    bool success = nh_ecmascript_defineOwnProperty(Target.p.object, keyStr, desc_p, Realm_p);
    
    if (!success) {
        return nh_ecmascript_throwTypeError("Cannot redefine property", Realm_p);
    }

    return nh_ecmascript_normalCompletion(Target);
}

nh_ecmascript_Completion nh_ecmascript_object_entries(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_object_freeze(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_object_getOwnPropertyDescriptor(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_object_getOwnPropertyDescriptors(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_object_getPrototypeOf(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 1) return nh_ecmascript_throwTypeError("Required argument missing", Realm_p);

    // 1. obj = ToObject(Arguments_p[0])
    nh_ecmascript_Completion objComp = nh_ecmascript_toObject(Arguments_p[0], Realm_p);
    if (objComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return objComp;

    // 2. Return obj.[[GetPrototypeOf]]()
    nh_ecmascript_Object *obj = objComp.value.p.object;
    nh_ecmascript_Object *proto = obj->prototype; // In a full engine, call internal method

    if (!proto) return nh_ecmascript_normalCompletion(nh_ecmascript_makeNull());
    return nh_ecmascript_normalCompletion(nh_ecmascript_makeObject(proto));
}

nh_ecmascript_Completion nh_ecmascript_object_is(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    nh_ecmascript_Value x = (argumentCount > 0) ? Arguments_p[0] : nh_ecmascript_makeUndefined();
    nh_ecmascript_Value y = (argumentCount > 1) ? Arguments_p[1] : nh_ecmascript_makeUndefined();

    bool same = nh_ecmascript_sameValue(x, y);
    return nh_ecmascript_normalCompletion(nh_ecmascript_makeBoolean(same));
}

nh_ecmascript_Completion nh_ecmascript_object_keys(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 1) return nh_ecmascript_throwTypeError("Object required", Realm_p);
    nh_ecmascript_Completion objComp = nh_ecmascript_toObject(Arguments_p[0], Realm_p);
    if (objComp.type == NH_ECMASCRIPT_COMPLETION_THROW) return objComp;
    nh_ecmascript_Object *obj = objComp.value.p.object;

    // 1. Collect enumerable keys
    nh_core_List *keyList = nh_core_createList();
    nh_core_HashMapIterator it = nh_core_getHashMapIterator(&obj->Properties);
    while (nh_core_hashMapNext(&it)) {
        nh_ecmascript_Property *p = it.value;
        if (p->enumerable) {
            nh_core_listPush(keyList, nh_ecmascript_makeString(it.key));
        }
    }

    // 2. Create a new Array Object from the list
    // This requires an nh_ecmascript_createArrayFromList helper
    nh_ecmascript_Object *array = nh_ecmascript_createArrayFromList(keyList, Realm_p);

    return nh_ecmascript_normalCompletion(nh_ecmascript_makeObject(array));
}

nh_ecmascript_Completion nh_ecmascript_object_preventExtensions(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_object_seal(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_object_setPrototypeOf(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_object_values(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}
