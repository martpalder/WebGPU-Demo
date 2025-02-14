#include "./buffer.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <cstdio>

WGPUBuffer createBufferVert(const WGPUDevice& device, const WGPUQueue& queue,
size_t dataSize, const float* vertexData)
{
	WGPUBuffer vertexBuffer;
	
	// Describe Vertex Buffer
	WGPUBufferDescriptor bufferDesc = {};
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = "VertexBuffer";
	bufferDesc.size = dataSize;
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
	
	// Assert
	MY_ASSERT(vertexBuffer != nullptr);

	// Upload the Geometry Data to the Buffer
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	wgpuQueueWriteBuffer(queue, vertexBuffer, 0, vertexData, bufferDesc.size);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);

	return vertexBuffer;
}

WGPUBuffer createBufferMatrix(const WGPUDevice& device, const WGPUQueue& queue,
const mat4x4& matrix)
{
	WGPUBuffer matrixBuffer;
	
	// Describe the Matrix Buffer
	WGPUBufferDescriptor bufferDesc = {};
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = "MatrixBuffer";
	bufferDesc.size = sizeof(matrix);
	bufferDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform; // Uniform usage here!
	bufferDesc.mappedAtCreation = false;
	
	// Create a Uniform Buffer
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	matrixBuffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Check for Errors
	if (matrixBuffer == nullptr)
	{
		perror("[ERROR]: Failed to create a Matrix Buffer");
		
		return nullptr;
	}
	else
	{
		puts("Created a Matrix Buffer");
	}
	
	// Assert
	MY_ASSERT(matrixBuffer != nullptr);

	// Upload the Matrix to the Buffer
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	wgpuQueueWriteBuffer(queue, matrixBuffer, 0, matrix, bufferDesc.size);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);

	return matrixBuffer;
}
