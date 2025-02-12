#include "./config.hpp"

WGPUSurfaceConfiguration createSurfConfig(int w, int h, const WGPUDevice& device)
{
	WGPUSurfaceConfiguration config = {};
	
	config.nextInChain = nullptr;
	// Configuration of the textures created for the underlying swap chain
	config.width = w;
	config.height = h;
	config.format = WGPUTextureFormat_BGRA8Unorm; // Set the texture format
	config.usage = WGPUTextureUsage_RenderAttachment; // Set the usage of the surface
	config.presentMode = WGPUPresentMode_Fifo;
	config.alphaMode = WGPUCompositeAlphaMode_Auto;
	// And we do not need any particular view format:
	config.viewFormatCount = 0;
	config.viewFormats = nullptr;
	config.device = device;
	
	return config;
}
