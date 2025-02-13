#include "./gpuenv.hpp"
#include "./instance.hpp"
#include "./request.hpp"
#include "./desc.hpp"
#include "./inspect.hpp"
#include "./callback.hpp"
#include "./glfw3webgpu.h"

#include <webgpu/webgpu.h>
#include <cstdio>

inline void assignNull(void* pObj, const char* name)
{
	pObj = nullptr;
	printf("Released the %s\n", name);
}

GPUEnv initGPUEnv(GLFWwindow* wnd)
{
	GPUEnv gpuEnv;
	
	// Create a WebGPU Instance
	gpuEnv.inst = createInstance();
	
	#ifndef __EMSCRIPTEN__
	// Get the Surface
	gpuEnv.surf = glfwGetWGPUSurface(gpuEnv.inst, wnd);
	#endif
	
	// Set the Adapter Options
	WGPURequestAdapterOptions adapterOpts = {};
	adapterOpts.nextInChain = nullptr;
	adapterOpts.compatibleSurface = gpuEnv.surf;
	adapterOpts.powerPreference = WGPUPowerPreference_LowPower;
	adapterOpts.backendType = WGPUBackendType_Undefined;
	
	// Request the Adapter
	WGPUAdapter adapter = requestAdapterSync(gpuEnv.inst, &adapterOpts);
	// Release the Instance
	wgpuInstanceRelease(gpuEnv.inst);
	// Inspect the Adapter
	inspectAdapter(adapter);

	// Request the Device
	WGPUDeviceDescriptor deviceDesc = createDeviceDesc();
	gpuEnv.dev = requestDeviceSync(adapter, &deviceDesc);
	// Release the Adapter
	wgpuAdapterRelease(adapter);
	// Inspect the Device
	inspectDevice(gpuEnv.dev);
	
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
