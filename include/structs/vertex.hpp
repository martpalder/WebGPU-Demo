#pragma once
#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED

#include "./stdafx.h"

typedef struct {
    vec2 pos;
} Vertex2D_Pos;

typedef struct {
    vec2 pos;
    vec3 color;
} Vertex2D_PosColor;

typedef struct {
    vec3 pos;
} Vertex3D_Pos;

typedef struct {
    vec3 pos;
    vec3 color;
} Vertex3D_PosColor;

#define CUSTOM_VERTEX Vertex3D_PosColor
#define VERTEX_SZ sizeof(CUSTOM_VERTEX)

inline void printVec2(const vec2& vec)
{
	printf("X: %f\tY: %f\n", vec[0], vec[1]);
}

inline void printVec3(const vec3& vec)
{
	printf("X: %f\tY: %f\tZ: %f\n", vec[0], vec[1], vec[2]);
}

#endif  // VERTEX_HPP_INCLUDED
