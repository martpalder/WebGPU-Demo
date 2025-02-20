#include "./buffer.hpp"
#include "./myassert.hpp"

void uploadToBuffer(const WGPUQueue& queue, const WGPUBuffer& buffer,
size_t dataSz, const void* data)
{
	wgpuQueueWriteBuffer(queue, buffer, 0, data, dataSz);
}

WGPUBuffer createBufferVert(const WGPUDevice& device, size_t dataSize)
{
	WGPUBuffer vertexBuffer = nullptr;
	
	// Describe Vertex Buffer
	WGPUBufferDescriptor bufferDesc = {};
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = "VertexBuffer";
	bufferDesc.size = dataSize;
	bufferDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex; // Vertex usage here!
	bufferDesc.mappedAtCreation = false;
	
	// Create a Vertex Buffer
	pushError(device);
	vertexBuffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
	popError(device);
	
	// Check for Errors
	if (vertexBuffer == nullptr)
	{
		perror("[ERROR]: Failed to create a Vertex Buffer");
		
		return nullptr;
	}
	puts("Created a Vertex Buffer");
	
	// Assert
	MY_ASSERT(vertexBuffer != nullptr);

	return vertexBuffer;
}

WGPUBuffer createBufferIdx(const WGPUDevice& device, size_t dataSize)
{
	WGPUBuffer indexBuffer;
	
	// Describe the Index Buffer
	WGPUBufferDescriptor bufferDesc = {};
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = "IndexBuffer";
	bufferDesc.size = dataSize;
	bufferDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index ; // Index usage here!
	bufferDesc.mappedAtCreation = false;
	
	// Create the Index Buffer
	pushError(device);
	indexBuffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
	popError(device);
	
	// Check for Errors
	if (indexBuffer == nullptr)
	{
		perror("[ERROR]: Failed to create an Index Buffer");
		
		return nullptr;
	}
	else
	{
		puts("Created an Index Buffer");
	}
	
	// Assert
	MY_ASSERT(indexBuffer != nullptr);

	return indexBuffer;
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
	pushError(gpuEnv.dev);
	matrixBuffer = wgpuDeviceCreateBuffer(gpuEnv.dev, &bufferDesc);
	popError(gpuEnv.dev);
	
	// Check for Errors
	if (matrixBuffer == nullptr)
	{
		perror("[ERROR]: Failed to create a Matrix Buffer");
		
		return nullptr;
	}
	else
	{
		puts("Created a Uniform Matrix Buffer");
	}
	
	// Assert
	MY_ASSERT(matrixBuffer != nullptr);

	// Upload the Matrix to the Buffer
	pushError(gpuEnv.dev);
	wgpuQueueWriteBuffer(gpuEnv.queue, matrixBuffer, 0, matrix, bufferDesc.size);
	popError(gpuEnv.dev);

	return matrixBuffer;
}
