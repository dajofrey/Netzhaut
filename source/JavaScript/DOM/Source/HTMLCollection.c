// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/HTMLCollection.h"
#include "../Header/Document.h"
#include "../Header/HTMLElement.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Memory.h"

#include <string.h>

#include NH_UTILS
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
    Nh_JS_Script *Script_p, Nh_JS_Object **Object_pp, Nh_List List, char *match_p, 
    Nh_JS_Object *Origin_p)
{
NH_BEGIN()

    NH_CHECK(Nh_JS_createObject(Script_p, NH_JS_OBJECT_HTML_COLLECTION, Object_pp))

    (*Object_pp)->data_p = Nh_allocate(sizeof(Nh_JS_HTMLCollection));
    NH_CHECK_MEM((*Object_pp)->data_p)

    ((Nh_JS_HTMLCollection*)(*Object_pp)->data_p)->Origin_p = Origin_p;
    ((Nh_JS_HTMLCollection*)(*Object_pp)->data_p)->List = List;
    ((Nh_JS_HTMLCollection*)(*Object_pp)->data_p)->match_p = match_p;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_updateHTMLCollection(
    Nh_JS_HTMLCollection *Collection_p)
{
NH_BEGIN()

    Nh_List Elements;
    NH_INIT_LIST(Elements)

    if (Collection_p->Origin_p != NULL && Collection_p->Origin_p->type == NH_JS_OBJECT_DOCUMENT)
    {
        Nh_JS_Document *Document_p = Collection_p->Origin_p->data_p;
        for (int i = 0; i < Document_p->Tree.Flat.count; ++i) 
        {
            Nh_JS_Object *Object_p = Nh_getListItem(&Document_p->Tree.Flat, i);
            if (Object_p->type == NH_JS_OBJECT_HTML_ELEMENT) 
            {
                Nh_HTML_Node *Node_p = ((Nh_JS_HTMLElement*)Object_p->data_p)->Node_p;
                if (Node_p != NULL && !strcmp(NH_HTML_TAGS_PP[Node_p->tag], Collection_p->match_p)) {
                    NH_CHECK(Nh_addListItem(&Elements, Object_p))
                }
            }
        }
    }

    Nh_destroyList(&Collection_p->List, false);
    Collection_p->List = Elements;

NH_END(NH_SUCCESS)
}

NH_RESULT Nh_JS_destroyHTMLCollectionObject(
    Nh_JS_Object *Object_p)
{
NH_BEGIN()

    Nh_JS_HTMLCollection *Collection_p = Object_p->data_p;

    Nh_destroyList(&Collection_p->List, false);
    Nh_free(Collection_p->match_p);

    Nh_free(Object_p->data_p);
    Nh_JS_destroyObject(Object_p);

NH_END(NH_SUCCESS)
}

