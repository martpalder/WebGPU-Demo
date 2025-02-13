#include "./layout.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <cstdio>

WGPUVertexAttribute createAttribVert(int idx)
{
	WGPUVertexAttribute attrib = {};
	
	// {{Describe the position attribute}}
	// == For each attribute, describe its layout, i.e., how to interpret the raw data ==
	// Means vec2f in the shader
	attrib.format = WGPUVertexFormat_Float32x2;
	// Index of the first element
	attrib.offset = idx;
	// Corresponds to @location(...)
	attrib.shaderLocation = idx;
	printf("Created a Vertex Attribute %d\n", idx);
	
	return attrib;
}

WGPUVertexBufferLayout createLayoutBufferVert(int numFloats, const WGPUVertexAttribute* pPosAttrib)
{
	WGPUVertexBufferLayout bufferLayout = {};
	
	/// Describe the Buffer Layout
	// {{Describe buffer stride and step mode}}
	bufferLayout.stepMode = WGPUVertexStepMode_Vertex;
	bufferLayout.arrayStride = numFloats * sizeof(float);
	// {{Set the Attributes}}
	if (pPosAttrib != nullptr)
	{
		bufferLayout.attributeCount = 1;
		bufferLayout.attributes = pPosAttrib;
	}
	puts("Created a Vertex Buffer Layout");
	
	return bufferLayout;
}

WGPUBindGroupLayoutEntry createLayoutBinding()
{
	// Define Binding Layout
	WGPUBindGroupLayoutEntry bindingLayout = {};
	
	// The binding index as used in the @binding attribute in the shader
	bindingLayout.binding = 0;
	// The stage that needs to access this resource
	bindingLayout.visibility = WGPUShaderStage_Vertex;
	bindingLayout.buffer.type = WGPUBufferBindingType_Uniform;
	bindingLayout.buffer.minBindingSize = 4 * sizeof(float);
	
	// Set Default Values
	// Buffer
	bindingLayout.buffer.nextInChain = nullptr;
	bindingLayout.buffer.type = WGPUBufferBindingType_Uniform;
	bindingLayout.buffer.hasDynamicOffset = false;
	// Sampler
	bindingLayout.sampler.nextInChain = nullptr;
	bindingLayout.sampler.type = WGPUSamplerBindingType_Filtering;
	// Texture
	bindingLayout.texture.nextInChain = nullptr;
	bindingLayout.texture.multisampled = false;
	bindingLayout.texture.sampleType = WGPUTextureSampleType_Float;
	bindingLayout.texture.viewDimension = WGPUTextureViewDimension_2D;
	// Storage Texture
	bindingLayout.storageTexture.nextInChain = nullptr;
	bindingLayout.storageTexture.access = WGPUStorageTextureAccess_WriteOnly;
	bindingLayout.storageTexture.format = WGPUTextureFormat_RGBA8Unorm;
	bindingLayout.storageTexture.viewDimension = WGPUTextureViewDimension_2D;
	
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

WGPUPipelineLayout createLayoutPipeline(const WGPUDevice& device, WGPUBindGroupLayout* pBindGroupLayout)
{
	WGPUPipelineLayout pipelineLayout = nullptr;

	// Describe the Pipeline Layout
	WGPUPipelineLayoutDescriptor layoutDesc{};
	layoutDesc.nextInChain = nullptr;
	layoutDesc.bindGroupLayoutCount = 1;
	layoutDesc.bindGroupLayouts = pBindGroupLayout;
	
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
