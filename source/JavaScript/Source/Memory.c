// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Memory.h"
#include "../Header/Macros.h"

#include "../DOM/Header/HTMLCollection.h"
#include "../DOM/Header/Event.h"
#include "../DOM/Header/MouseEvent.h"
#include "../DOM/Header/WheelEvent.h"

#include "../../Core/Header/Memory.h"

#include NH_DEFAULT_CHECK
#include NH_JS_DEBUG
#include NH_JS_UTILS

#include <string.h>

// FREE ============================================================================================

void Nh_JS_free(
    NH_JS_TYPE type, void *data_p)
{
NH_BEGIN()

    switch (type)
    {
        case NH_JS_TYPE_OBJECT :
        {
            Nh_JS_Object *Object_p = data_p;
            switch (Object_p->type)
            {
                case NH_JS_OBJECT_HTML_COLLECTION : Nh_JS_destroyHTMLCollectionObject(Object_p); break;
                case NH_JS_OBJECT_EVENT           : Nh_JS_destroyEventObject(Object_p); break;
                case NH_JS_OBJECT_MOUSE_EVENT     : Nh_JS_destroyMouseEventObject(Object_p); break;
                case NH_JS_OBJECT_WHEEL_EVENT     : Nh_JS_destroyWheelEventObject(Object_p); break;
            }
            break;
            Nh_free(data_p);
        }

        case NH_JS_TYPE_STRING : 
        case NH_JS_TYPE_NUMBER :
            Nh_free(data_p); 
            break;

        default : break;            
    }

NH_SILENT_END()
}

