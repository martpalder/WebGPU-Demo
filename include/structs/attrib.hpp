#pragma once
#ifndef ATTRIB_HPP_INCLUDED
#define ATTRIB_HPP_INCLUDED

#include "./stdafx.h"

WGPUVertexAttribute createAttribVertFloat(int numFloats, int idx, int offset, const char* label);

// 2D
WGPUVertexAttribute* attrib2DPos();
WGPUVertexAttribute* attrib2DPosColor();
// 3D
WGPUVertexAttribute* attrib3DPos();
WGPUVertexAttribute* attrib3DPosColor();

#endif
