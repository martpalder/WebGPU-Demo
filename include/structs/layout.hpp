#pragma once
#ifndef LAYOUT_HPP_INCLUDED
#define LAYOUT_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUVertexAttribute createAttribVert(int idx);
WGPUVertexBufferLayout createLayoutBufferVert(int numFloats, const WGPUVertexAttribute* pPosAttrib);
WGPUBindGroupLayoutEntry createLayoutBinding();
WGPUBindGroupLayout createLayoutBindGroup(const WGPUDevice& device,
WGPUBindGroupLayoutEntry* pBindingLayout);
WGPUPipelineLayout createLayoutPipeline(const WGPUDevice& device, WGPUBindGroupLayout* pBindGroupLayout);

#endif	// LAYOUT_HPP_INCLUDED
