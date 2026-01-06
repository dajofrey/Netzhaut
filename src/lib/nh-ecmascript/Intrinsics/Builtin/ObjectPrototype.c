nh_ecmascript_Completion nh_ecmascript_objectprototype___defineGetter__(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 2) return nh_ecmascript_throwTypeError("Getter requires key and function", Realm_p);

    const char *key = nh_ecmascript_toString(Arguments_p[0]);
    nh_ecmascript_Value getter = Arguments_p[1];

    if (!nh_ecmascript_isCallable(getter.p.object)) {
        return nh_ecmascript_throwTypeError("Getter must be a function", Realm_p);
    }

    // Prepare Descriptor
    nh_ecmascript_PropertyDescriptor desc;
    memset(&desc, 0, sizeof(desc));
    desc.Get = getter.p.object;
    desc.presence.hasGet = 1;
    desc.enumerable = true;
    desc.presence.hasEnumerable = 1;
    desc.configurable = true;
    desc.presence.hasConfigurable = 1;

    return nh_ecmascript_defineOwnProperty(Object_p, key, desc, Realm_p);
}

nh_ecmascript_Completion nh_ecmascript_objectprototype___defineSetter__(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{

}

nh_ecmascript_Completion nh_ecmascript_objectprototype___lookupGetter__(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 1) return nh_ecmascript_normalCompletion(nh_ecmascript_wrapUndefined());
    const char *key = nh_ecmascript_toString(Arguments_p[0]);

    nh_ecmascript_Object *current = Object_p;
    while (current != NULL) {
        nh_ecmascript_PropertyDescriptor desc;
        bool found = nh_ecmascript_getOwnProperty(current, key, &desc, Realm_p);

        if (found) {
            if (desc.presence.hasGet) {
                return nh_ecmascript_normalCompletion(nh_ecmascript_wrapObject(desc.Get));
            }
            return nh_ecmascript_normalCompletion(nh_ecmascript_wrapUndefined());
        }
        current = current->prototype_p; // Walk up
    }

    return nh_ecmascript_normalCompletion(nh_ecmascript_wrapUndefined());
}

nh_ecmascript_Completion nh_ecmascript_objectprototype___lookupSetter__(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 1) return nh_ecmascript_normalCompletion(nh_ecmascript_wrapUndefined());
    const char *key = nh_ecmascript_toString(Arguments_p[0]);

    nh_ecmascript_Object *current = Object_p;
    while (current != NULL) {
        nh_ecmascript_PropertyDescriptor desc;
        bool found = nh_ecmascript_getOwnProperty(current, key, &desc, Realm_p);

        if (found) {
            if (desc.presence.hasSet) {
                return nh_ecmascript_normalCompletion(nh_ecmascript_wrapObject(desc.Set));
            }
            return nh_ecmascript_normalCompletion(nh_ecmascript_wrapUndefined());
        }
        current = current->prototype_p;
    }

    return nh_ecmascript_normalCompletion(nh_ecmascript_wrapUndefined());
}

nh_ecmascript_Completion nh_ecmascript_objectprototype_hasOwnProperty(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 1) return nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(false));

    // 1. Get the key from the first argument
    const char *key = nh_ecmascript_toString(Arguments_p[0]); 

    // 2. Use your core function
    nh_ecmascript_PropertyDescriptor desc;
    bool found = nh_ecmascript_getOwnProperty(Object_p, key, &desc, Realm_p);

    return nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(found));
}

nh_ecmascript_Completion nh_ecmascript_objectprototype_isPrototypeOf(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 1 || Arguments_p[0].type != NH_ECMASCRIPT_VALUE_OBJECT) {
        return nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(false));
    }

    nh_ecmascript_Object *target = Arguments_p[0].p.object;

    while (target != NULL) {
        // Get internal [[Prototype]] slot
        target = target->prototype_p; 
        
        if (target == Object_p) {
            return nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(true));
        }
    }

    return nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(false));
}

nh_ecmascript_Completion nh_ecmascript_objectprototype_propertyIsEnumerable(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    if (argumentCount < 1) return nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(false));
    const char *key = nh_ecmascript_toString(Arguments_p[0]);

    nh_ecmascript_PropertyDescriptor desc;
    bool found = nh_ecmascript_getOwnProperty(Object_p, key, &desc, Realm_p);

    if (!found) return nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(false));
    
    return nh_ecmascript_normalCompletion(nh_ecmascript_wrapBoolean(desc.enumerable));
}

nh_ecmascript_Completion nh_ecmascript_objectprototype_toLocaleString(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    // Just call the toString implementation
    return nh_ecmascript_objectprototype_toString(Object_p, Arguments_p, argumentCount, Realm_p);
}

nh_ecmascript_Completion nh_ecmascript_objectprototype_toString(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    // Simplified: in a real engine, you'd check for [[Tag]] symbols
    // For now, return "[object Object]"
    const char *str = "[object Object]";
    return nh_ecmascript_normalCompletion(nh_ecmascript_wrapString(str));
}

nh_ecmascript_Completion nh_ecmascript_objectprototype_valueOf(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    // Object.prototype.valueOf simply returns 'this' as an object
    return nh_ecmascript_normalCompletion(nh_ecmascript_wrapObject(Object_p));
}
