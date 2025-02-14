#pragma once
#ifndef BUFFER_HPP_INCLUDED
#define BUFFER_HPP_INCLUDED

#include "./gpuenv.hpp"
#include "./vertex.hpp"

#include <webgpu/webgpu.h>
#include <linmath.h>

WGPUBuffer createBufferVert(const GPUEnv& gpuEnv,
size_t dataSize, const CUSTOM_VERTEX* vertexData);
WGPUBuffer createBufferMatrix(const GPUEnv& gpuEnv, const mat4x4& matrix);

#endif	// BUFFER_HPP_INCLUDED

