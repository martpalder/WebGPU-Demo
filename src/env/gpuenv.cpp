#include "./gpuenv.hpp"
#include "./instance.hpp"
#include "./request.hpp"
#include "./desc.hpp"
#include "./inspect.hpp"
#include "./config.hpp"
#include "./callback.hpp"

#include <webgpu/webgpu.h>
#include <cstdio>

#ifndef __EMSCRIPTEN__
#include <glfw3webgpu.h>
#endif

WGPURequestAdapterOptions createAdapterOpts(const WGPUSurface& surf)
{
	WGPURequestAdapterOptions adapterOpts = {};
	
	adapterOpts.nextInChain = nullptr;
	adapterOpts.compatibleSurface = surf;
	adapterOpts.powerPreference = WGPUPowerPreference_LowPower;
	adapterOpts.backendType = WGPUBackendType_Undefined;
	
	return adapterOpts;
}

GPUEnv initGPUEnv(GLFWwindow* wnd)
{
	GPUEnv gpuEnv;
	
	// Create a WebGPU Instance
	gpuEnv.inst = createInstance();
	// Get the Surface from GLFW
	gpuEnv.surf = glfwGetWGPUSurface(gpuEnv.inst, wnd);
	
	// Create the Adapter Options
	WGPURequestAdapterOptions adapterOpts;
	adapterOpts = createAdapterOpts(gpuEnv.surf);
	
	// Request the Adapter
	WGPUAdapter adapter = requestAdapterSync(gpuEnv.inst, &adapterOpts);
	// Release the Instance
	wgpuInstanceRelease(gpuEnv.inst);
	// Inspect the Adapter
	inspectAdapter(adapter);

	// Create the Device Descriptor
	WGPUDeviceDescriptor deviceDesc = createDeviceDesc();
	
	// Request the Device
	gpuEnv.dev = requestDeviceSync(adapter, &deviceDesc);
	// Release the Adapter
	wgpuAdapterRelease(adapter);
	// Inspect the Device
	inspectDevice(gpuEnv.dev);
	
	// Get the Window Size
	int w, h;
	glfwGetWindowSize(wnd, &w, &h);
	
	// Create a Surface Configuration
	WGPUSurfaceConfiguration config = createSurfConfig(w, h, gpuEnv.dev);
	
	// Configure the Surface
	wgpuDevicePushErrorScope(gpuEnv.dev, WGPUErrorFilter_Validation);
	wgpuSurfaceConfigure(gpuEnv.surf, &config);
	wgpuDevicePopErrorScope(gpuEnv.dev, errorCallback, nullptr);
	
	// Get the Queue
	wgpuDevicePushErrorScope(gpuEnv.dev, WGPUErrorFilter_Validation);
	gpuEnv.queue = wgpuDeviceGetQueue(gpuEnv.dev);
	wgpuDevicePopErrorScope(gpuEnv.dev, errorCallback, nullptr);
	
	// Set the Queue Callbacks
	wgpuDevicePushErrorScope(gpuEnv.dev, WGPUErrorFilter_Validation);
	wgpuQueueOnSubmittedWorkDone(gpuEnv.queue, onQueueWorkDone, nullptr /* pUserData */);
	wgpuDevicePopErrorScope(gpuEnv.dev, errorCallback, nullptr);
	
	return gpuEnv;
}

void quitGPUEnv(const GPUEnv& gpuEnv)
{
	// Release the Target View
	wgpuTextureViewRelease(gpuEnv.targetView);
	printRelease("Target View");
	
	// Release the Render Pipeline
	wgpuRenderPipelineRelease(gpuEnv.pipeline);
	printRelease("Render Pipeline");
	
	// Unconfigure the Surface
	wgpuSurfaceUnconfigure(gpuEnv.surf);
	puts("Unconfigured the Surface");
	
	// Release the Surface
	wgpuSurfaceRelease(gpuEnv.surf);
	printRelease("Surface");
	
	// Release the Queue
	wgpuQueueRelease(gpuEnv.queue);
	printRelease("Queue");
	
	// Release the Device
	wgpuDeviceRelease(gpuEnv.dev);
	printRelease("Device");
	
	// Release the Instance
	wgpuInstanceRelease(gpuEnv.inst);
	printRelease("Instance");
}
