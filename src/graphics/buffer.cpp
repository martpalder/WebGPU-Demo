#include "./buffer.hpp"
#include "./callback.hpp"

#include <cstdio>

// Vertex buffer data
// There are 2 floats per vertex, one for x and one for y.
// But in the end this is just a bunch of floats to the eyes of the GPU,
// the *layout* will tell how to interpret this.
const static float vertexData[] = {
    // x0, y0
    -0.5f, -0.5f,

    // x1, y1
    +0.5f, -0.5f,

    // x2, y2
    +0.0f, +0.5f
};

// We will declare vertexCount as a member of the Application class
uint32_t vertexCount = static_cast<uint32_t>(sizeof(vertexData) / sizeof(float));

WGPUBuffer createBufferVert(const WGPUDevice& device, const WGPUQueue& queue)
{
	WGPUBuffer vertexBuffer;
	
	// Describe Vertex Buffer
	WGPUBufferDescriptor bufferDesc{};
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = "VertexBuffer";
	bufferDesc.size = sizeof(vertexData);
	bufferDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex; // Vertex usage here!
	bufferDesc.mappedAtCreation = false;
	
	// Create a Vertex Buffer
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	vertexBuffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Check for Errors
	if (vertexBuffer == nullptr)
	{
		perror("[ERROR]: Failed to create a Vertex Buffer");
		
		return nullptr;
	}
	else
	{
		puts("Created a Vertex Buffer");
	}

	// Upload the Geometry Data to the Buffer
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	wgpuQueueWriteBuffer(queue, vertexBuffer, 0, vertexData, bufferDesc.size);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);

	return vertexBuffer;
}
