// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/HTMLCollection.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Memory.h"

#include <string.h>

#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result NH_JS_HTML_COLLECTION_HTMLCollection(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_HTML_COLLECTION_length(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_HTML_COLLECTION_item(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_HTML_COLLECTION_namedItem(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

// INTERNAL ========================================================================================

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG

NH_RESULT Nh_JS_createHTMLCollectionObject(
    Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_List List, char *match_p)
{
NH_BEGIN()

    NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_HTML_COLLECTION, Object_pp))

    (*Object_pp)->data_p = Nh_allocate(sizeof(NH_JS_HTMLCollection));
    NH_CHECK_MEM((*Object_pp)->data_p)

    ((NH_JS_HTMLCollection*)(*Object_pp)->data_p)->List = List;
    ((NH_JS_HTMLCollection*)(*Object_pp)->data_p)->match_p = match_p;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_destroyHTMLCollectionObject(
    Nh_JS_Object *Object_p)
{
NH_BEGIN()

    NH_JS_HTMLCollection *Collection_p = Object_p->data_p;

    Nh_destroyList(&Collection_p->List, false);
    Nh_free(Collection_p->match_p);

    Nh_free(Object_p->data_p);
    Nh_JS_destroyObject(Object_p);

NH_END(NH_SUCCESS)
}

