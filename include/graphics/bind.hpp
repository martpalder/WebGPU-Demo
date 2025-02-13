#pragma once
#ifndef BIND_HPP_INCLUDED
#define BIND_HPP_INCLUDED

#include <webgpu/webgpu.h>

typedef struct {
	// Binding
	WGPUBindGroupEntry binding;
	// Layouts
	WGPUBindGroupLayoutEntry bindingLayout;
	WGPUBindGroupLayout bindGroupLayout;
	// Binding Group
	WGPUBindGroup bindGroup;
} Bind;

WGPUBindGroupEntry createBinding(size_t bufferSz, const WGPUBuffer& buffer);
WGPUBindGroup createBindGroup(const WGPUDevice& device,
const WGPUBindGroupLayout& bindGroupLayout, WGPUBindGroupEntry* pBinding);

#endif	// BIND_HPP_INCLUDED
