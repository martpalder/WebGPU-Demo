#include "./desc.hpp"
#include "./callback.hpp"
#include "./stdafx.h"

WGPUDeviceDescriptor createDeviceDesc()
{
	WGPUDeviceDescriptor deviceDesc = {};
	
	deviceDesc.nextInChain = nullptr;
	deviceDesc.label = "MyDevice"; // anything works here, that's your call
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
	puts("Created a Device Descriptor");
	
	return deviceDesc;
}

WGPUCommandEncoderDescriptor createEncoderDesc()
{
	WGPUCommandEncoderDescriptor encoderDesc = {};
	
	encoderDesc.nextInChain = nullptr;
	encoderDesc.label = "CommandEncoder";
	
	return encoderDesc;
}

WGPURenderPassDescriptor createRenderPassDesc(const Attachments& attach, bool bDepthStencil)
{
	WGPURenderPassDescriptor renderPassDesc = {};
	
	renderPassDesc.nextInChain = nullptr;
	renderPassDesc.colorAttachmentCount = 1;
	renderPassDesc.colorAttachments = &attach.colorAttach;
	renderPassDesc.depthStencilAttachment = nullptr;
	if (bDepthStencil)
	{
		renderPassDesc.depthStencilAttachment = &attach.depthStencilAttach;
		puts("Set the Depth Stencil Attachment");
	}
	renderPassDesc.timestampWrites = nullptr;
	puts("Created a Render Pass Descriptor");
	
	return renderPassDesc;
}

WGPUCommandBufferDescriptor createCmdBufferDesc()
{
	WGPUCommandBufferDescriptor cmdBufferDesc = {};
	
	cmdBufferDesc.nextInChain = nullptr;
	cmdBufferDesc.label = "CommandBuffer";
	
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

WGPUShaderModuleDescriptor createShaderModDesc(WGPUChainedStruct* shaderCodeChain)
{
	WGPUShaderModuleDescriptor shaderModDesc = {};
	
	// Connect the chain
	shaderModDesc.nextInChain = shaderCodeChain;
	// Other Properties
	shaderModDesc.label = "Shader";
	#ifdef WEBGPU_BACKEND_WGPU
	shaderModDesc.hintCount = 0;
	shaderModDesc.hints = nullptr;
	#endif
	puts("Created a Shader Module Descriptor");

	return shaderModDesc;
}

WGPUBindGroupDescriptor createBindGroupDesc(const WGPUBindGroupLayout& bindGroupLayout,
size_t bindingCount, WGPUBindGroupEntry* pBindings, const char* label)
{
	WGPUBindGroupDescriptor bindGroupDesc = {};
	
	// Define the Bind Group
	bindGroupDesc.nextInChain = nullptr;
	bindGroupDesc.label = label;
	bindGroupDesc.layout = bindGroupLayout;
	// There must be as many bindings as declared in the layout!
	bindGroupDesc.entryCount = bindingCount;
	bindGroupDesc.entries = pBindings;
	printf("Created a Bind Group Descriptor: '%s'\n", label);
	
	return bindGroupDesc;
}

WGPURenderPipelineDescriptor createRenderPipelineDesc(const States& states,
const WGPUPipelineLayout& pipelineLayout, bool bDepthStencil)
{
	WGPURenderPipelineDescriptor pipelineDesc = {};
	
	// Define the Pipeline Layout
	pipelineDesc.nextInChain = nullptr;
	pipelineDesc.label = "RenderPipeline";
	pipelineDesc.layout = pipelineLayout;
	
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
	pipelineDesc.depthStencil = nullptr;
	if (bDepthStencil)
	{
		// Set the Depth Stencil State
		pipelineDesc.depthStencil = &states.depthStencil;
	}
	
	// Samples per pixel
	pipelineDesc.multisample.count = 1;
	// Default value for the mask, meaning "all bits on"
	pipelineDesc.multisample.mask = ~0u;
	// Default value as well (irrelevant for count = 1 anyways)
	pipelineDesc.multisample.alphaToCoverageEnabled = false;
	puts("Created a Render Pipeline Descriptor");
	
	return pipelineDesc;
}

Descriptors createDescriptors(const Attachments& attach, bool bDepthStencil)
{
	Descriptors descriptors = {};
	
	// DESCRIPTORS
	// Command Encoder Descriptor
	descriptors.encoderDesc = createEncoderDesc();
	// Render Pass Descriptor
	descriptors.renderPassDesc = createRenderPassDesc(attach, bDepthStencil);
	// Command Buffer Descriptor
	descriptors.cmdBufferDesc = createCmdBufferDesc();
	puts("Created the Descriptors");
	
	return descriptors;
}
