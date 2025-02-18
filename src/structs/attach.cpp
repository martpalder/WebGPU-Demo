#include "./attach.hpp"
#include "./texture.hpp"
#include "./callback.hpp"
#include "./stdafx.h"

WGPURenderPassColorAttachment createColorAttach(float r, float g, float b)
{
	// The attachment part of the render pass descriptor describes the target texture of the pass
	WGPURenderPassColorAttachment colorAttach = {};
	
	// Define the Color Attachment
	colorAttach.resolveTarget = nullptr;
	colorAttach.loadOp = WGPULoadOp_Clear;
	colorAttach.storeOp = WGPUStoreOp_Store;
	colorAttach.clearValue = WGPUColor{ r, g, b, 1.0 };
	#ifndef WEBGPU_BACKEND_WGPU
		colorAttach.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
	#endif // NOT WEBGPU_BACKEND_WGPU
	puts("Created a Color Attachment");
	
	return colorAttach;
}

WGPURenderPassDepthStencilAttachment createDepthStencilAttach(const WGPUDevice& device,
const WGPUTexture& depthTexture)
{	
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
	wgpuDevicePushErrorScope(device, WGPUErrorFilter_Validation);
	WGPUTextureView depthView = wgpuTextureCreateView(depthTexture, &depthViewDesc);
	wgpuDevicePopErrorScope(device, errorCallback, nullptr);
	
	// The Depth Stencil Attachment handles Z-Buffering
	WGPURenderPassDepthStencilAttachment depthStencilAttach = {};
	
	// Define the Depth Stencil Attachment
	// Attach the Depth Texture to View
	depthStencilAttach.view = depthView;
	// Depth
	depthStencilAttach.depthLoadOp = WGPULoadOp_Clear;
	depthStencilAttach.depthStoreOp = WGPUStoreOp_Store;
	depthStencilAttach.depthClearValue = 1.0f;
	depthStencilAttach.depthReadOnly = false;
	// Stencil
	depthStencilAttach.stencilLoadOp = WGPULoadOp_Undefined;
	depthStencilAttach.stencilStoreOp = WGPUStoreOp_Undefined;
	puts("Created a Depth Stencil Attachment");
	
	return depthStencilAttach;
}
