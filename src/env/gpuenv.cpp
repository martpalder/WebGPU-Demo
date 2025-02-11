#include "./gpuenv.hpp"
#include "./instance.hpp"
#include "./request.hpp"
#include "./desc.hpp"
#include "./inspect.hpp"
#include "./callback.hpp"
#include "./glfw3webgpu.h"

#include <webgpu/webgpu.h>
#include <cstdio>

GPUEnv initGPUEnv(GLFWwindow* wnd)
{
	GPUEnv gpuEnv;
	
	// Create a WebGPU Instance
	gpuEnv.inst = createInstance();
	
	// Get the Surface
	gpuEnv.surf = glfwGetWGPUSurface(gpuEnv.inst, wnd);
	
	// Request the Adapter
	WGPURequestAdapterOptions adapterOpts = {};
	adapterOpts.nextInChain = nullptr;
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

void quitGPUEnv(GPUEnv gpuEnv)
{
	// Release the Render Pipeline
	wgpuRenderPipelineRelease(gpuEnv.pipeline);
	puts("Released the Render Pipeline");

	// Unconfigure the Surface
	wgpuSurfaceUnconfigure(gpuEnv.surf);
	puts("Unconfigured the Surface");

	// Release the Surface
	wgpuSurfaceRelease(gpuEnv.surf);
	puts("Released the Surface");

	// Release the Queue
	wgpuQueueRelease(gpuEnv.queue);
	puts("Released the Queue");

	// Release the Device
	wgpuDeviceRelease(gpuEnv.dev);
	puts("Released the Device");

	// Release the Instance
	wgpuInstanceRelease(gpuEnv.inst);
	puts("Released the Instance");
}
