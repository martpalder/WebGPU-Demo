#pragma once
#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include <webgpu/webgpu.h>

typedef struct {
	WGPUVertexState vertex;
	WGPUFragmentState fragment;
} States;

WGPUBlendState createBlendState();
WGPUColorTargetState createColorTargetState(const WGPUBlendState& blendState);

WGPUVertexState createVertexState(const WGPUShaderModule& shaderMod);
WGPUFragmentState createFragmentState(const WGPUShaderModule& shaderMod,
const WGPUColorTargetState& colorTarget);

#endif	// STATE_HPP_INCLUDED
