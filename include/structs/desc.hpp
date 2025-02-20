#pragma once
#ifndef DESC_HPP_INCLUDED
#define DESC_HPP_INCLUDED

#include "./state.hpp"
#include "./attach.hpp"

#include <webgpu/webgpu.h>

typedef struct {
	WGPUCommandEncoderDescriptor encoderDesc;
	WGPURenderPassDescriptor renderPassDesc;
	WGPUCommandBufferDescriptor cmdBufferDesc;
} Descriptors;

// Device
WGPUDeviceDescriptor createDeviceDesc();

// Encoder
WGPUCommandEncoderDescriptor createEncoderDesc();
WGPUCommandBufferDescriptor createCmdBufferDesc();

// Shader
WGPUShaderModuleWGSLDescriptor createShaderCodeDesc(const char* shaderCode);
WGPUShaderModuleDescriptor createShaderModDesc(WGPUChainedStruct* shaderCodeChain);

// Bind Group
WGPUBindGroupDescriptor createBindGroupDesc(const WGPUBindGroupLayout& bindGroupLayout,
size_t bindingCount, WGPUBindGroupEntry* pBindings);

// Render Pass
WGPURenderPassDescriptor createRenderPassDesc(WGPURenderPassColorAttachment* pColorAttach,
WGPURenderPassDepthStencilAttachment* pDepthStencilAttach);

// Render Pipeline
WGPURenderPipelineDescriptor createRenderPipelineDesc(const States& states, bool bDepthStencil);

// Descriptors
Descriptors createDescriptors(WGPURenderPassColorAttachment* pColorAttach,
WGPURenderPassDepthStencilAttachment* pDepthStencilAttach);

#endif	// DESC_HPP_INCLUDED
