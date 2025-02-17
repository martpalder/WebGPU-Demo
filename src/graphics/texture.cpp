#include "./texture.hpp"
#include "./callback.hpp"
#include "./myassert.hpp"

#include <cstdio>

#include <cmath> // For std::log2 and std::floor

// Function to calculate mip level count
uint32_t calculateMipLevelCount(uint32_t width, uint32_t height) {
    return static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
}


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
	printf("Mip Levels: %u\n", textureDesc.mipLevelCount);
	
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
