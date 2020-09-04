// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE ========================================================================================

#include "../Header/Path2D.h"
#include "../Header/Macros.h"

#include "../../../Core/Header/Memory.h"

#include <string.h>

#include NH_CUSTOM_CHECK
#include NH_JS_DEBUG_FUNCTION

// API =============================================================================================

Nh_JS_Result NH_JS_PATH_2D_Path2D(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_addPath(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_closePath(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_moveTo(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_lineTo(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_bezierCurveTo(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_quadraticCurveTo(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_arc(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

    if (aCount >= 5)
    {
        NH_JS_Path2D *Path_p = Function_p->data_p;

        NH_JS_Path2DCommand *Command_p = Nh_allocate(sizeof(NH_JS_Path2DCommand));
        NH_JS_Path2DArc *Arc_p = Nh_allocate(sizeof(NH_JS_Path2DArc));
        NH_CHECK_NULL(Nh_JS_getNULLResult(), Command_p, Arc_p)

        Arc_p->x          = (float)*((double*)Arguments_p[0].data_p);
        Arc_p->y          = (float)*((double*)Arguments_p[1].data_p);
        Arc_p->radius     = (float)*((double*)Arguments_p[2].data_p);
        Arc_p->startAngle = (float)*((double*)Arguments_p[3].data_p);
        Arc_p->endAngle   = (float)*((double*)Arguments_p[4].data_p);

        Command_p->type = NH_JS_PATH_2D_COMMAND_ARC;
        Command_p->data_p = Arc_p;

        NH_CHECK(Nh_JS_getNULLResult(), Nh_addListItem(&Path_p->Commands, Command_p))
    }

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_arcTo(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_ellipse(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

Nh_JS_Result NH_JS_PATH_2D_rect(
    Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p)
{
NH_BEGIN()

NH_END(Nh_JS_getNULLResult())
}

