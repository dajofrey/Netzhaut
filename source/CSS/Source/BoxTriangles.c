// LICENSE NOTICE ==================================================================================

/**
 * Netzhaut - Web Browser Engine
 * Copyright (C) 2020  Dajo Frey
 * Published under LGPLv3
 */

// INCLUDE =========================================================================================

#include "../Header/BoxTriangles.h"
#include "../Header/Macros.h"

#include "../../Core/Header/Math.h"

#include NH_CSS_UTILS
#include NH_DEBUG
#include NH_DEFAULT_CHECK

#include <string.h>
#include <ctype.h>
#include <math.h>
#include <float.h>

// DECLARE ==========================================================================================

#define TOP_LEFT 0
#define TOP_RIGHT 1
#define BOTTOM_LEFT 2
#define BOTTOM_RIGHT 3

#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3

// DEFINE ==========================================================================================

typedef struct Nh_CSS_CornersData {
    float radii_p[4];
    struct {
        int leftCount;
        int rightCount;
        int bottomCount;
        int topCount;
        bool topLeft;
        bool topRight;
        bool bottomLeft;
        bool bottomRight;
    } Rounded;
    Nh_CSS_Box TopLeft;
    Nh_CSS_Box TopRight;
    Nh_CSS_Box BottomLeft;
    Nh_CSS_Box BottomRight;
} Nh_CSS_CornersData;

// DECLARE =========================================================================================

static int Nh_CSS_getTopTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_CSS_CornersData Corners, int triangleCount, float z
);
static int Nh_CSS_getBottomTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_CSS_CornersData Corners, int triangleCount, float z
);
static int Nh_CSS_getLeftTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_CSS_CornersData Corners, int triangleCount, float z
);
static int Nh_CSS_getRightTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_CSS_CornersData Corners, int triangleCount, float z
);

static int Nh_CSS_getTopLeftCornerTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_CSS_CornersData, int cornerTriangleCount, int triangleCount, int side, float z
);
static int Nh_CSS_getTopRightCornerTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_CSS_CornersData Corners, int cornerTriangleCount, int triangleCount, int side, float z
);
static int Nh_CSS_getBottomLeftCornerTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_CSS_CornersData Corners, int cornerTriangleCount, int triangleCount, int side, float z
);
static int Nh_CSS_getBottomRightCornerTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_CSS_CornersData Corners, int cornerTriangleCount, int triangleCount, int side, float z
);

static Nh_CSS_CornersData Nh_CSS_getCorners(
    Nh_CSS_Box EnclosingBox, float Radii_p[4]
);

// BOX TRIANGLES ===================================================================================

int Nh_CSS_getBoxTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, float radii_p[4], int cornerTriangleCount, 
    int side, Nh_Triangle *Triangles_p, bool enclosedPoint, int offset, float z)
{
NH_BEGIN()

    Nh_CSS_Box EnclosedPoint;

    EnclosedPoint.TopLeft.y = EnclosedBox.TopLeft.y + (NH_CSS_NORMALIZED_LENGTH(EnclosedBox.BottomRight.y) - NH_CSS_NORMALIZED_LENGTH(EnclosedBox.TopLeft.y)) / 2.0f;
    EnclosedPoint.BottomRight.y = EnclosedBox.TopLeft.y;

    EnclosedPoint.TopLeft.x = EnclosedBox.TopLeft.x + (NH_CSS_NORMALIZED_LENGTH(EnclosedBox.BottomRight.x) - NH_CSS_NORMALIZED_LENGTH(EnclosedBox.TopLeft.x)) / 2.0f;
    EnclosedPoint.BottomRight.x = EnclosedBox.TopLeft.x;

    int triangleCount = offset;
    Nh_CSS_CornersData Corners = Nh_CSS_getCorners(EnclosingBox, radii_p);

    switch (side)
    {
        case TOP :
            triangleCount = Nh_CSS_getTopTriangles(
                EnclosingBox, EnclosedBox, enclosedPoint ? &EnclosedPoint : NULL, Triangles_p, cornerTriangleCount, Corners, triangleCount, z
            );
            break;
        case BOTTOM :
            triangleCount = Nh_CSS_getBottomTriangles(
                EnclosingBox, EnclosedBox, enclosedPoint ? &EnclosedPoint : NULL, Triangles_p, cornerTriangleCount, Corners, triangleCount, z
            );
            break;
        case LEFT :
            triangleCount = Nh_CSS_getLeftTriangles(
                EnclosingBox, EnclosedBox, enclosedPoint ? &EnclosedPoint : NULL, Triangles_p, cornerTriangleCount, Corners, triangleCount, z
            );
            break;
        case RIGHT :
            triangleCount = Nh_CSS_getRightTriangles(
                EnclosingBox, EnclosedBox, enclosedPoint ? &EnclosedPoint : NULL, Triangles_p, cornerTriangleCount, Corners, triangleCount, z
            );
            break;
    }

NH_END(triangleCount)
}

static int Nh_CSS_getTopTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_CSS_CornersData Corners, int triangleCount, float z)
{
NH_BEGIN()

    Triangles_p[triangleCount].P1.x = Corners.TopLeft.BottomRight.x;
    Triangles_p[triangleCount].P1.y = Corners.TopLeft.TopLeft.y;
    Triangles_p[triangleCount].P1.z = z;

    Triangles_p[triangleCount].P2.x = Corners.TopRight.TopLeft.x;
    Triangles_p[triangleCount].P2.y = Corners.TopRight.TopLeft.y;
    Triangles_p[triangleCount].P2.z = z;

    Triangles_p[triangleCount].P3.x = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.x : EnclosedPoint_p->BottomRight.x;
    Triangles_p[triangleCount].P3.y = EnclosedPoint_p == NULL ? EnclosedBox.TopLeft.y : EnclosedPoint_p->TopLeft.y;
    Triangles_p[triangleCount].P3.z = z;

    triangleCount++;

    if (EnclosedPoint_p == NULL) 
    {
        Triangles_p[triangleCount].P1.x = Corners.TopLeft.BottomRight.x;
        Triangles_p[triangleCount].P1.y = Corners.TopLeft.TopLeft.y;
        Triangles_p[triangleCount].P1.z = z;
    
        Triangles_p[triangleCount].P2.x = EnclosedBox.TopLeft.x;
        Triangles_p[triangleCount].P2.y = EnclosedBox.TopLeft.y;
        Triangles_p[triangleCount].P2.z = z;
    
        Triangles_p[triangleCount].P3.x = EnclosedBox.BottomRight.x;
        Triangles_p[triangleCount].P3.y = EnclosedBox.TopLeft.y;
        Triangles_p[triangleCount].P3.z = z;

        triangleCount++;
    }

    if (Corners.Rounded.topLeft)
    {
        triangleCount = Nh_CSS_getTopLeftCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, TOP, z
        );
    }

    if (Corners.Rounded.topRight)
    {
        triangleCount = Nh_CSS_getTopRightCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, TOP, z
        );
    }

NH_END(triangleCount)
}

static int Nh_CSS_getBottomTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_CSS_CornersData Corners, int triangleCount, float z)
{
NH_BEGIN()

    Triangles_p[triangleCount].P1.x = Corners.BottomLeft.BottomRight.x;
    Triangles_p[triangleCount].P1.y = Corners.BottomLeft.BottomRight.y;
    Triangles_p[triangleCount].P1.z = z;

    Triangles_p[triangleCount].P2.x = Corners.BottomRight.TopLeft.x;
    Triangles_p[triangleCount].P2.y = Corners.BottomRight.BottomRight.y;
    Triangles_p[triangleCount].P2.z = z;

    Triangles_p[triangleCount].P3.x = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.x : EnclosedPoint_p->BottomRight.x;
    Triangles_p[triangleCount].P3.y = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.y : EnclosedPoint_p->BottomRight.y;
    Triangles_p[triangleCount].P3.z = z; 

    triangleCount++;

    if (EnclosedPoint_p == NULL)
    {
        Triangles_p[triangleCount].P1.x = Corners.BottomLeft.BottomRight.x;
        Triangles_p[triangleCount].P1.y = Corners.BottomLeft.BottomRight.y;
        Triangles_p[triangleCount].P1.z = z;
 
        Triangles_p[triangleCount].P2.x = EnclosedBox.TopLeft.x;
        Triangles_p[triangleCount].P2.y = EnclosedBox.BottomRight.y;
        Triangles_p[triangleCount].P2.z = z;

        Triangles_p[triangleCount].P3.x = EnclosedBox.BottomRight.x;
        Triangles_p[triangleCount].P3.y = EnclosedBox.BottomRight.y;
        Triangles_p[triangleCount].P3.z = z;

        triangleCount++;
    }

    if (Corners.Rounded.bottomLeft)
    {
        triangleCount = Nh_CSS_getBottomLeftCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, BOTTOM, z
        );
    }

    if (Corners.Rounded.bottomRight)
    {
        triangleCount = Nh_CSS_getBottomRightCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, BOTTOM, z
        );
    }

NH_END(triangleCount)
}

static int Nh_CSS_getLeftTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_CSS_CornersData Corners, int triangleCount, float z)
{
NH_BEGIN()

    Triangles_p[triangleCount].P1.x = Corners.TopLeft.TopLeft.x;
    Triangles_p[triangleCount].P1.y = Corners.TopLeft.BottomRight.y;
    Triangles_p[triangleCount].P1.z = z;

    Triangles_p[triangleCount].P2.x = Corners.BottomLeft.TopLeft.x;
    Triangles_p[triangleCount].P2.y = Corners.BottomLeft.TopLeft.y;
    Triangles_p[triangleCount].P2.z = z;

    Triangles_p[triangleCount].P3.x = EnclosedPoint_p == NULL ? EnclosedBox.TopLeft.x : EnclosedPoint_p->TopLeft.x;
    Triangles_p[triangleCount].P3.y = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.y : EnclosedPoint_p->BottomRight.y;
    Triangles_p[triangleCount].P3.z = z;

    triangleCount++;

    if (EnclosedPoint_p == NULL)
    {                         
        Triangles_p[triangleCount].P1.x = Corners.TopLeft.TopLeft.x;
        Triangles_p[triangleCount].P1.y = Corners.TopLeft.BottomRight.y;
        Triangles_p[triangleCount].P1.z = z;

        Triangles_p[triangleCount].P2.x = EnclosedBox.TopLeft.x;
        Triangles_p[triangleCount].P2.y = EnclosedBox.TopLeft.y;
        Triangles_p[triangleCount].P2.z = z;

        Triangles_p[triangleCount].P3.x = EnclosedBox.TopLeft.x;
        Triangles_p[triangleCount].P3.y = EnclosedBox.BottomRight.y;
        Triangles_p[triangleCount].P3.z = z;

        triangleCount++;
    }

    if (Corners.Rounded.topLeft)
    {
        triangleCount = Nh_CSS_getTopLeftCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, LEFT, z
        );
    }

    if (Corners.Rounded.bottomLeft)
    {
        triangleCount = Nh_CSS_getBottomLeftCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, LEFT, z
        );
    }

NH_END(triangleCount)
}

static int Nh_CSS_getRightTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    int cornerTriangleCount, Nh_CSS_CornersData Corners, int triangleCount, float z)
{
NH_BEGIN()

    Triangles_p[triangleCount].P1.x = Corners.TopRight.BottomRight.x;
    Triangles_p[triangleCount].P1.y = Corners.TopRight.BottomRight.y;
    Triangles_p[triangleCount].P1.z = z;

    Triangles_p[triangleCount].P2.x = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.x : EnclosedPoint_p->BottomRight.x;
    Triangles_p[triangleCount].P2.y = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.y : EnclosedPoint_p->BottomRight.y;
    Triangles_p[triangleCount].P2.z = z;

    Triangles_p[triangleCount].P3.x = Corners.BottomRight.BottomRight.x;
    Triangles_p[triangleCount].P3.y = Corners.BottomRight.TopLeft.y;
    Triangles_p[triangleCount].P3.z = z;

    triangleCount++;

    if (EnclosedPoint_p == NULL)
    {                       
        Triangles_p[triangleCount].P1.x = Corners.TopRight.BottomRight.x;
        Triangles_p[triangleCount].P1.y = Corners.TopRight.BottomRight.y;
        Triangles_p[triangleCount].P1.z = z;

        Triangles_p[triangleCount].P2.x = EnclosedBox.BottomRight.x;
        Triangles_p[triangleCount].P2.y = EnclosedBox.BottomRight.y;
        Triangles_p[triangleCount].P2.z = z;

        Triangles_p[triangleCount].P3.x = EnclosedBox.BottomRight.x;
        Triangles_p[triangleCount].P3.y = EnclosedBox.TopLeft.y;
        Triangles_p[triangleCount].P3.z = z;

        triangleCount++;
    }

    if (Corners.Rounded.topRight)
    {
        triangleCount = Nh_CSS_getTopRightCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, RIGHT, z
        );
    }

    if (Corners.Rounded.bottomRight)
    {
        triangleCount = Nh_CSS_getBottomRightCornerTriangles(
            EnclosingBox, EnclosedBox, EnclosedPoint_p, Triangles_p, Corners, cornerTriangleCount, triangleCount, RIGHT, z
        );
    }

NH_END(triangleCount)
}

static int Nh_CSS_getTopLeftCornerTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p,
    Nh_CSS_CornersData Corners, int cornerTriangleCount, int triangleCount, int side, float z)
{
NH_BEGIN()

    Nh_Vector2D EnclosingBoxTopLeft = {EnclosingBox.TopLeft.x, EnclosingBox.TopLeft.y};
    Nh_Vector2D EnclosedBoxTopLeft  = {EnclosedBox.TopLeft.x, EnclosedBox.TopLeft.y};
    Nh_Vector2D EnclosedBoxTopRight = {EnclosedBox.BottomRight.x, EnclosedBox.TopLeft.y};
       
    float totalRadians, divRadians, radians;

    if (side == TOP)
    {
        totalRadians = Nh_innerAngle2D(EnclosingBoxTopLeft, EnclosedBoxTopLeft, EnclosedBoxTopRight);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(360) - totalRadians;
    }
    else if (side == LEFT) 
    {
        totalRadians = Nh_degreesToRadians(90) - Nh_innerAngle2D(EnclosingBoxTopLeft, EnclosedBoxTopLeft, EnclosedBoxTopRight);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(270);
    }

    Nh_Vector2D Point, OldPoint;
    Nh_Vector2D Center = {Corners.TopLeft.BottomRight.x, Corners.TopLeft.BottomRight.y};
    OldPoint = Nh_getInversedPointOnCircle(Center, Corners.radii_p[TOP_LEFT], radians); 

    for (int i = 0; i < cornerTriangleCount; ++i) 
    {
        int index = triangleCount;
        radians += divRadians;

        Point = Nh_getInversedPointOnCircle(Center, Corners.radii_p[TOP_LEFT], radians); 

        Triangles_p[index].P1.x = EnclosedPoint_p == NULL ? EnclosedBox.TopLeft.x : EnclosedPoint_p->TopLeft.x;
        Triangles_p[index].P1.y = EnclosedPoint_p == NULL ? EnclosedBox.TopLeft.y : EnclosedPoint_p->TopLeft.y;
        Triangles_p[index].P1.z = z;

        Triangles_p[index].P2.x = OldPoint.x;
        Triangles_p[index].P2.y = OldPoint.y;
        Triangles_p[index].P2.z = z;

        Triangles_p[index].P3.x = Point.x;
        Triangles_p[index].P3.y = Point.y;
        Triangles_p[index].P3.z = z;

        triangleCount++;
        OldPoint = Point;
    }
 
NH_END(triangleCount)
}

static int Nh_CSS_getTopRightCornerTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p,
    Nh_CSS_CornersData Corners, int cornerTriangleCount, int triangleCount, int side, float z)
{
NH_BEGIN()

    Nh_Vector2D EnclosingBoxTopRight = {EnclosingBox.BottomRight.x, EnclosingBox.TopLeft.y};
    Nh_Vector2D EnclosedBoxTopRight  = {EnclosedBox.BottomRight.x, EnclosedBox.TopLeft.y};
    Nh_Vector2D EnclosedBoxTopLeft   = {EnclosedBox.TopLeft.x, EnclosedBox.TopLeft.y};

    float totalRadians, divRadians, radians;

    if (side == TOP)
    {
        totalRadians = Nh_innerAngle2D(EnclosingBoxTopRight, EnclosedBoxTopRight, EnclosedBoxTopLeft);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = 0.0f;
    }
    else if (side == RIGHT)
    {
        totalRadians = Nh_degreesToRadians(90) - Nh_innerAngle2D(EnclosingBoxTopRight, EnclosedBoxTopRight, EnclosedBoxTopLeft);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_innerAngle2D(EnclosingBoxTopRight, EnclosedBoxTopRight, EnclosedBoxTopLeft);
    }

    Nh_Vector2D Point, OldPoint;
    Nh_Vector2D Center = {Corners.TopRight.TopLeft.x, Corners.TopRight.BottomRight.y};
    OldPoint = Nh_getInversedPointOnCircle(Center, Corners.radii_p[TOP_RIGHT], radians); 

    for (int i = 0; i < cornerTriangleCount; ++i) 
    {
        int index = triangleCount;
        radians += divRadians;

        Point = Nh_getInversedPointOnCircle(Center, Corners.radii_p[TOP_RIGHT], radians); 

        Triangles_p[index].P1.x = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.x : EnclosedPoint_p->BottomRight.x;
        Triangles_p[index].P1.y = EnclosedPoint_p == NULL ? EnclosedBox.TopLeft.y : EnclosedPoint_p->TopLeft.y;
        Triangles_p[index].P1.z = z;

        Triangles_p[index].P2.x = OldPoint.x;
        Triangles_p[index].P2.y = OldPoint.y;
        Triangles_p[index].P2.z = z;

        Triangles_p[index].P3.x = Point.x;
        Triangles_p[index].P3.y = Point.y;
        Triangles_p[index].P3.z = z;

        triangleCount++;
        OldPoint = Point;
    }

NH_END(triangleCount)
}

static int Nh_CSS_getBottomLeftCornerTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_CSS_CornersData Corners, int cornerTriangleCount, int triangleCount, int side, float z)
{
NH_BEGIN()

    Nh_Vector2D EnclosingBoxBottomLeft = {EnclosingBox.TopLeft.x, EnclosingBox.BottomRight.y};
    Nh_Vector2D EnclosedBoxBottomLeft  = {EnclosedBox.TopLeft.x, EnclosedBox.BottomRight.y};
    Nh_Vector2D EnclosedBoxBottomRight = {EnclosedBox.BottomRight.x, EnclosedBox.BottomRight.y};

    float totalRadians, divRadians, radians;

    if (side == BOTTOM) 
    {
        totalRadians = Nh_innerAngle2D(EnclosingBoxBottomLeft, EnclosedBoxBottomLeft, EnclosedBoxBottomRight);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(180);
    }
    else if (side == LEFT)
    {
        totalRadians = Nh_degreesToRadians(90) - Nh_innerAngle2D(EnclosingBoxBottomLeft, EnclosedBoxBottomLeft, EnclosedBoxBottomRight);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(180) + Nh_innerAngle2D(EnclosingBoxBottomLeft, EnclosedBoxBottomLeft, EnclosedBoxBottomRight);
    }

    Nh_Vector2D Point, OldPoint;
    Nh_Vector2D Center = {Corners.BottomLeft.BottomRight.x, Corners.BottomLeft.TopLeft.y};
    OldPoint = Nh_getInversedPointOnCircle(Center, Corners.radii_p[BOTTOM_LEFT], radians); 

    for (int i = 0; i < cornerTriangleCount; ++i) 
    {
        int index = triangleCount;
        radians += divRadians;

        Point = Nh_getInversedPointOnCircle(Center, Corners.radii_p[BOTTOM_LEFT], radians); 

        Triangles_p[index].P1.x = EnclosedPoint_p == NULL ? EnclosedBox.TopLeft.x : EnclosedPoint_p->TopLeft.x;
        Triangles_p[index].P1.y = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.y : EnclosedPoint_p->BottomRight.y;
        Triangles_p[index].P1.z = z;

        Triangles_p[index].P2.x = OldPoint.x;
        Triangles_p[index].P2.y = OldPoint.y;
        Triangles_p[index].P2.z = z;

        Triangles_p[index].P3.x = Point.x;
        Triangles_p[index].P3.y = Point.y;
        Triangles_p[index].P3.z = z;

        triangleCount++;
        OldPoint = Point;
    } 

NH_END(triangleCount)
}

static int Nh_CSS_getBottomRightCornerTriangles(
    Nh_CSS_Box EnclosingBox, Nh_CSS_Box EnclosedBox, Nh_CSS_Box *EnclosedPoint_p, Nh_Triangle *Triangles_p, 
    Nh_CSS_CornersData Corners, int cornerTriangleCount, int triangleCount, int side, float z)
{
NH_BEGIN()

    Nh_Vector2D EnclosingBoxBottomRight = {EnclosingBox.BottomRight.x, EnclosingBox.BottomRight.y};
    Nh_Vector2D EnclosedBoxBottomRight = {EnclosedBox.BottomRight.x, EnclosedBox.BottomRight.y};
    Nh_Vector2D EnclosedBoxBottomLeft = {EnclosedBox.TopLeft.x, EnclosedBox.BottomRight.y};

    float totalRadians, divRadians, radians;

    if (side == BOTTOM)
    {
        totalRadians = Nh_innerAngle2D(EnclosingBoxBottomRight, EnclosedBoxBottomRight, EnclosedBoxBottomLeft);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(180) - totalRadians;
    }
    else if (side == RIGHT)
    {
        totalRadians = Nh_degreesToRadians(90) - Nh_innerAngle2D(EnclosingBoxBottomRight, EnclosedBoxBottomRight, EnclosedBoxBottomLeft);
        divRadians = totalRadians / ((float)cornerTriangleCount);
        radians = Nh_degreesToRadians(90);
    }

    Nh_Vector2D Point, OldPoint;
    Nh_Vector2D Center = {Corners.BottomRight.TopLeft.x, Corners.BottomRight.TopLeft.y};
    OldPoint = Nh_getInversedPointOnCircle(Center, Corners.radii_p[BOTTOM_RIGHT], radians); 

    for (int i = 0; i < cornerTriangleCount; ++i) 
    {
        int index = triangleCount;
        radians += divRadians;

        Point = Nh_getInversedPointOnCircle(Center, Corners.radii_p[BOTTOM_RIGHT], radians); 

        Triangles_p[index].P1.x = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.x : EnclosedPoint_p->BottomRight.x;
        Triangles_p[index].P1.y = EnclosedPoint_p == NULL ? EnclosedBox.BottomRight.y : EnclosedPoint_p->BottomRight.y;
        Triangles_p[index].P1.z = z;

        Triangles_p[index].P2.x = OldPoint.x;
        Triangles_p[index].P2.y = OldPoint.y;
        Triangles_p[index].P2.z = z;

        Triangles_p[index].P3.x = Point.x;
        Triangles_p[index].P3.y = Point.y;
        Triangles_p[index].P3.z = z;

        triangleCount++;
        OldPoint = Point;
    }

NH_END(triangleCount)
}

static Nh_CSS_CornersData Nh_CSS_getCorners(
    Nh_CSS_Box EnclosingBox, float radii_p[4])
{
NH_BEGIN()

    Nh_CSS_CornersData Corners = {0};

    memcpy(Corners.radii_p, radii_p, sizeof(float) * 4);

    Corners.Rounded.leftCount = 0;
    Corners.Rounded.rightCount = 0;
    Corners.Rounded.bottomCount = 0; 
    Corners.Rounded.topCount = 0;

    Corners.Rounded.topLeft     = radii_p[TOP_LEFT] > 0.000001f;
    Corners.Rounded.topRight    = radii_p[TOP_RIGHT] > 0.000001f;
    Corners.Rounded.bottomLeft  = radii_p[BOTTOM_LEFT] > 0.00001f;
    Corners.Rounded.bottomRight = radii_p[BOTTOM_RIGHT] > 0.00001f;

    if (Corners.Rounded.topLeft)     {Corners.Rounded.topCount++; Corners.Rounded.leftCount++;}
    if (Corners.Rounded.topRight)    {Corners.Rounded.topCount++; Corners.Rounded.rightCount++;}
    if (Corners.Rounded.bottomLeft)  {Corners.Rounded.bottomCount++; Corners.Rounded.leftCount++;}
    if (Corners.Rounded.bottomRight) {Corners.Rounded.bottomCount++; Corners.Rounded.rightCount++;}

    Corners.TopLeft.TopLeft       = EnclosingBox.TopLeft; 
    Corners.TopLeft.BottomRight.x = Corners.Rounded.topLeft ? EnclosingBox.TopLeft.x + radii_p[TOP_LEFT] : EnclosingBox.TopLeft.x; 
    Corners.TopLeft.BottomRight.y = Corners.Rounded.topLeft ? EnclosingBox.TopLeft.y + radii_p[TOP_LEFT] : EnclosingBox.TopLeft.y; 

    Corners.TopRight.TopLeft.x     = Corners.Rounded.topRight ? EnclosingBox.BottomRight.x - radii_p[TOP_RIGHT] : EnclosingBox.BottomRight.x; 
    Corners.TopRight.BottomRight.x = EnclosingBox.BottomRight.x; 
    Corners.TopRight.TopLeft.y     = EnclosingBox.TopLeft.y; 
    Corners.TopRight.BottomRight.y = Corners.Rounded.topRight ? EnclosingBox.TopLeft.y + radii_p[TOP_RIGHT] : EnclosingBox.TopLeft.y; 

    Corners.BottomLeft.TopLeft.x     = EnclosingBox.TopLeft.x; 
    Corners.BottomLeft.BottomRight.x = Corners.Rounded.bottomLeft ? EnclosingBox.TopLeft.x + radii_p[BOTTOM_LEFT] : EnclosingBox.TopLeft.x; 
    Corners.BottomLeft.TopLeft.y     = Corners.Rounded.bottomLeft ? EnclosingBox.BottomRight.y - radii_p[BOTTOM_LEFT] : EnclosingBox.BottomRight.y; 
    Corners.BottomLeft.BottomRight.y = EnclosingBox.BottomRight.y; 

    Corners.BottomRight.BottomRight = EnclosingBox.BottomRight; 
    Corners.BottomRight.TopLeft.x   = Corners.Rounded.bottomRight ? EnclosingBox.BottomRight.x - radii_p[BOTTOM_RIGHT] : EnclosingBox.BottomRight.x; 
    Corners.BottomRight.TopLeft.y   = Corners.Rounded.bottomRight ? EnclosingBox.BottomRight.y - radii_p[BOTTOM_RIGHT] : EnclosingBox.BottomRight.y; 

NH_END(Corners)
}

