// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/Math.h"

#include <math.h>
#include <stdio.h>
#include <float.h>

// VECTOR ==========================================================================================

Nh_Vector2D Nh_subtract2D(
    Nh_Vector2D V1, Nh_Vector2D V2) 
{
    Nh_Vector2D result = {V1.x - V2.x, V1.y - V2.y};
    return result;
}

double Nh_norm2D(
    Nh_Vector2D V) 
{
    return sqrt(V.x * V.x + V.y * V.y);
}

double Nh_dotProduct2D(
    Nh_Vector2D V1, Nh_Vector2D V2) 
{
    return V1.x * V2.x + V1.y * V2.y;
}

float Nh_innerAngle2D(
    Nh_Vector2D PIntersect, Nh_Vector2D P1, Nh_Vector2D P2) 
{
    P1 = Nh_subtract2D(P1, PIntersect);
    P2 = Nh_subtract2D(P2, PIntersect);
    return acos(Nh_dotProduct2D(P1, P2) / (Nh_norm2D(P1) * Nh_norm2D(P2)));
}

float Nh_radiansToDegrees(
    float radians) 
{
    return radians * 180.0 / M_PI;
}

float Nh_degreesToRadians(
    float degrees) 
{
    return (degrees * M_PI) / 180.0;
}

Nh_Vector2D Nh_lineLineIntersection(
    Nh_Vector2D A, Nh_Vector2D B, Nh_Vector2D C, Nh_Vector2D D) 
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
        Nh_Vector2D Result = {FLT_MAX, FLT_MAX};
        return Result; 
    } 
    else
    { 
        double x = (b2*c1 - b1*c2)/determinant; 
        double y = (a1*c2 - a2*c1)/determinant; 
        Nh_Vector2D Result = {x, y};
        return Result; 
    } 
} 

bool Nh_pointInBox(
    Nh_Vector2D TopLeft, Nh_Vector2D BottomRight, Nh_Vector2D Point)
{
    return TopLeft.x <= Point.x && TopLeft.y <= Point.y && BottomRight.x >= Point.x && BottomRight.y >= Point.y;
}

Nh_Vector2D Nh_getPointOnCircle(
    Nh_Vector2D Center, float radius, float radians) 
{
    Nh_Vector2D Result = {Center.x + radius*sin(radians), Center.y + radius*cos(radians)};
    return Result;
}

Nh_Vector2D Nh_getInversedPointOnCircle(
    Nh_Vector2D Center, float radius, float radians) 
{
    Nh_Vector2D Result = {Center.x + radius*sin(radians), Center.y - radius*cos(radians)};
    return Result;
}

void Nh_trianglesToArray(
    Nh_Triangle *Triangles_p, float *values_p, int count)
{
    for (int i = 0, j = 0; i < count; ++i) {
        values_p[j++] = Triangles_p[i].P1.x;
        values_p[j++] = Triangles_p[i].P1.y;
        values_p[j++] = Triangles_p[i].P1.z;
        values_p[j++] = Triangles_p[i].P2.x;
        values_p[j++] = Triangles_p[i].P2.y;
        values_p[j++] = Triangles_p[i].P2.z;
        values_p[j++] = Triangles_p[i].P3.x;
        values_p[j++] = Triangles_p[i].P3.y;
        values_p[j++] = Triangles_p[i].P3.z;
    }
}

