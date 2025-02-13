#pragma once
#ifndef BUFFER_HPP_INCLUDED
#define BUFFER_HPP_INCLUDED

#include <webgpu/webgpu.h>
#include <linmath/linmath.h>

WGPUBuffer createBufferVert(const WGPUDevice& device, const WGPUQueue& queue,
size_t dataSize, const float* vertexData);
WGPUBuffer createBufferMat4x4(const WGPUDevice& device, const WGPUQueue& queue, const mat4x4& mat);

#endif	// BUFFER_HPP_INCLUDED

