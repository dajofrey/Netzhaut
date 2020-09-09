/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_HTML_COLLECTION_H
#define NH_JS_HTML_COLLECTION_H

#include "../../Header/Interpreter.h"

/** @addtogroup Structs
 *  @{
 */

    typedef struct Nh_JS_HTMLCollection {
        Nh_JS_Object *Origin_p;
        char *match_p;
        Nh_List List;
    } Nh_JS_HTMLCollection;

/** @} */

/** @addtogroup Functions
 *  @{
 */

// API

    Nh_JS_Result Nh_JS_HTMLCollection_HTMLCollection(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result Nh_JS_HTMLCollection_length(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_HTMLCollection_item(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result Nh_JS_HTMLCollection_namedItem(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

// INTERNAL

    NH_RESULT Nh_JS_createHTMLCollectionObject(
        Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_List List, char *match_p,
        Nh_JS_Object *Origin_p
    );

    NH_RESULT Nh_JS_destroyHTMLCollectionObject(
       Nh_JS_Object *Object_p
    );
 
    NH_RESULT Nh_JS_updateHTMLCollection(
        Nh_JS_HTMLCollection *Collection_p
    );

/** @} */

#endif 
