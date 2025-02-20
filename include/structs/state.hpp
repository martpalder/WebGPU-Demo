#pragma once
#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include <webgpu/webgpu.h>

typedef struct {
	WGPUBlendState blend;
	WGPUColorTargetState colorTarget;
	WGPUVertexState vertex;
	WGPUFragmentState fragment;
	WGPUDepthStencilState depthStencil;
} States;

WGPUBlendState createBlendState();
WGPUColorTargetState createColorTargetState(const WGPUBlendState& blendState);
WGPUVertexState createVertexState(const WGPUShaderModule& shaderMod);

WGPUFragmentState createFragmentState(const WGPUShaderModule& shaderMod,
const WGPUColorTargetState& colorTarget);

WGPUDepthStencilState createDepthStencilState(WGPUTextureFormat format);

States createStates(const WGPUShaderModule& shaderMod, WGPUTextureFormat depthFormat);

#endif	// STATE_HPP_INCLUDED
