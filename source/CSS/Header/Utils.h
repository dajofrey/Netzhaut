#ifndef NH_CSS_UTILS_H
#define NH_CSS_UTILS_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

#endif

/** @addtogroup CSSStructs Structs
 *  \ingroup CSS
 *  @{
 */

    typedef struct NH_CSS_Generic {
        int type;
        float value;
        void *extra_p;
    } NH_CSS_Generic;

    typedef struct NH_CSS_Corners {
        float topLeft;
        float topRight;
        float bottomRight;
        float bottomLeft;
    } NH_CSS_Corners;
    
    typedef struct NH_CSS_Sides {
        float top;
        float right;
        float bottom;
        float left; 
    } NH_CSS_Sides;

    typedef struct NH_CSS_Types {
        int top;
        int right;
        int bottom;
        int left; 
    } NH_CSS_Types;
   
    typedef struct NH_CSS_SidesColor {
        float top_p[4];
        float right_p[4];
        float bottom_p[4];
        float left_p[4]; 
    } NH_CSS_SidesColor;

/** @} */

#endif
