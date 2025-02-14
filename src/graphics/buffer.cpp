#include "./buffer.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <cstdio>

WGPUBuffer createBufferVert(const GPUEnv& gpuEnv,
size_t dataSize, const CUSTOM_VERTEX* vertexData)
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
	wgpuDevicePushErrorScope(gpuEnv.dev, WGPUErrorFilter_Validation);
	vertexBuffer = wgpuDeviceCreateBuffer(gpuEnv.dev, &bufferDesc);
	wgpuDevicePopErrorScope(gpuEnv.dev, errorCallback, nullptr);
	
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
	wgpuDevicePushErrorScope(gpuEnv.dev, WGPUErrorFilter_Validation);
	wgpuQueueWriteBuffer(gpuEnv.queue, vertexBuffer, 0, vertexData, bufferDesc.size);
	wgpuDevicePopErrorScope(gpuEnv.dev, errorCallback, nullptr);

	return vertexBuffer;
}

WGPUBuffer createBufferMatrix(const GPUEnv& gpuEnv, const mat4x4& matrix)
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
	wgpuDevicePushErrorScope(gpuEnv.dev, WGPUErrorFilter_Validation);
	matrixBuffer = wgpuDeviceCreateBuffer(gpuEnv.dev, &bufferDesc);
	wgpuDevicePopErrorScope(gpuEnv.dev, errorCallback, nullptr);
	
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
	wgpuDevicePushErrorScope(gpuEnv.dev, WGPUErrorFilter_Validation);
	wgpuQueueWriteBuffer(gpuEnv.queue, matrixBuffer, 0, matrix, bufferDesc.size);
	wgpuDevicePopErrorScope(gpuEnv.dev, errorCallback, nullptr);

	return matrixBuffer;
}
