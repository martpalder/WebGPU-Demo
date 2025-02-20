#include "./texture.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

WGPUTexture createDepthTexture(const WGPUDevice& device, uint32_t w, uint32_t h)
{
	WGPUTexture depthTexture = {};
	
	// Set Texture Size
	WGPUExtent3D extent = { w, h, 1 };
	
	// Define the Depth Texture
	WGPUTextureDescriptor textureDesc = {};
	textureDesc.nextInChain = nullptr;
	textureDesc.label = "DepthTexture";
	textureDesc.usage = WGPUTextureUsage_RenderAttachment | WGPUTextureUsage_TextureBinding;
	textureDesc.dimension = WGPUTextureDimension_2D;
	textureDesc.size = extent;
	textureDesc.format = WGPUTextureFormat_Depth24Plus;
	textureDesc.mipLevelCount = 1;
	textureDesc.sampleCount = 1;
	textureDesc.viewFormatCount = 0;
	textureDesc.viewFormats = nullptr;
	
	// Create a Depth Texture
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	depthTexture = wgpuDeviceCreateTexture(device, &textureDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// Check for Error
	if (depthTexture == nullptr)
	{
		perror("[ERROR: Failed to create a Depth Texture");
	}
	else
	{
		puts("Created a Depth Texture");
	}
	
	// Assert
	MY_ASSERT(depthTexture != nullptr);
	
	return depthTexture;
}
