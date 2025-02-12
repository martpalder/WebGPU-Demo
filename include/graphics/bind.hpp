#pragma once
#ifndef BIND_HPP_INCLUDED
#define BIND_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUBindGroupEntry createBinding(size_t bufferSz, const WGPUBuffer& buffer);
WGPUBindGroup createBindGroup(const WGPUDevice& device,
size_t bufferSz, const WGPUBuffer& buffer);

#endif	// BIND_HPP_INCLUDED
