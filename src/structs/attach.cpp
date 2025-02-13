#include "./attach.hpp"

WGPURenderPassColorAttachment createRenderPassColorAttach(float r, float g, float b)
{
	// The attachment part of the render pass descriptor describes the target texture of the pass
	WGPURenderPassColorAttachment renderPassColorAttach = {};
	
	renderPassColorAttach.resolveTarget = nullptr;
	renderPassColorAttach.loadOp = WGPULoadOp_Clear;
	renderPassColorAttach.storeOp = WGPUStoreOp_Store;
	renderPassColorAttach.clearValue = WGPUColor{ r, g, b, 1.0 };
	#ifndef WEBGPU_BACKEND_WGPU
		renderPassColorAttach.depthSlice = WGPU_DEPTH_SLICE_UNDEFINED;
	#endif // NOT WEBGPU_BACKEND_WGPU
	
	return renderPassColorAttach;
}
