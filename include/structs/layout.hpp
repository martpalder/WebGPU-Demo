#pragma once
#ifndef LAYOUT_HPP_INCLUDED
#define LAYOUT_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUVertexAttribute createAttribVert(int idx);
WGPUVertexBufferLayout createBufferLayoutVert(const WGPUVertexAttribute* pPosAttrib);

#endif	// LAYOUT_HPP_INCLUDED
