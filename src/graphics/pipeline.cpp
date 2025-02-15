#include "./pipeline.hpp"
#include "./desc.hpp"
#include "./attrib.hpp"
#include "./layout.hpp"
#include "./state.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <cstdio>

WGPURenderPipeline createRenderPipeline(const WGPUDevice& device, const WGPUShaderModule& shaderMod,
WGPUBindGroupLayout* pBindGroupLayout)
{
    WGPURenderPipeline pipeline;
    
    // Configure Pipeline States:
    States states;
    // Create Blend State and Color Target State
	WGPUBlendState blendState = createBlendState();
	WGPUColorTargetState colorTarget = createColorTargetState(blendState);
	// Create Vertex and Fragment State
	states.vertex = createVertexState(shaderMod);
	states.fragment = createFragmentState(shaderMod, colorTarget);
    
	// Create a Render Pipeline Descriptor
    WGPURenderPipelineDescriptor pipelineDesc = {};
    pipelineDesc = createRenderPipelineDesc(shaderMod, states);
	
	// VERTEX FETCH
	// Create Vertex Attributes
	WGPUVertexAttribute attribs[] = {
		createAttribVertFloat(3, 0, 0, "position"),
		createAttribVertFloat(3, 1, 3 * sizeof(float), "color"),
	};
	// Set Vertex Size
	size_t vertexSz = 6 * sizeof(float);	// 6 Floats
	
	// Create and set the Buffer Layout
	WGPUVertexBufferLayout bufferLayout = {};
	bufferLayout = createLayoutBufferVert(vertexSz, 2, &attribs[0]);
	pipelineDesc.vertex.bufferCount = 1;
	pipelineDesc.vertex.buffers = &bufferLayout;
	
	// PIPELINE LAYOUT
	// Assign the PipelineLayout to the RenderPipelineDescriptor's layout field
	pipelineDesc.layout = createLayoutPipeline(device, pBindGroupLayout);
	puts("Assigned the PipelineLayout to the RenderPipelineDescriptor");

	// Create the Render Pipeline:
    // Use the device to create the render pipeline.
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	pipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Chek for Errors
	if (pipeline == nullptr)
	{
		perror("[ERROR]: Failed to create the Render Pipeline");
	}
	{
		puts("Created the Render Pipeline");
	}
	
	// Assert
	MY_ASSERT(pipeline != nullptr);

	// Use Pipeline in Command Buffers:
    // Integrate the pipeline in command buffers to execute rendering commands.
	
    return pipeline;
}
