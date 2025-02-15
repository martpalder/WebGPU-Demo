#pragma once
#ifndef DESC_HPP_INCLUDED
#define DESC_HPP_INCLUDED

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
WGPUShaderModuleDescriptor createShaderModDesc(const WGPUChainedStruct* shaderCodeChain);

// Bind Group
WGPUBindGroupDescriptor createBindGroupDesc(const WGPUBindGroupLayout& bindGroupLayout,
WGPUBindGroupEntry* pBinding);

// Render Pass
WGPURenderPassDescriptor createRenderPassDesc(const WGPURenderPassColorAttachment& renderPassColorAttach);
WGPURenderPipelineDescriptor createRenderPipelineDesc(const WGPUShaderModule& shaderMod,
const WGPUFragmentState& fragmentState);

// Descriptors
Descriptors createDescriptors(const WGPURenderPassColorAttachment& colorAttach);

#endif	// DESC_HPP_INCLUDED
