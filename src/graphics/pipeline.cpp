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
	// Create the Vertex Attributes
	WGPUVertexAttribute* pAttribs = attrib3DPosColor();
	// Calculate the Vertex Size
	int numFloats = 6;
	size_t vertexSz = numFloats * sizeof(float);
	
	// Create a Render Pipeline Descriptor
	WGPURenderPipelineDescriptor pipelineDesc = {};
	pipelineDesc = createRenderPipelineDesc(states, false);
	
	// Create and set the Buffer Layout
	WGPUVertexBufferLayout bufferLayout = {};
	bufferLayout = createLayoutBufferVert(vertexSz, 2, pAttribs); // Number of Attributes
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
	
	// Release the Attributes
	delete[] pAttribs;
	pAttribs = nullptr;
	
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
