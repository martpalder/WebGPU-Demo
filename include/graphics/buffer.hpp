#pragma once
#ifndef BUFFER_HPP_INCLUDED
#define BUFFER_HPP_INCLUDED

#include "./gpuenv.hpp"
#include "./vertex.hpp"

#include <webgpu/webgpu.h>
#include <linmath.h>

WGPUBuffer createBufferVert(const GPUEnv& gpuEnv,
size_t dataSize, const void* pVertexData);
WGPUBuffer createBufferIdx(const GPUEnv& gpuEnv,
size_t dataSize, const void* pIndexData);
WGPUBuffer createBufferMatrix(const GPUEnv& gpuEnv, const mat4x4& matrix);

#endif	// BUFFER_HPP_INCLUDED

