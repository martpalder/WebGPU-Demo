#include "./desc.hpp"
#include "./callback.hpp"

WGPUDeviceDescriptor createDeviceDesc()
{
	WGPUDeviceDescriptor deviceDesc = {};
	
	deviceDesc.nextInChain = nullptr;
	deviceDesc.label = "My Device"; // anything works here, that's your call
	deviceDesc.requiredFeatureCount = 0; // we do not require any specific feature
	deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
	deviceDesc.defaultQueue.nextInChain = nullptr;
	deviceDesc.defaultQueue.label = "The default queue";
	// {{Set device lost callback}}
	deviceDesc.deviceLostCallback = deviceLostCallback;
	// {{Set uncaptured error callback}}
	WGPUUncapturedErrorCallbackInfo callbackInfo = {};
	callbackInfo.nextInChain = nullptr;
	callbackInfo.callback = onDeviceError;
	deviceDesc.uncapturedErrorCallbackInfo = callbackInfo;
	
	return deviceDesc;
}

WGPUCommandEncoderDescriptor createEncoderDesc()
{
	WGPUCommandEncoderDescriptor encoderDesc = {};
	
	encoderDesc.nextInChain = nullptr;
	encoderDesc.label = "My command encoder";
	
	return encoderDesc;
}

WGPURenderPassDescriptor createRenderPassDesc(const WGPURenderPassColorAttachment& renderPassColorAttach)
{
	WGPURenderPassDescriptor renderPassDesc = {};
	
	renderPassDesc.nextInChain = nullptr;
	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &renderPassColorAttach;
	renderPassDesc.depthStencilAttachment = nullptr;
	renderPassDesc.timestampWrites = nullptr;
	
	return renderPassDesc;
}

WGPUCommandBufferDescriptor createCmdBufferDesc()
{
	WGPUCommandBufferDescriptor cmdBufferDesc = {};
	
	cmdBufferDesc.nextInChain = nullptr;
	cmdBufferDesc.label = "Command buffer";
	
	return cmdBufferDesc;
}

WGPUShaderModuleWGSLDescriptor createShaderCodeDesc(const char* shaderCode)
{
	WGPUShaderModuleWGSLDescriptor shaderCodeDesc = {};

	// Set the chained struct's header
	shaderCodeDesc.chain.next = nullptr;
	shaderCodeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
	// Set the Shader Code
	shaderCodeDesc.code = shaderCode;

	return shaderCodeDesc;
}

WGPUShaderModuleDescriptor createShaderModDesc(const WGPUChainedStruct* shaderCodeChain)
{
	WGPUShaderModuleDescriptor shaderModDesc = {};

	shaderModDesc.label = "Shader";
	#ifdef WEBGPU_BACKEND_WGPU
	shaderModDesc.hintCount = 0;
	shaderModDesc.hints = nullptr;
	#endif

	// Connect the chain
	shaderModDesc.nextInChain = shaderCodeChain;

	return shaderModDesc;
}

WGPUBindGroupDescriptor createBindGroupDesc(const WGPUBindGroupLayout& bindGroupLayout,
const WGPUBindGroupEntry& binding)
{
	WGPUBindGroupDescriptor bindGroupDesc = {};
	
	// Define the Bind Group
	bindGroupDesc.nextInChain = nullptr;
	bindGroupDesc.layout = bindGroupLayout;
	// There must be as many bindings as declared in the layout!
	bindGroupDesc.entryCount = 1;
	bindGroupDesc.entries = &binding;
	
	return bindGroupDesc;
}

WGPURenderPipelineDescriptor createRenderPipelineDesc(const WGPUShaderModule& shaderMod, const WGPUFragmentState& fragmentState)
{
	WGPURenderPipelineDescriptor pipelineDesc{};
	
	// Define the Pipeline Layout
	pipelineDesc.nextInChain = nullptr;
    // Create a pipeline layout to define the input structure.
	// We do not use any vertex buffer for this first simplistic example
	pipelineDesc.vertex.bufferCount = 0;
	pipelineDesc.vertex.buffers = nullptr;
	
	// Set Up Shader Stages:
    // Specify the vertex and fragment shader stages.
	// NB: We define the 'shaderModule' in the second part of this chapter.
	// Here we tell that the programmable vertex shader stage is described
	// by the function called 'vs_main' in that module.
	pipelineDesc.vertex.module = shaderMod;
	pipelineDesc.vertex.entryPoint = "vs_main";
	pipelineDesc.vertex.constantCount = 0;
	pipelineDesc.vertex.constants = nullptr;
	
	// Each sequence of 3 vertices is considered as a triangle
	pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
	// We'll see later how to specify the order in which vertices should be
	// connected. When not specified, vertices are considered sequentially.
	pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
	// The face orientation is defined by assuming that when looking
	// from the front of the face, its corner vertices are enumerated
	// in the counter-clockwise (CCW) order.
	pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
	// But the face orientation does not matter much because we do not
	// cull (i.e. "hide") the faces pointing away from us (which is often
	// used for optimization).
	pipelineDesc.primitive.cullMode = WGPUCullMode_None;
	
	// Set the Fragment State
	pipelineDesc.fragment = &fragmentState;
	// We do not use stencil/depth testing for now
	pipelineDesc.depthStencil = nullptr;
	
	// Samples per pixel
	pipelineDesc.multisample.count = 1;
	// Default value for the mask, meaning "all bits on"
	pipelineDesc.multisample.mask = ~0u;
	// Default value as well (irrelevant for count = 1 anyways)
	pipelineDesc.multisample.alphaToCoverageEnabled = false;
	pipelineDesc.layout = nullptr;
	
	return pipelineDesc;
}
