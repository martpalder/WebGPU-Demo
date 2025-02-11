#pragma once
#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include <webgpu/webgpu.h>

WGPUBlendState createBlendState();
WGPUColorTargetState createColorTargetState(const WGPUBlendState& blendState);
WGPUFragmentState createFragmentState(const WGPUShaderModule& shaderMod, const WGPUColorTargetState& colorTarget);

#endif	// STATE_HPP_INCLUDED
