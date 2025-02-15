#pragma once
#ifndef VERTEX_HPP_INCLUDED
#define VERTEX_HPP_INCLUDED

#include <linmath.h>

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

#endif  // VERTEX_HPP_INCLUDED
