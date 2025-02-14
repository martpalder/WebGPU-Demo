#include "./layout.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <cstdio>

WGPUVertexBufferLayout createLayoutBufferVert(size_t vertexSz, size_t attribCount,
WGPUVertexAttribute* pAttribs)
{
	WGPUVertexBufferLayout bufferLayout = {};
	
	/// Describe the Buffer Layout
	// {{Describe buffer stride and step mode}}
	bufferLayout.stepMode = WGPUVertexStepMode_Vertex;
	bufferLayout.arrayStride = vertexSz;
	// {{Set the Attributes}}
	if (attribCount != 0)
	{
		bufferLayout.attributeCount = attribCount;
		bufferLayout.attributes = pAttribs;
	}
	else
	{
		bufferLayout.attributeCount = 0;
		bufferLayout.attributes = nullptr;
	}
	puts("Created a Vertex Buffer Layout");
	
	return bufferLayout;
}

WGPUBindGroupLayoutEntry createLayoutBinding(size_t minBindingSize)
{
	// Define Binding Layout
	WGPUBindGroupLayoutEntry bindingLayout = {};
	
	// The binding index as used in the @binding attribute in the shader
	bindingLayout.binding = 0;
	// The stage that needs to access this resource
	bindingLayout.visibility = WGPUShaderStage_Vertex;
	bindingLayout.buffer.type = WGPUBufferBindingType_Uniform;
	bindingLayout.buffer.minBindingSize = minBindingSize;
	
	// Set Default Values
	// Buffer
	bindingLayout.buffer.nextInChain = nullptr;
	bindingLayout.buffer.type = WGPUBufferBindingType_Uniform;
	bindingLayout.buffer.hasDynamicOffset = false;
	// Sampler
	bindingLayout.sampler.nextInChain = nullptr;
	bindingLayout.sampler.type = WGPUSamplerBindingType_Undefined;
	// Texture
	bindingLayout.texture.nextInChain = nullptr;
	bindingLayout.texture.multisampled = false;
	bindingLayout.texture.sampleType = WGPUTextureSampleType_Undefined;
	bindingLayout.texture.viewDimension = WGPUTextureViewDimension_Undefined;
	// Storage Texture
	bindingLayout.storageTexture.nextInChain = nullptr;
	bindingLayout.storageTexture.access = WGPUStorageTextureAccess_Undefined;
	bindingLayout.storageTexture.format = WGPUTextureFormat_Undefined;
	bindingLayout.storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;
	
	puts("Created a Binding Layout");
	
	return bindingLayout;
}

WGPUBindGroupLayout createLayoutBindGroup(const WGPUDevice& device,
WGPUBindGroupLayoutEntry* pBindingLayout)
{
	WGPUBindGroupLayout bindGroupLayout = nullptr;
	
	// Describe the bind group layout
	WGPUBindGroupLayoutDescriptor bindGroupLayoutDesc{};
	bindGroupLayoutDesc.nextInChain = nullptr;
	bindGroupLayoutDesc.label = "BindGroupLayout";
	// Set the Entries
	if (pBindingLayout != nullptr)
	{
		bindGroupLayoutDesc.entryCount = 1;
		bindGroupLayoutDesc.entries = pBindingLayout;
	}
	
	// Create the bind group layout
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bindGroupLayoutDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Check for Errors
	if (bindGroupLayout == nullptr)
	{
		perror("[ERROR]: Failed to create a Bind Group Layout");
	}
	else
	{
		puts("Created a Bind Group Layout");
	}
	
	// Assert
	MY_ASSERT(bindGroupLayout != nullptr);
	
	return bindGroupLayout;
}

WGPUPipelineLayout createLayoutPipeline(const WGPUDevice& device,
WGPUBindGroupLayout* pBindGroupLayout)
{
	WGPUPipelineLayout pipelineLayout = nullptr;

	// Describe the Pipeline Layout
	WGPUPipelineLayoutDescriptor layoutDesc{};
	layoutDesc.nextInChain = nullptr;
	layoutDesc.bindGroupLayoutCount = 0;
	layoutDesc.bindGroupLayouts = nullptr;
	
	if (pBindGroupLayout != nullptr)
	{
		// Set the Bind Group Layout
		layoutDesc.bindGroupLayoutCount = 1;
		layoutDesc.bindGroupLayouts = pBindGroupLayout;
		puts("Set the Bind Group Layout");
	}
	
	// Create the Pipeline Layout
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	pipelineLayout = wgpuDeviceCreatePipelineLayout(device, &layoutDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Check for Errors
	if (pipelineLayout == nullptr)
	{
		perror("[ERROR]: Failed to create a Render Pipeline");
	}
	else
	{
		puts("Created a Pipeline Layout");
	}
	
	// Assert
	MY_ASSERT(pipelineLayout != nullptr);
	
	return pipelineLayout;
}
