#pragma once
#ifndef BUFFER_HPP_INCLUDED
#define BUFFER_HPP_INCLUDED

#include "./gpuenv.hpp"
#include "./stdafx.h"

void uploadToBuffer(const WGPUQueue& queue, const WGPUBuffer& buffer,
size_t dataSz, const void* data);

WGPUBuffer createBufferVert(const WGPUDevice& device, size_t dataSize);
WGPUBuffer createBufferIdx(const WGPUDevice& device, size_t dataSize);
WGPUBuffer createBufferMatrix(const GPUEnv& gpuEnv, const mat4x4& matrix);

#endif	// BUFFER_HPP_INCLUDED

