// Internal Helper for Section 20.2.1.1.1
static nh_ecmascript_Completion nh_ecmascript_createDynamicFunction(
    nh_ecmascript_Object *constructor,
    nh_ecmascript_Object *newTarget,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    // 1. If no arguments, body is "", else last argument is body
    // 2. All other arguments are parameter names separated by commas
    // 3. This requires calling your Parser/Lexer at runtime!
    
    // For now, let's look at the basic plumbing:
    if (argumentCount == 0) {
        // Create empty function: function() {}
    }

    // You will eventually need to:
    // a) Concatenate arguments into a parameter string and a body string.
    // b) Call: Parser_ParseFunction(parameterString, bodyString)
    // c) Return a new Function Object with the resulting code.
    
    return nh_ecmascript_throwTypeError("Dynamic function creation via 'new Function' not yet implemented in Parser", Realm_p);
}

nh_ecmascript_Completion nh_ecmascript_functionConstructorCall(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    // Call is equivalent to Construct
    return nh_ecmascript_createDynamicFunction(Object_p, NULL, Arguments_p, argumentCount, Realm_p);
}

nh_ecmascript_Completion nh_ecmascript_functionConstructorConstruct(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    return nh_ecmascript_createDynamicFunction(Object_p, Object_p, Arguments_p, argumentCount, Realm_p);
}

nh_ecmascript_Completion nh_ecmascript_functionprototype_toString(
    nh_ecmascript_Object *Object_p,
    nh_ecmascript_Value *Arguments_p,
    size_t argumentCount,
    nh_ecmascript_Realm *Realm_p)
{
    // 1. Ensure 'this' is actually a function
    if (!nh_ecmascript_isCallable(Object_p)) {
        return nh_ecmascript_throwTypeError("Function.prototype.toString called on non-callable", Realm_p);
    }

    // 2. If it's a native function, return the native code string
    if (Object_p->isNative) {
        // You can use the 'name' property of the object if you store it
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "function %s() { [native code] }", Object_p->name);
        return nh_ecmascript_normalCompletion(nh_ecmascript_wrapString(buffer));
    }

    // 3. If it's a JS function, return the source code (if you stored it)
    if (Object_p->sourceCode_p) {
        return nh_ecmascript_normalCompletion(nh_ecmascript_wrapString(Object_p->sourceCode_p));
    }

    return nh_ecmascript_normalCompletion(nh_ecmascript_wrapString("function () { }"));
}
