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

#endif  // VERTEX_HPP_INCLUDED
