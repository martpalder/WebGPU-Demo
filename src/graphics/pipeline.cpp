#include "./pipeline.hpp"
#include "./state.hpp"
#include "./desc.hpp"
#include "./attrib.hpp"
#include "./layout.hpp"
#include "./myassert.hpp"
#include "./stdafx.h"

WGPURenderPipeline createRenderPipeline(const GPUEnv& gpuEnv,
const States& states, WGPUBindGroupLayout* pBindGroupLayout)
{
    WGPURenderPipeline pipeline;
	
	// VERTEX FETCH
	// Create Vertex Attributes
	WGPUVertexAttribute attribs[] = {
		createAttribVertFloat(2, 0, 0, "pos"),
		//createAttribVertFloat(3, 1, 3 * sizeof(float), "color"),
	};
	// Set Vertex Size
	float numFloats = 2;
	size_t vertexSz = numFloats * sizeof(float);
	
	// Create a Render Pipeline Descriptor
	WGPURenderPipelineDescriptor pipelineDesc = {};
	pipelineDesc = createRenderPipelineDesc(states, false);
	
	// Create and set the Buffer Layout
	WGPUVertexBufferLayout bufferLayout = {};
	bufferLayout = createLayoutBufferVert(vertexSz, 1, &attribs[0]);
	pipelineDesc.vertex.bufferCount = 1;
	pipelineDesc.vertex.buffers = &bufferLayout;
	
	// PIPELINE LAYOUT
	// Assign the PipelineLayout to the RenderPipelineDescriptor's layout field
	pipelineDesc.layout = createLayoutPipeline(gpuEnv.dev, pBindGroupLayout);
	puts("Assigned the PipelineLayout to the RenderPipelineDescriptor");

	// Create the Render Pipeline:
    // Use the device to create the render pipeline.
	pushError(gpuEnv.dev);
	pipeline = wgpuDeviceCreateRenderPipeline(gpuEnv.dev, &pipelineDesc);
	popError(gpuEnv.dev);
	
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
