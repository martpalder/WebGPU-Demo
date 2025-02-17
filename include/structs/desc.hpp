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
WGPURenderPassDescriptor createRenderPassDesc(const Attachments& attachments,
bool bDepthStencilAttach);

// Render Pipeline
WGPURenderPipelineDescriptor createRenderPipelineDesc(const WGPUShaderModule& shaderMod,
const States& states);

// Descriptors
Descriptors createDescriptors(const Attachments& attachments,
bool bDepthStencilAttach);

#endif	// DESC_HPP_INCLUDED
