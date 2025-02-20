#include "./view.hpp"
#include "./myassert.hpp"

void getNextTargetView(const WGPUSurface& surf, WGPUTextureView* pTargetView)
{
    // {{Get the next surface texture}}
	WGPUSurfaceTexture surfaceTexture;
	wgpuSurfaceGetCurrentTexture(surf, &surfaceTexture);
	if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success)
	{
		return;
	}
	
    // {{Create surface texture view}}
	WGPUTextureViewDescriptor viewDescriptor = {};
	viewDescriptor.nextInChain = nullptr;
	viewDescriptor.label = "Surface texture view";
	viewDescriptor.format = wgpuTextureGetFormat(surfaceTexture.texture);
	viewDescriptor.dimension = WGPUTextureViewDimension_2D;
	viewDescriptor.baseMipLevel = 0;
	viewDescriptor.mipLevelCount = 1;
	viewDescriptor.baseArrayLayer = 0;
	viewDescriptor.arrayLayerCount = 1;
	viewDescriptor.aspect = WGPUTextureAspect_All;
	*pTargetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);
	
	#ifdef __EMSCRIPTEN__
	// We no longer need the texture, only its view
	// (NB: with wgpu-native, surface textures must not be manually released)
	wgpuTextureRelease(surfaceTexture.texture);
	#endif // __EMSCRIPTEN__
}

WGPUTextureView createDepthView(const WGPUTexture& depthTexture)
{
	WGPUTextureView depthView;
	
	// Define Depth Texture View
	WGPUTextureViewDescriptor depthViewDesc = {};
	depthViewDesc.nextInChain = nullptr;
	depthViewDesc.label = "DepthView";
	depthViewDesc.format = wgpuTextureGetFormat(depthTexture);
	depthViewDesc.dimension = WGPUTextureViewDimension_2D;
	depthViewDesc.baseMipLevel = 0;
	depthViewDesc.mipLevelCount = 1;
	depthViewDesc.baseArrayLayer = 0;
	depthViewDesc.arrayLayerCount = 1;

	// Create a Depth Texture View
	depthView = wgpuTextureCreateView(depthTexture, &depthViewDesc);
	
	// Check for Error
	if (depthView == nullptr)
	{
		perror("[ERROR]: Failed to create a Depth Texture View");
		exit(-1);
	}
	
	// Assert
	MY_ASSERT(depthView != nullptr);
	
	return depthView;
}
