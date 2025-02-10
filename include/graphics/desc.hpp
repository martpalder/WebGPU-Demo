#pragma once
#ifndef DESC_HPP_INCLUDED
#define DESC_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUDeviceDescriptor createDeviceDesc();
WGPUCommandEncoderDescriptor createEncoderDesc();
WGPURenderPassDescriptor createRenderPassDesc(const WGPURenderPassColorAttachment& renderPassColorAttach);

#endif	// DESC_HPP_INCLUDED
