#pragma once
#ifndef BUFFER_HPP_INCLUDED
#define BUFFER_HPP_INCLUDED

#include <webgpu/webgpu.h>
#include <linmath.h>

WGPUBuffer createBufferVert(const WGPUDevice& device, const WGPUQueue& queue,
size_t dataSize, const float* vertexData);
WGPUBuffer createBufferMatrix(const WGPUDevice& device, const WGPUQueue& queue, const mat4x4& matrix);

#endif	// BUFFER_HPP_INCLUDED

