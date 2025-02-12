#include "./layout.hpp"
#include "./callback.hpp"

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
	
	return attrib;
}

WGPUVertexBufferLayout createLayoutBufferVert(int numFloats, const WGPUVertexAttribute* pPosAttrib)
{
	WGPUVertexBufferLayout bufferLayout = {};
	
	/// Describe the Buffer Layout
	// {{Describe buffer stride and step mode}}
	bufferLayout.stepMode = WGPUVertexStepMode_Vertex;
	bufferLayout.arrayStride = numFloats * sizeof(float);
	// {{Set Attributes}}
	if (pPosAttrib != nullptr)
	{
		bufferLayout.attributeCount = 1;
		bufferLayout.attributes = pPosAttrib;
	}
	
	return bufferLayout;
}

WGPUBindGroupLayoutEntry createLayoutBinding()
{
	// Define Binding Layout
	WGPUBindGroupLayoutEntry bindingLayout{};
	
	// Set Default Values
	// Buffer
	bindingLayout.buffer.nextInChain = nullptr;
	bindingLayout.buffer.type = WGPUBufferBindingType_Undefined;
	bindingLayout.buffer.hasDynamicOffset = false;
	// Sampler
	bindingLayout.sampler.nextInChain = nullptr;
	bindingLayout.sampler.type = WGPUSamplerBindingType_Undefined;
	// Storage Texture
	bindingLayout.storageTexture.nextInChain = nullptr;
	bindingLayout.storageTexture.access = WGPUStorageTextureAccess_Undefined;
	bindingLayout.storageTexture.format = WGPUTextureFormat_Undefined;
	bindingLayout.storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;
	// Texture
	bindingLayout.texture.nextInChain = nullptr;
	bindingLayout.texture.multisampled = false;
	bindingLayout.texture.sampleType = WGPUTextureSampleType_Undefined;
	bindingLayout.texture.viewDimension = WGPUTextureViewDimension_Undefined;
	
	// The binding index as used in the @binding attribute in the shader
	bindingLayout.binding = 0;
	// The stage that needs to access this resource
	bindingLayout.visibility = WGPUShaderStage_Vertex;
	bindingLayout.buffer.type = WGPUBufferBindingType_Uniform;
	bindingLayout.buffer.minBindingSize = 4 * sizeof(float);
	
	return bindingLayout;
}

WGPUBindGroupLayout createLayoutBindGroup(const WGPUDevice& device,
const WGPUBindGroupLayoutEntry& bindingLayout)
{
	WGPUBindGroupLayout bindGroupLayout;
	
	// Describe the bind group layout
	WGPUBindGroupLayoutDescriptor bindGroupLayoutDesc{};
	bindGroupLayoutDesc.nextInChain = nullptr;
	bindGroupLayoutDesc.entryCount = 1;
	bindGroupLayoutDesc.entries = &bindingLayout;
	
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
	
	return bindGroupLayout;
}

WGPUPipelineLayout createLayoutPipeline(const WGPUDevice& device)
{
	WGPUPipelineLayout pipelineLayout;
	
	// {{Define bindingLayout}}
	WGPUBindGroupLayoutEntry bindingLayout = createLayoutBinding();
	
	WGPUBindGroupLayoutDescriptor bindGroupLayoutDesc{};
	bindGroupLayoutDesc.nextInChain = nullptr;
	bindGroupLayoutDesc.entryCount = 1;
	bindGroupLayoutDesc.entries = &bindingLayout;
	WGPUBindGroupLayout bindGroupLayout;
	
	// Create the Bind Group Layout
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	bindGroupLayout = wgpuDeviceCreateBindGroupLayout(device, &bindGroupLayoutDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Check for Errors
	if (pipelineLayout == nullptr)
	{
		perror("[ERROR]: Failed to create a Bind Group Layout");
	}
	else
	{
		puts("Created a Bind Group Layout");
	}

	// Describe the Pipeline Layout
	WGPUPipelineLayoutDescriptor layoutDesc{};
	layoutDesc.nextInChain = nullptr;
	layoutDesc.bindGroupLayoutCount = 1;
	layoutDesc.bindGroupLayouts = &bindGroupLayout;
	
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
	
	return pipelineLayout;
}
