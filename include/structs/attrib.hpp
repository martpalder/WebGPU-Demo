#pragma once
#ifndef ATTRIB_HPP_INCLUDED
#define ATTRIB_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUVertexAttribute createAttribVertFloat(int numFloats, int idx, int offset, const char* label);

#endif
