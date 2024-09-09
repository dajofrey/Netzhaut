#ifndef NH_CORE_MATH_H
#define NH_CORE_MATH_H

#ifndef DOXYGEN_SHOULD_SKIP_THIS

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

#include "../Common/Includes.h"

#endif

/** @addtogroup lib_nh-core_structs
 *  @{
 */

    typedef struct nh_Vector2D {
        double x, y;
    } nh_Vector2D;

    typedef struct nh_Vector3D {
        double x, y, z;
    } nh_Vector3D;

    typedef struct nh_UV {
        double u, v;
    } nh_UV;

    typedef struct nh_Vertex {
        double x, y, z;
        double u, v;
    } nh_Vertex;

    typedef struct nh_PixelVertex {
        NH_PIXEL x, y;
        double z;
        double u, v;
    } nh_PixelVertex;

    typedef struct nh_Triangle {
        nh_Vertex V1, V2, V3; 
    } nh_Triangle;

/** @} */

/** @addtogroup lib_nh-core_functions
 *  @{
 */

    nh_Vector2D nh_subtract2D(
        nh_Vector2D V1, nh_Vector2D V2
    );

    double nh_norm2D(
        nh_Vector2D V
    );

    double nh_dotProduct2D(
        nh_Vector2D V1, nh_Vector2D V2
    );

    float nh_innerAngle2D(
        nh_Vector2D PIntersect, nh_Vector2D P1, nh_Vector2D P2
    );

    float nh_radiansToDegrees(
        float radians
    );

    float nh_degreesToRadians(
        float degrees
    ); 

    nh_Vector2D nh_lineLineIntersection(
        nh_Vector2D A, nh_Vector2D B, nh_Vector2D C, nh_Vector2D D
    ); 

    NH_BOOL nh_pointInBox(
        nh_Vector2D TopLeft, nh_Vector2D BottomRight, nh_Vector2D Point
    );

    nh_Vector2D nh_core_getPointOnCircle(
        nh_Vector2D Center, float radius, float radians
    ); 

    nh_Vector2D nh_core_getInversedPointOnCircle(
        nh_Vector2D Center, float radius, float radians
    ); 

    int nh_verticesToArray(
        nh_Vertex *Vertices_p, float *values_p, int count, NH_BOOL uv, int offset
    );
    
    int nh_trianglesToArray(
        nh_Triangle *Triangles_p, float *values_p, int count, NH_BOOL uv
    );
    
//    void nh_scrollVertices(
//        nh_Content *Content_p, nh_Vertex *Vertices_p, int count
//    );
//    
//    void nh_scrollTriangles(
//        nh_Content *Content_p, nh_Triangle *Triangles_p, int triangleCount
//    );

/** @} */

#endif
