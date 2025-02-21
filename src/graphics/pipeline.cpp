#include "./pipeline.hpp"
#include "./state.hpp"
#include "./desc.hpp"
#include "./attrib.hpp"
#include "./layout.hpp"
#include "./myassert.hpp"
#include "./stdafx.h"

WGPURenderPipeline createRenderPipeline(const WGPUDevice& device,
WGPURenderPipelineDescriptor& pipelineDesc)
{
    WGPURenderPipeline pipeline;
	
	// VERTEX FETCH
	// Create the Vertex Attributes
	WGPUVertexAttribute* pAttribs = attrib3DPosColor();
	// Calculate the Vertex Size
	int numFloats = 6;
	size_t vertexSz = numFloats * sizeof(float);;
	
	// Create and set the Buffer Layout
	WGPUVertexBufferLayout bufferLayout = {};
	bufferLayout = createLayoutBufferVert(vertexSz, 2, pAttribs); // Number of Attributes
	pipelineDesc.vertex.bufferCount = 1;
	pipelineDesc.vertex.buffers = &bufferLayout;
	pAttribs = nullptr;
	
	// Create the Render Pipeline:
    // Use the device to create the render pipeline.
	pushError(device);
	pipeline = wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);
	popError(device);
	
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
