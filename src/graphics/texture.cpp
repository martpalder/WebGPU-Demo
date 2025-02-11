#include "./texture.hpp"

WGPUTextureView getNextTextureView(const WGPUSurface& surf)
{
    // {{Get the next surface texture}}
	WGPUSurfaceTexture surfaceTexture;
	wgpuSurfaceGetCurrentTexture(surf, &surfaceTexture);
	if (surfaceTexture.status != WGPUSurfaceGetCurrentTextureStatus_Success)
	{
		return nullptr;
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
	WGPUTextureView targetView = wgpuTextureCreateView(surfaceTexture.texture, &viewDescriptor);
	
	#ifndef WEBGPU_BACKEND_WGPU
	// We no longer need the texture, only its view
	// (NB: with wgpu-native, surface textures must not be manually released)
	//wgpuTextureRelease(surfaceTexture.texture);
	#endif // WEBGPU_BACKEND_WGPU

    return targetView;
}
