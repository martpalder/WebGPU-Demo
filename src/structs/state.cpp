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
	
	// Describe the Color Target State
	colorTarget.nextInChain = nullptr;
	colorTarget.format = WGPUTextureFormat_BGRA8Unorm;
	colorTarget.blend = &blendState;
	colorTarget.writeMask = WGPUColorWriteMask_All; // We could write to only some of the color channels.
	
	return colorTarget;
}

WGPUVertexState createVertexState(const WGPUShaderModule& shaderMod)
{
	WGPUVertexState vertexState = {};
	
	// Create a pipeline layout to define the input structure.
	vertexState.nextInChain = nullptr;
	vertexState.bufferCount = 0;
	vertexState.buffers = nullptr;
	
	// Set Up Shader Stages:
    // Specify the vertex and fragment shader stages.
	// NB: We define the 'shaderModule' in the second part of this chapter.
	// Here we tell that the programmable vertex shader stage is described
	// by the function called 'vs_main' in that module.
	vertexState.module = shaderMod;
	vertexState.entryPoint = "vs_main";
	vertexState.constantCount = 0;
	vertexState.constants = nullptr;
	
	return vertexState;
}

WGPUFragmentState createFragmentState(const WGPUShaderModule& shaderMod,
const WGPUColorTargetState& colorTarget)
{
	WGPUFragmentState fragmentState = {};
	
	// We tell that the programmable fragment shader stage is described
	// by the function called 'fs_main' in the shader module.
	fragmentState.nextInChain = nullptr;
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

WGPUDepthStencilState createDepthStencilState()
{
	WGPUDepthStencilState depthStencilState = {};
	
	WGPUStencilFaceState stencilFrontState = {};
	WGPUStencilFaceState stencilBackState = {};
	
	// Define the Stencil Front State
	stencilFrontState.compare = WGPUCompareFunction_Less;
	stencilFrontState.failOp = WGPUStencilOperation_Zero;
	stencilFrontState.depthFailOp = WGPUStencilOperation_Zero;
	stencilFrontState.passOp = WGPUStencilOperation_Zero;
	// Define the Stencil Back State
	stencilBackState.compare = WGPUCompareFunction_Less;
	stencilBackState.failOp = WGPUStencilOperation_Zero;
	stencilBackState.depthFailOp = WGPUStencilOperation_Zero;
	stencilBackState.passOp = WGPUStencilOperation_Zero;
	
	// Define the Depth Stencil State
	// Depth
	depthStencilState.nextInChain = nullptr;
	depthStencilState.format = WGPUTextureFormat_Depth24Plus;
	depthStencilState.depthWriteEnabled = true;
	depthStencilState.depthCompare = WGPUCompareFunction_Less;
	// Stencil
	depthStencilState.stencilFront = stencilFrontState;
	depthStencilState.stencilBack = stencilBackState;
	depthStencilState.stencilReadMask = 0;
	depthStencilState.stencilWriteMask = 0;
	// Depth Bias
	depthStencilState.depthBias = 0;
	depthStencilState.depthBiasSlopeScale = 0.0f;
	depthStencilState.depthBiasClamp = 0.0f;
	
	return depthStencilState;
}

States createStates(const WGPUShaderModule& shaderMod)
{
	States states = {};
	
    // Create Blend State and Color Target State
	states.blend = createBlendState();
	states.colorTarget = createColorTargetState(states.blend);
	// Create Vertex and Fragment State
	states.vertex = createVertexState(shaderMod);
	states.fragment = createFragmentState(shaderMod, states.colorTarget);
	states.depthStencil = {};
	// Create the Depth Stencil State
	states.depthStencil = createDepthStencilState();
	
	return states;
}
