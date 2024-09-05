#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include "QAngle.h"

typedef struct CViewMatrix
{
    float Matrix[16];
};
namespace tool
{
    extern CViewMatrix GetMatrixViewport();
    extern Vector3 ViewportTransform(CViewMatrix m, Vector3 v);
    extern Vector3 AngleToPixels(Vector3 angle, int fov, int width, int height);
    extern void MoveMouse(Vector2 point);
    extern void SetViewAngle(Vector3 angle, int fov, Vector3 sourceAngle);
}