#include "./state.hpp"

WGPUBlendState createBlendState()
{
	WGPUBlendState blendState = {};
	
	// Configure the Blend State
	blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
	blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
	blendState.color.operation = WGPUBlendOperation_Add;
	blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
	blendState.alpha.dstFactor = WGPUBlendFactor_One;
	blendState.alpha.operation = WGPUBlendOperation_Add;
	
	return blendState;
}

WGPUColorTargetState createColorTargetState(const WGPUBlendState& blendState)
{
	WGPUColorTargetState colorTarget = {};
	
	colorTarget.format = WGPUTextureFormat_BGRA8Unorm;
	colorTarget.blend = &blendState;
	colorTarget.writeMask = WGPUColorWriteMask_All; // We could write to only some of the color channels.
	
	return colorTarget;
}

WGPUFragmentState createFragmentState(const WGPUShaderModule& shaderMod, const WGPUColorTargetState& colorTarget)
{
	WGPUFragmentState fragmentState = {};
	
	// We tell that the programmable fragment shader stage is described
	// by the function called 'fs_main' in the shader module.
	fragmentState.module = shaderMod;
	fragmentState.entryPoint = "fs_main";
	fragmentState.constantCount = 0;
	fragmentState.constants = nullptr;
	// We have only one target because our render pass has only one output color
	// attachment.
	fragmentState.targetCount = 1;
	fragmentState.targets = &colorTarget;
	
	return fragmentState;
}
