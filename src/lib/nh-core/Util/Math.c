// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2022  Dajo Frey
 * Published under GNU LGPL. See Netzhaut/LICENSE.LGPL file.
 */

// INCLUDES ========================================================================================

#include "Math.h"

#include "../Common/Macros.h"

#include <math.h>
#include <stdio.h>
#include <float.h>

// DEFINES =========================================================================================

#define NH_CLIP_LENG \
(                    \
    clipValue + 1.0f \
) 
 
#define NH_CLIP_TO_PIXEL(clipValue, Content_p, basedOnWidth)                                                                             \
(                                                                                                                                        \
    basedOnWidth ? (int)(((((float)clipValue + 1.0f) / Content_p->Settings.scale) * ((float)Content_p->Tab_p->Size.width)) / 2.0f)  \
                 : (int)(((((float)clipValue + 1.0f) / Content_p->Settings.scale) * ((float)Content_p->Tab_p->Size.height)) / 2.0f) \
) 

#define int_TO_CLIP(pixel, Content_p, basedOnWidth)                                                                 \
(                                                                                                                        \
    basedOnWidth ? ((((float)pixel) / ((float)Content_p->Tab_p->Size.width))  * Content_p->Settings.scale) * 2.0f - 1.0f \
                 : ((((float)pixel) / ((float)Content_p->Tab_p->Size.height)) * Content_p->Settings.scale) * 2.0f - 1.0f \
) 

// VECTOR ==========================================================================================

nh_Vector2D nh_subtract2D(
    nh_Vector2D V1, nh_Vector2D V2) 
{
    nh_Vector2D result = {V1.x - V2.x, V1.y - V2.y};
    return result;
}

double nh_norm2D(
    nh_Vector2D V) 
{
    return sqrt(V.x * V.x + V.y * V.y);
}

double nh_dotProduct2D(
    nh_Vector2D V1, nh_Vector2D V2) 
{
    return V1.x * V2.x + V1.y * V2.y;
}

float nh_innerAngle2D(
    nh_Vector2D PIntersect, nh_Vector2D V1, nh_Vector2D V2) 
{
    V1 = nh_subtract2D(V1, PIntersect);
    V2 = nh_subtract2D(V2, PIntersect);
    return acos(nh_dotProduct2D(V1, V2) / (nh_norm2D(V1) * nh_norm2D(V2)));
}

float nh_radiansToDegrees(
    float radians) 
{
    return radians * 180.0 / M_PI;
}

float nh_degreesToRadians(
    float degrees) 
{
    return (degrees * M_PI) / 180.0;
}

nh_Vector2D nh_lineLineIntersection(
    nh_Vector2D A, nh_Vector2D B, nh_Vector2D C, nh_Vector2D D) 
{ 
//printf("%f %f %f %f %f %f %f %f\n", A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);

    // Line AB represented as a1x + b1y = c1 
    double a1 = B.y - A.y; 
    double b1 = A.x - B.x; 
    double c1 = a1*(A.x) + b1*(A.y); 
  
    // Line CD represented as a2x + b2y = c2 
    double a2 = D.y - C.y; 
    double b2 = C.x - D.x; 
    double c2 = a2*(C.x)+ b2*(C.y); 
  
    double determinant = a1*b2 - a2*b1; 
  
    if (determinant == 0) 
    { 
        // The lines are parallel. This is simplified 
        // by returning a pair of FLT_MAX 
        nh_Vector2D Result = {FLT_MAX, FLT_MAX};
        return Result; 
    } 
    else
    { 
        double x = (b2*c1 - b1*c2)/determinant; 
        double y = (a1*c2 - a2*c1)/determinant; 
        nh_Vector2D Result = {x, y};
        return Result; 
    } 
} 

bool nh_pointInBox(
    nh_Vector2D TopLeft, nh_Vector2D BottomRight, nh_Vector2D Point)
{
    return TopLeft.x <= Point.x && TopLeft.y <= Point.y && BottomRight.x >= Point.x && BottomRight.y >= Point.y;
}

nh_Vector2D nh_core_getPointOnCircle(
    nh_Vector2D Center, float radius, float radians) 
{
    nh_Vector2D Result = {Center.x + radius*sin(radians), Center.y + radius*cos(radians)};
    return Result;
}

nh_Vector2D nh_core_getInversedPointOnCircle(
    nh_Vector2D Center, float radius, float radians) 
{
    nh_Vector2D Result = {Center.x + radius*sin(radians), Center.y - radius*cos(radians)};
    return Result;
}

int nh_verticesToArray(
    nh_Vertex *Vertices_p, float *values_p, int count, bool uv, int offset)
{
    for (int i = 0; i < count; ++i) 
    {
        values_p[offset++] = Vertices_p[i].x;
        values_p[offset++] = Vertices_p[i].y;
        values_p[offset++] = Vertices_p[i].z;
        if (uv) {
            values_p[offset++] = Vertices_p[i].u;
            values_p[offset++] = Vertices_p[i].v;
        }
    }

    return offset;
}

int nh_trianglesToArray(
    nh_Triangle *Triangles_p, float *values_p, int count, bool uv)
{
    int offset = 0;

    for (int i = 0; i < count; ++i) 
    {
        offset = nh_verticesToArray(&Triangles_p[i].V1, values_p, 1, uv, offset);
        offset = nh_verticesToArray(&Triangles_p[i].V2, values_p, 1, uv, offset);
        offset = nh_verticesToArray(&Triangles_p[i].V3, values_p, 1, uv, offset);
    }

    return offset;
}

//void nh_scrollVertices(
//    nh_Content *Content_p, nh_Vertex *Vertices_p, int count)
//{
//NH_CORE_BEGIN()
//
//    if (Content_p->Position.x == 0 && Content_p->Position.y == 0) {NH_CORE_SILENT_END()}
//
//    float xOff = NH_CLIP_LENGTH(int_TO_CLIP(Content_p->Position.x, Content_p, true));
//    float yOff = NH_CLIP_LENGTH(int_TO_CLIP(Content_p->Position.y, Content_p, false));
//
//    for (int i = 0; i < count; ++i) {
//        Vertices_p[i].x += xOff; 
//        Vertices_p[i].y += yOff;
//    }
//
//NH_CORE_SILENT_END()
//}
//
//void nh_scrollTriangles(
//    nh_Content *Content_p, nh_Triangle *Triangles_p, int triangleCount)
//{
//NH_CORE_BEGIN()
//
//    for (int i = 0; i < triangleCount; ++i) {
//        nh_scrollVertices(Content_p, &Triangles_p[i].V1, 1);
//        nh_scrollVertices(Content_p, &Triangles_p[i].V2, 1);
//        nh_scrollVertices(Content_p, &Triangles_p[i].V3, 1);
//    }
//
//NH_CORE_SILENT_END()
//}
//
