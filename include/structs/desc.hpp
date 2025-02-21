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
size_t bindingCount, WGPUBindGroupEntry* pBindings, const char* label);

// Render Pass
WGPURenderPassDescriptor createRenderPassDesc(const Attachments& attach,
const WGPUPipelineLayout& pipelineLayout, bool bDepthStencil);

// Render Pipeline
WGPURenderPipelineDescriptor createRenderPipelineDesc(const States& states,
const WGPUPipelineLayout& pipelineLayout, bool bDepthStencil);

// Descriptors
Descriptors createDescriptors(const Attachments& attach, bool bDepthStencil);

#endif	// DESC_HPP_INCLUDED
