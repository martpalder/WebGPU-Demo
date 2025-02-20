#include "./texture_loader.hpp"
#include "./myassert.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

WGPUImageCopyTexture loadTexture(const GPUEnv& gpuEnv, const char* fileName)
{
	// Define the texture descriptor
	WGPUTextureDescriptor textureDesc = {};
	textureDesc.nextInChain = nullptr;
	textureDesc.size.width = 64;  // Set the desired width
	textureDesc.size.height = 128; // Set the desired height
	textureDesc.size.depthOrArrayLayers = 1;
	textureDesc.mipLevelCount = 1;
	textureDesc.sampleCount = 1;
	textureDesc.dimension = WGPUTextureDimension_2D;
	textureDesc.format = WGPUTextureFormat_RGBA8Unorm; // Set the desired format
	textureDesc.usage = WGPUTextureUsage_CopyDst | WGPUTextureUsage_TextureBinding;
	textureDesc.viewFormatCount = 0;
	textureDesc.viewFormats = nullptr;

	// Create the texture
	pushError(gpuEnv.dev);
	WGPUTexture texture = wgpuDeviceCreateTexture(gpuEnv.dev, &textureDesc);
	popError(gpuEnv.dev);
	// Check if loaded
	if (texture == nullptr)
	{
		perror("[ERROR]: Failed to create a Texture");
		exit(-1);
	}
	
	MY_ASSERT(texture != nullptr);

	WGPUImageCopyTexture copyTexture = {};
	copyTexture.nextInChain = nullptr;
	copyTexture.texture = texture;
	copyTexture.mipLevel = 0;
	copyTexture.origin = { 0, 0, 0};
	copyTexture.aspect = WGPUTextureAspect_All;
	
	// Set the Filepath
	char path[32];
	sprintf(path, "./data/textures/%s", fileName);
	
	// Load an Image
	int w, h, nCh;
	stbi_uc* data = stbi_load(path, &w, &h, &nCh, 4);
	// Check if loaded
	if (data == nullptr)
	{
		perror("[ERROR]: Failed to load an Image");
		exit(-1);
	}
	puts("Loaded an Image");
	printf("W: %d H: %d nCh: %d\n", w, h, nCh);
	
	MY_ASSERT(data != nullptr);
	
	// Calculate the Data Size
	size_t dataSz = (size_t)(w * h * 4);
	
	// Define the Data Layout
	WGPUTextureDataLayout dataLayout = {};
	dataLayout.nextInChain = nullptr;
	dataLayout.offset = 0;
	dataLayout.bytesPerRow = (w * 4 + 255) & ~255;  // Align bytesPerRow to the nearest 256
	dataLayout.rowsPerImage = h;
	
	// Set the Extent
	WGPUExtent3D extent = { (uint32_t)w, (uint32_t)h, 1 };
	
	// Write the Texture
	pushError(gpuEnv.dev);
	wgpuQueueWriteTexture(
		gpuEnv.queue,
		&copyTexture,
		data, dataSz,
		&dataLayout,
		&extent
	);
	popError(gpuEnv.dev);
	
	// Release the Image Data
	stbi_image_free(data);
	data = nullptr;
	printf("Loaded a Texture: '%s'\n", fileName);
	
	return copyTexture;
}
