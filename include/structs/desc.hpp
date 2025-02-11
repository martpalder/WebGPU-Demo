#pragma once
#ifndef DESC_HPP_INCLUDED
#define DESC_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUDeviceDescriptor createDeviceDesc();
WGPUCommandEncoderDescriptor createEncoderDesc();
WGPURenderPassDescriptor createRenderPassDesc(const WGPURenderPassColorAttachment& renderPassColorAttach);
WGPUCommandBufferDescriptor createCmdBufferDesc();
WGPUShaderModuleWGSLDescriptor createShaderCodeDesc(const char* shaderCode);
WGPUShaderModuleDescriptor createShaderModDesc(const WGPUChainedStruct* shaderCodeChain);

#endif	// DESC_HPP_INCLUDED
