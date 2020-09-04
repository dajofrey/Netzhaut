#ifndef NH_JS_OBJECT_H
#define NH_JS_OBJECT_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#include "../../../API/Header/Netzhaut.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct Nh_JS_Result Nh_JS_Result;
typedef struct Nh_JS_Script Nh_JS_Script;

#endif

#ifdef DOXYGEN_JS_API

/** @addtogroup JSCoreObject Object
 *  \ingroup JSCore
 *  @{
 */

    Constructor Object();                      /**<Not yet implemented.*/

    Static Method assign();                    /**<Not yet implemented.*/
    Static Method create();                    /**<Not yet implemented.*/
    Static Method defineProperty();            /**<Not yet implemented.*/
    Static Method defineProperties();          /**<Not yet implemented.*/
    Static Method entries();                   /**<Not yet implemented.*/
    Static Method freeze();                    /**<Not yet implemented.*/
    Static Method fromEntries();               /**<Not yet implemented.*/
    Static Method getOwnPropertyDescriptor();  /**<Not yet implemented.*/
    Static Method getOwnPropertyDescriptors(); /**<Not yet implemented.*/
    Static Method getOwnPropertyNames();       /**<Not yet implemented.*/  
    Static Method getOwnPropertySymbols();     /**<Not yet implemented.*/  
    Static Method getPrototypeOf();            /**<Not yet implemented.*/  
    Static Method is();                        /**<Not yet implemented.*/  
    Static Method isExtensible();              /**<Not yet implemented.*/  
    Static Method isFrozen();                  /**<Not yet implemented.*/  
    Static Method isSealed();                  /**<Not yet implemented.*/  
    Static Method keys();                      /**<Not yet implemented.*/  
    Static Method preventExtensions();         /**<Not yet implemented.*/  
    Static Method seal();                      /**<Not yet implemented.*/  
    Static Method setPrototypeOf();            /**<Not yet implemented.*/  
    Static Method values();                    /**<Not yet implemented.*/  

    Instance Property constructor;             /**<Not yet implemented.*/  
    Instance Property __proto__;               /**<Not yet implemented.*/  
    Instance Property __noSuchMethod__;        /**<Not yet implemented.*/  

    Instance Method __defineGetter__();        /**<Not yet implemented.*/  
    Instance Method __defineSetter__();        /**<Not yet implemented.*/  
    Instance Method __lookupGetter__();        /**<Not yet implemented.*/  
    Instance Method __lookupSetter__();        /**<Not yet implemented.*/  
    Instance Method hasOwnProperty();          /**<Not yet implemented.*/  
    Instance Method isPrototypeOf();           /**<Not yet implemented.*/  
    Instance Method propertyIsEnumerable();    /**<Not yet implemented.*/  
    Instance Method toLocaleString();          /**<Not yet implemented.*/
    Instance Method toString();                /**<Not yet implemented.*/
    Instance Method unwatch();                 /**<Not yet implemented.*/
    Instance Method valueOf();                 /**<Not yet implemented.*/
    Instance Method watch();                   /**<Not yet implemented.*/

/** @} */

#else

/** @addtogroup JSEnums Enums
 *  \ingroup JS
 *  @{
 */

    typedef enum NH_JS_OBJECT {
        NH_JS_OBJECT_EVENT_TARGET,
        NH_JS_OBJECT_NODE,
        NH_JS_OBJECT_ELEMENT,
        NH_JS_OBJECT_HTML_ELEMENT,
        NH_JS_OBJECT_LOCATION, 
        NH_JS_OBJECT_DOCUMENT, 
        NH_JS_OBJECT_CONSOLE,  
        NH_JS_OBJECT_WINDOW,
        NH_JS_OBJECT_NH_WINDOW,
        NH_JS_OBJECT_NH_TAB,
        NH_JS_OBJECT_HTML_COLLECTION,
        NH_JS_OBJECT_EVENT,
        NH_JS_OBJECT_CANVAS_RENDERING_CONTEXT_2D,
        NH_JS_OBJECT_HTML_CANVAS_ELEMENT,
        NH_JS_OBJECT_PATH_2D,
        NH_JS_OBJECT_MATH,
        NH_JS_OBJECT_UI_EVENT,
        NH_JS_OBJECT_MOUSE_EVENT,
        NH_JS_OBJECT_WHEEL_EVENT,
        NH_JS_OBJECT_EVENT_LISTENER,
        NH_JS_OBJECT_FUNCTION,
        NH_JS_OBJECT_OBJECT,
        NH_JS_OBJECT_STRING,
        NH_JS_OBJECT_COUNT,
    } NH_JS_OBJECT;

/** @} */

/** @addtogroup JSCoreStructs Structs
 *  \ingroup JSCore
 *  @{
 */

    typedef struct Nh_JS_Object {
        NH_JS_OBJECT type;
        void *data_p;
        size_t functionCount;
        struct Nh_JS_Object *Functions_p;
        struct Nh_JS_Object *Inherit_p;    /**A pointer to the object that the object inherits.*/
        struct Nh_JS_Object *Inheriting_p; /**A pointer to the object that the object is inheriting.*/
    } Nh_JS_Object;

/** @} */

/** @addtogroup JSCoreFunctions Functions
 *  \ingroup JSCore
 *  @{
 */

// API

    Nh_JS_Result Nh_JS_Object_Object(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_Object_assign(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_create(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_defineProperty(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_defineProperties(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_entries(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_freeze(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_fromEntries(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_getOwnPropertyDescriptor(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_getOwnPropertyDescriptors(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_getOwnPropertyNames(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_getOwnPropertySymbols(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_getPrototypeOf(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_is(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_isExtensible(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_isFrozen(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_isSealed(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_keys(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_preventExtensions(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_seal(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_setPrototypeOf(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_Object_values(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

// INTERNAL

    NH_RESULT Nh_JS_createObject(
        Nh_JS_Script *Script_p, NH_JS_OBJECT type, Nh_JS_Object **Object_pp
    );

    void Nh_JS_destroyObject(
        Nh_JS_Object *Object_p
    );

    Nh_JS_Object *Nh_JS_getInheritedObject(
        Nh_JS_Object *Object_p, NH_JS_OBJECT type
    );

    Nh_JS_Object *Nh_JS_getObject(
        Nh_JS_Object *Object_p, NH_JS_OBJECT type
    );

    Nh_JS_Object *Nh_JS_getInheritingObject(
        Nh_JS_Object *Object_p, NH_JS_OBJECT type
    );

    char *Nh_JS_stringifyObject(
        Nh_JS_Object *Object_p, bool newline, bool extended
    );

/** @} */

#endif

#endif 
