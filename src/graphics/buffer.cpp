#include "./buffer.hpp"
#include "./myassert.hpp"

size_t getAlignedSize(size_t dataSz)
{
	return ( ((dataSz + COPY_BUFFER_ALIGNMENT - 1) / COPY_BUFFER_ALIGNMENT) * COPY_BUFFER_ALIGNMENT );
}

void uploadToBuffer(const WGPUQueue& queue, const WGPUBuffer& buffer,
size_t dataSz, const void* data)
{
	wgpuQueueWriteBuffer(queue, buffer, 0, data, dataSz);
}

WGPUBuffer createBufferVert(const WGPUDevice& device, size_t dataSz,
const char* label)
{
	WGPUBuffer vertexBuffer = nullptr;
	
	// Describe Vertex Buffer
	WGPUBufferDescriptor bufferDesc = {};
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = label;
	bufferDesc.size = dataSz;
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

WGPUBuffer createBufferIdx(const WGPUDevice& device, size_t dataSz,
const char* label)
{
	WGPUBuffer indexBuffer;
	
	// Describe the Index Buffer
	WGPUBufferDescriptor bufferDesc = {};
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = label;
	bufferDesc.size = dataSz;
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

WGPUBuffer createBufferUniformMat(const GPUEnv& gpuEnv, const mat4x4& matrix,
const char* label)
{
	WGPUBuffer matrixBuffer;
	
	// Describe the Matrix Buffer
	WGPUBufferDescriptor bufferDesc = {};
	bufferDesc.nextInChain = nullptr;
	bufferDesc.label = label;
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

	return matrixBuffer;
}
