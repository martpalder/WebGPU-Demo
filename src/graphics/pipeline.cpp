#include "./pipeline.hpp"
#include "./desc.hpp"
#include "./state.hpp"
#include "./layout.hpp"
#include "./callback.hpp"

#include <cstdio>

WGPURenderPipeline createRenderPipeline(WGPUDevice device, const WGPUShaderModule& shaderMod)
{
    WGPURenderPipeline pipeline;

	// Configure Pipeline States:
    // Configure vertex input, rasterization, and color blending states.
	WGPUBlendState blendState = createBlendState();
	WGPUColorTargetState colorTarget = createColorTargetState(blendState);
	
	// Create and set the Fragment State
	WGPUFragmentState fragmentState = createFragmentState(shaderMod, colorTarget);
	
	// Create a Render Pipeline Descriptor
    WGPURenderPipelineDescriptor pipelineDesc = createRenderPipelineDesc(shaderMod, fragmentState);
	
	// VERTEX FETCH
	// Create Position Attribute
	WGPUVertexAttribute posAttrib = createAttribVert(0);
	// Create and set the Buffer Layout
	WGPUVertexBufferLayout bufferLayout = createBufferLayoutVert(&posAttrib);
	pipelineDesc.vertex.bufferCount = 1;
	pipelineDesc.vertex.buffers = &bufferLayout;

	// Create Render Pipeline:
    // Use the device to create the render pipeline.
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	pipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	puts("Created the Render Pipeline");

	// Use Pipeline in Command Buffers:
    // Integrate the pipeline in command buffers to execute rendering commands.
	
    return pipeline;
}
