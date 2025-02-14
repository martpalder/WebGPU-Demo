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
	
	gpuEnv.surf = nullptr;
	#ifndef __EMSCRIPTEN__
	// Get the Surface from GLFW
	gpuEnv.surf = glfwGetWGPUSurface(gpuEnv.inst, wnd);
	#endif
	
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
	
	#ifndef __EMSCRIPTEN__
	// Get the Window Size
	int w, h;
	glfwGetWindowSize(wnd, &w, &h);
	gpuEnv.surf = glfwGetWGPUSurface(gpuEnv.inst, wnd);
	// Configure the Surface
	WGPUSurfaceConfiguration config = createSurfConfig(w, h, gpuEnv.dev);
	wgpuSurfaceConfigure(gpuEnv.surf, &config);
	#endif
	
	// Get the Queue
	gpuEnv.queue = wgpuDeviceGetQueue(gpuEnv.dev);
	// Set the Queue Callbacks
	wgpuQueueOnSubmittedWorkDone(gpuEnv.queue, onQueueWorkDone, nullptr /* pUserData */);
	
	return gpuEnv;
}

void quitGPUEnv(const GPUEnv& gpuEnv)
{
	// Release the Target View
	wgpuTextureViewRelease(gpuEnv.targetView);
	assignNull(gpuEnv.targetView, "Target View");
	
	// Release the Render Pipeline
	wgpuRenderPipelineRelease(gpuEnv.pipeline);
	assignNull(gpuEnv.pipeline, "Render Pipeline");
	
	// Unconfigure the Surface
	wgpuSurfaceUnconfigure(gpuEnv.surf);
	puts("Unconfigured the Surface");
	
	// Release the Surface
	wgpuSurfaceRelease(gpuEnv.surf);
	assignNull(gpuEnv.surf, "Surface");
	
	// Release the Queue
	wgpuQueueRelease(gpuEnv.queue);
	assignNull(gpuEnv.queue, "Queue");
	
	// Release the Device
	wgpuDeviceRelease(gpuEnv.dev);
	assignNull(gpuEnv.dev, "Device");
	
	// Release the Instance
	wgpuInstanceRelease(gpuEnv.inst);
	assignNull(gpuEnv.inst, "Instance");
}
