#include "./desc.hpp"
#include "./callback.hpp"

#include <cstdio>

WGPUDeviceDescriptor createDeviceDesc()
{
	WGPUDeviceDescriptor deviceDesc = {};
	
	deviceDesc.nextInChain = nullptr;
	deviceDesc.label = "My Device"; // anything works here, that's your call
	deviceDesc.requiredFeatureCount = 0; // we do not require any specific feature
	deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
	deviceDesc.defaultQueue.nextInChain = nullptr;
	deviceDesc.defaultQueue.label = "Queue";
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
	encoderDesc.label = "Command Encoder";
	
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
	cmdBufferDesc.label = "Command Buffer";
	
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
WGPUBindGroupEntry* pBinding)
{
	WGPUBindGroupDescriptor bindGroupDesc = {};
	
	// Define the Bind Group
	bindGroupDesc.nextInChain = nullptr;
	bindGroupDesc.label = "BindGroup";
	bindGroupDesc.layout = bindGroupLayout;
	// There must be as many bindings as declared in the layout!
	bindGroupDesc.entryCount = 1;
	bindGroupDesc.entries = pBinding;
	
	return bindGroupDesc;
}

WGPURenderPipelineDescriptor createRenderPipelineDesc(const WGPUShaderModule& shaderMod,
const States& states)
{
	WGPURenderPipelineDescriptor pipelineDesc{};
	
	// Define the Pipeline Layout
	pipelineDesc.nextInChain = nullptr;
	pipelineDesc.label = "RenderPipeline";
	pipelineDesc.layout = nullptr;
	
	// Set the Vertex State
	pipelineDesc.vertex = states.vertex;
		
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
	pipelineDesc.primitive.cullMode = WGPUCullMode_Back;	// Back-face culling
	
	// Set the Fragment State
	pipelineDesc.fragment = &states.fragment;
	// We do not use stencil/depth testing for now
	pipelineDesc.depthStencil = nullptr;
	
	// Samples per pixel
	pipelineDesc.multisample.count = 1;
	// Default value for the mask, meaning "all bits on"
	pipelineDesc.multisample.mask = ~0u;
	// Default value as well (irrelevant for count = 1 anyways)
	pipelineDesc.multisample.alphaToCoverageEnabled = false;
	
	return pipelineDesc;
}

Descriptors createDescriptors(const WGPURenderPassColorAttachment& colorAttach)
{
	Descriptors descriptors;
	
	// DESCRIPTORS
	// Command Encoder Descriptor
	descriptors.encoderDesc = createEncoderDesc();
	// Render Pass Descriptor
	descriptors.renderPassDesc = createRenderPassDesc(colorAttach);
	// Command Buffer Descriptor
	descriptors.cmdBufferDesc = createCmdBufferDesc();
	puts("Created the Descriptors");
	
	return descriptors;
}
