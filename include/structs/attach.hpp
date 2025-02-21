#pragma once
#ifndef ATTACH_HPP_INCLUDED
#define ATTACH_HPP_INCLUDED

#include <webgpu/webgpu.h>

typedef struct {
	WGPURenderPassColorAttachment colorAttach;
	WGPURenderPassDepthStencilAttachment depthStencilAttach;
} Attachments;

WGPURenderPassColorAttachment createColorAttach(float r, float g, float b);

WGPURenderPassDepthStencilAttachment createDepthStencilAttach(const WGPUTextureView& depthView);

#endif	// ATTACH_HPP_INCLUDED
