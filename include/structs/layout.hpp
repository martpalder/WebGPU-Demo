#pragma once
#ifndef LAYOUT_HPP_INCLUDED
#define LAYOUT_HPP_INCLUDED

#include "./stdafx.h"

typedef struct {
	WGPUBindGroupLayoutEntry bindingLayout;
	WGPUBindGroupLayout bindGroupLayout;
	WGPUPipelineLayout pipelineLayout;
} Layouts;

// Buffer
WGPUVertexBufferLayout createLayoutBufferVert(size_t vertexSz, size_t attribCount,
WGPUVertexAttribute* pAttribs);
// Bind
WGPUBindGroupLayoutEntry createLayoutBinding(size_t minBindingSize);
WGPUBindGroupLayout createLayoutBindGroup(const WGPUDevice& device,
WGPUBindGroupLayoutEntry* pBindingLayout);
// Pipeline
WGPUPipelineLayout createLayoutPipeline(const WGPUDevice& device,
WGPUBindGroupLayout* pBindGroupLayout);

#endif	// LAYOUT_HPP_INCLUDED
