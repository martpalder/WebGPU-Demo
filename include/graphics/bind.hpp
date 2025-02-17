#pragma once
#ifndef BIND_HPP_INCLUDED
#define BIND_HPP_INCLUDED

#include <webgpu/webgpu.h>

typedef struct {
	// Layouts
	WGPUBindGroupLayoutEntry bindingLayout;
	WGPUBindGroupLayout bindGroupLayout;
	// Binding Group
	WGPUBindGroup bindGroup;
} Bind;

WGPUBindGroupEntry createBinding(int idx, const WGPUBuffer& buffer);

WGPUBindGroup createBindGroup(const WGPUDevice& device, const WGPUBindGroupLayout& bindGroupLayout,
size_t bindingCount, WGPUBindGroupEntry* pBindings);

Bind bindBuffer(const WGPUDevice& device, int idx,
WGPUBindGroupEntry* pBindings);

#endif	// BIND_HPP_INCLUDED
