/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#ifndef NH_JS_PATH_2D_H
#define NH_JS_PATH_2D_H

#include "../../Header/Interpreter.h"

/** @addtogroup Enums
 *  @{
 */

    typedef enum NH_JS_PATH_2D_COMMAND {
        NH_JS_PATH_2D_COMMAND_ARC,
    } NH_JS_PATH_2D_COMMAND;

/** @} */

/** @addtogroup Structs
 *  @{
 */

    typedef struct NH_JS_Path2DArc {
        float x, y;
        float radius;
        float startAngle, endAngle;
        bool anticlockwise;
    } NH_JS_Path2DArc;

    typedef struct NH_JS_Path2DCommand {
        NH_JS_PATH_2D_COMMAND type;
        void *data_p;
    } NH_JS_Path2DCommand;

    typedef struct NH_JS_Path2D {
        Nh_List Commands;
    } NH_JS_Path2D;

/** @} */

/** @addtogroup Functions
 *  @{
 */

    Nh_JS_Result NH_JS_PATH_2D_Path2D(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

    Nh_JS_Result NH_JS_PATH_2D_addPath(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_closePath(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_moveTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_lineTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_bezierCurveTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_quadraticCurveTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_arc(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_arcTo(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_ellipse(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );
    
    Nh_JS_Result NH_JS_PATH_2D_rect(
        Nh_JS_Script *Script_p, Nh_JS_Object *Function_p, int aCount, Nh_JS_Result *Arguments_p
    );

/** @} */

#endif 
